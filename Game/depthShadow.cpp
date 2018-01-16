//--------------------------------------------------------------------------------------
//  デプスシャドウ   ( stencilShadow.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "depthShadow.h"
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
LPDIRECT3DTEXTURE9	DepthShadow::m_depthShadowTexture = nullptr;			//  テクスチャ
LPDIRECT3DSURFACE9	DepthShadow::m_depthShadowSurface = nullptr;			//  レンダーターゲット用サーフェイス
LPDIRECT3DSURFACE9	DepthShadow::m_shadowMap = nullptr;						//  テクスチャ用の深度バッファー
float				DepthShadow::m_bias = 0.000003f;
const UINT			DepthShadow::TEXTURE_WIDTH = 2048;						//  テクスチャの幅
const UINT			DepthShadow::TEXTURE_HEIGHT = 2048;						//  テクスチャの高さ

//--------------------------------------------------------------------------------------
//  デプスシャドウクラスのコンストラクタ
//--------------------------------------------------------------------------------------
DepthShadow::DepthShadow( )
{

}

//--------------------------------------------------------------------------------------
//  デプスシャドウのデストラクタ
//--------------------------------------------------------------------------------------
DepthShadow::~DepthShadow( )
{

}

//--------------------------------------------------------------------------------------
//  デプスシャドウの初期化処理
//--------------------------------------------------------------------------------------
HRESULT DepthShadow::Init( void )
{
	m_bias = 0.000003f;

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  レンダーターゲット用の空のテクスチャ生成
	D3DXCreateTexture( pDevice ,						//  デバイス
					   TEXTURE_WIDTH ,					//  スクリーンの幅
					   TEXTURE_HEIGHT ,					//  スクリーンの高さ
					   1 ,								//  ミップマップの数
					   D3DUSAGE_RENDERTARGET ,			//  使用用途
					   D3DFMT_A32B32G32R32F ,			//  色の要素
					   D3DPOOL_DEFAULT ,				//  メモリの配置方法
					   &m_depthShadowTexture );			//	テクスチャ格納場所

	//  サーフェイスの取得
	m_depthShadowTexture->GetSurfaceLevel( 0 , &m_depthShadowSurface );

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
//  デプスシャドウの終了処理
//--------------------------------------------------------------------------------------
void DepthShadow::Uninit( void )
{
	//  深度バッファ用のサーフェイスの破棄
	if( m_depthShadowSurface != nullptr )
	{
		m_depthShadowSurface->Release( );
		m_depthShadowSurface = nullptr;
	}

	//  深度バッファ用のテクスチャの破棄
	if( m_depthShadowTexture != nullptr )
	{
		m_depthShadowTexture->Release( );
		m_depthShadowTexture = nullptr;
	}

	//  シャドウマップ用のサーフェイスの破棄
	if( m_shadowMap != nullptr )
	{
		m_shadowMap->Release( );
		m_shadowMap = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  デプスシャドウのバッファのクリア処理
//--------------------------------------------------------------------------------------
void DepthShadow::DrawClearBuffer( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  バックバッファ＆Ｚバッファのクリア
	pDevice->Clear( 0 , NULL , ( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ) , D3DCOLOR_RGBA( 255 , 255 , 255 , 255 ) , 1.0f , 0 );
}

//--------------------------------------------------------------------------------------
//  レンダーターゲットの設定処理
//--------------------------------------------------------------------------------------
void DepthShadow::SetRendererTarget( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( m_depthShadowSurface != nullptr )
	{
		pDevice->SetRenderTarget( 0 , m_depthShadowSurface );
	}
}

//--------------------------------------------------------------------------------------
//  深度バッファのサーフェースの設定処理
//--------------------------------------------------------------------------------------
void DepthShadow::SetDepthSerface( void )
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
void DepthShadow::SetBias( float bias )
{
	m_bias = bias;
}

//--------------------------------------------------------------------------------------
//  バイアス値の取得処理
//--------------------------------------------------------------------------------------
float DepthShadow::GetBias( void )
{
	return m_bias;
}

//--------------------------------------------------------------------------------------
//  レンダーターゲットのテクスチャ取得
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 DepthShadow::GetRendereTargetTexture( void )
{
	return m_depthShadowTexture;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
DepthShadow* DepthShadow::Create( void )
{
	DepthShadow *pDepthShadow;

	//  インスタンス生成
	pDepthShadow = new DepthShadow;

	//  初期化
	pDepthShadow->Init( );

	return pDepthShadow;
}