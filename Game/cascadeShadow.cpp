//--------------------------------------------------------------------------------------
//  デプスシャドウ   ( stencilShadow.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "cascadeShadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "shaderManager.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9	CascadeShadow::m_cascadeShadowTexture[ MAX_CASCADE ] = { nullptr };		//  テクスチャ
LPDIRECT3DSURFACE9	CascadeShadow::m_cascadeShadowSurface[ MAX_CASCADE ] = { nullptr };		//  レンダーターゲット用サーフェイス
LPDIRECT3DSURFACE9	CascadeShadow::m_shadowMap = nullptr;									//  テクスチャ用の深度バッファー
float				CascadeShadow::m_bias = 0.000013f;										//  バイアス値
const UINT			CascadeShadow::TEXTURE_WIDTH = 2048;									//  テクスチャの幅
const UINT			CascadeShadow::TEXTURE_HEIGHT = 2048;									//  テクスチャの高さ

//--------------------------------------------------------------------------------------
//  カスケードシャドウクラスのコンストラクタ
//--------------------------------------------------------------------------------------
CascadeShadow::CascadeShadow( )
{

}

//--------------------------------------------------------------------------------------
//  カスケードシャドウのデストラクタ
//--------------------------------------------------------------------------------------
CascadeShadow::~CascadeShadow( )
{

}

//--------------------------------------------------------------------------------------
//  カスケードシャドウの初期化処理
//--------------------------------------------------------------------------------------
HRESULT CascadeShadow::Init( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	for( int countCascade = 0; countCascade < MAX_CASCADE; countCascade++ )
	{
		//  レンダーターゲット用の空のテクスチャ生成
		D3DXCreateTexture( pDevice ,										//  デバイス
						   TEXTURE_WIDTH ,									//  スクリーンの幅
						   TEXTURE_HEIGHT ,									//  スクリーンの高さ
						   1 ,												//  ミップマップの数
						   D3DUSAGE_RENDERTARGET ,							//  使用用途
						   D3DFMT_A32B32G32R32F ,							//  色の要素
						   D3DPOOL_DEFAULT ,								//  メモリの配置方法
						   &m_cascadeShadowTexture[ countCascade ] );		//	テクスチャ格納場所

		//  サーフェイスの取得
		m_cascadeShadowTexture[ countCascade ]->GetSurfaceLevel( 0 , &m_cascadeShadowSurface[ countCascade ] );
	}

	// テクスチャへのレンダリングに使う深度バッファーの作成
	if( FAILED( pDevice->CreateDepthStencilSurface( TEXTURE_WIDTH ,
													TEXTURE_HEIGHT ,
													D3DFMT_D24S8,
													D3DMULTISAMPLE_NONE,
													0,
													TRUE,
													&m_shadowMap,
													NULL ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  カスケードシャドウの終了処理
//--------------------------------------------------------------------------------------
void CascadeShadow::Uninit( void )
{
	for( int countCascade = 0; countCascade < MAX_CASCADE; countCascade++ )
	{
		//  深度バッファ用のサーフェイスの破棄
		if( m_cascadeShadowSurface[ countCascade ] != nullptr )
		{
			m_cascadeShadowSurface[ countCascade ]->Release( );
			m_cascadeShadowSurface[ countCascade ] = nullptr;
		}

		//  深度バッファ用のテクスチャの破棄
		if( m_cascadeShadowTexture[ countCascade ] != nullptr )
		{
			m_cascadeShadowTexture[ countCascade ]->Release( );
			m_cascadeShadowTexture[ countCascade ] = nullptr;
		}
	}

	//  シャドウマップ用のサーフェイスの破棄
	if( m_shadowMap != nullptr )
	{
		m_shadowMap->Release( );
		m_shadowMap = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  カスケードシャドウのバッファのクリア処理
//--------------------------------------------------------------------------------------
void CascadeShadow::DrawClearBuffer( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  バックバッファ＆Ｚバッファのクリア
	pDevice->Clear( 0 , NULL , ( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ) , D3DCOLOR_RGBA( 255 , 255 , 255 , 255 ) , 1.0f , 0 );
}

//--------------------------------------------------------------------------------------
//  レンダーターゲットの設定処理
//--------------------------------------------------------------------------------------
void CascadeShadow::SetRendererTarget( int index )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( m_cascadeShadowSurface != nullptr )
	{
		pDevice->SetRenderTarget( 0 , m_cascadeShadowSurface[ index ] );
	}
}

//--------------------------------------------------------------------------------------
//  深度バッファのサーフェースの設定処理
//--------------------------------------------------------------------------------------
void CascadeShadow::SetDepthSerface( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( m_shadowMap != nullptr )
	{
		//  テクスチャ用の深度バッファを設定
		pDevice->SetDepthStencilSurface( m_shadowMap );
	}
}

//--------------------------------------------------------------------------------------
//  バイアス値の設定処理
//--------------------------------------------------------------------------------------
void CascadeShadow::SetBias( float bias )
{
	m_bias = bias;
}

//--------------------------------------------------------------------------------------
//  バイアス値の取得処理
//--------------------------------------------------------------------------------------
float CascadeShadow::GetBias( void )
{
	return m_bias;
}

//--------------------------------------------------------------------------------------
//  レンダーターゲットのテクスチャ取得
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CascadeShadow::GetRendereTargetTexture( int index )
{
	return m_cascadeShadowTexture[ index ];
}

//--------------------------------------------------------------------------------------
//  平行分割シャドウマップの分割位置の計算
//--------------------------------------------------------------------------------------
void CascadeShadow::SetSplitShadowPosition( int numberSplit ,	
											float lamda , 
											float nearValue ,
											float farValue ,
											float* positions )
{

}