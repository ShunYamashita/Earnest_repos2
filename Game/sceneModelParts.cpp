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
#include "sceneModelParts.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sceneModel.h"
#include "shaderManager.h"
#include "light.h"
#include "game.h"
#include "depthShadow.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define TEXTURE_FILEPATH							"data\\TEXTURE"					//  テクスチャへのファイルパス

static const char THUN_SHADER_TEXTURE_NAME[ 256 ] = "data/SHADER/thun001.png";		//  トゥーンマップ用テクスチャ

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  xモデルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneModelParts::SceneModelParts( )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_posAt = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_scale = D3DXVECTOR3( 1.0f, 1.0f , 1.0f );
	m_rot = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  xモデルのデストラクタ
//--------------------------------------------------------------------------------------
SceneModelParts::~SceneModelParts( )
{

}

//--------------------------------------------------------------------------------------
//  xモデルの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SceneModelParts::Init( void )
{
	D3DXMATERIAL*	pMat = NULL;									//  マテリアル
	char			aTextureFileName[ 128 ] = { };
	int				nIndex = 0;

	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	LPD3DXMESH oldMesh = nullptr;

	//  xファイルの読み込み
	if( FAILED( D3DXLoadMeshFromX( m_aModelName ,				//  ファイル名
		 						   D3DXMESH_SYSTEMMEM ,			//  オプション
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

	D3DVERTEXELEMENT9 elements[ ] = 
	{  
		//  引数( 1 : パイプライン番号 , 2 : オフセット , 3 : データサイズ( 型 ) , 4 : , 5 : 使用用途 , 6 : , ( 5～6はセマンティクス ) )
		//  ( 自分で考えたデータを送る場合はTEXCOORDで送るように )
		{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 } ,
		{ 0 , sizeof( float ) * 3 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL, 0 } ,
		{ 0 , sizeof( float ) * 6 , D3DDECLTYPE_D3DCOLOR , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_COLOR , 0 } ,
		{ 0 , sizeof( float ) * 6 + sizeof( D3DCOLOR ) , D3DDECLTYPE_FLOAT2 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 0 } ,		
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
			pTexture->SetTextureImage( THUN_SHADER_TEXTURE_NAME );
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  xモデルの終了処理
//--------------------------------------------------------------------------------------
void SceneModelParts::Uninit( void )
{
	//  メッシュ情報の解放
	SAFE_RELEASE( m_pMeshModel );

	//  マテリアル情報の解放
	SAFE_RELEASE( m_pBuffMatModel );
}

//--------------------------------------------------------------------------------------
//  xモデルの更新処理
//--------------------------------------------------------------------------------------
void SceneModelParts::Update( D3DXMATRIX mtxParent )
{
	D3DXMATRIX		mtxWorld;						//  ワールド行列
	D3DXMATRIX		mtxWorld2;						//  ワールド行列2
	D3DXMATRIX		mtxScale;						//  拡大行列
	D3DXMATRIX		mtxRot;							//  回転行列
	D3DXMATRIX		mtxRot2;						//  回転行列
	D3DXMATRIX		mtxTrans;						//  平行移動行列	
	D3DXVECTOR3		workRot( 0.0f, 0.0f , 0.0f );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &m_mtxWorld );

	//  回転行列の作成
	D3DXMatrixRotationYawPitchRoll( &mtxRot ,
									m_rot.y ,
									m_rot.x ,
									m_rot.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &m_mtxWorld ,
						&m_mtxWorld ,
						&mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans ,
						   m_position.x + m_seekPos.x ,
						   m_position.y + m_seekPos.y ,
						   m_position.z + m_seekPos.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &m_mtxWorld ,
						&m_mtxWorld ,
						&mtxTrans );

	if( m_nParent != -1 )
	{
		//  平行移動行列の掛け算
		D3DXMatrixMultiply( &m_mtxWorld , &m_mtxWorld , &mtxParent );
	}
}

//--------------------------------------------------------------------------------------
//  xモデルの描画処理
//--------------------------------------------------------------------------------------
void SceneModelParts::Draw( D3DXVECTOR3 position , D3DXVECTOR3 posAt , D3DXCOLOR color )
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
	D3DXMatrixScaling( &mtxScale ,
						m_scale.x ,
						m_scale.y ,
						m_scale.z );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld ,
						&m_mtxWorld ,
						&mtxScale );

	//  回転行列の作成
	SceneModel::GetFixedLookAtMatrix( &mtxRot , &position , &posAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , position.x , position.y , position.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

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
	D3DXMatrixInverse( &worldInverseMatrix , NULL , &mtxWorld );
	D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
	D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

	D3DXMATRIX worldInverseTransposeMatrix;
	D3DXMatrixInverse( &worldInverseTransposeMatrix , NULL , &mtxWorld );
	D3DXMatrixTranspose( &worldInverseTransposeMatrix , &worldInverseTransposeMatrix );
	float specularPower = 10.0f;

	Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
	D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
	D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
	D3DXVECTOR3 cameraPositionEye = camera->GetCameraPosEye( );
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

	//  シェーダー情報の取得
	Shader3DThun* shader3DThun = ( Shader3DThun* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_THUN );

	//  サンプラーのインデックス情報取得
	UINT textureIndex = shader3DThun->GetSamplerTextureIndex( );
	UINT thunIndex = shader3DThun->GetSamplerThunIndex( );

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  カラー情報の取得
		D3DXCOLOR color = D3DXCOLOR( pMat[ nCntMaterial ].MatD3D.Diffuse.r ,
									 pMat[ nCntMaterial ].MatD3D.Diffuse.g ,
									 pMat[ nCntMaterial ].MatD3D.Diffuse.b ,
									 pMat[ nCntMaterial ].MatD3D.Diffuse.a );

		if( pMat[ nCntMaterial ].pTextureFilename == NULL )
		{
			pDevice->SetTexture( 0 , nullptr );

			//  シェーダーに必要な情報の設定
			shader3DNoTexture->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , color );

			//  シェーダー3Dの描画開始
			shader3DNoTexture->DrawBegin( );
		}
		else
		{
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_aTextureName[ nCntMaterial ] ) );

			////  シェーダーに必要な情報の設定
			//shader3D->SetShaderInfo( mtxWorld , lightDirectLocal );

			////  シェーダー3Dの描画開始
			//shader3D->DrawBegin( );

			////  シェーダーに必要な情報の設定
			//shader3DRimLightTexture->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix ,
			//										worldInverseTransposeMatrix , cameraPositionEye , lightDirectWorld , 
			//										specularPower , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
			shader3DDepthShadow->SetShaderInfo( mtxWorld , viewMatrix ,projectionMatrix ,
												lightDirectLocal , lightViewProjectionMatrix , tmpOffset , bias );
			//shader3DThun->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal );

			//  シェーダー3Dの描画開始
			//shader3DRimLightTexture->DrawBegin( );
			shader3DDepthShadow->DrawBegin( );
			//shader3DThun->DrawBegin( );
		}

		//  メッシュの描画
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  シェーダー3Dの描画終了
		ShaderManager::DrawEnd( );
	}
}

//--------------------------------------------------------------------------------------
//  xモデルのデプス値の書き込み処理
//--------------------------------------------------------------------------------------
void SceneModelParts::DrawDepth( D3DXVECTOR3 position , D3DXVECTOR3 posAt , D3DXCOLOR color )
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

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale ,
						m_scale.x ,
						m_scale.y ,
						m_scale.z );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld ,
						&m_mtxWorld ,
						&mtxScale );

	//  回転行列の作成
	SceneModel::GetFixedLookAtMatrix( &mtxRot , &position , &posAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , position.x , position.y , position.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  マテリアル情報の先頭アドレスを取得
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

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
		//  空のテクスチャの設定
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
void SceneModelParts::SetScale( D3DXVECTOR3 scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  xポリゴンの大きさを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 SceneModelParts::GetScale( void )
{
	return m_scale;
}

//--------------------------------------------------------------------------------------
//  xモデルの回転角を設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelParts::SetRot( D3DXVECTOR3 rot )
{
	m_rot = rot;
}

//--------------------------------------------------------------------------------------
//  xポリゴンの回転角を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 SceneModelParts::GetRot( void )
{
	return m_rot;
}

//--------------------------------------------------------------------------------------
//  xモデルのずらす座標を設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelParts::SetSeekPos( D3DXVECTOR3 seekPos )
{
	m_seekPos = seekPos;
}

//--------------------------------------------------------------------------------------
//  xモデルの注視点設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelParts::SetPositionAt( D3DXVECTOR3 posAt )
{
	m_posAt = posAt;
}

//--------------------------------------------------------------------------------------
//  モデル名の設定
//--------------------------------------------------------------------------------------
void SceneModelParts::SetModelName( char* pFileName )
{
	strcpy( m_aModelName , pFileName );
}

//--------------------------------------------------------------------------------------
//  識別番号の設定
//--------------------------------------------------------------------------------------
void SceneModelParts::SetIndex( int nIndex )
{
	m_nIndex = nIndex;
}

//--------------------------------------------------------------------------------------
//  識別番号を取得する関数
//--------------------------------------------------------------------------------------
int SceneModelParts::GetIndex( void )
{
	return m_nIndex;
}

//--------------------------------------------------------------------------------------
//  親番号の設定
//--------------------------------------------------------------------------------------
void SceneModelParts::SetParent( int nParent )
{
	m_nParent = nParent;
}

//--------------------------------------------------------------------------------------
//  親番号を取得する関数
//--------------------------------------------------------------------------------------
int SceneModelParts::GetParent( void )
{
	return m_nParent;
}

//--------------------------------------------------------------------------------------
//  親行列を設定する関数
//--------------------------------------------------------------------------------------
D3DXMATRIX SceneModelParts::GetMtxWorld( void )
{
	return m_mtxWorld;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
SceneModelParts* SceneModelParts::Create( int nIndex , int nParent , char* pFileName , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	SceneModelParts *pSceneModel;

	//  インスタンス生成
	pSceneModel = new SceneModelParts;

	//  識別番号の代入
	pSceneModel->m_nIndex = nIndex;

	//  親番号の代入
	pSceneModel->m_nParent = nParent;

	//  モデル名の設定
	pSceneModel->SetModelName( pFileName);

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