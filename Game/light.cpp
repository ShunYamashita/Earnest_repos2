//--------------------------------------------------------------------------------------
//  ライト   ( light.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "shaderManager.h"
#include "imgui.h"
#include "texture.h"
#include "camera.h"
#include "depthShadow.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define	MODEL_FILENAME		"data/MODEL/light000.x"
#define TEXTURE_FILEPATH	"data\\TEXTURE"						//  テクスチャへのファイルパス

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ライトクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Light::Light( )
{

}

//--------------------------------------------------------------------------------------
//  ライトクラスのデストラクタ
//--------------------------------------------------------------------------------------
Light::~Light( )
{

}

//--------------------------------------------------------------------------------------
//  ライトの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Light::Init( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ライト最大数分のループ
	for( int countLight = 0; countLight < MAX_LIGHT; ++countLight )
	{
		//  光源クラスの初期化
		ZeroMemory( &m_light , sizeof( D3DLIGHT9 ) );
	}

	m_light[ 0 ].Type = D3DLIGHT_DIRECTIONAL;									//  光源の種類を平行光源に		
	m_light[ 0 ].Diffuse = D3DXCOLOR( 1.0f , 0.9f , 0.3f , 1.0f );				//  拡散光

	//  光源の座標を代入
	m_position = D3DXVECTOR3( 150.0f , 300.0f , 150.0f );
	m_positionAt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	D3DXVECTOR3 vecDir = m_positionAt - m_position;								//  法線の向き
	D3DXVec3Normalize( &vecDir , &vecDir );
	m_vectorDirection = vecDir;
	m_distance = 100.0f;

	//  ビュー行列の設定
	D3DXMatrixIdentity( &m_viewMatrix );
	D3DXMatrixLookAtLH( &m_viewMatrix , 
						&m_position , 
						&m_positionAt , 
						&D3DXVECTOR3( 0.0f , 1.0f, 0.0f ) );

	m_fov = 1.43f;
	m_near = 1.0f;
	m_far = 683.0f;

	//  プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH( &m_projectionMatrix ,														//  プロジェクション行列のアドレス
								m_fov ,																		//  画角
								( float )DepthShadow::TEXTURE_WIDTH / DepthShadow::TEXTURE_HEIGHT,			//  アスペクト比
								m_near ,																	//  near( 0.0f < near )
								m_far );																	//  far

	D3DXMATERIAL*	pMat = NULL;								//  マテリアル
	char			aTextureFileName[ 128 ] = { };
	int				nIndex = 0;
	LPD3DXMESH		oldMesh = nullptr;

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

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

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  ライトの終了処理
//--------------------------------------------------------------------------------------
void Light::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  ライトの更新処理
//--------------------------------------------------------------------------------------
void Light::Update( void )
{

}

//--------------------------------------------------------------------------------------
//  ライトの描画処理
//--------------------------------------------------------------------------------------
void Light::Draw( void )
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

	//  行列を単位行列に変換w
	D3DXMatrixIdentity( &mtxWorld );

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale , 0.3f , 0.3f , 0.3f );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	D3DXMatrixRotationYawPitchRoll( &mtxRot , 0.0f , 0.0f , 0.0f );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

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

	D3DXMATRIX lightViewProjectionMatrix = SceneManager::GetLight( )->GetViewMatrix( ) * SceneManager::GetLight( )->GetProjectionMatrix( );
	D3DXVECTOR4	tmpOffset;
	tmpOffset.x = 0.5f / DepthShadow::TEXTURE_WIDTH;
	tmpOffset.y = 0.5f / DepthShadow::TEXTURE_HEIGHT;
	tmpOffset.z = 0.0f;
	tmpOffset.w = 0.0f;

	//  シェーダーに必要な情報の設定
	shader3D->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor );
	//shader3DDepthShadow->SetShaderInfo( mtxWorld , viewMatrix ,projectionMatrix ,
	//									lightDirectLocal , lightViewProjectionMatrix , tmpOffset );

	UINT textureSampler = shader3DDepthShadow->GetSamplerTextureIndex( );
	UINT shadowSampler = shader3DDepthShadow->GetSamplerShadowIndex( );

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
			shader3DNoTexture->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , color );

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
			//shader3DDepthShadow->SetShaderInfo( mtxWorld , viewMatrix ,projectionMatrix ,
			//									lightDirectLocal , lightViewProjectionMatrix , tmpOffset );
			shader3D->SetShaderInfo( mtxWorld , viewMatrix ,projectionMatrix , lightDirectLocal , lightDiffuseColor );

			////  シェーダー3Dの描画開始
			shader3D->DrawBegin( );
			//shader3DDepthShadow->DrawBegin( );
		}

		//  メッシュの描画
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  シェーダー描画の終了
		ShaderManager::DrawEnd( );
	}
}

//--------------------------------------------------------------------------------------
//  光を点ける関数
//--------------------------------------------------------------------------------------
void Light::SetDiffuseColor( D3DXCOLOR color , int nNo )
{
	D3DLIGHT9 light;													//  光源

	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  光源クラスの初期化
	ZeroMemory( &light , sizeof( D3DLIGHT9 ) );

	light.Type = D3DLIGHT_DIRECTIONAL;									//  光源の種類を平行光源に		
	light.Diffuse = color;												//  拡散光

	D3DXVECTOR3 vecDir = D3DXVECTOR3( 1.0f , -0.4f , -1.0f );			//  法線の向き

	//  法線を1.0fに変換
	D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction , &vecDir );
}

//--------------------------------------------------------------------------------------
//  光を点ける関数
//--------------------------------------------------------------------------------------
void Light::LightOn( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ライトを点ける
	pDevice->SetRenderState( D3DRS_LIGHTING , true );
}

//--------------------------------------------------------------------------------------
//  光を消す関数
//--------------------------------------------------------------------------------------
void Light::LightOff( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ライトを消す
	pDevice->SetRenderState( D3DRS_LIGHTING , false );
}

//--------------------------------------------------------------------------------------
//	ビュー行列の設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetViewMatrix( D3DXVECTOR3 position , D3DXVECTOR3 positionAt )
{
	//  情報の代入
	m_position = position;
	m_positionAt = positionAt;

	D3DXVECTOR3 vecDir = m_positionAt - m_position;    //  光源の向き
	D3DXVec3Normalize( &vecDir , &vecDir );
	m_vectorDirection = vecDir;

	//  ビュー行列の設定
	D3DXMatrixIdentity( &m_viewMatrix );
	D3DXMatrixLookAtLH( &m_viewMatrix , 
						&m_position , 
						&m_positionAt , 
						&D3DXVECTOR3( 0.0f , 1.0f, 0.0f ) );
}

//--------------------------------------------------------------------------------------
//	ビュー行列の設定をする関数
//--------------------------------------------------------------------------------------
const D3DXMATRIX& Light::GetDirectionalViewMatrix( D3DXVECTOR3 position )
{
	//  情報の代入
	m_positionAt = position;

	//  座標の算出
	m_position = position - m_vectorDirection * m_distance;
	
	//  ビュー行列の設定
	D3DXMatrixIdentity( &m_viewMatrix );
	D3DXMatrixLookAtLH( &m_viewMatrix , 
						&m_position , 
						&m_positionAt , 
						&D3DXVECTOR3( 0.0f , 1.0f, 0.0f ) );

	return m_viewMatrix;
}

//--------------------------------------------------------------------------------------
//  プロジェクション行列の設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetProjectionMatrix( float fov , float fNear , float fFar )
{
	m_fov = fov;
	m_near = fNear;
	m_far = fFar;

	//  プロジェクション行列の作成
	D3DXMatrixPerspectiveFovLH( &m_projectionMatrix ,														//  プロジェクション行列のアドレス
								m_fov ,																		//  画角
								( float )DepthShadow::TEXTURE_WIDTH / DepthShadow::TEXTURE_HEIGHT,			//  アスペクト比
								m_near ,																	//  near( 0.0f < near )
								m_far );																	//  far
}

//--------------------------------------------------------------------------------------
//  距離の設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetDistance( float distance )
{
	m_distance = distance;
}

//--------------------------------------------------------------------------------------
//  進行方向ベクトルの設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetVectorDirection( D3DXVECTOR3 direction )
{
	m_vectorDirection = direction;
	D3DXVec3Normalize( &m_vectorDirection , &m_vectorDirection );
}

//--------------------------------------------------------------------------------------
//  進行方向Xベクトルの設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetVectorDirectionX( float directionX )
{
	m_vectorDirection.x = directionX;
	D3DXVec3Normalize( &m_vectorDirection , &m_vectorDirection );
}

//--------------------------------------------------------------------------------------
//  進行方向Yベクトルの設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetVectorDirectionY( float directionY )
{
	m_vectorDirection.y = directionY;
	D3DXVec3Normalize( &m_vectorDirection , &m_vectorDirection );
}

//--------------------------------------------------------------------------------------
//  進行方向Yベクトルの設定をする関数
//--------------------------------------------------------------------------------------
void Light::SetVectorDirectionZ( float directionZ )
{
	m_vectorDirection.z = directionZ;
	D3DXVec3Normalize( &m_vectorDirection , &m_vectorDirection );
}

//--------------------------------------------------------------------------------------
//  指定番号のライト情報の取得する関数
//--------------------------------------------------------------------------------------
const D3DLIGHT9& Light::GetLight( int index )
{
	return m_light[ index ];
}

//--------------------------------------------------------------------------------------
//  指定番号のライト方向ベクトルの取得する関数
//--------------------------------------------------------------------------------------
const D3DXVECTOR3& Light::GetDirection( void )
{
	return m_vectorDirection;
}

//--------------------------------------------------------------------------------------
//  指定番号の座標の取得する関数
//--------------------------------------------------------------------------------------
const D3DXVECTOR3&	Light::GetPosition( void )
{
	return m_position;
}

//--------------------------------------------------------------------------------------
//  指定番号の注視点の取得をする関数
//--------------------------------------------------------------------------------------
const D3DXVECTOR3& Light::GetPositionAt( void )
{
	return m_positionAt;
}

//--------------------------------------------------------------------------------------
//  ビュー行列の取得する関数
//--------------------------------------------------------------------------------------
const D3DXMATRIX& Light::GetViewMatrix( void )
{
	return m_viewMatrix;
}

//--------------------------------------------------------------------------------------
//  プロジェクション行列の取得する関数
//--------------------------------------------------------------------------------------
const D3DXMATRIX& Light::GetProjectionMatrix( void )
{
	return m_projectionMatrix;
}

//--------------------------------------------------------------------------------------
//  画角の取得する関数
//--------------------------------------------------------------------------------------
const float& Light::GetFov( void )
{
	return m_fov;
}

//--------------------------------------------------------------------------------------
//  ニアの取得する関数
//--------------------------------------------------------------------------------------
const float& Light::GetNear( void )
{
	return m_near;
}

//--------------------------------------------------------------------------------------
//  ファーの取得する関数
//--------------------------------------------------------------------------------------
const float& Light::GetFar( void )
{
	return m_far;
}

//--------------------------------------------------------------------------------------
//  距離の取得する関数
//--------------------------------------------------------------------------------------
const float& Light::GetDistance( void )
{
	return m_distance;
}