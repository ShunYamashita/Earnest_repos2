//--------------------------------------------------------------------------------------
//  モデルレンダリング処理   ( sceneModel.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "sceneModel.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "keyboard.h"
#include "sceneBG.h"
#include "shaderManager.h"
#include "light.h"
#include "depthShadow.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define	MODEL_FILENAME		"data/MODEL/tree.x"
#define	MODEL_FILENAME2		"data/MODEL/castle.x"
#define	MODEL_FILENAME3		"data/MODEL/field2.x"
#define	MODEL_FILENAME4		"data/MODEL/hitField2.x"
#define	MODEL_FILENAME5		"data/MODEL/field.x"
#define	MODEL_FILENAME6		"data/MODEL/hitField.x"
#define	MODEL_FILENAME7		"data/MODEL/wall.x"

#define TEXTURE_FILEPATH	"data\\TEXTURE"						//  テクスチャへのファイルパス

#define MAX_MODEL_PARTS		( 20 )								//  マテリアル最大数

#define SHADOW_TEXTURENAME	"data/TEXTURE/fade.png"				//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  xモデルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneModel::SceneModel( )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_posAt = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_scale = D3DXVECTOR3( 1.0f, 1.0f , 1.0f );
	m_rot = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );

	D3DXQuaternionIdentity( &m_quaternion );
}

//--------------------------------------------------------------------------------------
//  xモデルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneModel::SceneModel( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_position = position;
	m_rot = rot;
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  xモデルのデストラクタ
//--------------------------------------------------------------------------------------
SceneModel::~SceneModel( )
{

}

//--------------------------------------------------------------------------------------
//  xモデルの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SceneModel::Init( void )
{
	D3DXMATERIAL*	pMat = NULL;									//  マテリアル
	char			aTextureFileName[ 128 ] = { };
	int				nIndex = 0;

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	LPD3DXMESH oldMesh = nullptr;

	if( m_type == TYPE_TREE )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME ,				//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &oldMesh ) ) )				//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == TYPE_CASTLE )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME2 ,			//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &oldMesh ) ) )				//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == TYPE_FIELD000 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME3 ,			//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &oldMesh ) ) )				//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == TYPE_HIT_FIELD000 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME4 ,			//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &oldMesh ) ) )				//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == TYPE_FIELD001 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME5 ,			//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &oldMesh ) ) )				//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == TYPE_HIT_FIELD001 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME6 ,			//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &oldMesh ) ) )				//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == TYPE_WALL000 )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME7 ,			//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &oldMesh ) ) )				//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}

	D3DVERTEXELEMENT9 elements[ ] = 
	{  
		//  引数( 1 : パイプライン番号 , 2 : オフセット , 3 : データサイズ( 型 ) , 4 : , 5 : 使用用途 , 6 : , ( 5～6はセマンティクス ) )
		//  ( 自分で考えたデータを送る場合はTEXCOORDで送るように )
		{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 } ,
		{ 0 , sizeof( float ) * 3 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL, 0 } ,
		{ 0 , sizeof( float ) * 6 , D3DDECLTYPE_FLOAT2 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 0 } ,		
		D3DDECL_END( )  //  終了
	};

	//  メッシュをシェーダーに適用
	HRESULT hr = oldMesh->CloneMesh( D3DXMESH_MANAGED | D3DXMESH_WRITEONLY , elements , pDevice , &m_pMeshModel );

	//  エラーチェック
	if( FAILED( hr ) )
	{
		MessageBox( NULL , "Xファイルの変換が上手くいきませんでした" , "CloneMeshでのエラー" , MB_OK );

		return E_FAIL;
	}

	//  マテリアル情報の先頭アドレスを取得
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  テクスチャポインタがある場合
		if( pMat[ nCntMaterial ].pTextureFilename != NULL )
		{
			//  テクスチャファイルパス用の文字列
			char aTextureFilePath[ 128 ] = { TEXTURE_FILEPATH };

			//  文字数分のループ
			for( int nCntLength = strlen( pMat[ nCntMaterial ].pTextureFilename ) - 1; nCntLength >= 0 ; nCntLength-- )
			{
				//  テクスチャ名の取り出し
				aTextureFileName[ nIndex ] = pMat[ nCntMaterial ].pTextureFilename[ nCntLength ];

				nIndex++;

				//  \があるとループを抜け出す
				if( pMat[ nCntMaterial ].pTextureFilename[ nCntLength ] == '\\' )
				{
					break;
				}
			}

			//  \0の格納
			aTextureFileName[ nIndex ] = '\0';

			//  インデックス番号の初期化
			nIndex = 0;

			char aWork;											//  １文字分の格納用
			int	 nLength = strlen( aTextureFileName );			//  文字の長さ

			//  文字数分のループ
			for( int nCntLength = 0; nCntLength < nLength / 2; nCntLength++ )
			{
				//  テクスチャ名の反転
				aWork = aTextureFileName[ nCntLength ];
				aTextureFileName[ nCntLength ] = aTextureFileName[ nLength - nCntLength - 1 ];
				aTextureFileName[ nLength - nCntLength - 1 ] = aWork;
			}

			//  テクスチャ名をテクスチャパス名に
			strcat( aTextureFilePath , aTextureFileName );

			//  テクスチャ名のコピー
			strcpy( m_aTextureName[ nCntMaterial ] , aTextureFilePath );

			//  テクスチャの登録
			pTexture->SetTextureImage( aTextureFilePath );
		}
	}

	//  テクスチャの登録
	pTexture->SetTextureImage( SHADOW_TEXTURENAME );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  xモデルの終了処理
//--------------------------------------------------------------------------------------
void SceneModel::Uninit( void )
{
	//  メッシュ情報の解放
	SAFE_RELEASE( m_pMeshModel );

	//  マテリアル情報の解放
	SAFE_RELEASE( m_pBuffMatModel );
}

//--------------------------------------------------------------------------------------
//  xモデルの更新処理
//--------------------------------------------------------------------------------------
void SceneModel::Update( void )
{
	D3DXQUATERNION quaternion;

	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	/*  ボール用 

	if( pKeyboard->GetKeyboardPress( DIK_W ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &m_quaternion , &quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_S ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( -1.0f , 0.0f , 0.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &m_quaternion , &quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_D ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 0.0f , 0.0f , -1.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &m_quaternion , &quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_A ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &m_quaternion , &quaternion );
	}

	*/

	/*  戦闘機用

	if( pKeyboard->GetKeyboardPress( DIK_W ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , -0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &quaternion , &m_quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_S ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &quaternion , &m_quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_D ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) , -0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &quaternion , &m_quaternion );
	}

	if( pKeyboard->GetKeyboardPress( DIK_A ) )
	{
		D3DXVec3Normalize( &m_vecAxis , &( m_posAt - m_position ) );

		D3DXQuaternionRotationAxis( &quaternion , &D3DXVECTOR3( 0.0f , 0.0f , 1.0f ) , 0.1f );

		D3DXQuaternionMultiply( &m_quaternion , &quaternion , &m_quaternion );
	}

	*/
}

//--------------------------------------------------------------------------------------
//  xモデルの描画処理
//--------------------------------------------------------------------------------------
void SceneModel::Draw( void )
{
	D3DXMATERIAL*	pMat = NULL;					//  マテリアル

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  スケール行列
	D3DXMATRIX mtxRot;								//  回転行列

	D3DXMATRIX mtxView;								//  ビュー座標変換行列
	D3DXMATRIX mtxProj;								//  プロジェクション座標行列

	LPDIRECT3DDEVICE9 pDevice;

	//  シェーダー情報の取得
	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );
	Shader3DNoTexture* shader3DNoTexture = ( Shader3DNoTexture* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_NO_TEXTURE );
	Shader3DRimLightTexture* shader3DRimLightTexture = ( Shader3DRimLightTexture* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_RIM_LIGHT_TEXTURE );

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale , m_scale.x , m_scale.y , m_scale.z );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	if( m_type == TYPE_TREE )
	{
		D3DXMatrixRotationQuaternion( &mtxRot , &m_quaternion );
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.y , m_rot.x , m_rot.z );
	}

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y + 1.0f , m_position.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  マテリアル情報の先頭アドレスを取得
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

	//  ライトの方向ベクトルを取得する
	D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( );
	D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );

	//  ローカル単位ベクトルに変更
	D3DXVECTOR3 lightDirectLocal;

	D3DXMATRIX worldInverseMatrix;
	D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
	D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
	D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

	Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
	D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
	D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
	D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

	//  シェーダー情報の取得
	Shader3DDepthShadow* shader3DDepthShadow = ( Shader3DDepthShadow* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_DEPTH_SHADOW );

	//  光源のビュープロジェクション行列の取得
	D3DXMATRIX lightViewProjectionMatrix = SceneManager::GetLight( )->GetViewMatrix( ) * SceneManager::GetLight( )->GetProjectionMatrix( );

	//  UVオフセット値の計算
	D3DXVECTOR4	tmpOffset;
	tmpOffset.x = 0.5f / DepthShadow::TEXTURE_WIDTH;
	tmpOffset.y = 0.5f / DepthShadow::TEXTURE_HEIGHT;
	tmpOffset.z = 0.0f;
	tmpOffset.w = 0.0f;

	//  バイアス値の取得
	float bias = DepthShadow::GetBias( );

	//  シェーダーに必要な情報の設定
	//shader3D->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor );

	UINT textureSampler = shader3DDepthShadow->GetSamplerTextureIndex( );
	UINT shadowSampler = shader3DDepthShadow->GetSamplerShadowIndex( );

	//if( m_type == SceneModel::TYPE_TREE || 
	//	m_type == SceneModel::TYPE_FIELD000 )
	//{
	//	//  シェーダー情報の取得
	//	Shader3DOutline* shader3DOutline = ( Shader3DOutline* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_OUTLINE );

	//	//  レンダーステートの設定
	//	pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CW );				//  裏面描画

	//	//  マテリアルの数分のループ
	//	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	//	{
	//		//  シェーダーに必要な情報の設定
	//		shader3DOutline->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix );

	//		//  シェーダー描画開始
	//		shader3DOutline->DrawBegin( );

	//		//  メッシュの描画
	//		m_pMeshModel->DrawSubset( nCntMaterial );

	//		//  シェーダー3Dの描画終了
	//		ShaderManager::DrawEnd( );
	//	}

	//	//  レンダーステートの設定
	//	pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );			//  表面描画
	//}

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		D3DXCOLOR color = D3DXCOLOR( pMat[ nCntMaterial ].MatD3D.Diffuse.r ,
									 pMat[ nCntMaterial ].MatD3D.Diffuse.g ,
									 pMat[ nCntMaterial ].MatD3D.Diffuse.b ,
									 pMat[ nCntMaterial ].MatD3D.Diffuse.a );

		if( pMat[ nCntMaterial ].pTextureFilename == NULL )
		{
			//  テクスチャの設定
			pDevice->SetTexture( 0 , nullptr );

			//  シェーダーに必要な情報の設定
			shader3DNoTexture->SetShaderInfo( mtxWorld ,  viewMatrix , projectionMatrix , lightDirectLocal , color );

			//  シェーダー3Dの描画開始
			shader3DNoTexture->DrawBegin( );
		}
		else
		{
			//  シャドウマップテクスチャの設定
			pDevice->SetTexture( shadowSampler , DepthShadow::GetRendereTargetTexture( ) );
			
			//  テクスチャの設定
			pDevice->SetTexture( textureSampler , pTexture->GetTextureImage( m_aTextureName[ nCntMaterial ] ) );

			//  シェーダーに必要な情報の設定
			shader3DDepthShadow->SetShaderInfo( mtxWorld , viewMatrix ,projectionMatrix ,
												lightDirectLocal , lightViewProjectionMatrix , tmpOffset , bias );
			//shader3D->SetShaderInfo( mtxWorld , viewMatrix ,projectionMatrix , lightDirectLocal , lightDiffuseColor );

			////  シェーダー3Dの描画開始
			//shader3D->DrawBegin( );
			shader3DDepthShadow->DrawBegin( );
		}

		//  メッシュの描画
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  シェーダー描画の終了
		ShaderManager::DrawEnd( );
	}
}

//--------------------------------------------------------------------------------------
//  xモデルのデプス値の書き込み処理
//--------------------------------------------------------------------------------------
void SceneModel::DrawDepth( void )
{
	D3DXMATERIAL*	pMat = NULL;					//  マテリアル

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  スケール行列
	D3DXMATRIX mtxRot;								//  回転行列

	D3DXMATRIX mtxView;								//  ビュー座標変換行列
	D3DXMATRIX mtxProj;								//  プロジェクション座標行列

	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale , m_scale.x , m_scale.y , m_scale.z );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	if( m_type == TYPE_TREE )
	{
		D3DXMatrixRotationQuaternion( &mtxRot , &m_quaternion );
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.y , m_rot.x , m_rot.z );
	}

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y + 1.0f , m_position.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  シェーダー情報の取得
	Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

	//  カメラクラスの取得
	Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

	D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
	D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );
	float fFar = SceneManager::GetLight( )->GetFar( );

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  テクスチャの設定
		pDevice->SetTexture( 0 , nullptr );

		//  シェーダーに必要な情報の設定
		shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix , fFar );

		//  シェーダー描画開始
		shader3DShadowMap->DrawBegin( );

		//  メッシュの描画
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  描画終了
		ShaderManager::DrawEnd( );
	}
}

//--------------------------------------------------------------------------------------
//  xモデルの大きさ設定をする関数
//--------------------------------------------------------------------------------------
void SceneModel::SetScale( D3DXVECTOR3 scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  xポリゴンの大きさを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 SceneModel::GetScale( void )
{
	return m_scale;
}

//--------------------------------------------------------------------------------------
//  xモデルの注視点設定をする関数
//--------------------------------------------------------------------------------------
void SceneModel::SetPositionAt( D3DXVECTOR3 posAt )
{
	m_posAt = posAt;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
SceneModel* SceneModel::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	SceneModel *pSceneModel;

	//  インスタンス生成
	pSceneModel = new SceneModel;

	//  種類の代入
	pSceneModel->m_type = type;

	//  座標の代入
	pSceneModel->m_position = position;

	//  回転角の代入
	pSceneModel->m_rot = rot;

	//  大きさ倍率の代入
	pSceneModel->m_scale = scale;

	//  初期化
	pSceneModel->Init( );

	return pSceneModel;
}

//--------------------------------------------------------------------------------------
//  外積によって求められる垂直ベクトルから行列を作る関数
//--------------------------------------------------------------------------------------
D3DXMATRIX* SceneModel::GetFixedLookAtMatrix( D3DXMATRIX* pMtx , D3DXVECTOR3* pPos , D3DXVECTOR3* pPosAt , D3DXVECTOR3* pVecUp )
{
	//  ローカル変数の宣言
	D3DXVECTOR3 X , Y , Z , D;

	//  Z軸方向のベクトルを求める
	D = *pPosAt - *pPos;

	//  Zを単位ベクトル化
	D3DXVec3Normalize( &D , &D );

	//  上方向ベクトルとZ軸ベクトルの外積よりX軸ベクトルを求める
	D3DXVec3Cross( &X , D3DXVec3Normalize( &Y , pVecUp ) , &D );

	//  Xを単位ベクトル化
	D3DXVec3Normalize( &X , &X );

	//  Yを単位ベクトル化
	D3DXVec3Normalize( &Z , D3DXVec3Cross( &Z , &X , &Y ) );

	//  1行目の代入
	pMtx->_11 = X.x;
	pMtx->_12 = X.y;
	pMtx->_13 = X.z;
	pMtx->_14 = 0;

	//  2行目の代入
	pMtx->_21 = Y.x;
	pMtx->_22 = Y.y;
	pMtx->_23 = Y.z;
	pMtx->_24 = 0;

	//  3行目の代入
	pMtx->_31 = Z.x;
	pMtx->_32 = Z.y;
	pMtx->_33 = Z.z;
	pMtx->_34 = 0;

	//  4行目の代入
	pMtx->_41 = 0.0f;
	pMtx->_42 = 0.0f;
	pMtx->_43 = 0.0f;
	pMtx->_44 = 1.0f;

	return pMtx;
}

//--------------------------------------------------------------------------------------
//  視界に入ってるかどうか判定する関数
//--------------------------------------------------------------------------------------
bool SceneModel::IntoView( D3DXVECTOR3 vec1 , D3DXVECTOR3 vec2 , float fViewAngle )
{
	//  ローカル変数の宣言
	float fAngle = 0.0f;

	//  二つのベクトルの内積をとる
	fAngle = D3DXVec3Dot( &vec1 , &vec2 );

	//  その結果から二つのベクトルのなすラジアン角を求める
	fAngle = acosf( fAngle );

	if( fAngle > fViewAngle || fAngle < -fViewAngle )
	{
		return false;
	}

	return true;
}