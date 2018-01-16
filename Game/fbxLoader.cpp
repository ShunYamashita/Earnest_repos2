//--------------------------------------------------------------------------------------
//  FBXLoader  ( fbxLoader.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "fbxLoader.h"
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
static const char	THUN_SHADER_TEXTURE_NAME[ 256 ]		= "data/SHADER/thun001.png";
static const char	NONE_SPECULAR_TEXTURE_NAME[ 256 ]	= "data\\TEXTURE\\fade.png";

static const char	TEXTURE_FILEPATH[ 256 ]				= "data\\FBX\\texture";

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
FBXLoader::FBXLoader( )
{
	m_currentFrame = 0;
	m_allTime = 0;
	m_scale = 1.0f;
	m_translation = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_motionIndex = 0;
	m_motionFinish = false;
	m_fixedPose = true;
}

//--------------------------------------------------------------------------------------
//  デスストラクタ
//--------------------------------------------------------------------------------------
FBXLoader::~FBXLoader( )
{

}

//--------------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------------
HRESULT FBXLoader::Init( void )
{
	//  FBXモデル読み込み実験用プログラム
	FbxManager* lSdkManager = FbxManager::Create( ); 
 
	// Create the IO settings object. 
	FbxIOSettings *ios = FbxIOSettings::Create( lSdkManager , IOSROOT ); lSdkManager->SetIOSettings( ios ); 
 
	// Create an importer using the SDK manager. 
	FbxImporter* lImporter = FbxImporter::Create( lSdkManager ,"" ); 
 
	if( m_type == FBXLoader::TYPE::WINKS )
	{
		// Use the first argument as the filename for the importer. 
		if( !lImporter->Initialize( "data/FBX/winks/winks.fbx" , -1, lSdkManager->GetIOSettings( ) ) ) 
		{      
			char buf[ 256 ];     
			sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
			MessageBox( NULL , buf , "error" , MB_OK );     
			return E_FAIL; 
		}  
	}
	else if( m_type == FBXLoader::TYPE::RAUDER )
	{
		// Use the first argument as the filename for the importer. 
		if( !lImporter->Initialize( "data/FBX/rauder/rauder.fbx", -1, lSdkManager->GetIOSettings( ) ) ) 
		{      
			char buf[ 256 ];     
			sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
			MessageBox( NULL , buf , "error" , MB_OK );     
			return E_FAIL; 
		}  
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
		AnalyzeMaterial( mesh ); 

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
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet名の取得         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 

				int layer = relFileName.rfind( "\\" );
				relFileName.erase( 0 , layer );
 
				// 辞書登録         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = TEXTURE_FILEPATH + relFileName;    

				//  テクスチャクラスの取得
				Texture* pTexture2 = SceneManager::GetTexture( );

				// テクスチャの読み込みと管理     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// テクスチャの読み込みと管理         
				m_textures.push_back( strPathName ); 

				//  テクスチャマップにマテリアル名とテクスチャ名の代入
				m_diffuseMap.emplace( materialName , strPathName );
			}         
		}     
		else 
		{ 
			// レイヤードテクスチャあり         
			MessageBox( NULL , "レイヤードテクスチャ" , "マテリアルの取得" , MB_OK );     
		}    

		// ディフューズ情報の取得     
		FbxProperty specularProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sSpecular ); 

		// プロパティが持っているレイヤードテクスチャの枚数をチェック     
		int layeredSpecularCount = specularProperty.GetSrcObjectCount< FbxLayeredTexture >( ); 
 
		// レイヤードテクスチャが無ければ通常テクスチャ     
		if ( layeredSpecularCount == 0 ) 
		{ 
			// 通常テクスチャの枚数をチェック         
			int textureCount = specularProperty.GetSrcObjectCount< FbxFileTexture >( ); 
   
			// 各テクスチャについてテクスチャ情報をゲット         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i番目のテクスチャオブジェクト取得         
				FbxFileTexture* pTexture = specularProperty.GetSrcObject< FbxFileTexture >( 0 ); 

				// テクスチャファイル名の取得       
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet名の取得         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 

				int layer = relFileName.rfind( "\\" );
				relFileName.erase( 0 , layer );
 
				// 辞書登録         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = TEXTURE_FILEPATH + relFileName;    

				//  テクスチャクラスの取得
				Texture* pTexture2 = SceneManager::GetTexture( );

				// テクスチャの読み込みと管理     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// テクスチャの読み込みと管理         
				m_textures.push_back( strPathName ); 

				//  テクスチャマップにマテリアル名とテクスチャ名の代入
				m_specularMap.emplace( materialName , strPathName );
			}         
		}     
		else 
		{ 
			// レイヤードテクスチャあり         
			MessageBox( NULL , "レイヤードテクスチャ" , "マテリアルの取得" , MB_OK );     
		}   

		// ディフューズ情報の取得     
		FbxProperty normalProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sNormalMap ); 

		// プロパティが持っているレイヤードテクスチャの枚数をチェック     
		int layeredNormalCount = normalProperty.GetSrcObjectCount< FbxLayeredTexture >( ); 
 
		// レイヤードテクスチャが無ければ通常テクスチャ     
		if ( layeredNormalCount == 0 ) 
		{ 
			// 通常テクスチャの枚数をチェック         
			int textureCount = normalProperty.GetSrcObjectCount< FbxFileTexture >( ); 
   
			// 各テクスチャについてテクスチャ情報をゲット         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i番目のテクスチャオブジェクト取得         
				FbxFileTexture* pTexture = normalProperty.GetSrcObject< FbxFileTexture >( 0 ); 

				// テクスチャファイル名の取得              
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet名の取得         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 

				int layer = relFileName.rfind( "\\" );
				relFileName.erase( 0 , layer );
 
				// 辞書登録         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = TEXTURE_FILEPATH + relFileName;    

				//  テクスチャクラスの取得
				Texture* pTexture2 = SceneManager::GetTexture( );

				// テクスチャの読み込みと管理     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// テクスチャの読み込みと管理         
				m_textures.push_back( strPathName ); 

				//  テクスチャマップにマテリアル名とテクスチャ名の代入
				m_normalMap.emplace( materialName , strPathName );
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
	texture->SetTextureImage( NONE_SPECULAR_TEXTURE_NAME );

	//  トゥーンシェーダー用のテクスチャ取得
	texture->SetTextureImage( THUN_SHADER_TEXTURE_NAME );

	char winksFileName[ ( int )MOTION::MAX - 1 ][ 256 ] = {
																"data/FBX/winks/Animations/idle.fbx" ,
																"data/FBX/winks/Animations/run.fbx" ,
																"data/FBX/winks/Animations/jump.fbx" ,
																"data/FBX/winks/Animations/landing.fbx" ,
																"data/FBX/winks/Animations/attack01.fbx" ,
																"data/FBX/winks/Animations/attack02.fbx" ,
																"data/FBX/winks/Animations/attack03.fbx" ,
																"data/FBX/winks/Animations/hp_attack.fbx" ,
																"data/FBX/winks/Animations/dash_start.fbx" ,
																"data/FBX/winks/Animations/dash_end.fbx" ,
																"data/FBX/winks/Animations/ground_guard_start.fbx" ,
																"data/FBX/winks/Animations/ground_guard_end.fbx" ,
																"data/FBX/winks/Animations/air_guard_start.fbx" ,
																"data/FBX/winks/Animations/air_guard_end.fbx" ,
														  };

	bool winksMotionLoop[ ( int )MOTION::MAX - 1 ] = {
														true ,
														true ,
														false ,
														false ,
														false ,
														false ,
														false ,
														false ,
														false ,
														false , 
														false ,
														false ,
														false ,
														false ,
													 };

	char rauderFileName[ ( int )MOTION::MAX - 1  ][ 256 ] = {
																"data/FBX/rauder/Animations/idle.fbx" ,
																"data/FBX/rauder/Animations/run.fbx" ,
																"data/FBX/rauder/Animations/jump.fbx" ,
																"data/FBX/rauder/Animations/landing.fbx" ,
																"data/FBX/rauder/Animations/attack01.fbx" ,
																"data/FBX/rauder/Animations/attack02.fbx" ,
																"data/FBX/rauder/Animations/attack03.fbx" ,
																"data/FBX/rauder/Animations/ground_hp_attack.fbx" ,
																"data/FBX/rauder/Animations/step_front.fbx" ,
																"data/FBX/rauder/Animations/step_back.fbx" ,
																"data/FBX/rauder/Animations/ground_guard_start.fbx" ,
																"data/FBX/rauder/Animations/ground_guard_end.fbx" ,
																"data/FBX/rauder/Animations/air_guard_start.fbx" ,
																"data/FBX/rauder/Animations/air_guard_end.fbx" ,
															};

	bool rauderMotionLoop[ ( int )MOTION::MAX - 1 ] = {
															true ,
															true ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
													  };

	if( m_type == FBXLoader::TYPE::WINKS )
	{
		for( int countAnimation = 0; countAnimation < ( int )MOTION::MAX - 1; countAnimation++ )
		{
			// Use the first argument as the filename for the importer. 
			if( !lImporter->Initialize( winksFileName[ countAnimation ] , -1 , lSdkManager->GetIOSettings( ) ) ) 
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
			AnalyzeNoMeshAnimation( lImporter , lScene , animator , winksMotionLoop[ countAnimation ] );
		}
	}
	else if( m_type == FBXLoader::TYPE::RAUDER )
	{
		for( int countAnimation = 0; countAnimation < ( int )MOTION::MAX - 1; countAnimation++ )
		{
			// Use the first argument as the filename for the importer. 
			if( !lImporter->Initialize( rauderFileName[ countAnimation ] , -1 , lSdkManager->GetIOSettings( ) ) ) 
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
			AnalyzeNoMeshAnimation( lImporter , lScene , animator , winksMotionLoop[ countAnimation ] );
		}
	}

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
void FBXLoader::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------------
void FBXLoader::Update( void )
{
	if( m_animator->motion[ m_motionIndex ].loop )
	{
		m_currentFrame += 1;
	}
	else
	{
		if( m_currentFrame < m_animator->motion[ m_motionIndex ].frame.size( ) - 1 )
		{
			m_currentFrame += 1;
		}
		else
		{
			m_motionFinish = true;
		}
	}
}

//--------------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------------
void FBXLoader::Draw( void )
{  
	//  再帰的に描画
    recursiveDraw( ); 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に描画処理
//--------------------------------------------------------------------------------------
void FBXLoader::recursiveDraw( void )
{ 
	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  回転座標変換行列

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  シェーダー情報の取得
	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );

	//  シェーダー情報の取得
	Shader3DRimLightTexture* shader3DRimLightTexture = ( Shader3DRimLightTexture* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_RIM_LIGHT_TEXTURE );

	//  シェーダー情報の取得
	Shader3DPlayer* shader3DPlayer = ( Shader3DPlayer* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_PLAYER );

	//  シェーダー情報の取得
	Shader3DOutline* shader3DOutline = ( Shader3DOutline* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_OUTLINE );

	//  レンダーステートの設定
    pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CW );				//  裏面描画

	for( int countMesh = 0; countMesh != m_meshes.size( ); ++countMesh ) 
	{  
		if( m_meshes[ countMesh ].materialName == "hair" || 
			m_meshes[ countMesh ].materialName == "eye" || 
			m_meshes[ countMesh ].materialName == "eyelashes" )
		{
			continue;
		}

		// 頂点の座標変換             
		std::vector< D3DXVECTOR3 > positions;
		int size = m_meshes[ countMesh ].points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             
				
		for( unsigned int countPoint = 0; countPoint < m_meshes[ countMesh ].points.size( ); ++countPoint ) 
		{
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ m_motionIndex ].frame[ m_currentFrame % m_animator->motion[ m_motionIndex ].frame.size( ) ].matrix[ m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBorn ].index ] * m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBorn ].weight;
			}

			if( m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ) == 0 )
			{
				D3DXMatrixIdentity( &mtx );
			}

			D3DXVECTOR3 pos = m_meshes[ countMesh ].points[ countPoint ].positions;

			D3DXVec3TransformCoord( &pos , &pos , &mtx );                
			positions.push_back( pos );             
		} 

		VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

		if( m_pVtxBuffers.at( countMesh ) != NULL )
		{
			//  頂点バッファをロックして、仮想アドレスを取得する
			m_pVtxBuffers.at( countMesh )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
													( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
													0 );							//  ロックの種類

			//  反転用のループカウント
			int countLoop = 0;

			for( int i = m_meshes[ countMesh ].positionIndices.size( ) - 1; i >= 0; i-- ) 
			{             
				//  頂点座標の設定( 3D座標 ・ 右回り )
				pVtx[ countLoop ].position = positions[ m_meshes[ countMesh ].positionIndices[ i ] ];
				pVtx[ countLoop ].position.x = -pVtx[ countLoop ].position.x;

				//  法線の指定
				pVtx[ countLoop ].normal = m_meshes[ countMesh ].normals[ m_meshes[ countMesh ].normalIndices[ i ] ];

				//  頂点色の設定( 0 ～ 255 の整数値 )
				pVtx[ countLoop ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV座標の指定
				pVtx[ countLoop ].texcoord = m_meshes[ countMesh ].texcoords[ m_meshes[ countMesh ].texcoordIndices[ i ] ];

				countLoop++;
			}

			//  頂点バッファのアンロック
			m_pVtxBuffers.at( countMesh )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  拡大行列の作成
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  拡大行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  固定ポーズの場合
		if( m_fixedPose )
		{
			//  回転行列の作成
			Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}
		else
		{
			//  回転行列の作成
			Utility::GetLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}

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
								  m_pVtxBuffers.at( countMesh ) ,	//  頂点バッファのアドレス
								  0 ,								//  オフセット( byte )
								  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

		//  シェーダー3Dの描画開始
		shader3DOutline->DrawBegin( );

		// ポリゴンの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,										//  プリミティブの種類
								0 ,															//  オフセット( 何番目の頂点から描画するか選べる )
								m_meshes[ countMesh ].positionIndices.size( ) / 3 );		//  プリミティブ数

		//  シェーダー3Dの描画終了
		ShaderManager::DrawEnd( );
	}

	//  レンダーステートの設定
    pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );			//  表面描画

	//  サンプラーのインデックス情報取得
	UINT textureIndex = shader3DPlayer->GetSamplerTextureIndex( );
	UINT thunIndex = shader3DPlayer->GetSamplerThunIndex( );
	UINT normalIndex = shader3DPlayer->GetSamplerNormalIndex( );
	UINT specularIndex = shader3DPlayer->GetSamplerSpecularIndex( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  テクスチャの設定
	pDevice->SetTexture( thunIndex , pTexture->GetTextureImage( THUN_SHADER_TEXTURE_NAME ) );

	for( int countMesh = 0; countMesh != m_meshes.size( ); ++countMesh ) 
	{  
		// 骨あり（つまりワンスキンなど） 
 
		// 頂点の座標変換             
		std::vector< D3DXVECTOR3 > positions;
		int size = m_meshes[ countMesh ].points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             

		for( unsigned int countPoint = 0; countPoint < m_meshes[ countMesh ].points.size( ); ++countPoint ) 
		{
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ m_motionIndex ].frame[ m_currentFrame % m_animator->motion[ m_motionIndex ].frame.size( ) ].matrix[ m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBorn ].index ] * m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBorn ].weight;
			}

			if( m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ) == 0 )
			{
				D3DXMatrixIdentity( &mtx );
			}

			D3DXVECTOR3 pos = m_meshes[ countMesh ].points[ countPoint ].positions;

			D3DXVec3TransformCoord( &pos , &pos , &mtx );                
			positions.push_back( pos );             
		} 

		VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

		if( m_pVtxBuffers.at( countMesh ) != NULL )
		{
			//  頂点バッファをロックして、仮想アドレスを取得する
			m_pVtxBuffers.at( countMesh )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
												 ( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
												 0 );							//  ロックの種類

			//  反転用のループカウント
			int countLoop = 0;

			//for( int i = 0; i < m_meshes[ countMesh ].positionIndices.size( ); i++ ) 
			for( int i = m_meshes[ countMesh ].positionIndices.size( ) - 1; i >= 0; i-- ) 
			{             
				//  頂点座標の設定( 3D座標 ・ 右回り )
				pVtx[ countLoop ].position = positions[ m_meshes[ countMesh ].positionIndices[ i ] ];
				pVtx[ countLoop ].position.x = -pVtx[ countLoop ].position.x;

				//  法線の指定
				pVtx[ countLoop ].normal = m_meshes[ countMesh ].normals[ m_meshes[ countMesh ].normalIndices[ i ] ];

				//  頂点色の設定( 0 ～ 255 の整数値 )
				pVtx[ countLoop ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV座標の指定
				pVtx[ countLoop ].texcoord = m_meshes[ countMesh ].texcoords[ m_meshes[ countMesh ].texcoordIndices[ i ] ];

				countLoop++;
			}

			//  頂点バッファのアンロック
			m_pVtxBuffers.at( countMesh )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  拡大行列の作成
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  拡大行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  固定ポーズの場合
		if( m_fixedPose )
		{
			//  回転行列の作成
			Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}
		else
		{
			//  回転行列の作成
			Utility::GetLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}

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
		D3DXVECTOR3 cameraPositionEyeLocal = camera->GetCameraPosEye( );
		D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
		D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
		D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

		//  カメラのローカル座標の取得
		D3DXVec3TransformCoord( &cameraPositionEyeLocal , &cameraPositionEyeLocal , &worldInverseMatrix );

		//  シェーダーに必要な情報の設定
		shader3DPlayer->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor , cameraPositionEyeLocal );

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource( 0 ,									//  パイプライン番号
								  m_pVtxBuffers.at( countMesh ) ,		//  頂点バッファのアドレス
								  0 ,									//  オフセット( byte )
								  sizeof( VERTEX_3D ) );				//  一個分の頂点データのサイズ( ストライド )

		//  テクスチャが存在している場合
		if( m_diffuseMap.find( m_meshes[ countMesh ].materialName ) != m_diffuseMap.end( ) )
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( m_diffuseMap[ m_meshes[ countMesh ].materialName ].c_str( ) ) ); 
		}		
		else
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( NONE_SPECULAR_TEXTURE_NAME ) ); 
		}

		//  法線マップテクスチャが存在している場合
		if( m_normalMap.find( m_meshes[ countMesh ].materialName ) != m_normalMap.end( ) )
		{
			//  法線マップテクスチャの設定
			pDevice->SetTexture( normalIndex , pTexture->GetTextureImage( m_normalMap[m_meshes[ countMesh ].materialName ].c_str( ) ) ); 
		}		
		else
		{
			//  法線マップテクスチャの設定
			pDevice->SetTexture( normalIndex , nullptr ); 
		}

		//  スペキュラーマップテクスチャが存在している場合
		if( m_specularMap.find( m_meshes[ countMesh ].materialName ) != m_specularMap.end( ) )
		{
			//  スペキュラーマップテクスチャの設定
			pDevice->SetTexture( specularIndex , pTexture->GetTextureImage( m_specularMap[ m_meshes[ countMesh ].materialName ].c_str( ) ) ); 
		}		
		else
		{
			//  スペキュラーマップテクスチャの設定
			pDevice->SetTexture( specularIndex , pTexture->GetTextureImage( NONE_SPECULAR_TEXTURE_NAME ) ); 
		}

		//  αテストの有効
		pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );

		//  条件式
		pDevice->SetRenderState( D3DRS_ALPHAREF , 1 );

		//  演算式
		pDevice->SetRenderState( D3DRS_ALPHAFUNC , D3DCMP_GREATER );

		//  シェーダー3Dの描画開始
		shader3DPlayer->DrawBegin( );

		// ポリゴンの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,										//  プリミティブの種類
								0 ,															//  オフセット( 何番目の頂点から描画するか選べる )
								m_meshes[ countMesh ].positionIndices.size( ) / 3 );		//  プリミティブ数

		//  シェーダー3Dの描画終了
		ShaderManager::DrawEnd( );

		//  αテストの無効
		pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );

		pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );					//  αブレンド設定( 半透過処理 )
	}
}

//--------------------------------------------------------------------------------------
//  FBXのデプス値書き込み処理
//--------------------------------------------------------------------------------------
void FBXLoader::DrawDepth( void )
{
	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  回転座標変換行列

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	for( int countMesh = 0; countMesh != m_meshes.size( ); ++countMesh ) 
	{  
		// テクスチャの設定
		pDevice->SetTexture( 0 , nullptr );

		// 骨あり（つまりワンスキンなど） 
 
		// 頂点の座標変換             
		std::vector< D3DXVECTOR3 > positions;
		int size = m_meshes[ countMesh ].points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             
				
		for( auto it = m_meshes[ countMesh ].points.begin( ); it != m_meshes[ countMesh ].points.end( ); ++it ) 
		{            
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < it->bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ m_motionIndex ].frame[ m_currentFrame % m_animator->motion[ m_motionIndex ].frame.size( ) ].matrix[ it->bornRefarences[ countBorn ].index ] * it->bornRefarences[ countBorn ].weight;
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

		if( m_pVtxBuffers.at( countMesh ) != NULL )
		{
			//  頂点バッファをロックして、仮想アドレスを取得する
			m_pVtxBuffers.at( countMesh )->Lock( 0 , 0 ,						//  取る先頭と、サイズ( 0 , 0 で全部 )
												 ( void** )&pVtx ,				//  アドレスが書かれたメモ帳のアドレス
												 0 );							//  ロックの種類

			//  反転用のループカウント
			int countLoop = 0;

			for( int i = m_meshes[ countMesh ].positionIndices.size( ) - 1; i >= 0; i-- ) 
			{             
				//  頂点座標の設定( 3D座標 ・ 右回り )
				pVtx[ countLoop ].position = positions[ m_meshes[ countMesh ].positionIndices[ i ] ];
				pVtx[ countLoop ].position.x = -pVtx[ countLoop ].position.x;

				//  法線の指定
				pVtx[ countLoop ].normal = m_meshes[ countMesh ].normals[ m_meshes[ countMesh ].normalIndices[ i ] ];

				//  頂点色の設定( 0 ～ 255 の整数値 )
				pVtx[ countLoop ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV座標の指定
				pVtx[ countLoop ].texcoord = m_meshes[ countMesh ].texcoords[ m_meshes[ countMesh ].texcoordIndices[ i ] ];

				countLoop++;
			}

			//  頂点バッファのアンロック
			m_pVtxBuffers.at( countMesh )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  拡大行列の作成
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  拡大行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  固定ポーズの場合
		if( m_fixedPose )
		{
			//  回転行列の作成
			Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}
		else
		{
			//  回転行列の作成
			Utility::GetLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}

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
								  m_pVtxBuffers.at( countMesh ) ,		//  頂点バッファのアドレス
								  0 ,									//  オフセット( byte )
								  sizeof( VERTEX_3D ) );				//  一個分の頂点データのサイズ( ストライド )

		//  シェーダー描画開始
		shader3DShadowMap->DrawBegin( );

		// ポリゴンの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
								0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
								m_meshes[ countMesh ].positionIndices.size( ) / 3 );		//  プリミティブ数
			
		//  描画終了
		ShaderManager::DrawEnd( );
	}
}

//--------------------------------------------------------------------------------------
//  モーションの設定する関数
//--------------------------------------------------------------------------------------
void FBXLoader::SetMotion( MOTION motion )
{
	m_motionIndex = ( int )motion;

	m_motionFinish = false;

	m_currentFrame = 0;
}

//--------------------------------------------------------------------------------------
//  モーションフレーム取得
//--------------------------------------------------------------------------------------
int FBXLoader::GetMotionFrame( void )
{
	return m_currentFrame;
}

//--------------------------------------------------------------------------------------
//  モーション全体からフレーム割合取得
//--------------------------------------------------------------------------------------
float FBXLoader::GetMotionFrameRate( void )
{
	float rate = ( float )m_currentFrame / ( float )m_animator->motion[ m_motionIndex ].frame.size( );

	return rate;
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void FBXLoader::AnalyzePosition( FbxMesh* pMesh ) 
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
void FBXLoader::AnalyzeNormal( FbxMesh* pMesh ) 
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
void FBXLoader::AnalyzeTexcoord( FbxMesh* pMesh , bool bRevers ) 
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
void FBXLoader::AnalyzeMaterial( FbxMesh* pMesh ) 
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
FbxAMatrix FBXLoader::GetGeometry(FbxNode* pNode) 
{     
	const FbxVector4 lT = pNode->GetGeometricTranslation( FbxNode::eSourcePivot );     
	const FbxVector4 lR = pNode->GetGeometricRotation( FbxNode::eSourcePivot );     
	const FbxVector4 lS = pNode->GetGeometricScaling( FbxNode::eSourcePivot ); 
 
    return FbxAMatrix( lT , lR , lS ); 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void FBXLoader::AnalyzeCluster( FbxMesh* pMesh , int index ) 
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
FBXLoader::Animator* FBXLoader::AnalyzeAnimation( FbxImporter* importer , FbxScene* scene ) 
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

		for( FbxTime currentTime = startTime; currentTime < endTime; currentTime += oneFrameTime )
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
void FBXLoader::AnalyzeNoMeshAnimation( FbxImporter* importer , FbxScene* scene , Animator* animator , bool loop )
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

	if( skeleton.size( ) < animator->cluster.size( ) )
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

		//  ループ情報の代入
		motion.loop = loop;

		//  アニメーション情報の取得
		FbxTakeInfo* pTakeInfo = scene->GetTakeInfo( motionName[ countAnimation ]->Buffer( ) );

		//  アニメーションの開始・終了時間の取得
		FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
		FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );

		//  総合フレーム数の取得
		int allTime = ( int )( ( endTime - startTime ) / oneFrameTime ).Get( );

		//  フレーム数分のメモリ確保
		motion.frame.reserve( allTime );

		for( FbxTime currentTime = startTime; currentTime < endTime; currentTime += oneFrameTime )
		{
			Frame frame;
			frame.matrix.resize( skeleton.size( ) );

			for( auto single : skeleton )
			{
				//  スケルトン名の取得
				std::string name = single->GetName( );

				int clusterNo = -1;

				for( unsigned int countCluster = 0; countCluster < animator->cluster.size( ); ++countCluster )
				{
					if( name == animator->cluster[ countCluster ].name )
					{
						clusterNo = countCluster;

						break;
					}
				}

				if( clusterNo == -1 )
				{
					continue;
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
void FBXLoader::FindSkeleton( FbxNode* node , std::list< FbxNode* > &skeletonList )
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
void FBXLoader::MatchBoneReferenceIndex( void )
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
std::string FBXLoader::GetAttributeTypeName(FbxNodeAttribute::EType type) 
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
void FBXLoader::MakeVertex( int size )
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
FBXLoader* FBXLoader::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 positionAt , float scale )
{
	FBXLoader* fbxLoader = new FBXLoader;

	//  種類代入
	fbxLoader->m_type = type;

	//  座標代入
	fbxLoader->m_position = position;

	//  注視点代入
	fbxLoader->m_positionAt = positionAt;

	//  大きさ倍率代入
	fbxLoader->m_scale = scale;

	//  初期化
	fbxLoader->Init( );

	return fbxLoader;
}