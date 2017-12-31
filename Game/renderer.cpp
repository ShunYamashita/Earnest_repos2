//--------------------------------------------------------------------------------------
//  レンダリング処理   ( renderer.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "scene.h"
#include "fade.h"
#include "manager.h"
#include "game.h"
#include "meshField.h"
#include "effekseerManager.h"
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9	Renderer::m_pD3DDevice = NULL;			//  デバイス

//--------------------------------------------------------------------------------------
//  レンダラークラスのコンストラクタ
//--------------------------------------------------------------------------------------
Renderer::Renderer( )
{
	//  値のクリア
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_wireFrame = false;

#ifdef _DEBUG

	m_pFont = NULL;
	m_nCountFPS = 0;

#endif
}

//--------------------------------------------------------------------------------------
//  レンダラークラスのデストラクタ
//--------------------------------------------------------------------------------------
Renderer::~Renderer( )
{

}

//--------------------------------------------------------------------------------------
//  レンダラークラスの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Renderer::Init( HWND hWnd, bool bWindow )
{
	//  値のクリア
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	//  Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	//  Direct3Dインターフェースがない場合
	if( m_pD3D == NULL )
	{
		return E_FAIL;
	}

	//  現在のディスプレイモードを取得
    if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		return E_FAIL;
	}

	//  デバイスのプレゼンテーションパラメータの設定
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );								//  ワークをゼロクリア	
	d3dpp.BackBufferWidth			 = ( UINT )SCREEN_WIDTH;			//  ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			 = ( UINT )SCREEN_HEIGHT;			//  ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			 = d3ddm.Format;					//  カラーモードの指定
	d3dpp.BackBufferCount			 = 1;								//  バックバッファの数
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;			//  映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil	 = TRUE;							//  デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D24S8;					//  デプスバッファとして24bit、ステンシルバッファとして8bitを使う
	d3dpp.Windowed					 = bWindow;							//  ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			//  リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		//  インターバル

	//  デバイスの生成
	//  ディスプレイアダプタを表すためのデバイスを作成
	//  描画と頂点処理をハードウェアで行なう
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT , 
									  D3DDEVTYPE_HAL , 
									  hWnd, 
									  D3DCREATE_HARDWARE_VERTEXPROCESSING , 
									  &d3dpp , &m_pD3DDevice ) ) )
	{
		//  上記の設定が失敗したら
		//  描画をハードウェアで行い、頂点処理はCPUで行なう
		if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT , 
										  D3DDEVTYPE_HAL , 
										  hWnd , 
										  D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										  &d3dpp , &m_pD3DDevice ) ) )
		{
			//  上記の設定が失敗したら
			//  描画と頂点処理をCPUで行なう
			if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT , 
											  D3DDEVTYPE_REF , hWnd , 
											  D3DCREATE_SOFTWARE_VERTEXPROCESSING , 
											  &d3dpp , &m_pD3DDevice ) ) )
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	//  レンダーステートの設定
    m_pD3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );					//  裏面をカリング
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE , TRUE );							//  Zバッファを使用
	m_pD3DDevice->SetRenderState( D3DRS_STENCILENABLE , FALSE );					//  ステンシルシャドウを使わない設定
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );					//  αブレンドを行う
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );				//  αソースカラーの指定
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );			//  αデスティネーションカラーの指定

	for( int countSampler = 0; countSampler < 2; ++countSampler )
	{
		//  サンプラーステートの設定
		m_pD3DDevice->SetSamplerState( countSampler , D3DSAMP_ADDRESSU , D3DTADDRESS_WRAP );		//  テクスチャＵ値の繰り返し設定
		m_pD3DDevice->SetSamplerState( countSampler , D3DSAMP_ADDRESSV , D3DTADDRESS_WRAP );		//  テクスチャＶ値の繰り返し設定
		m_pD3DDevice->SetSamplerState( countSampler , D3DSAMP_MINFILTER , D3DTEXF_ANISOTROPIC );	//  テクスチャ縮小時の補間設定
		m_pD3DDevice->SetSamplerState( countSampler , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );			//  テクスチャ拡大時の補間設定
		m_pD3DDevice->SetSamplerState( countSampler , D3DSAMP_MAXANISOTROPY , 8 );					//  テクスチャの補間精度の設定
																									    
		//  テクスチャステージステートの設定														    
		m_pD3DDevice->SetTextureStageState( countSampler , D3DTSS_ALPHAOP , D3DTOP_SELECTARG1 );	//  アルファブレンディング処理(初期値はD3DTOP_SELECTARG1)
		m_pD3DDevice->SetTextureStageState( countSampler , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );		//  最初のアルファ引数(初期値はD3DTA_TEXTURE、テクスチャがない場合はD3DTA_DIFFUSE)
		m_pD3DDevice->SetTextureStageState( countSampler , D3DTSS_ALPHAARG2 , D3DTA_CURRENT );		//  ２番目のアルファ引数(初期値はD3DTA_CURRENT)

		//  α要素の設定
		m_pD3DDevice->SetTextureStageState( countSampler , D3DTSS_ALPHAOP , D3DTOP_MODULATE );
		m_pD3DDevice->SetTextureStageState( countSampler , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
		m_pD3DDevice->SetTextureStageState( countSampler , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );
	}

	//  レンダーターゲット用の空のテクスチャ生成
	D3DXCreateTexture( m_pD3DDevice ,					//  デバイス
					   SCREEN_WIDTH ,					//  スクリーンの幅
					   SCREEN_HEIGHT ,					//  スクリーンの高さ
					   1 ,								//  ミップマップの数
					   D3DUSAGE_RENDERTARGET ,			//  使用用途
					   D3DFMT_A8R8G8B8 ,				//  色の要素
					   D3DPOOL_DEFAULT ,				//  メモリの配置方法
					   &m_rendererTargetTexture );		//	テクスチャ格納場所

	//  サーフェイスの取得
	m_rendererTargetTexture->GetSurfaceLevel( 0 , &m_rendererTargetSurface );

	//  レンダーターゲットの取得
	m_pD3DDevice->GetRenderTarget( 0 , &m_backBufferSurface );

	// テクスチャへのレンダリングに使う深度バッファーの作成
	if( FAILED( m_pD3DDevice->CreateDepthStencilSurface( SCREEN_WIDTH ,
														 SCREEN_HEIGHT ,
														 D3DFMT_D24S8,
														 D3DMULTISAMPLE_NONE,
														 0,
														 TRUE,
														 &m_backBufferDepth,
														 NULL ) ) )
	{
		return E_FAIL;
	}
						

#ifdef _DEBUG	

	// デバッグ情報表示用フォントの生成
	D3DXCreateFont( m_pD3DDevice , 18 , 0 , 0 , 0 , FALSE, SHIFTJIS_CHARSET ,
				    OUT_DEFAULT_PRECIS , DEFAULT_QUALITY , DEFAULT_PITCH  , "Terminal" , &m_pFont );

#endif

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  レンダラークラスの終了処理
//--------------------------------------------------------------------------------------
void Renderer::Uninit( void )
{
	//  デバイスの破棄
	if( m_pD3DDevice != NULL )
	{
		m_pD3DDevice->Release( );
		m_pD3DDevice = NULL;
	}

	//  Direct3Dオブジェクトの破棄
	if( m_pD3D != NULL )
	{
		m_pD3D->Release( );
		m_pD3D = NULL;
	}

	//  バックバッファの破棄
	if( m_backBufferSurface != nullptr )
	{
		m_backBufferSurface->Release( );
		m_backBufferSurface = nullptr;
	}

	//  バックバッファの破棄
	if( m_backBufferDepth != nullptr )
	{
		m_backBufferDepth->Release( );
		m_backBufferDepth = nullptr;
	}

	//  レンダーターゲット用のサーフェイスの破棄
	if( m_rendererTargetSurface != nullptr )
	{
		m_rendererTargetSurface->Release( );
		m_rendererTargetSurface = nullptr;
	}

	//  レンダーターゲット用のテクスチャの破棄
	if( m_rendererTargetTexture != nullptr )
	{
		m_rendererTargetTexture->Release( );
		m_rendererTargetTexture = nullptr;
	}

#ifdef _DEBUG

	//  デバッグ情報表示用フォントの破棄
	if( m_pFont != NULL )
	{
		m_pFont->Release( );
		m_pFont = NULL;
	}

#endif
}

//--------------------------------------------------------------------------------------
//  レンダラークラスの更新処理
//--------------------------------------------------------------------------------------
void Renderer::Update( bool bUpdate )
{
	//  更新する状態の場合
	if( bUpdate == true )
	{
		//  ポリゴンの更新
		Scene::UpdateAll( );
	}

	//  ポリゴンの更新
	Scene::UpdateAllDelete( );
}

//--------------------------------------------------------------------------------------
//  レンダラークラスの描画開始処理
//--------------------------------------------------------------------------------------
void Renderer::DrawBegin( void )
{
	//  Direct3Dによる描画の開始
	m_pD3DDevice->BeginScene( );
}

//--------------------------------------------------------------------------------------
//  レンダラークラスのバッファのクリア処理
//--------------------------------------------------------------------------------------
void Renderer::DrawClearBuffer( void )
{
	//  バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear( 0 , NULL , ( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ) , D3DCOLOR_RGBA( 0 , 0 , 0 , 255 ) , 1.0f , 0 );
}

//--------------------------------------------------------------------------------------
//  レンダラークラスの描画終了処理
//--------------------------------------------------------------------------------------
void Renderer::DrawEnd( void )
{
	//  Direct3Dによる描画の終了
	m_pD3DDevice->EndScene( );
}

//--------------------------------------------------------------------------------------
//  レンダラークラスの描画終了処理
//--------------------------------------------------------------------------------------
void Renderer::DrawEndPresent( void )
{
	//  Direct3Dによる描画の終了
	m_pD3DDevice->EndScene( );

	//  バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

//--------------------------------------------------------------------------------------
//  レンダーターゲットの設定処理
//--------------------------------------------------------------------------------------
void Renderer::SetRendererTarget( RENDERE_TARGET target )
{
	if( target == Renderer::RENDERE_TARGET::BACKBUFFER )
	{
		if( m_backBufferSurface != nullptr )
		{
			m_pD3DDevice->SetRenderTarget( 0 , m_backBufferSurface );
		}
		
	}
	else if( target == Renderer::RENDERE_TARGET::SURFACE )
	{
		if( m_rendererTargetSurface != nullptr )
		{
			m_pD3DDevice->SetRenderTarget( 0 , m_rendererTargetSurface );
		}
	}
}

//--------------------------------------------------------------------------------------
//  バックバッファの深度バッファ設定処理
//--------------------------------------------------------------------------------------
void Renderer::SetBackBufferDepth( void )
{
	m_pD3DDevice->SetDepthStencilSurface( m_backBufferDepth );
}

//--------------------------------------------------------------------------------------
//  レンダーターゲットのテクスチャ取得
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 Renderer::GetRendereTargetTexture( void )
{
	return m_rendererTargetTexture;
}

//--------------------------------------------------------------------------------------
//  デバイス情報を渡す関数
//--------------------------------------------------------------------------------------
LPDIRECT3DDEVICE9 Renderer::GetDevice( void )
{
	return m_pD3DDevice;
}

//--------------------------------------------------------------------------------------
//  描画形式の変更をする関数
//--------------------------------------------------------------------------------------
void Renderer::ChangeFillMode( void )
{
	//  フラグの切り替え
	m_wireFrame = !m_wireFrame;

	if( m_wireFrame == false )
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE , D3DFILL_SOLID );	
	}
	else
	{
		// ワイヤーフレーム描画
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE , D3DFILL_WIREFRAME );	
	}
}

#ifdef _DEBUG

//--------------------------------------------------------------------------------------
//  FPS表示
//--------------------------------------------------------------------------------------
void Renderer::DrawFPS( void )
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[ 256 ];

	//  FPS情報の取得
	m_nCountFPS = GetFPS( );

	wsprintf( str, "FPS:%d\n", m_nCountFPS );

	//  テキスト描画
	m_pFont->DrawText( NULL , str , -1 , &rect , DT_LEFT , D3DXCOLOR( 0.0f , 1.0f , 0.0f, 1.0f ) );
}

#endif