//--------------------------------------------------------------------------------------
//  管理プログラム   ( manager.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "input.h"
#include "keyboard.h"
#include "xboxController.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneModel.h"
#include "light.h"
#include "teamLogo.h"
#include "title.h"
#include "game.h"
#include "stageSelect.h"
#include "result.h"
#include "joystick.h"
#include "sound.h"
#include "effekseerManager.h"
#include "fade.h"
#include "tutorial.h"
#include "test.h"
#include "Wwise.h"
#include "shaderManager.h"
#include "player.h"
#include "depthShadow.h"
#include "cascadeShadow.h"
#include "debugManager.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
Renderer*				SceneManager::m_pRenderer = NULL;							//  シーンクラスのポインタ
Keyboard*				SceneManager::m_pKeyboard = NULL;							//  キーボードクラスのポインタ
XboxController*			SceneManager::m_pXboxInput = NULL;							//  Xboxコントローラークラスのポインタ
PS4Controller*			SceneManager::m_pPS4Input = NULL;							//  PS4コントローラークラスのポインタ
Texture*				SceneManager::m_pTexture = NULL;							//  テクスチャクラスのポインタ
Camera*					SceneManager::m_pCamera[ MAX_CAMERA_NUMBER ] = { NULL };	//  カメラクラスのポインタ
Light*					SceneManager::m_pLight = NULL;								//  ライトクラスのポインタ
Sound*					SceneManager::m_pSound = NULL;								//  サウンドクラスのポインタ
Wwise*					SceneManager::m_pWwise = NULL;								//  Wwiseクラスのポインタ
WWISE_GAMEOBJ*			SceneManager::m_wwiseObject = NULL;							//  Wwiseオブジェクト  
Fade*					SceneManager::m_pFade = NULL;								//  フェードクラスのポインタ
Mode*					SceneManager::m_pMode = NULL;								//  モードクラスのポインタ
Mode::MODE				SceneManager::m_mode = Mode::MODE::STAGE_SELECT;			//  モード
SceneManager::STAGE		SceneManager::m_stage = STAGE_MAX;							//  ステージの種類
int						SceneManager::m_nLoop = 0;									//  ループ数

//--------------------------------------------------------------------------------------
//  管理クラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneManager::SceneManager( )
{

}

//--------------------------------------------------------------------------------------
//  管理クラスのデストラクタ
//--------------------------------------------------------------------------------------
SceneManager::~SceneManager( )
{

}

//--------------------------------------------------------------------------------------
//  管理クラスの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SceneManager::Init( HINSTANCE hInstance , HWND hWnd, bool bWindow )
{
	//  クラスポインタの初期化
	m_pRenderer = NULL;
	m_pTexture = NULL;
	m_pKeyboard = NULL;
	m_pPS4Input = NULL;
	m_pLight = NULL;
	m_pSound = NULL;
	m_pWwise = NULL;
	m_pFade = NULL;
	m_nLoop = 0;

	for( int countCamera = 0; countCamera < MAX_CAMERA_NUMBER; ++countCamera )
	{
		m_pCamera[ countCamera ] = NULL;
	}

	//  テクスチャクラスポインタが空の場合
	if( m_pTexture == NULL )
	{
		//  テクスチャクラスの生成
		m_pTexture = new Texture;
		m_pTexture->Init( );
	}

	//  メッセージの表示
	UINT nID = MessageBox( NULL , "フルスクリーンモードで起動しますか?" , "Full Screen" , MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );

	//  はいと答えた場合
	if( nID == IDYES )
	{
		bWindow = FALSE;
	}

	//	ポインタが空の場合
	if( m_pRenderer == NULL )
	{
		//  レンダラーの生成
		m_pRenderer = new Renderer;

		//  レンダラーの初期化
		if( FAILED( m_pRenderer->Init( hWnd , bWindow ) ) )
		{
			return E_FAIL;
		}
	}

	//  エフェクシア管理クラスの初期化
	EffekseerManager::Init( );

	//  シェーダー管理クラスの初期化
	ShaderManager::Init( );

	//  デプスシャドウの初期化
	DepthShadow::Init( );

	//  カスケードシャドウの初期化
	CascadeShadow::Init( );

	//  キーボードクラスポインタが空の場合
	if( m_pKeyboard == NULL )
	{
		//  キーボードクラスの生成
		m_pKeyboard = new Keyboard;

		//  キーボードの初期化
		if( FAILED( m_pKeyboard->Init( hInstance , hWnd ) ) )
		{
			return E_FAIL;
		}
	}

	//  Xboxコントローラークラスポインタが空の場合
	if( m_pXboxInput == NULL )
	{
		//  Xboxコントローラークラスの生成
		m_pXboxInput = new XboxController;

		//  Xboxコントローラーの初期化
		if( FAILED( m_pXboxInput->Init( ) ) )
		{

		}
	}

	//  PS4コントローラークラスポインタが空の場合
	if( m_pPS4Input == NULL )
	{
		//  PS4コントローラークラスの生成
		m_pPS4Input = new PS4Controller;

		//  PS4コントローラーの初期化
		if( FAILED( m_pPS4Input->Init( hInstance , hWnd ) ) )
		{
			//return E_FAIL;
		}
	}

	//  カメラクラスの生成
	for( int countCamera = 0; countCamera < MAX_CAMERA_NUMBER; ++countCamera )
	{
		if( m_pCamera[ countCamera ] == NULL )
		{
			m_pCamera[ countCamera ] = new Camera;
		}
	}

	//  ライトクラスポインタが空の場合
	if( m_pLight == NULL )
	{
		//  ライトクラスの生成
		m_pLight = new Light;
		m_pLight->Init( );
	}

	//  Wwiseクラスポインタが空の場合
	if( m_pWwise == NULL )
	{
		//  Wwiseクラスの生成
		m_pWwise = new Wwise;

		//  Wwiseの初期化
		m_pWwise->Init( );
	}

	//  フェードクラスポインタが空の場合
	if( m_pFade == NULL )
	{
		//  フェードの生成
		m_pFade = Fade::Create( );
	}

	//  デバッグ管理クラスの初期化
	DebugManager::Init( );

	//  モードの設定
	SetMode( m_mode );

	m_vertexBuffer = nullptr;

	LPDIRECT3DDEVICE9 device = m_pRenderer->GetDevice( );

	//  頂点バッファの作成
	if( FAILED( device->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX ,		//  作成したい頂点バッファのサイズ
										    D3DUSAGE_WRITEONLY ,					//  使用方法
										    0 ,										//  
										    D3DPOOL_MANAGED ,						//  メモリ管理方法( MANAGED → お任せ )
										    &m_vertexBuffer ,						//  バッファ
										    NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return E_FAIL;
	}

	VERTEX_2D* pVtx = NULL;				//  頂点バッファのポインタ

	if( m_vertexBuffer != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_vertexBuffer->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
							  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
							  0 );										//  ロックの種類

		//  頂点座標の設定( 3D座標 ・ 右回り )
		pVtx[ 0 ].position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 1 ].position = D3DXVECTOR3( SCREEN_WIDTH , 0.0f , 0.0f );
		pVtx[ 2 ].position = D3DXVECTOR3( 0.0f , SCREEN_HEIGHT , 0.0f );
		pVtx[ 3 ].position = D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f );

		//  法線の指定
		pVtx[ 0 ].rhw = 1.0f;
		pVtx[ 1 ].rhw = 1.0f;
		pVtx[ 2 ].rhw = 1.0f;
		pVtx[ 3 ].rhw = 1.0f;

		//  頂点色の設定( 0 ～ 255 の整数値 )
		pVtx[ 0 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
		pVtx[ 1 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
		pVtx[ 2 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
		pVtx[ 3 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

		//  UV座標の指定
		pVtx[ 0 ].texcoord = D3DXVECTOR2( 0.0f , 0.0f );
		pVtx[ 1 ].texcoord = D3DXVECTOR2( 1.0f , 0.0f );
		pVtx[ 2 ].texcoord = D3DXVECTOR2( 0.0f , 1.0f );
		pVtx[ 3 ].texcoord = D3DXVECTOR2( 1.0f , 1.0f );

		//  頂点バッファのアンロック
		m_vertexBuffer->Unlock( );
	}

	m_shadowMap = nullptr;

	//  頂点バッファの作成
	if( FAILED( device->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX ,		//  作成したい頂点バッファのサイズ
										    D3DUSAGE_WRITEONLY ,					//  使用方法
										    0 ,										//  
										    D3DPOOL_MANAGED ,						//  メモリ管理方法( MANAGED → お任せ )
										    &m_shadowMap ,							//  バッファ
										    NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return E_FAIL;
	}

	pVtx = NULL;				//  頂点バッファのポインタ

	if( m_shadowMap != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_shadowMap->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
						   ( void** )&pVtx ,						//  アドレスが書かれたメモ帳のアドレス
						   0 );										//  ロックの種類

		//  頂点座標の設定( 3D座標 ・ 右回り )
		pVtx[ 0 ].position = D3DXVECTOR3( SCREEN_WIDTH * 0.84f , SCREEN_HEIGHT * 0.01f , 0.0f );
		pVtx[ 1 ].position = D3DXVECTOR3( SCREEN_WIDTH * 0.99f , SCREEN_HEIGHT * 0.01f , 0.0f );
		pVtx[ 2 ].position = D3DXVECTOR3( SCREEN_WIDTH * 0.84f , SCREEN_HEIGHT * 0.16f , 0.0f );
		pVtx[ 3 ].position = D3DXVECTOR3( SCREEN_WIDTH * 0.99f , SCREEN_HEIGHT * 0.16f , 0.0f );

		//  法線の指定
		pVtx[ 0 ].rhw = 1.0f;
		pVtx[ 1 ].rhw = 1.0f;
		pVtx[ 2 ].rhw = 1.0f;
		pVtx[ 3 ].rhw = 1.0f;

		//  頂点色の設定( 0 ～ 255 の整数値 )
		pVtx[ 0 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
		pVtx[ 1 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
		pVtx[ 2 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
		pVtx[ 3 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

		//  UV座標の指定
		pVtx[ 0 ].texcoord = D3DXVECTOR2( 0.0f , 0.0f );
		pVtx[ 1 ].texcoord = D3DXVECTOR2( 1.0f , 0.0f );
		pVtx[ 2 ].texcoord = D3DXVECTOR2( 0.0f , 1.0f );
		pVtx[ 3 ].texcoord = D3DXVECTOR2( 1.0f , 1.0f );

		//  頂点バッファのアンロック
		m_shadowMap->Unlock( );
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  管理クラスの終了処理
//--------------------------------------------------------------------------------------
void SceneManager::Uninit( void )
{
	//  デバッグ管理クラスの終了
	DebugManager::Uninit( );

	//  カスケードシャドウの初期化
	CascadeShadow::Uninit( );

	//  デプスシャドウの終了
	DepthShadow::Uninit( );

	//  エフェクシア管理クラスの終了
	EffekseerManager::Uninit( );

	//  シェーダー管理クラスの終了
	ShaderManager::Uninit( );

	if( m_vertexBuffer != NULL )
	{
		m_vertexBuffer->Release( );
		m_vertexBuffer = nullptr;
	}

	//  フェードクラスポインタが空ではない場合
	if( m_pFade != NULL )
	{
		//  フェードの終了処理
		m_pFade->Uninit( );
		delete m_pFade;
		m_pFade = NULL;
	}

	//  モードクラスの終了処理
	if( m_pMode != NULL )
	{
		m_pMode->Uninit( );
		delete m_pMode;
		m_pMode = NULL;
	}

	//  レンダラークラスの終了処理
	if( m_pRenderer != NULL )
	{
		m_pRenderer->Uninit( );
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//  テクスチャクラスポインタが空ではない場合
	if( m_pTexture != NULL )
	{
		//  テクスチャクラスの破棄
		m_pTexture->Uninit( );
		delete m_pTexture;
		m_pTexture = NULL;
	}

	//  キーボードクラスポインタが空ではない場合
	if( m_pKeyboard != NULL )
	{
		//  キーボードクラスの破棄
		m_pKeyboard->Uninit( );
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	//  Xboxコントローラークラスポインタが空ではない場合
	if( m_pXboxInput != NULL )
	{
		//  Xboxコントローラークラスの破棄
		m_pXboxInput->Uninit( );
		delete m_pXboxInput;
		m_pXboxInput = NULL;
	}

	//  PS4コントローラークラスポインタが空ではない場合
	if( m_pPS4Input != NULL )
	{
		//  PS4コントローラークラスの破棄
		m_pPS4Input->Uninit( );
		delete m_pPS4Input;
		m_pPS4Input = NULL;
	}

	//  Wwiseクラスポインタが空ではない場合
	if( m_pWwise != NULL )
	{
		//  Wwiseの破棄
		m_pWwise->Uninit( );
		delete m_pWwise;
		m_pWwise = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  管理クラスの更新処理
//--------------------------------------------------------------------------------------
void SceneManager::Update( void )
{
	//  キーボードクラスポインタが空ではない場合
	if( m_pKeyboard != NULL )
	{
		m_pKeyboard->Update( );
	}

	//  Xboxコントローラークラスポインタが空ではない場合
	if( m_pXboxInput != NULL )
	{
		m_pXboxInput->Update( );
	}

	//  PS4コントローラークラスポインタが空ではない場合
	if( m_pPS4Input != NULL )
	{
		m_pPS4Input->Update( );
	}

	//  レンダラークラスポインタが空ではない場合
	if( m_pRenderer != NULL )
	{
		m_pRenderer->Update( true );
	}

	//  モードクラスポインタが空ではない場合
	if( m_pMode != NULL )
	{
		m_pMode->Update( );
	}

	//  エフェクシアの更新
	EffekseerManager::Update( );

	//  フェードクラスポインタが空ではない場合
	if( m_pFade != NULL )
	{
		//  フェードの更新
		m_pFade->Update( );
	}

	//  Wwiseクラスポインタが空ではない場合
	if( m_pWwise != NULL )
	{
		//  Wwiseの更新
		m_pWwise->Update( );
	}

	//  描画形式の変更
	if( m_pKeyboard->GetKeyboardTrigger( DIK_F4 ) )
	{
		m_pRenderer->ChangeFillMode( );
	}

//#ifdef _DEBUG

	//if( m_mode == Mode::TEST )
	{
		//  デバッグ管理クラスの更新
		DebugManager::Update( );
	}

//#endif

	if( Fade::GetFade( ) == Fade::FADE_NONE )
	{
		if( m_pKeyboard->GetKeyboardTrigger( DIK_F1 ) )
		{
			//  フェードの設定
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE::TITLE , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
		else if( m_pKeyboard->GetKeyboardTrigger( DIK_F2 ) )
		{
			//  フェードの設定
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE::STAGE_SELECT , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
		else if( m_pKeyboard->GetKeyboardTrigger( DIK_F3 ) )
		{
			//  フェードの設定
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE::TEST , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
	}
}

//--------------------------------------------------------------------------------------
//  管理クラスの描画処理
//--------------------------------------------------------------------------------------
void SceneManager::Draw( void )
{
	//  レンダラークラスが空ではない場合
	if( m_pRenderer != NULL )
	{
		//  ループ数の初期化
		m_nLoop = 0;

		//  モードゲーム以外または、CPU対戦の場合
		if( ( Game::GetModeVS( ) == Game::MODE_VS_CPU && m_mode == Mode::MODE::GAME ) ||
			m_mode != Mode::MODE::GAME )
		{
			//  レンダーターゲットの変更
			DepthShadow::SetRendererTarget( );

			//  深度バッファの設定
			DepthShadow::SetDepthSerface( );

			//  シャドウマップの深度バッファ値の初期化
			DepthShadow::DrawClearBuffer( );

			D3DVIEWPORT9 vp;

			vp.X = 0;
			vp.Y = 0;
			vp.Width = DepthShadow::TEXTURE_WIDTH;
			vp.Height = DepthShadow::TEXTURE_HEIGHT;
			vp.MinZ = 0.0f;
			vp.MaxZ = 1.0f;

			//  ビューポート変換の設定
			m_pRenderer->GetDevice( )->SetViewport( &vp );

			//  描画開始
			m_pRenderer->DrawBegin( );

			//  深度バッファの書き込み
			Scene::DrawDepthAll( );

			//  描画終了
			m_pRenderer->DrawEnd( );

			//  エフェクシアの行列を設定する関数
			EffekseerManager::SetMatrix( 0 );

			//  サーフェイスにレンダーターゲットを設定
			m_pRenderer->SetRendererTarget( Renderer::RENDERE_TARGET::SURFACE );

			//  バックバッファ用の深度バッファ設定
			m_pRenderer->SetBackBufferDepth( );

			//  バックバッファ＆Ｚバッファのクリア
			m_pRenderer->DrawClearBuffer( );

			//  カメラのビューポート行列の設定
			if( m_pCamera[ 0 ] != nullptr )
			{	
				m_pCamera[ 0 ]->SetCamera( );
			}

			//  描画開始
			m_pRenderer->DrawBegin( );

			//  モードクラスポインタが空ではない場合
			if( m_pMode != NULL )
			{
				m_pMode->Draw( );
			}

			//  描画終了
			m_pRenderer->DrawEnd( );

			//  バックバッファにレンダーターゲットを設定
			m_pRenderer->SetRendererTarget( Renderer::RENDERE_TARGET::BACKBUFFER );

			// バックバッファ＆Ｚバッファのクリア
			m_pRenderer->DrawClearBuffer( );

			//  デバイス情報の取得
			LPDIRECT3DDEVICE9 pDevice = m_pRenderer->GetDevice( );

			//  描画開始
			m_pRenderer->DrawBegin( );

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource( 0 ,								//  パイプライン番号
									  m_vertexBuffer ,					//  頂点バッファのアドレス
									  0 ,								//  オフセット( byte )
									  sizeof( VERTEX_2D ) );			//  一個分の頂点データのサイズ( ストライド )

			// 頂点フォーマットの設定
			pDevice->SetFVF( FVF_VERTEX_2D );

			if( m_pRenderer != nullptr )
			{
				// テクスチャの設定
				pDevice->SetTexture( 0 , m_pRenderer->GetRendereTargetTexture( ) ); 
			}
			else
			{
				// テクスチャの設定
				pDevice->SetTexture( 0 , NULL ); 
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
									0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
									NUM_POLYGON );						//  プリミティブ数

//#ifdef _DEBUG

			//if( m_mode == Mode::TEST )
			{
				// 頂点バッファをデータストリームに設定
				pDevice->SetStreamSource( 0 ,								//  パイプライン番号
										  m_shadowMap ,						//  頂点バッファのアドレス
										  0 ,								//  オフセット( byte )
										  sizeof( VERTEX_2D ) );			//  一個分の頂点データのサイズ( ストライド )

				// 頂点フォーマットの設定
				pDevice->SetFVF( FVF_VERTEX_2D );

				if( DepthShadow::GetRendereTargetTexture( ) != nullptr )
				{
					// テクスチャの設定
					pDevice->SetTexture( 0 , DepthShadow::GetRendereTargetTexture( ) ); 
				}
				else
				{
					// テクスチャの設定
					pDevice->SetTexture( 0 , NULL );
				}

				// ポリゴンの描画
				pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
										0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
										NUM_POLYGON );						//  プリミティブ数
			}

//#endif

			//  フェードクラスポインタが空ではない場合
			if( m_pFade != NULL )
			{
				//  フェードの描画
				m_pFade->Draw( );
			}

//#ifdef _DEBUG

			//if( m_mode == Mode::TEST )
			{
				//  デバッグ管理クラスの描画
				DebugManager::Draw( );
			}

//#endif

			//  描画終了
			m_pRenderer->DrawEndPresent( );
		}
		//  プレイヤー対戦の場合
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER &&
				 m_mode == Mode::MODE::GAME )
		{
			//  レンダーターゲットの変更
			DepthShadow::SetRendererTarget( );

			//  深度バッファの設定
			DepthShadow::SetDepthSerface( );

			//  シャドウマップの深度バッファ値の初期化
			DepthShadow::DrawClearBuffer( );

			D3DVIEWPORT9 vp;

			vp.X = 0;
			vp.Y = 0;
			vp.Width = DepthShadow::TEXTURE_WIDTH;
			vp.Height = DepthShadow::TEXTURE_HEIGHT;
			vp.MinZ = 0.0f;
			vp.MaxZ = 1.0f;

			//  ビューポート変換の設定
			m_pRenderer->GetDevice( )->SetViewport( &vp );

			//  描画開始
			m_pRenderer->DrawBegin( );

			//  深度バッファの書き込み
			Scene::DrawDepthAll( );

			//  描画終了
			m_pRenderer->DrawEnd( );

			//  サーフェイスにレンダーターゲットを設定
			m_pRenderer->SetRendererTarget( Renderer::RENDERE_TARGET::SURFACE );

			//  バックバッファ用の深度バッファ設定
			m_pRenderer->SetBackBufferDepth( );

			//  バックバッファ＆Ｚバッファのクリア
			m_pRenderer->DrawClearBuffer( );

			//  描画開始
			m_pRenderer->DrawBegin( );

			for( int i = 0; i < 2; i++ )
			{
				//  カメラのビューポート行列の設定
				if( m_pCamera[ i ] != nullptr )
				{
					m_pCamera[ i ]->SetCamera( );
				}

				//  エフェクシアの行列を設定する関数
				EffekseerManager::SetMatrix( i );

				//  モードクラスポインタが空ではない場合
				if( m_pMode != NULL )
				{
					m_pMode->Draw( );
				}

				//  ループ数のカウント
				m_nLoop++;
			}

			//  描画終了
			m_pRenderer->DrawEnd( );

			//  バックバッファにレンダーターゲットを設定
			m_pRenderer->SetRendererTarget( Renderer::RENDERE_TARGET::BACKBUFFER );

			// バックバッファ＆Ｚバッファのクリア
			m_pRenderer->DrawClearBuffer( );

			//  描画開始
			m_pRenderer->DrawBegin( );

			//  デバイス情報の取得
			LPDIRECT3DDEVICE9 pDevice = m_pRenderer->GetDevice( );

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource( 0 ,								//  パイプライン番号
									  m_vertexBuffer ,					//  頂点バッファのアドレス
									  0 ,								//  オフセット( byte )
									  sizeof( VERTEX_2D ) );			//  一個分の頂点データのサイズ( ストライド )

			// 頂点フォーマットの設定
			pDevice->SetFVF( FVF_VERTEX_2D );

			if( m_pRenderer != nullptr )
			{
				// テクスチャの設定
				pDevice->SetTexture( 0 , m_pRenderer->GetRendereTargetTexture( ) ); 
			}
			else
			{
				// テクスチャの設定
				pDevice->SetTexture( 0 , NULL ); 
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
									0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
									NUM_POLYGON );						//  プリミティブ数

#ifdef _DEBUG

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource( 0 ,								//  パイプライン番号
									  m_shadowMap ,						//  頂点バッファのアドレス
									  0 ,								//  オフセット( byte )
									  sizeof( VERTEX_2D ) );			//  一個分の頂点データのサイズ( ストライド )

			// 頂点フォーマットの設定
			pDevice->SetFVF( FVF_VERTEX_2D );

			if( DepthShadow::GetRendereTargetTexture( ) != nullptr )
			{
				// テクスチャの設定
				pDevice->SetTexture( 0 , DepthShadow::GetRendereTargetTexture( ) ); 
			}
			else
			{
				// テクスチャの設定
				pDevice->SetTexture( 0 , NULL ); 
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
									0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
									NUM_POLYGON );						//  プリミティブ数

#endif

			//  フェードクラスポインタが空ではない場合
			if( m_pFade != NULL )
			{
				//  フェードの描画
				m_pFade->Draw( );
			}

#ifdef _DEBUG

			//  デバッグ管理クラスの終了
			DebugManager::Draw( );

#endif

			//  描画終了
			m_pRenderer->DrawEndPresent( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  管理クラスの描画処理
//--------------------------------------------------------------------------------------
void SceneManager::SetMode( Mode::MODE mode )
{
	/* 終了処理  */

	//  ゲームの終了
	if( NULL != m_pMode )
	{
		//  エフェクシア全ての停止
		EffekseerManager::StopAll( );

		m_pMode->Uninit( );
		m_pMode = NULL;
	}

	/*  初期化処理  */

	//  次のモードによっての場合分け
	switch( mode )
	{
		case Mode::MODE::TEAM_LOGO:
		{
			//  タイトルの初期化
			if( NULL == m_pMode )
			{
				m_pMode = new TeamLogo;
				m_pMode->Init( );
			}

			break;
		}
		case Mode::MODE::TITLE:
		{
			//  タイトルの初期化
			if( NULL == m_pMode )
			{
				m_pMode = new Title;
				m_pMode->Init( );
			}

			break;
		}
		case Mode::MODE::TUTORIAL:
		{
			//  チュートリアルの初期化
			m_pMode = new Tutorial;
			m_pMode->Init( );

			break;
		}
		case Mode::MODE::STAGE_SELECT:
		{
			//  ステージセレクトの初期化
			m_pMode = new StageSelect;
			m_pMode->Init( );

			break;
		}
		case Mode::MODE::GAME:
		{
			//  ゲームの初期化
			m_pMode = new Game( );
			m_pMode->Init( );

			break;
		}
		case Mode::MODE::RESULT:
		{
			//  結果の初期化
			m_pMode = new Result;
			m_pMode->Init( );

			break;
		}
		case Mode::MODE::TEST:
		{
			//  結果の初期化
			m_pMode = new Test;
			m_pMode->Init( );

			break;
		}
		default :
		{
			MessageBox( NULL , "正常な値が入っていません。" , "エラー" , MB_OK );

			break;
		}
	}

	//  現在のモードに次のモードを代入
	m_mode = mode;
}

//--------------------------------------------------------------------------------------
//  レンダラークラスを取得をする関数
//--------------------------------------------------------------------------------------
Renderer* SceneManager::GetRenderer( void )
{
	if( m_pRenderer == NULL )
	{
		return NULL;
	}

	return m_pRenderer;
}

//--------------------------------------------------------------------------------------
//  テクスチャクラスを取得をする関数
//--------------------------------------------------------------------------------------
Texture* SceneManager::GetTexture( void )
{
	if( m_pTexture == NULL )
	{
		return NULL;
	}

	return m_pTexture;
}

//--------------------------------------------------------------------------------------
//  キーボードクラスを取得をする関数
//--------------------------------------------------------------------------------------
Keyboard* SceneManager::GetKeyboard( void )
{
	if( m_pKeyboard == NULL )
	{
		return NULL;
	}

	return m_pKeyboard;
}

//--------------------------------------------------------------------------------------
//  Xboxコントローラークラスを取得をする関数
//--------------------------------------------------------------------------------------
XboxController* SceneManager::GetXInput( void )
{
	if( m_pXboxInput == NULL )
	{
		return NULL;
	}

	return m_pXboxInput;
}

//--------------------------------------------------------------------------------------
//  PS4コントローラークラスを取得をする関数
//--------------------------------------------------------------------------------------
PS4Controller* SceneManager::GetPS4Input( void )
{
	if( m_pPS4Input == NULL )
	{
		return NULL;
	}

	return m_pPS4Input;
}

//--------------------------------------------------------------------------------------
//  指定番号のカメラクラスの取得をする関数
//--------------------------------------------------------------------------------------
Camera*	SceneManager::GetCamera( int cameraNo )
{
	if( m_pCamera[ cameraNo ] == NULL || cameraNo >= MAX_CAMERA_NUMBER )
	{
		return NULL;
	}

	return m_pCamera[ cameraNo ];
}

//--------------------------------------------------------------------------------------
//  ライトクラスを取得をする関数
//--------------------------------------------------------------------------------------
Light* SceneManager::GetLight( void )
{
	if( m_pLight == NULL )
	{
		return NULL;
	}

	return m_pLight;
}

//--------------------------------------------------------------------------------------
//  モード情報の取得をする関数
//--------------------------------------------------------------------------------------
Mode::MODE SceneManager::GetMode( void )
{
	return m_mode;
}

//--------------------------------------------------------------------------------------
//  モード情報の取得をする関数
//--------------------------------------------------------------------------------------
Mode* SceneManager::GetModeClass( void )
{
	return m_pMode;
}

//--------------------------------------------------------------------------------------
//  ステージの種類を設定する関数
//--------------------------------------------------------------------------------------
void SceneManager::SetStage( SceneManager::STAGE stage )
{
	m_stage = stage;
}

//--------------------------------------------------------------------------------------
//  ステージの種類を取得をする関数
//--------------------------------------------------------------------------------------
SceneManager::STAGE SceneManager::GetStage( void )
{
	return m_stage;
}

//--------------------------------------------------------------------------------------
//  サウンド情報の取得をする関数
//--------------------------------------------------------------------------------------
Sound* SceneManager::GetSound( void )
{
	return m_pSound;
}

//--------------------------------------------------------------------------------------
//  Wwise情報の取得をする関数
//--------------------------------------------------------------------------------------
Wwise* SceneManager::GetWwise( void )
{
	return m_pWwise;
}

//--------------------------------------------------------------------------------------
//  Wwiseオブジェクト情報の取得をする関数
//--------------------------------------------------------------------------------------
WWISE_GAMEOBJ* SceneManager::GetWwiseObject( void )
{
	return m_wwiseObject;
}

//--------------------------------------------------------------------------------------
//  描画ループ数を取得をする関数
//--------------------------------------------------------------------------------------
int SceneManager::GetLoop( void )
{
	return m_nLoop;
}