//--------------------------------------------------------------------------------------
//  3Dレンダリング処理   ( scene3D.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "scene3D.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "shaderManager.h"

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  3Dポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Scene3D::Scene3D( int nPriority ) : Scene( nPriority )
{
	//  値のクリア
	m_pVtxBuff = NULL;
	m_pIndexBuff = NULL;
	m_position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	m_color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_posUV = D3DXVECTOR2( 0.0f , 0.0f );
	m_divideUV = D3DXVECTOR2( 1.0f , 1.0f );
	m_fScale = 1.0f;
	m_bInverse = false;
}

//--------------------------------------------------------------------------------------
//  3Dポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Scene3D::Scene3D( D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR3 normal , D3DXVECTOR3 rot , D3DXCOLOR color )
{
	//  値の代入
	m_position = position;
	m_normal = normal;
	m_color = color;
	m_rot = rot;
	m_size = size;
	m_bInverse = false;
	m_fScale = 1.0f;
	m_fRot = 0.0f;
}

//--------------------------------------------------------------------------------------
//  3Dポリゴンのデストラクタ
//--------------------------------------------------------------------------------------
Scene3D::~Scene3D( )
{

}

//--------------------------------------------------------------------------------------
//  3Dポリゴンの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Scene3D::Init( void )
{
	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  テクスチャの登録
	pTexture->SetTextureImage( m_aFileName );

	// 頂点バッファの生成
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  3Dポリゴンの終了処理
//--------------------------------------------------------------------------------------
void Scene3D::Uninit( void )
{
	// 頂点バッファの破棄
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release( );
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if( m_pIndexBuff != NULL )
	{
		m_pIndexBuff->Release( );
		m_pIndexBuff = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  3Dポリゴンの更新処理
//--------------------------------------------------------------------------------------
void Scene3D::Update( void )
{
	//  回転させる
	m_fRot += m_fAddRot;

	//  頂点の設定
	SetVertex( );
}

//--------------------------------------------------------------------------------------
//  3Dポリゴンの描画処理
//--------------------------------------------------------------------------------------
void Scene3D::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  ビュー座標変換行列

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  カメラクラスの取得
	Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	if( m_bInverse == false )
	{
		D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.x , m_rot.y , m_rot.z );

		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );
	}
	else
	{
		//  ビルボード用の回転行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &pCamera->GetInvRotateMat( m_position ) );
	}

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale , 1.0f , 1.0f , 1.0f );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  シェーダー情報の取得
	Shader3DNoLight* shader3DNoLight = ( Shader3DNoLight* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_NO_LIGHT );

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0 ,								//  パイプライン番号
							  m_pVtxBuff ,						//  頂点バッファのアドレス
							  0 ,								//  オフセット( byte )
							  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

	// テクスチャの設定
	pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_aFileName ) ); 

	Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
	D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
	D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );

	//  シェーダーに必要な情報の設定
	shader3DNoLight->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix );

	shader3DNoLight->DrawBegin( );

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
							0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
							NUM_POLYGON );						//  プリミティブ数

	ShaderManager::DrawEnd( );
}

//--------------------------------------------------------------------------------------
//  3Dポリゴンのデプス値の書き込み処理
//--------------------------------------------------------------------------------------
void Scene3D::DrawDepth( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  拡大行列
	D3DXMATRIX mtxRot;								//  ビュー座標変換行列

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  カメラクラスの取得
	Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	if( m_bInverse == false )
	{
		D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.x , m_rot.y , m_rot.z );

		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );
	}
	else
	{
		//  ビルボード用の回転行列の掛け算
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &pCamera->GetInvRotateMat( m_position ) );
	}

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale , 1.0f , 1.0f , 1.0f );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  シェーダー情報の取得
	Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

	D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
	D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );
	float fFar = SceneManager::GetLight( )->GetFar( );

	shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix , fFar );

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0 ,								//  パイプライン番号
							  m_pVtxBuff ,						//  頂点バッファのアドレス
							  0 ,								//  オフセット( byte )
							  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

	// テクスチャの設定
	pDevice->SetTexture( 0 , nullptr ); 

	//  シェーダー描画開始
	shader3DShadowMap->DrawBegin( );

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
							0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
							NUM_POLYGON );						//  プリミティブ数

	//  描画終了
	ShaderManager::DrawEnd( );
}

//--------------------------------------------------------------------------------------
//  3Dポリゴンの大きさの設定をする関数
//--------------------------------------------------------------------------------------
void Scene3D::SetSize( D3DXVECTOR3 size )
{
	m_size = size;
}

//--------------------------------------------------------------------------------------
// ポリゴンの大きさを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Scene3D::GetSize( void )
{
	return m_size;
}

//--------------------------------------------------------------------------------------
//  3Dポリゴンの大きさ比率の設定をする関数
//--------------------------------------------------------------------------------------
void Scene3D::SetScale( float fScale )
{
	m_fScale = fScale;
}

//--------------------------------------------------------------------------------------
//  テクスチャ名の設定
//--------------------------------------------------------------------------------------
void Scene3D::SetTextureName( char* pFileName )
{
	strcpy( m_aFileName , pFileName );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Scene3D* Scene3D::Create( D3DXVECTOR3 position , D3DXVECTOR3 normal , D3DXVECTOR3 size , D3DXVECTOR3 rot , D3DXCOLOR color ,
							char *pTextureName , float fScale  , bool bInverse , float fPosU , float fPosV , float fDivideU , float fDivideV ,
							float fAddRot )
{
	Scene3D *pScene3D;

	//  インスタンス生成
	pScene3D = new Scene3D( 7 );

	//  座標の代入
	pScene3D->m_position = position;

	//  法線の代入
	pScene3D->m_normal = normal;

	//  大きさの代入
	pScene3D->m_size = size;

	//  回転角の代入
	pScene3D->m_rot = rot;

	//  色の代入
	pScene3D->m_color = color;

	//  テクスチャ名の設定
	pScene3D->SetTextureName( pTextureName );

	//  大きさの代入
	pScene3D->m_fScale = fScale;

	//  ビルボードの設定
	pScene3D->m_bInverse = bInverse;

	// UV座標の指定
	pScene3D->m_posUV = D3DXVECTOR2( fPosU , fPosV );

	// UV分割数の指定
	pScene3D->m_divideUV = D3DXVECTOR2( fDivideU , fDivideV );

	//  回転角増加分の代入
	pScene3D->m_fAddRot = fAddRot;

	//  初期化
	pScene3D->Init( );

	return pScene3D;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
HRESULT Scene3D::MakeVertex( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX ,		//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,					//  使用方法
											 FVF_VERTEX_3D ,						//  
											 D3DPOOL_MANAGED ,						//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuff ,							//  バッファ
											 NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return E_FAIL;
	}

	VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ
	D3DXVECTOR3 LeftTop;				//  左上座標
	D3DXVECTOR3 RightTop;				//  右上座標
	D3DXVECTOR3 LeftButtom;				//  左下座標
	D3DXVECTOR3 RightButtom;			//  右下座標

	float fLength = sqrtf( 0.0f * 0.0f + 0.0f * 0.0f ) * 0.5f;
	float fAngle = atan2f( 0.0f , 0.0f );

	//  左上座標の算出
	LeftTop = D3DXVECTOR3( cosf( D3DX_PI + fAngle ) * fLength ,
						   sinf( D3DX_PI + fAngle ) * fLength ,
						   0.0f );

	//  右上座標の算出
	RightTop = D3DXVECTOR3( cosf( - fAngle ) * fLength ,
							sinf( - fAngle ) * fLength ,
							0.0f );

	//  左下座標の算出
	LeftButtom = D3DXVECTOR3( cosf( D3DX_PI - fAngle ) * fLength ,
							  sinf( D3DX_PI - fAngle ) * fLength ,
							  0.0f );

	//  右下座標の算出
	RightButtom = D3DXVECTOR3( cosf( fAngle ) * fLength ,
							   sinf( fAngle ) * fLength ,
							   0.0f );

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
						  0 );										//  ロックの種類

		//  頂点座標の設定( 3D座標 ・ 右回り )
		pVtx[ 0 ].position = LeftTop;
		pVtx[ 1 ].position = RightTop;
		pVtx[ 2 ].position = LeftButtom;
		pVtx[ 3 ].position = RightButtom;

		//  法線の指定
		pVtx[ 0 ].normal = m_normal;
		pVtx[ 1 ].normal = m_normal;
		pVtx[ 2 ].normal = m_normal;
		pVtx[ 3 ].normal = m_normal;

		//  頂点色の設定( 0 ～ 255 の整数値 )
		pVtx[ 0 ].color = m_color;
		pVtx[ 1 ].color = m_color;
		pVtx[ 2 ].color = m_color;
		pVtx[ 3 ].color = m_color;

		//  UV座標の指定
		pVtx[ 0 ].texcoord = D3DXVECTOR2( m_posUV.x , m_posUV.y );
		pVtx[ 1 ].texcoord = D3DXVECTOR2( m_posUV.x + 1.0f / m_divideUV.x , m_posUV.y );
		pVtx[ 2 ].texcoord = D3DXVECTOR2( m_posUV.x , m_posUV.y + 1.0f / m_divideUV.y );
		pVtx[ 3 ].texcoord = D3DXVECTOR2( m_posUV.x + 1.0f / m_divideUV.x , m_posUV.y + 1.0f / m_divideUV.y );

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void Scene3D::SetVertex( void )
{
	VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ
	D3DXVECTOR3 LeftTop;				//  左上座標
	D3DXVECTOR3 RightTop;				//  右上座標
	D3DXVECTOR3 LeftButtom;				//  左下座標
	D3DXVECTOR3 RightButtom;			//  右下座標

	float fLength = sqrtf( m_size.x * m_size.x + m_size.z * m_size.z ) * 0.5f * m_fScale;
	float fAngle = atan2f( m_size.z , m_size.x );

	//  左上座標の算出
	LeftTop = D3DXVECTOR3( cosf( D3DX_PI + fAngle + m_fRot ) * fLength ,
						   sinf( D3DX_PI + fAngle + m_fRot ) * fLength ,
						   0.0f );

	//  右上座標の算出
	RightTop = D3DXVECTOR3( cosf( -fAngle + m_fRot ) * fLength ,
							sinf( -fAngle + m_fRot ) * fLength ,
							0.0f );

	//  左下座標の算出
	LeftButtom = D3DXVECTOR3( cosf( D3DX_PI - fAngle + m_fRot ) * fLength ,
							  sinf( D3DX_PI - fAngle + m_fRot ) * fLength ,
							  0.0f );

	//  右下座標の算出
	RightButtom = D3DXVECTOR3( cosf( fAngle + m_fRot ) * fLength ,
							   sinf( fAngle + m_fRot ) * fLength ,
							   0.0f );

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
						  0 );										//  ロックの種類

		//  頂点座標の設定( 3D座標 ・ 右回り )
		pVtx[ 0 ].position = LeftTop;
		pVtx[ 1 ].position = RightTop;
		pVtx[ 2 ].position = LeftButtom;
		pVtx[ 3 ].position = RightButtom;

		//  頂点色の設定( 0 ～ 255 の整数値 )
		pVtx[ 0 ].color = m_color;
		pVtx[ 1 ].color = m_color;
		pVtx[ 2 ].color = m_color;
		pVtx[ 3 ].color = m_color;

		//  UV座標の指定
		pVtx[ 0 ].texcoord = D3DXVECTOR2( m_posUV.x , m_posUV.y );
		pVtx[ 1 ].texcoord = D3DXVECTOR2( m_posUV.x + 1.0f / m_divideUV.x , m_posUV.y );
		pVtx[ 2 ].texcoord = D3DXVECTOR2( m_posUV.x , m_posUV.y + 1.0f / m_divideUV.y );
		pVtx[ 3 ].texcoord = D3DXVECTOR2( m_posUV.x + 1.0f / m_divideUV.x , m_posUV.y + 1.0f / m_divideUV.y );

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}
}