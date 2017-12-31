//--------------------------------------------------------------------------------------
//  チュートリアルプログラム   ( tutorial.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "tutorial.h"
#include "manager.h"
#include "sceneBG.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "fade.h"
#include "renderer.h"
#include "camera.h"
#include "meshDome.h"
#include "effekseerManager.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define TITLE_PRESS_POS_X				( SCREEN_WIDTH * 0.5f )				//  種類000座標X
#define TITLE_PRESS_POS_Y				( SCREEN_HEIGHT * 0.75f )			//  種類000座標Y
#define TITLE_PRESS_SIZE_X				( 600.0f )							//  種類000サイズX
#define TITLE_PRESS_SIZE_Y				( 180.0f )							//  種類000サイズY

static const float	ATMOSPHERE_SIZE = 15.0f; 
static const int	ATMOSPHERE_NUM = 2;

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
bool	Tutorial::m_bPush = false;							//  ボタンを押したかどうか

//--------------------------------------------------------------------------------------
//  チュートリアルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Tutorial::Tutorial( )
{

}

//--------------------------------------------------------------------------------------
//  チュートリアルクラスのデストラクタ
//--------------------------------------------------------------------------------------
Tutorial::~Tutorial( )
{

}

//--------------------------------------------------------------------------------------
//  チュートリアルクラスの初期化処理
//--------------------------------------------------------------------------------------
void Tutorial::Init( void )
{
	m_bPush = false;

	//  背景
	SceneBG::Create( SceneBG::TYPE_TUTORIAL , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
					  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
					  D3DXVECTOR2( 1.0f , 1.0f ) );

	//  クラスポインタの初期化
	m_pCamera = NULL;

	//  カメラクラスポインタが空の場合
	if( m_pCamera == NULL )
	{
		//  カメラクラスの生成
		m_pCamera = new Camera;
		m_pCamera->Init( D3DXVECTOR3( 0.0f , 30.0f , -250.0f ) , D3DXVECTOR3( 0.0f , 200.0f , 0.0f ) ,
						 D3DX_PI / 3.0f , 1.0f , 10000.0f );
	}

	//  メッシュドームの生成
	MeshDome::Create( MeshDome::TYPE_COSMO ,
					   D3DXVECTOR3( 0.0f , -10.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
					   D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.0001f , 0.0f ) ,
					   32 , 32 );
}

//--------------------------------------------------------------------------------------
//  チュートリアルクラスの終了処理
//--------------------------------------------------------------------------------------
void Tutorial::Uninit( void )
{
	//  オブジェクトクラスの全解放
	Scene::ReleaseAll( );

	//  カメラクラスポインタが空ではない場合
	if( m_pCamera != NULL )
	{
		//  カメラクラスの破棄
		m_pCamera->Uninit( );
		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  チュートリアルクラスの更新処理
//--------------------------------------------------------------------------------------
void Tutorial::Update( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  Xboxコントローラー情報の取得
	XboxController*	pXInput = SceneManager::GetXInput( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	if( Fade::GetFade( ) == Fade::FADE_NONE )
	{
		if( ( pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CIRCLE ) || pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_OPTIONS ) ) ||
			 pKeyboard->GetKeyboardTrigger( DIK_RETURN ) || pKeyboard->GetKeyboardTrigger( DIK_SPACE ) )
		{
			//  フェードの設定
			Fade::SetFade( Fade::FADE_OUT , Mode::MODE::STAGE_SELECT , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
		}
	}

	//  カメラクラスポインタが空ではない場合
	if( m_pCamera != NULL )
	{
		//  カメラのビューポート行列の設定
		m_pCamera->SetCamera( );
	}
}

//--------------------------------------------------------------------------------------
//  チュートリアルクラスの描画処理
//--------------------------------------------------------------------------------------
void Tutorial::Draw( void )
{
	Scene::DrawAll( );
}