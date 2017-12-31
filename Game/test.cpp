//--------------------------------------------------------------------------------------
//  テストプログラム   ( title.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "test.h"
#include "manager.h"
#include "sceneBG.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "fade.h"
#include "renderer.h"
#include "field.h"
#include "meshDome.h"
#include "titlePress.h"
#include "effekseerManager.h"
#include "meshField.h"
#include "objectFBX.h"
#include "sceneFBX.h"
#include "unityFBX.h"
#include "light.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define TITLE_PRESS_POS_X				( SCREEN_WIDTH * 0.5f )				//  種類000座標X
#define TITLE_PRESS_POS_Y				( SCREEN_HEIGHT * 0.8f )			//  種類000座標Y
#define TITLE_PRESS_SIZE_X				( 580.0f )							//  種類000サイズX
#define TITLE_PRESS_SIZE_Y				( 120.0f )							//  種類000サイズY

static const float	ATMOSPHERE_SIZE = 15.0f; 
static const int	ATMOSPHERE_NUM = 1;

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  テストクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Test::Test( )
{

}

//--------------------------------------------------------------------------------------
//  テストクラスのデストラクタ
//--------------------------------------------------------------------------------------
Test::~Test( )
{

}

//--------------------------------------------------------------------------------------
//  テストクラスの初期化処理
//--------------------------------------------------------------------------------------
void Test::Init( void )
{
	Camera* camera = SceneManager::GetCamera( 0 );

	//  カメラクラスポインタが空の場合
	if( camera != nullptr )
	{
		//  カメラクラスの初期化
		camera->Init( D3DXVECTOR3( 0.0f , 30.0f , -250.0f ) , D3DXVECTOR3( 0.0f , 200.0f , 0.0f ) ,
					  D3DX_PI / 3.0f , 1.0f , 10000.0f );
		camera->ChangeMode( );
	}

	Light* pLight = SceneManager::GetLight( );

	if( pLight != NULL )
	{
		pLight->SetDiffuseColor( D3DXCOLOR( 1.0f , 0.3f , 0.3f , 1.0f ) );
	}

	//  メッシュドームの生成
	MeshDome::Create( MeshDome::TYPE_COSMO ,
					  D3DXVECTOR3( 0.0f , -1000.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
					  D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.0001f , 0.0f ) ,
					  32 , 32 );

	//  メッシュフィールドの生成
	MeshField::Create( MeshField::TYPE_FIELD , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
					   D3DXVECTOR3( 1000.0f , 0.0f , 1000.0f ) , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) , 10 , 10 , 
					   D3DXVECTOR2( 0.0f , 0.0f ) );

	//  FBXの生成
	ObjectFBX::Create( D3DXVECTOR3( 0.0f , 0.0f , 100.0f ) , 0.05f );
	//UnityFBX::Create( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , 0.05f );

	for( int i = 0; i < ATMOSPHERE_NUM; i++ )
	{
		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		/////////////////////////////////////////////////////
		//  第1ライン
		/////////////////////////////////////////////////////

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE , 
								  D3DXVECTOR3( 0.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		/////////////////////////////////////////////////////
		//  第2ライン
		/////////////////////////////////////////////////////

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE , 
								  D3DXVECTOR3( 0.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );
	}
}

//--------------------------------------------------------------------------------------
//  テストクラスの終了処理
//--------------------------------------------------------------------------------------
void Test::Uninit( void )
{
	//  オブジェクトクラスの全解放
	Scene::ReleaseAll( );
}

//--------------------------------------------------------------------------------------
//  テストクラスの更新処理
//--------------------------------------------------------------------------------------
void Test::Update( void )
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

	Camera* camera = SceneManager::GetCamera( 0 );

	//  カメラクラスの更新
	if( camera != nullptr )
	{
		camera->Update( );
	}
}

//--------------------------------------------------------------------------------------
//  テストクラスの描画処理
//--------------------------------------------------------------------------------------
void Test::Draw( void )
{
	Scene::DrawAll( );
}