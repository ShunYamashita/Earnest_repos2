//--------------------------------------------------------------------------------------
//  UnityFBX  ( unityFBX.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "unityFBX.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shaderManager.h"
#include "light.h"
#include "test.h"
#include "utility.h"
#include "keyboard.h"
#include "boneTexture.h"
#include <algorithm>
#include <functional>
#include <unordered_map>

//--------------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------------
static const char TEXTURE_FILENAME_BODY[ 256 ] = "data/UnityChan/body_01.tga";
static const char TEXTURE_FILENAME_FACE[ 256 ] = "data/UnityChan/face_00.tga";
static const char TEXTURE_FILENAME_EYEBASE[ 256 ] = "data/UnityChan/eyeline_00.tga";
static const char TEXTURE_FILENAME_LEFT_EYE[ 256 ] = "data/UnityChan/eye_iris_L_00.tga";
static const char TEXTURE_FILENAME_RIGHT_EYE[ 256 ] = "data/UnityChan/eye_iris_R_00.tga";
static const char TEXTURE_FILENAME_EYELINE[ 256 ] = "data/UnityChan/eyeline_00.tga";
static const char TEXTURE_FILENAME_HAIR[ 256 ] = "data/UnityChan/hair_01.tga";
static const char TEXTURE_FILENAME_SKIN[ 256 ] = "data/UnityChan/skin_01.tga";
static const char TEXTURE_FILENAME_CHEEK[ 256 ] = "data/UnityChan/cheak_00.tga";

static const char SD_TEXTURE_FILENAME_ENV[ 256 ] = "data/UnityChan-SD/ENV2.tga";
static const char SD_TEXTURE_FILENAME_CLOTH[ 256 ] = "data/UnityChan-SD/FO_CLOTH1.tga";
static const char SD_TEXTURE_FILENAME_FO_RIM[ 256 ] = "data/UnityChan-SD/FO_RIM1.tga";
static const char SD_TEXTURE_FILENAME_FO_SKIN[ 256 ] = "data/UnityChan-SD/FO_SKIN1.tga";
static const char SD_TEXTURE_FILENAME_ALL[ 256 ] = "data/UnityChan-SD/utc-all2.tga";
static const char SD_TEXTURE_FILENAME_NORMAL[ 256 ] = "data/UnityChan-SD/utc-normal.tga";
static const char SD_TEXTURE_FILENAME_SPEC[ 256 ] = "data/UnityChan-SD/utc-spec.tga";

static const float PLAYER_TO_CAMERA_DISTANCE_Y = 8.0f;
static const float PLAYER_TO_CAMERA_DISTANCE_Z = 20.0f;

static const float CAMERA_ROTATE_ANGLE = 0.085f;

static const float PLAYER_SPEED = 0.5f;

static const char THUN_SHADER_TEXTURE_NAME[ 256 ] = "data/SHADER/thun001.png";

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
UnityFBX::UnityFBX( )
{
	m_startTime = 0;
	m_endTime = 0;
	m_currentFrame = 0;
	m_allTime = 0;
	m_scale = 1.0f;
	m_translation = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_drawDepth = true;
}

//--------------------------------------------------------------------------------------
//  デスストラクタ
//--------------------------------------------------------------------------------------
UnityFBX::~UnityFBX( )
{

}

//--------------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------------
HRESULT UnityFBX::Init( void )
{
	//  FBXモデル読み込み実験用プログラム
	FbxManager* lSdkManager = FbxManager::Create( ); 
 
	// Create the IO settings object. 
	FbxIOSettings *ios = FbxIOSettings::Create( lSdkManager , IOSROOT ); lSdkManager->SetIOSettings( ios ); 
 
	// Create an importer using the SDK manager. 
	FbxImporter* lImporter = FbxImporter::Create( lSdkManager ,"" ); 
 
	// Use the first argument as the filename for the importer. 
	if( !lImporter->Initialize( "data/UnityChan/unitychan3.fbx", -1, lSdkManager->GetIOSettings( ) ) ) 
	{      
		char buf[ 256 ];     
		sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
		MessageBox( NULL , buf , "error" , MB_OK );     
		return E_FAIL; 
	}  
 
	// Create a new scene so that it can be populated by the imported file. 
	FbxScene* lScene = FbxScene::Create( lSdkManager , "myScene" ); 
 
	// Import the contents of the file into the scene. 
	lImporter->Import( lScene ); 
 
	FbxGeometryConverter converter( lSdkManager ); 
	
	// 三角ポリゴン化 
	converter.Triangulate( lScene , true ); 
	
	// 縮退メッシュを削除 
	converter.RemoveBadPolygonsFromMeshes( lScene ); 
	
	// マテリアルごとにメッシュ分離 
	converter.SplitMeshesPerMaterial( lScene , true ); 

	//  アニメーション個数の取得
	lImporter->GetAnimStackCount( );

	//  アニメーション一覧の取得
	FbxArray< FbxString* > animationNames;
	lScene->FillAnimStackNameArray( animationNames );

	//  アニメーション情報の取得
	FbxTakeInfo* pTakeInfo = lScene->GetTakeInfo( animationNames[ 0 ]->Buffer( ) );

	//  アニメーションの開始・終了時間の取得
	m_startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
	m_endTime = pTakeInfo->mLocalTimeSpan.GetStop( );

	D3DXVECTOR3 translation;
	translation.x = static_cast<float>(lScene->GetRootNode( )->LclTranslation.Get()[0]);         
	translation.y = static_cast<float>(lScene->GetRootNode( )->LclTranslation.Get()[1]);         
	translation.z = static_cast<float>(lScene->GetRootNode( )->LclTranslation.Get()[2]);  
	int cnt = lScene->GetGeometryCount();

	//  スキニングされてない場合、親ノードへ強制的にスキニングする
	{
		for (int i = 0; i < cnt; ++i) 
		{
			FbxGeometry * geometry = lScene->GetGeometry( i );

			FbxMesh *mesh = ( FbxMesh* )geometry;
			int indexCount = ( mesh->GetPolygonCount( ) * 3 );

			if( mesh->GetDeformerCount( FbxDeformer::eSkin ) ==0 )
			{
				std::function< FbxNode*( FbxNode* node , FbxGeometry* geo ) > findGeometry = [ &findGeometry ]( FbxNode* node , FbxGeometry* geo )->FbxNode*
				{
					if( node->GetGeometry() == geo )
					{
						return node;
					}

					for (int i = 0; i < node->GetChildCount( ); ++i )
					{
						auto n = findGeometry( node->GetChild( i ), geo );

						if ( n )
						{
							return n;
						}
					}

					return nullptr;
				};

				FbxNode* nChild = findGeometry( lScene->GetRootNode( ) , geometry );
				FbxNode* n = nChild->GetParent( );
				n->RemoveChild( nChild );
				lScene->GetRootNode( )->AddChild( nChild );
				FbxAnimEvaluator* mySceneEvaluator = lScene->GetAnimationEvaluator( );
				auto mBase = mySceneEvaluator->GetNodeGlobalTransform( n );

				FbxCluster *lClusterToRoot = FbxCluster::Create( lScene, "" );
				lClusterToRoot->SetLink( n );
				lClusterToRoot->SetTransformLinkMatrix( mBase );
				lClusterToRoot->SetLinkMode( FbxCluster::eTotalOne );

				for( int j = 0; j < mesh->GetControlPointsCount( ); ++j )
				{
					lClusterToRoot->AddControlPointIndex( j , 1.0f );
				}

				FbxSkin* lSkin = FbxSkin::Create( lScene, "" );
				lSkin->AddCluster( lClusterToRoot );
				mesh->AddDeformer( lSkin );
			}
		}
	}

	//  メッシュ数の取得
	int numberMesh = lScene->GetMemberCount< FbxMesh >( );

	//  マテリアル数の取得
	int numberMaterial = lScene->GetMaterialCount( );

	//  メッシュの数分のループ
	for( int countMesh = 0; countMesh < numberMesh; ++countMesh )
	{
		m_meshes.push_back( Mesh( ) );

		//  指定メッシュの情報の取得
		FbxMesh* mesh = lScene->GetSrcObject< FbxMesh >( countMesh );

		//  座標の取得
		AnalyzePosition( mesh );

		//  法線解析                 
		AnalyzeNormal( mesh );                 
				
		//  UV解析                 
		AnalyzeTexcoord( mesh , true ); 

		//  マテリアル解析（参照情報の取得）                 
		//AnalyzeMaterial( mesh ); 

		//  ボーン解析
		AnalyzeCluster( mesh , countMesh );

		//  インデックス
		FbxLayerElementArrayTemplate< int >* index;

		//  
		if( mesh->GetMaterialIndices( &index ) && index->GetCount( ) > 0 )
		{
			// マテリアル情報の取得     
			FbxSurfaceMaterial* material = mesh->GetNode( )->GetMaterial( index->GetAt( 0 ) ); 

			std::string work = material->GetName( );

			//  マテリアル名の保存
			m_meshes.back( ).materialName = material->GetName( ); 
		}
	}

	//  マテリアルの数分のループ
	for( int countMaterial = 0; countMaterial < numberMaterial; ++countMaterial )
	{
		// マテリアル情報の取得     
		FbxSurfaceMaterial* pMaterial = lScene->GetMaterial( countMaterial ); 

		//  マテリアル名の取得
		std::string materialName = pMaterial->GetName( );

		// ディフューズ情報の取得     
		FbxProperty diffuseProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse ); 
 
		// プロパティが持っているレイヤードテクスチャの枚数をチェック     
		int layeredTextureCount = diffuseProperty.GetSrcObjectCount< FbxLayeredTexture >( ); 
 
		// レイヤードテクスチャが無ければ通常テクスチャ     
		if ( layeredTextureCount == 0 ) 
		{ 
			// 通常テクスチャの枚数をチェック         
			int textureCount = diffuseProperty.GetSrcObjectCount< FbxFileTexture >( ); 
   
			// 各テクスチャについてテクスチャ情報をゲット         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i番目のテクスチャオブジェクト取得         
				FbxFileTexture* pTexture = diffuseProperty.GetSrcObject< FbxFileTexture >( 0 ); 

				// テクスチャファイル名の取得         
				//std::string fileName = pTexture->GetFileName( );         
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet名の取得         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 
 
				// 辞書登録         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = "data/UnityChan/" + relFileName;        

				//  テクスチャクラスの取得
				Texture* pTexture2 = SceneManager::GetTexture( );

				// テクスチャの読み込みと管理     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// テクスチャの読み込みと管理         
				m_textures.push_back( strPathName );    

				//  テクスチャマップにマテリアル名とテクスチャ名の代入
				m_texturesMap.emplace( materialName , strPathName );
			}         
		}     
		else 
		{ 
			// レイヤードテクスチャあり         
			MessageBox( NULL , "レイヤードテクスチャ" , "マテリアルの取得" , MB_OK );     
		}     
	}

	//  アニメーション情報取得
	Animator* animator = AnalyzeAnimation( lImporter , lScene );

	//  メッシュの生成
	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		m_pVtxBuff = nullptr;
		m_pVtxBuffers.push_back( m_pVtxBuff );

		//  骨あり( つまりワンスキンなど )
		MakeVertex( itm->positionIndices.size( ) );
	}

	//  テクスチャクラスの取得
	Texture* texture = SceneManager::GetTexture( );
	texture->SetTextureImage( TEXTURE_FILENAME_BODY );
	texture->SetTextureImage( TEXTURE_FILENAME_EYEBASE );
	texture->SetTextureImage( TEXTURE_FILENAME_EYELINE );
	texture->SetTextureImage( TEXTURE_FILENAME_FACE );
	texture->SetTextureImage( TEXTURE_FILENAME_HAIR );
	texture->SetTextureImage( TEXTURE_FILENAME_LEFT_EYE );
	texture->SetTextureImage( TEXTURE_FILENAME_RIGHT_EYE );
	texture->SetTextureImage( TEXTURE_FILENAME_SKIN );

	//  トゥーンシェーダー用のテクスチャ取得
	texture->SetTextureImage( THUN_SHADER_TEXTURE_NAME );

	lImporter->Destroy( ); 
	lScene->Destroy( ); 
	lSdkManager->Destroy( ); 

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

	D3DXVECTOR3 workPosEye = m_position;

	workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y;
	workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;

	//  カメラの視点設定
	pCamera->SetCameraPosEyeDirect( workPosEye );

	D3DXVECTOR3 workPosAt = m_position;
	workPosAt.z += 5.0f;

	//  カメラの注視点設定
	pCamera->SetCameraPosAtDirect( workPosAt );

	//  プレイヤー注視点の設定
	m_positionAt = m_position + cameraVecDirect * 5.0f;

	//  方向ベクトルの代入
	D3DXVec3Normalize( &m_vectorDirect , &( m_positionAt - m_position ) );

	//  FBXモデル読み込み実験用プログラム
	lSdkManager = FbxManager::Create( ); 
 
	// Create the IO settings object. 
	ios = FbxIOSettings::Create( lSdkManager , IOSROOT ); lSdkManager->SetIOSettings( ios ); 
 
	// Create an importer using the SDK manager. 
	lImporter = FbxImporter::Create( lSdkManager ,"" ); 

	// Use the first argument as the filename for the importer. 
	if( !lImporter->Initialize( "data/UnityChan/Animations/unitychan_ARpose1.fbx", -1, lSdkManager->GetIOSettings( ) ) ) 
	{      
		char buf[ 256 ];     
		sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
		MessageBox( NULL , buf , "error" , MB_OK );     
		return E_FAIL; 
	}  
 
	// Create a new scene so that it can be populated by the imported file. 
	lScene = FbxScene::Create( lSdkManager , "myScene" ); 
 
	// Import the contents of the file into the scene. 
	lImporter->Import( lScene ); 

	//  メッシュがないアニメーションのデータ取得
	AnalyzeNoMeshAnimation( lImporter , lScene , animator );

	//  アニメーションデータの保存
	m_animator = animator;

	//  ボーンのインデックス番号の割り当て
	MatchBoneReferenceIndex( );

	lImporter->Destroy( ); 
	lScene->Destroy( ); 
	lSdkManager->Destroy( ); 

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------------
void UnityFBX::Uninit( void )
{
	Scene::Release( );
}

//--------------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------------
void UnityFBX::Update( void )
{
	m_currentFrame += 1;

	m_animator->motion[ 0 ].frame[ m_currentFrame ].matrix;

	////  カメラの取得
	//Camera* pCamera = SceneManager::GetCamera( );

	//// キーボード情報の取得
	//Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	////  移動量の初期化
	//D3DXVECTOR3 move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//if( pKeyboard != nullptr )
	//{
	//	if( pKeyboard->GetKeyboardPress( DIK_W ) )
	//	{
	//		move.z += 1.0f;
	//	}
	//	else if( pKeyboard->GetKeyboardPress( DIK_S ) )
	//	{
	//		move.z -= 1.0f;
	//	}

	//	if( pKeyboard->GetKeyboardPress( DIK_A ) )
	//	{
	//		move.x -= 1.0f;
	//	}
	//	else if( pKeyboard->GetKeyboardPress( DIK_D ) )
	//	{
	//		move.x += 1.0f;
	//	}
	//}

	//if( move.x != 0 ||
	//	move.z != 0 )
	//{
	//	if( pCamera != NULL )
	//	{
	//		D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

	//		float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

	//		D3DXMATRIX mtxRot;
	//		D3DXMatrixIdentity( &mtxRot );
	//		D3DXMatrixRotationY( &mtxRot , fAngle );

	//		D3DXVec3Normalize( &move , &move );

	//		D3DXVec3TransformNormal( &m_vectorDirect , &move , &mtxRot );
	//		D3DXVec3Normalize( &m_vectorDirect , &m_vectorDirect );
	//	}
	//	else
	//	{
	//		D3DXVec3Normalize( &m_vectorDirect , &move );
	//	}

	//	//  座標の更新( 進行方向 × 速度 分の移動 )
	//	m_position += m_vectorDirect * PLAYER_SPEED;
	//}

	//D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );
	//cameraVecDirect.y = 0.0f;
	//D3DXVec3Normalize( &cameraVecDirect , &cameraVecDirect );

	////  注視点の代入
	//m_positionAt = m_position + m_vectorDirect * 5.0f;

	//D3DXVECTOR3 workPos;
	//D3DXVECTOR3 workVec = pCamera->GetCameraVecDirect( );
	//workVec.y = 0.0f;

	//D3DXVec3Normalize( &workVec , &workVec );

	//workPos = m_position - m_vectorDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
	//workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
	//
	////  カメラの視点設定
	//pCamera->SetCameraPosEye( workPos , 0.08f );

	////  カメラの注視点設定
	//pCamera->SetCameraPosAt( m_position , 0.08f );
}

//--------------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------------
void UnityFBX::Draw( void )
{  
	//  再帰的に描画
    recursiveDraw( ); 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に描画処理
//--------------------------------------------------------------------------------------
void UnityFBX::recursiveDraw( void )
{ 
	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  回転座標変換行列

	int countLoop = 0;

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  シェーダー情報の取得
	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );

	//  シェーダー情報の取得
	Shader3DRimLightTexture* shader3DRimLightTexture = ( Shader3DRimLightTexture* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_RIM_LIGHT_TEXTURE );

	//  シェーダー情報の取得
	Shader3DThun* shader3DThun = ( Shader3DThun* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_THUN );

	//  シェーダー情報の取得
	Shader3DOutline* shader3DOutline = ( Shader3DOutline* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_OUTLINE );

	//  サンプラーのインデックス情報取得
	UINT textureIndex = shader3DThun->GetSamplerTextureIndex( );
	UINT thunIndex = shader3DThun->GetSamplerThunIndex( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  テクスチャの設定
	pDevice->SetTexture( thunIndex , pTexture->GetTextureImage( THUN_SHADER_TEXTURE_NAME ) );

	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		if( itm->materialName == "face" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_FACE ) );
		}
		else if( itm->materialName == "eyebase" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_EYEBASE ) );
		}
		else if( itm->materialName == "eye_L1" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_LEFT_EYE ) );
		}
		else if( itm->materialName == "eye_R1" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_RIGHT_EYE ) );
		}
		else if( itm->materialName == "eyeline" )
		{
			//  ループカウント
			countLoop++;

			continue;

			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_EYELINE ) );
		}
		else if( itm->materialName == "hair" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_HAIR ) );
		}
		else if( itm->materialName == "body" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_BODY ) );
		}
		else if( itm->materialName == "skin1" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_SKIN ) );
		}
		else if( itm->materialName == "mat_cheek" )
		{
			//  ループカウント
			countLoop++;

			continue;

			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( TEXTURE_FILENAME_CHEEK ) );
		}
		else if( itm->materialName == "mouth_mat" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( SD_TEXTURE_FILENAME_ALL ) );
		}
		else if( itm->materialName == "skin_mat" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( SD_TEXTURE_FILENAME_FO_SKIN ) );
		}
		else if( itm->materialName == "hair_mat" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( SD_TEXTURE_FILENAME_ALL ) );
		}
		else if( itm->materialName == "def_mat" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( SD_TEXTURE_FILENAME_ALL ) );
		}
		else if( itm->materialName == "nor_mat" )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( SD_TEXTURE_FILENAME_ALL ) );
		}
		else
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , nullptr );
		}

		// 骨あり（つまりワンスキンなど） 
 
		// 頂点の座標変換             
		std::vector< D3DXVECTOR3 > positions;
		int size = itm->points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             
				
		for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
		{            
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < it->bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ 0 ].frame[ m_currentFrame % m_animator->motion[ 0 ].frame.size( ) ].matrix[ it->bornRefarences[ countBorn ].index ] * it->bornRefarences[ countBorn ].weight;
			}

			if( it->bornRefarences.size( ) == 0 )
			{
				D3DXMatrixIdentity( &mtx );
			}

			D3DXVECTOR3 pos = it->positions;

			D3DXVec3TransformCoord( &pos , &pos , &mtx );                
			positions.push_back( pos );             
		} 

		VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

		if( m_pVtxBuffers.at( countLoop ) != NULL )
		{
			//  頂点バッファをロックして、仮想アドレスを取得する
			m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
													( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
													0 );							//  ロックの種類

			for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
			{             
				//  頂点座標の設定( 3D座標 ・ 右回り )
				pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

				//  法線の指定
				pVtx[ i ].normal = itm->normals[ itm->normalIndices[ i ] ];

				//  頂点色の設定( 0 ～ 255 の整数値 )
				pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV座標の指定
				pVtx[ i ].texcoord = itm->texcoords[ itm->texcoordIndices[ i ] ];
			}

			//  頂点バッファのアンロック
			m_pVtxBuffers.at( countLoop )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  拡大行列の作成
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  拡大行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  回転行列の作成
		Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );

		//  平行移動行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

		//  平行移動行列の作成
		D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

		//  平行移動行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

		//  ライトの方向ベクトルを取得する
		D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( );
		D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );

		//  ローカル単位ベクトルに変更
		D3DXVECTOR3 lightDirectLocal;

		D3DXMATRIX worldInverseMatrix;
		D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
		D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
		D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

		D3DXMATRIX worldInverseTransposeMatrix;
		D3DXMatrixInverse( &worldInverseTransposeMatrix , NULL , &mtxWorld );
		D3DXMatrixTranspose( &worldInverseTransposeMatrix , &worldInverseTransposeMatrix );
		float specularPower = 10.0f;

		Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
		D3DXVECTOR3 cameraPositionEye = camera->GetCameraPosEye( );
		D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
		D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
		D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

		//  シェーダーに必要な情報の設定
		//shader3D->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor );

		////  シェーダーに必要な情報の設定
		//shader3DRimLightTexture->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix ,
		//										worldInverseTransposeMatrix , cameraPositionEye , lightDirectWorld , 
		//										specularPower , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

		//  シェーダーに必要な情報の設定
		shader3DThun->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal );

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource( 0 ,									//  パイプライン番号
								  m_pVtxBuffers.at( countLoop ) ,		//  頂点バッファのアドレス
								  0 ,									//  オフセット( byte )
								  sizeof( VERTEX_3D ) );				//  一個分の頂点データのサイズ( ストライド )

		// テクスチャの設定
		//pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_textures[ itm->materialIndex ].c_str( ) ) ); 

		//  シェーダー3Dの描画開始
		shader3DThun->DrawBegin( );
		//shader3DRimLightTexture->DrawBegin( );

		// ポリゴンの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
								0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
								itm->positionIndices.size( ) / 3 );		//  プリミティブ数

		//  シェーダー3Dの描画終了
		ShaderManager::DrawEnd( );

		//  ループカウント
		countLoop++;
	}

	//  ループ数の初期化
	countLoop = 0;

	//  レンダーステートの設定
    pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CW );				//  裏面描画

	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		// 頂点の座標変換             
		std::vector< D3DXVECTOR3 > positions;
		int size = itm->points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             
				
		for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
		{            
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < it->bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ 0 ].frame[ m_currentFrame % m_animator->motion[ 0 ].frame.size( ) ].matrix[ it->bornRefarences[ countBorn ].index ] * it->bornRefarences[ countBorn ].weight;
			}

			if( it->bornRefarences.size( ) == 0 )
			{
				D3DXMatrixIdentity( &mtx );
			}

			D3DXVECTOR3 pos = it->positions;

			D3DXVec3TransformCoord( &pos , &pos , &mtx );                
			positions.push_back( pos );             
		} 

		VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

		if( m_pVtxBuffers.at( countLoop ) != NULL )
		{
			//  頂点バッファをロックして、仮想アドレスを取得する
			m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
													( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
													0 );							//  ロックの種類

			for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
			{             
				//  頂点座標の設定( 3D座標 ・ 右回り )
				pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

				//  法線の指定
				pVtx[ i ].normal = itm->normals[ itm->normalIndices[ i ] ];

				//  頂点色の設定( 0 ～ 255 の整数値 )
				pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV座標の指定
				pVtx[ i ].texcoord = itm->texcoords[ itm->texcoordIndices[ i ] ];
			}

			//  頂点バッファのアンロック
			m_pVtxBuffers.at( countLoop )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  拡大行列の作成
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  拡大行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  回転行列の作成
		Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );

		//  平行移動行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

		//  平行移動行列の作成
		D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

		//  平行移動行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

		Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
		D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
		D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );

		//  シェーダーに必要な情報の設定
		shader3DOutline->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix );

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource( 0 ,								//  パイプライン番号
								  m_pVtxBuffers.at( countLoop ) ,	//  頂点バッファのアドレス
								  0 ,								//  オフセット( byte )
								  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

		//  シェーダー3Dの描画開始
		shader3DOutline->DrawBegin( );

		// ポリゴンの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
								0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
								itm->positionIndices.size( ) / 3 );		//  プリミティブ数

		//  シェーダー3Dの描画終了
		ShaderManager::DrawEnd( );

		//  ループカウント
		countLoop++;
	}

	//  レンダーステートの設定
    pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );			//  表面描画
}

//--------------------------------------------------------------------------------------
//  FBXのデプス値書き込み処理
//--------------------------------------------------------------------------------------
void UnityFBX::DrawDepth( void )
{
	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  回転座標変換行列

	int countLoop = 0;

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		// テクスチャの設定
		pDevice->SetTexture( 0 , nullptr );

		// 骨あり（つまりワンスキンなど） 
 
		// 頂点の座標変換             
		std::vector< D3DXVECTOR3 > positions;
		int size = itm->points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             
				
		for( auto it = itm->points.begin( ); it != itm->points.end( ); ++it ) 
		{            
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < it->bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ 0 ].frame[ m_currentFrame % m_animator->motion[ 0 ].frame.size( ) ].matrix[ it->bornRefarences[ countBorn ].index ] * it->bornRefarences[ countBorn ].weight;
			}

			if( it->bornRefarences.size( ) == 0 )
			{
				D3DXMatrixIdentity( &mtx );
			}

			D3DXVECTOR3 pos = it->positions;

			D3DXVec3TransformCoord( &pos , &pos , &mtx );                
			positions.push_back( pos );             
		} 

		VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

		if( m_pVtxBuffers.at( countLoop ) != NULL )
		{
			//  頂点バッファをロックして、仮想アドレスを取得する
			m_pVtxBuffers.at( countLoop )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
												 ( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
												 0 );							//  ロックの種類

			for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
			{             
				//  頂点座標の設定( 3D座標 ・ 右回り )
				pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

				//  法線の指定
				pVtx[ i ].normal = itm->normals[ itm->normalIndices[ i ] ];

				//  頂点色の設定( 0 ～ 255 の整数値 )
				pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV座標の指定
				pVtx[ i ].texcoord = itm->texcoords[ itm->texcoordIndices[ i ] ];
			}

			//  頂点バッファのアンロック
			m_pVtxBuffers.at( countLoop )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  拡大行列の作成
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  拡大行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  回転行列の作成
		Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );

		//  平行移動行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

		//  平行移動行列の作成
		D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

		//  平行移動行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

		//  シェーダー情報の取得
		Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

		//  カメラクラスの取得
		Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

		D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
		D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );
		float fFar = SceneManager::GetLight( )->GetFar( );

		shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix , fFar );

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource( 0 ,									//  パイプライン番号
								  m_pVtxBuffers.at( countLoop ) ,		//  頂点バッファのアドレス
								  0 ,									//  オフセット( byte )
								  sizeof( VERTEX_3D ) );				//  一個分の頂点データのサイズ( ストライド )

		//  シェーダー描画開始
		shader3DShadowMap->DrawBegin( );

		// ポリゴンの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
								0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
								itm->positionIndices.size( ) / 3 );		//  プリミティブ数
			
		//  描画終了
		ShaderManager::DrawEnd( );

		//  ループカウント
		countLoop++;
	}
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void UnityFBX::AnalyzePosition( FbxMesh* pMesh ) 
{     
	// コントロールポイント数の取得     
	int controlPointsCount = pMesh->GetControlPointsCount( ); 
 
    // 頂点座標用コンテナの領域予約     
	m_meshes.back( ).points.reserve( controlPointsCount ); 
 
    // 頂点データの取得     
	FbxVector4* pP = pMesh->GetControlPoints( );     
	
	for( int i = 0; i < pMesh->GetControlPointsCount( ); i++ ) 
	{         
		m_meshes.back( ).points.push_back( D3DXVECTOR3( static_cast< float >( pP[ i ][ 0 ] ) ,                          
													    static_cast< float >( pP[ i ][ 1 ] ) ,                          
													    static_cast< float >( pP[ i ][ 2 ] ) ) );     
	} 
 
    /* 頂点インデックスの取得 */ 
 
    // インデックス数を取得     
	int polygonVertexCount = pMesh->GetPolygonVertexCount( ); 
 
    // 頂点座標インデックス用コンテナの領域予約     
	m_meshes.back( ).positionIndices.reserve( polygonVertexCount );          
	
	// インデックスバッファの取得     
	for( int i = 0; i < polygonVertexCount; i++ )
	{         
		m_meshes.back( ).positionIndices.push_back( static_cast< unsigned short >( pMesh->GetPolygonVertices( )[ i ] ) );     
	} 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void UnityFBX::AnalyzeNormal( FbxMesh* pMesh ) 
{     
	// レイヤー数の取得     
	int layerCount = pMesh->GetLayerCount( ); 
 
    for(int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // 法線の取得         
		FbxLayerElementNormal* pElementNormal = pMesh->GetElementNormal( layer ); 
 
        if( !pElementNormal ) 
		{ 
			continue; 
		} 
 
        // 法線数の取得         
		int normalCount = pElementNormal->GetDirectArray( ).GetCount( ); 
 
        // 法線格納用コンテナの領域予約         
		m_meshes.back( ).normals.reserve( normalCount );
 
        // 法線データの取得         
		for( int i = 0; i < normalCount; i++ ) 
		{             
			m_meshes.back( ).normals.push_back( D3DXVECTOR3( static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 0 ] ) ,                 
														     static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 1 ] ) ,                 
														     static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 2 ] ) ) );         
		} 
 
        // マッピングモード・リファレンスモード取得         
		FbxLayerElement::EMappingMode mappingMode = pElementNormal->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementNormal->GetReferenceMode( ); 
 
        switch( mappingMode ) 
		{ 
			case FbxLayerElement::eNone:  
			{
				MessageBox( NULL , "Normal MappingMode = mappingMode" , "未実装" , MB_OK );      
				break; 
			}
			case FbxLayerElement::eByControlPoint: 
			{
				// 頂点バッファと同じインデックスを使用 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                 
					// 法線インデックス格納用コンテナの領域予約                 
					m_meshes.back( ).normalIndices.reserve( m_meshes.back( ).points.size( ) );                 
				
					// 頂点バッファと同じインデックスをコピー                 
					m_meshes.back( ).normalIndices.assign( m_meshes.back( ).positionIndices.begin( ) , m_meshes.back( ).positionIndices.end( ) );             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{                 
					MessageBox( NULL ,  "Normal ReferenceMode = eIndexToDirect or eIndex, MappingMode = eByControlPoint" ,  "未実装" , MB_OK );             
				}             
			
				break; 
			}
			case FbxLayerElement::eByPolygonVertex:
			{
				/* 法線独自のインデックスを使用 */ 
 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                                  
					// インデックス参照の必要なし => インデックスを作成 
 
					// 法線インデックス格納用コンテナの領域予約                 
					m_meshes.back( ).normalIndices.reserve( normalCount );                 
				
					// 法線インデックスの作成                 
					for( int i = 0; i < normalCount; i++ ) 
					{                     
						m_meshes.back( ).normalIndices.push_back( i );                 
					}             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{ 
					// 独自インデックスを所持 
 
					// インデックス数の取得                 
					int normalIndexCount = pElementNormal->GetIndexArray( ).GetCount( );                 
				
					// 法線インデックス格納用コンテナの領域予約                 
					m_meshes.back( ).normalIndices.reserve( normalIndexCount );                 
				
					// 法線インデックスの取得                 
					for( int i = 0; i < normalIndexCount; i++ ) 
					{                     
						m_meshes.back( ).normalIndices.push_back( pElementNormal->GetIndexArray( )[ i ] );                 
					}             
				}

				break; 
			}
			case FbxLayerElement::eByPolygon:       
			{
				MessageBox( NULL , "Normal MappingMode = eByPolygon", "未実装" , MB_OK );             
				break;       
			}
			case FbxLayerElement::eByEdge: 
			{
				MessageBox( NULL , "Normal MappingMode = eByEdge" , "未実装" , MB_OK );             
				break;         
			}
			case FbxLayerElement::eAllSame:  
			{
				MessageBox( NULL , "Normal MappingMode = eAllSame" , "未実装" , MB_OK );            
				break;   
			}
			default: 
			{
				MessageBox( NULL , "Normal ???" , "未実装" , MB_OK );             
				break;         
			}     
		}
	} 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void UnityFBX::AnalyzeTexcoord( FbxMesh* pMesh , bool bRevers ) 
{ 
	int layerCount = pMesh->GetLayerCount( ); 
 
	if( !layerCount ) 
	{ 
		MessageBox(NULL, "レイヤーを持っていないメッシュを確認", "Analyze Texcoord", MB_OK); 
		return; 
	} 
 
	for( int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // UVの取得         
		FbxLayerElementUV* pElementUV = pMesh->GetLayer( layer )->GetUVs( ); 
 
        if( !pElementUV ) 
		{             
			MessageBox( NULL , "...UVのないメッシュレイヤーを確認" , "Analyze Texcoord" , MB_OK );             
			continue;         
		} 
 
        // UVセット名を取得         
		// = pElementUV->GetName(); 
 
        // マッピングモード・リファレンスモード取得         
		FbxLayerElement::EMappingMode mappingMode = pElementUV->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementUV->GetReferenceMode( ); 
 
        if( mappingMode == FbxLayerElement::eByPolygonVertex ) 
		{             
			if( referenceMode == FbxLayerElement::eIndexToDirect || referenceMode == FbxLayerElement::eIndex ) 
			{                 
				int uvIndexCount = pElementUV->GetIndexArray( ).GetCount( );                
				m_meshes.back( ).texcoordIndices.reserve( uvIndexCount ); 
 
                for( int i = 0; i < uvIndexCount; i++ ) 
				{                     
					m_meshes.back( ).texcoordIndices.push_back( pElementUV->GetIndexArray( ).GetAt( i ) );                 
				} 
 
                int uvCount = pElementUV->GetDirectArray( ).GetCount( );      

				m_meshes.back().texcoords.reserve( uvCount ); 
 
                for( int i = 0; i < uvCount; i++ ) 
				{ 
                    m_meshes.back( ).texcoords.push_back( D3DXVECTOR2( static_cast< float >( pElementUV->GetDirectArray( ).GetAt( i )[ 0 ] ),                         
																	   static_cast< float >( bRevers ? 1 - pElementUV->GetDirectArray( ).GetAt( i )[ 1 ]  : pElementUV->GetDirectArray( ).GetAt( i )[ 1 ] ) ) );                 
				}             																   
			}             
			else 
			{                 
				MessageBox(NULL, "Texcoord::未対応のリファレンスモードを取得", "FbxLayerElement::eByPolygonVertex", MB_OK);                 
				break;             
			}         
		}         
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{             
			MessageBox( NULL , "...未対応マッピングモード[eByControlPoint]を取得した", "Analyze Texcoord" , MB_OK );         
		}         
		else if( mappingMode == FbxLayerElement::eByPolygon) 
		{             
			MessageBox( NULL , "...未対応マッピングモード[eByPolygon]を取得した", "Analyze Texcoord" , MB_OK );         
		}         
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{             
			MessageBox( NULL , "...未対応マッピングモード[eByEdge]を取得した", "Analyze Texcoord" , MB_OK );         
		}         
		else 
		{             
			MessageBox( NULL , "...知らないマッピングモードを取得した", "Analyze Texcoord" , MB_OK );         
		} 
 
        break; // とりあえず１個めだけ     
	} 
}
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void UnityFBX::AnalyzeMaterial( FbxMesh* pMesh ) 
{     
	int layerCount = pMesh->GetLayerCount( ); 
 
    for(int layer = 0; layer < layerCount; layer++) 
	{ 
		FbxLayerElementMaterial* pElementMaterial = pMesh->GetLayer( layer )->GetMaterials( );

		if( !pElementMaterial ) 
		{ 
			continue; 
		} 
 
		int materialIndexCount = pElementMaterial->GetIndexArray( ).GetCount( ); 
 
		if( materialIndexCount == 0 ) 
		{ 
			continue; 
		} 
 
		FbxLayerElement::EMappingMode mappingMode = pElementMaterial->GetMappingMode( );     
		FbxLayerElement::EReferenceMode referenceMode = pElementMaterial->GetReferenceMode( ); 
 
		if( mappingMode == FbxLayerElement::eAllSame ) 
		{         
			if( referenceMode == FbxLayerElement::eIndexToDirect ) 
			{        
				// メッシュ全部がこのマテリアルインデックス         
				m_meshes.back( ).materialIndex = pElementMaterial->GetIndexArray( ).GetAt( 0 );         
			}         
			else 
			{         
				MessageBox(NULL, "...未対応のリファレンスモードを取得した", "Material MappingMode = eAllSame", MB_OK);         
			}     
		}     
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{         
			MessageBox(NULL, "...未対応のマッピングモード[eByControlPoint]を取得した", "Material MappingMode", MB_OK);     
		}     
		else if( mappingMode == FbxLayerElement::eByPolygon ) 
		{         
			// マテリアル分割されているはずだから、一番はじめのだけでいい         
			m_meshes.back( ).materialIndex = pElementMaterial->GetIndexArray( ).GetAt( 0 );     
		}     
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{         
			MessageBox(NULL, "...未対応のマッピングモード[eByEdge]を取得した", "Material MappingMode", MB_OK);     
		}     
		else 
		{         
			MessageBox(NULL, "...未対応のマッピングモードを取得した", "Material MappingMode", MB_OK);     
		}     
	} 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
// Get the geometry offset to a node. It is never inherited by the children. 
FbxAMatrix UnityFBX::GetGeometry(FbxNode* pNode) 
{     
	const FbxVector4 lT = pNode->GetGeometricTranslation( FbxNode::eSourcePivot );     
	const FbxVector4 lR = pNode->GetGeometricRotation( FbxNode::eSourcePivot );     
	const FbxVector4 lS = pNode->GetGeometricScaling( FbxNode::eSourcePivot ); 
 
    return FbxAMatrix( lT , lR , lS ); 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void UnityFBX::AnalyzeCluster( FbxMesh* pMesh , int index ) 
{     
	D3DXMATRIX mtxIdentitiy;     
	D3DXMatrixIdentity( &mtxIdentitiy ); 

    // スキンの数を取得     
	int skinCount = pMesh->GetDeformerCount( FbxDeformer::eSkin ); 
 
    for( int skinNum = 0; skinNum < skinCount; skinNum++ ) 
	{ 
        // スキンを取得         
		FbxSkin* pSkin = ( FbxSkin* )pMesh->GetDeformer( skinNum , FbxDeformer::eSkin ); 
 
        // クラスターの数を取得         
		int clusterCount = pSkin->GetClusterCount( ); 

		//  クラスターの数分のメモリ確保
		m_meshes.at( index ).initInvMatrix.resize( clusterCount );
 
        for( int clusterNum = 0; clusterNum < clusterCount; clusterNum++ ) 
		{ 
			// クラスタを取得             
			FbxCluster* pCluster = pSkin->GetCluster( clusterNum ); 
 
			// このクラスタが影響を及ぼす頂点インデックスの個数を取得             
			int pointIndexCount = pCluster->GetControlPointIndicesCount( ); 

			int* pointIndexArray = pCluster->GetControlPointIndices( );             
			double* weightArray = pCluster->GetControlPointWeights( ); 

			std::string name = pCluster->GetLink( )->GetName( );
 
			for( int i = 0 ; i < pointIndexCount; i++ ) 
			{              
				m_meshes.at( index ).points[ pointIndexArray[ i ] ].bornRefarences.push_back( BornRefarence( clusterNum ,
																											 static_cast< float >( weightArray[ i ] ) ,
																											 name ) );     
			}  
		}		
	} 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
UnityFBX::Animator* UnityFBX::AnalyzeAnimation( FbxImporter* importer , FbxScene* scene ) 
{     
	//  アニメーション数の取得
	int numberAnimation = importer->GetAnimStackCount( );

	if( numberAnimation == 0 )
	{
		return nullptr;
	}

	//  クラスターの数の取得
	int numberCluster = scene->GetMemberCount< FbxCluster >( );

	std::unordered_map< std::string , FbxCluster* > mapCluster;

	for( int countCluster = 0; countCluster < numberCluster; ++countCluster )
	{
		FbxCluster* workCluster = scene->GetSrcObject< FbxCluster >( countCluster );

		std::string clusterName = workCluster->GetLink( )->GetName( );

		//  同一名のクラスターの検索
		if( mapCluster.find( clusterName ) == mapCluster.end( ) )
		{
			mapCluster.emplace( clusterName , workCluster );
		}
	}

	//  アニメーションの数分のデータ生成
	Animator* animator = new Animator( );
	animator->motion.reserve( numberAnimation );

	animator->cluster.resize( mapCluster.size( ) );

	int countCluster = 0;

	for( auto pair : mapCluster )
	{
		animator->cluster[ countCluster ].name = pair.first;

		FbxAMatrix meshGlobalInitPosition;
		FbxAMatrix bornGlobalInitPosition;

		pair.second->GetTransformMatrix( meshGlobalInitPosition );
		pair.second->GetTransformLinkMatrix( bornGlobalInitPosition );

		//  初期姿勢行列
		FbxAMatrix bindPoseInverse = bornGlobalInitPosition.Inverse( ) * meshGlobalInitPosition;

		for( int y = 0; y < 4; y++ ) 
		{                 
			for( int x = 0; x < 4; x++ ) 
			{ 
				animator->cluster[ countCluster ].initInvMatrix( x , y ) = ( float )bindPoseInverse.Get( x , y );                 
			}  
		} 	

		countCluster++;
	}

	//  モーション名の取得
	FbxArray< FbxString* > motionName;
	scene->FillAnimStackNameArray( motionName );

	//  1フレーム
	FbxTime oneFrameTime;
	oneFrameTime.SetTime( 0 , 0 , 0 , 1 , 0 , 0 , FbxTime::eFrames60 );

	for( int countAnimation = 0; countAnimation < numberAnimation; ++countAnimation )
	{	
		Motion motion;
		motion.name = motionName[ countAnimation ]->Buffer( );

		//  アニメーション情報の取得
		FbxTakeInfo* pTakeInfo = scene->GetTakeInfo( motionName[ countAnimation ]->Buffer( ) );

		//  アニメーションの開始・終了時間の取得
		FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
		FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );

		//  総合フレーム数の取得
		int allTime = ( int )( ( endTime - startTime ) / oneFrameTime ).Get( );

		//  フレーム数分のメモリ確保
		motion.frame.reserve( allTime );

		for( FbxTime currentTime = m_startTime; currentTime < m_endTime; currentTime += oneFrameTime )
		{
			Frame frame;
			frame.matrix.resize( mapCluster.size( ) );

			countCluster = 0;

			for( auto pair : mapCluster )
			{
				FbxAMatrix bornMatrix = pair.second->GetLink( )->EvaluateGlobalTransform( currentTime );

				D3DXMATRIX workMatrix;

				for( int y = 0; y < 4; y++ ) 
				{                 
					for( int x = 0; x < 4; x++ ) 
					{ 
						workMatrix( x , y ) = ( float )bornMatrix.Get( x , y );                 
					}  
				} 	


				frame.matrix[ countCluster ] = animator->cluster[ countCluster ].initInvMatrix * workMatrix;

				countCluster++;
			}

			motion.frame.push_back( frame );
		}

		//  メモリを合わせる
		motion.frame.shrink_to_fit( );

		animator->motion.push_back( motion );
	}

	//  メモリを合わせる
	animator->motion.shrink_to_fit( );

	return animator;
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void UnityFBX::AnalyzeNoMeshAnimation( FbxImporter* importer , FbxScene* scene , Animator* animator )
{
	//  アニメーション数の取得
	int numberAnimation = importer->GetAnimStackCount( );

	if( numberAnimation == 0 )
	{
		return;
	}

	std::list< FbxNode* > skeleton;

	//  ルートノード検索
	FbxNode* rootNode = scene->GetRootNode( );

	//  再帰的にスケルトン検索
	FindSkeleton( rootNode , skeleton );

	if( skeleton.size( ) != animator->cluster.size( ) )
	{
		return;
	}

	animator->motion.reserve( numberAnimation + animator->motion.size( ) );

	//  モーション名の取得
	FbxArray< FbxString* > motionName;
	scene->FillAnimStackNameArray( motionName );

	//  1フレーム
	FbxTime oneFrameTime;
	oneFrameTime.SetTime( 0 , 0 , 0 , 1 , 0 , 0 , FbxTime::eFrames60 );

	int countCluster = 0;

	for( int countAnimation = 0; countAnimation < numberAnimation; ++countAnimation )
	{	
		Motion motion;
		motion.name = motionName[ countAnimation ]->Buffer( );

		//  アニメーション情報の取得
		FbxTakeInfo* pTakeInfo = scene->GetTakeInfo( motionName[ countAnimation ]->Buffer( ) );

		//  アニメーションの開始・終了時間の取得
		FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
		FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );

		//  総合フレーム数の取得
		int allTime = ( int )( ( endTime - startTime ) / oneFrameTime ).Get( );

		//  フレーム数分のメモリ確保
		motion.frame.reserve( allTime );

		for( FbxTime currentTime = m_startTime; currentTime < m_endTime; currentTime += oneFrameTime )
		{
			Frame frame;
			frame.matrix.resize( skeleton.size( ) );

			for( auto single : skeleton )
			{
				//  スケルトン名の取得
				std::string name = single->GetName( );

				int clusterNo;

				for( unsigned int countCluster = 0; countCluster < animator->cluster.size( ); ++countCluster )
				{
					if( name == animator->cluster[ countCluster ].name )
					{
						clusterNo = countCluster;

						break;
					}
				}

				FbxAMatrix bornMatrix = single->EvaluateGlobalTransform( currentTime );

				for( int y = 0; y < 4; y++ ) 
				{                 
					for( int x = 0; x < 4; x++ ) 
					{ 
						frame.matrix[ clusterNo ]( x , y ) = ( float )bornMatrix.Get( x , y );                 
					}  
				} 	

				frame.matrix[ clusterNo ] = animator->cluster[ clusterNo ].initInvMatrix * frame.matrix[ clusterNo ];
			}

			motion.frame.push_back( frame );
		}

		//  メモリを合わせる
		motion.frame.shrink_to_fit( );

		animator->motion.push_back( motion );
	}

	//  メモリを合わせる
	animator->motion.shrink_to_fit( );
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void UnityFBX::FindSkeleton( FbxNode* node , std::list< FbxNode* > &skeletonList )
{
	if( node == nullptr )
	{
		return;
	}

	for( int countNode = 0; countNode < node->GetNodeAttributeCount( ); ++countNode )
	{
		if( node->GetNodeAttributeByIndex( countNode )->GetAttributeType( ) == FbxNodeAttribute::eSkeleton )
		{
			skeletonList.push_back( node );
		}
	}

	for( int countChild = 0; countChild < node->GetChildCount( ); ++countChild )
	{
		FindSkeleton( node->GetChild( countChild ) , skeletonList );
	}
}

//--------------------------------------------------------------------------------------
//  ボーンのインデックス番号の割り当て
//--------------------------------------------------------------------------------------
void UnityFBX::MatchBoneReferenceIndex( void )
{
	for( unsigned int countMesh = 0; countMesh < m_meshes.size( ); ++countMesh )
	{
		for( unsigned int countPoint = 0; countPoint < m_meshes[ countMesh ].points.size( ); ++countPoint )
		{
			for( unsigned int countBone = 0; countBone < m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ); ++countBone )
			{
				for( unsigned int countCluster = 0; countCluster < m_animator->cluster.size( ); ++countCluster )
				{
					if( m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBone ].name == 
						m_animator->cluster[ countCluster ].name )
					{
						m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBone ].index = countCluster;

						break;
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列の数をカウントする関数
//--------------------------------------------------------------------------------------
std::string UnityFBX::GetAttributeTypeName(FbxNodeAttribute::EType type) 
{ 
    switch(type) { 
        case FbxNodeAttribute::eUnknown: return "unidentified"; 
        case FbxNodeAttribute::eNull: return "null"; 
        case FbxNodeAttribute::eMarker: return "marker"; 
        case FbxNodeAttribute::eSkeleton: return "skeleton"; 
        case FbxNodeAttribute::eMesh: return "mesh"; 
        case FbxNodeAttribute::eNurbs: return "nurbs"; 
        case FbxNodeAttribute::ePatch: return "patch"; 
        case FbxNodeAttribute::eCamera: return "camera"; 
        case FbxNodeAttribute::eCameraStereo: return "stereo"; 
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
        case FbxNodeAttribute::eLight: return "light"; 
        case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
        case FbxNodeAttribute::eOpticalMarker: return "marker"; 
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
        case FbxNodeAttribute::eBoundary: return "boundary"; 
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
        case FbxNodeAttribute::eShape: return "shape"; 
        case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
        case FbxNodeAttribute::eSubDiv: return "subdiv"; 
        default: return "unknown"; 
    } 
}

//--------------------------------------------------------------------------------------
//  頂点バッファを作成する関数
//--------------------------------------------------------------------------------------
void UnityFBX::MakeVertex( int size )
{     
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * size ,				//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,				//  使用方法
											 0 ,								//  
											 D3DPOOL_MANAGED ,					//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuffers.back( ) ,			//  バッファ
											 NULL ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return;
	}
}

//--------------------------------------------------------------------------------------
//  生成する関数
//--------------------------------------------------------------------------------------
UnityFBX* UnityFBX::Create( D3DXVECTOR3 position , float scale )
{
	UnityFBX* unityFBX = new UnityFBX;

	//  座標代入
	unityFBX->m_position = position;

	//  大きさ倍率代入
	unityFBX->m_scale = scale;

	//  初期化
	unityFBX->Init( );

	return unityFBX;
}