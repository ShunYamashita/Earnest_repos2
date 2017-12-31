//--------------------------------------------------------------------------------------
//  ステージセレクトプログラム   ( stageSelect.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "stageSelect.h"
#include "stageSelect2D.h"
#include "manager.h"
#include "sceneBG.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "fade.h"
#include "renderer.h"
#include "game.h"
#include "effekseerManager.h"
#include "selectVSMode.h"
#include "crystalMagic.h"
#include "camera.h"
#include "meshDome.h"
#include "readySelect.h"
#include "light.h"
#include <random>
#include "xact.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float VS_SOLO_POS_X = SCREEN_WIDTH * 0.4f;
static const float VS_SOLO_POS_Y = SCREEN_HEIGHT * 0.48f;
static const float VS_SOLO_SIZE_X = 320.0f;
static const float VS_SOLO_SIZE_Y = 200.0f;
	    
static const float VS_PARTY_POS_X = SCREEN_WIDTH * 0.6f;
static const float VS_PARTY_POS_Y = SCREEN_HEIGHT * 0.48f;
static const float VS_PARTY_SIZE_X = 320.0f;
static const float VS_PARTY_SIZE_Y = 200.0f;
	    
static const float VS_TUTORIAL_POS_X = SCREEN_WIDTH * 0.5f;
static const float VS_TUTORIAL_POS_Y = SCREEN_HEIGHT * 0.8f;
static const float VS_TUTORIAL_SIZE_X = SCREEN_WIDTH * 0.45f;
static const float VS_TUTORIAL_SIZE_Y = SCREEN_HEIGHT * 0.15f;

static const float CRYSTAL_SOLO_MAGIC_FIRE_POS_X = SCREEN_WIDTH * 0.3f;
static const float CRYSTAL_SOLO_MAGIC_FIRE_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL_SOLO_MAGIC_FIRE_SIZE_X = 120.0f;
static const float CRYSTAL_SOLO_MAGIC_FIRE_SIZE_Y = 200.0f;
						  
static const float CRYSTAL_SOLO_MAGIC_TORNADE_POS_X = SCREEN_WIDTH * 0.5f;
static const float CRYSTAL_SOLO_MAGIC_TORNADE_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL_SOLO_MAGIC_TORNADE_SIZE_X = 120.0f;
static const float CRYSTAL_SOLO_MAGIC_TORNADE_SIZE_Y = 200.0f;
						  
static const float CRYSTAL_SOLO_MAGIC_LIGHTNING_POS_X = SCREEN_WIDTH * 0.7f;
static const float CRYSTAL_SOLO_MAGIC_LIGHTNING_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL_SOLO_MAGIC_LIGHTNING_SIZE_X = 120.0f;
static const float CRYSTAL_SOLO_MAGIC_LIGHTNING_SIZE_Y = 200.0f;

static const float CRYSTAL1_MAGIC_FIRE_POS_X = SCREEN_WIDTH * 0.15f;
static const float CRYSTAL1_MAGIC_FIRE_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL1_MAGIC_FIRE_SIZE_X = 120.0f;
static const float CRYSTAL1_MAGIC_FIRE_SIZE_Y = 200.0f;

static const float CRYSTAL1_MAGIC_TORNADE_POS_X = SCREEN_WIDTH * 0.27f;
static const float CRYSTAL1_MAGIC_TORNADE_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL1_MAGIC_TORNADE_SIZE_X = 120.0f;
static const float CRYSTAL1_MAGIC_TORNADE_SIZE_Y = 200.0f;

static const float CRYSTAL1_MAGIC_LIGHTNING_POS_X = SCREEN_WIDTH * 0.39f;
static const float CRYSTAL1_MAGIC_LIGHTNING_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL1_MAGIC_LIGHTNING_SIZE_X = 120.0f;
static const float CRYSTAL1_MAGIC_LIGHTNING_SIZE_Y = 200.0f;

static const float CRYSTAL2_MAGIC_FIRE_POS_X = SCREEN_WIDTH * 0.61f;
static const float CRYSTAL2_MAGIC_FIRE_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL2_MAGIC_FIRE_SIZE_X = 120.0f;
static const float CRYSTAL2_MAGIC_FIRE_SIZE_Y = 200.0f;

static const float CRYSTAL2_MAGIC_TORNADE_POS_X = SCREEN_WIDTH * 0.73f;
static const float CRYSTAL2_MAGIC_TORNADE_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL2_MAGIC_TORNADE_SIZE_X = 120.0f;
static const float CRYSTAL2_MAGIC_TORNADE_SIZE_Y = 200.0f;

static const float CRYSTAL2_MAGIC_LIGHTNING_POS_X = SCREEN_WIDTH * 0.85f;
static const float CRYSTAL2_MAGIC_LIGHTNING_POS_Y = SCREEN_HEIGHT * 0.75f;
static const float CRYSTAL2_MAGIC_LIGHTNING_SIZE_X = 120.0f;
static const float CRYSTAL2_MAGIC_LIGHTNING_SIZE_Y = 200.0f;

static const float READY_POS_X = SCREEN_WIDTH * 0.5f;
static const float READY_POS_Y = SCREEN_HEIGHT * 0.6f;
static const float READY_SIZE_X = 840.0f;
static const float READY_SIZE_Y = 140.0f;

static const float THIEF_SIZE = 1.0f; 

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  タイトルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
StageSelect::StageSelect( )
{
	m_nWaitTime = 0;
	m_nFase = 0;
	m_fAddScale = 0.0f;
	m_bAdd = true;
}

//--------------------------------------------------------------------------------------
//  タイトルクラスのデストラクタ
//--------------------------------------------------------------------------------------
StageSelect::~StageSelect( )
{

}

//--------------------------------------------------------------------------------------
//  タイトルクラスの初期化処理
//--------------------------------------------------------------------------------------
void StageSelect::Init( void )
{
	m_vs = VS_SOLO;
	m_foucus = FOCUS_STAGE1;
	m_crystal = Game::CRYSTAL_FIRE;
	m_crystal2 = Game::CRYSTAL_FIRE;
	
	m_nWaitTime = 0;
	m_nFase = 0;

	for( int i = 0; i < FOCUS_MAX; i++ )
	{
		m_pScene2D[ i ] = NULL;
	}

	for( int i = 0; i < VS_MAX; i++ )
	{
		m_pVSMode[ i ] = NULL;
	}

	for( int i = 0; i < Game::CRYSTAL_MAX; i++ )
	{
		m_pCrystalMagic[ i ] = NULL;
	}

	for( int i = 0; i < 2; i++ )
	{
		m_pPlayer[ i ] = NULL;

		m_bCheckPlayer[ i ] = false;
	}

	m_pReadySelect = NULL;

	//  背景
	SceneBG::Create( SceneBG::TYPE_STAGE_SELECT , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
					  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
					  D3DXVECTOR2( 1.0f , 1.0f ) );

	//  メッシュドームの生成
	MeshDome::Create( MeshDome::TYPE_SKY ,
					   D3DXVECTOR3( 0.0f , -1.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
					   D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.00003f , 0.0f ) ,
					   32 , 32 );

	//  メッシュドームの生成
	MeshDome::Create( MeshDome::TYPE_SKY ,
					   D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) , D3DXVECTOR3( D3DX_PI , 0.0f , 0.0f ) ,
					   D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( -0.00003f , 0.0f ) ,
					   32 , 32 );

	if( m_pVSMode[ 0 ] == NULL )
	{
		m_pVSMode[ 0 ] = SelectVSMode::Create( SelectVSMode::TYPE_SOLO , D3DXVECTOR3( VS_SOLO_POS_X , VS_SOLO_POS_Y , 0.0f ) ,
												D3DXVECTOR3( VS_SOLO_SIZE_X  , VS_SOLO_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
												D3DXVECTOR2( 1.0f , 1.0f ) );
	}

	if( m_pVSMode[ 1 ] == NULL )
	{
		m_pVSMode[ 1 ] = SelectVSMode::Create( SelectVSMode::TYPE_PARTY , D3DXVECTOR3( VS_PARTY_POS_X , VS_PARTY_POS_Y , 0.0f ) ,
												D3DXVECTOR3( VS_PARTY_SIZE_X  , VS_PARTY_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
												D3DXVECTOR2( 1.0f , 1.0f ) );
	}

	if( m_pVSMode[ 2 ] == NULL )
	{
		m_pVSMode[ 2 ] = SelectVSMode::Create( SelectVSMode::TYPE_TUTORIAL , D3DXVECTOR3( VS_TUTORIAL_POS_X , VS_TUTORIAL_POS_Y , 0.0f ) ,
												D3DXVECTOR3( VS_TUTORIAL_SIZE_X  , VS_TUTORIAL_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
												D3DXVECTOR2( 1.0f , 1.0f ) );
	}

	Camera* camera = SceneManager::GetCamera( 0 );

	//  カメラクラスポインタが空の場合
	if( camera != nullptr )
	{
		//  カメラクラスの初期化
		camera->Init( D3DXVECTOR3( 0.0f , 3.0f , 20.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
					  D3DX_PI / 3.0f , 1.0f , 10000.0f );
	}
}

//--------------------------------------------------------------------------------------
//  タイトルクラスの終了処理
//--------------------------------------------------------------------------------------
void StageSelect::Uninit( void )
{
	Scene::ReleaseAll( );

	for( int nCntVS = 0; nCntVS < VS_MAX; nCntVS++ )
	{
		if( m_pVSMode[ nCntVS ] != NULL )
		{
			m_pVSMode[ nCntVS ] = NULL;
		}
	}

	for( int nCntStage = 0; nCntStage < FOCUS_MAX; nCntStage++ )
	{
		if( m_pScene2D[ nCntStage ] != NULL )
		{
			m_pScene2D[ nCntStage ] = NULL;
		}
	}

	for( int nCntCrystal = 0; nCntCrystal < Game::CRYSTAL_MAX; nCntCrystal++ )
	{
		if( m_pCrystalMagic[ nCntCrystal ] != NULL )
		{
			m_pCrystalMagic[ nCntCrystal ] = NULL;
		}
	}

	if( m_pReadySelect != NULL )
	{
		m_pReadySelect = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  タイトルクラスの更新処理
//--------------------------------------------------------------------------------------
void StageSelect::Update( void )
{
	// キーボード情報の取得
	Keyboard*			pKeyboard = SceneManager::GetKeyboard( );

	//  Xboxコントローラー情報の取得
	XboxController*	pXInput = SceneManager::GetXInput( );

	//  PS4コントローラー情報の取得
	PS4Controller*		pPS4Input = SceneManager::GetPS4Input( );

	Light* pLight = SceneManager::GetLight( );

	if( pLight != NULL )
	{
		pLight->SetDiffuseColor( D3DXCOLOR( 1.0f , 0.9f , 0.3f , 1.0f ) );
	}

	if( Fade::GetFade( ) == Fade::FADE_NONE )
	{
		if( m_nFase == 0 )
		{
			if( m_pVSMode[ 0 ] != NULL && m_pVSMode[ 1 ] != NULL && m_pVSMode[ 2 ] != NULL )
			{
				//  フォーカスがあっているものによって大きさを変える
				switch( m_vs )
				{
					case VS_SOLO:
					{
						m_pVSMode[ 0 ]->SetScale( 1.0f + m_fAddScale );
						m_pVSMode[ 1 ]->SetScale( 1.0f );
						m_pVSMode[ 2 ]->SetScale( 1.0f );

						break;
					}
					case VS_PARTY:
					{
						m_pVSMode[ 0 ]->SetScale( 1.0f );
						m_pVSMode[ 1 ]->SetScale( 1.0f + m_fAddScale );	
						m_pVSMode[ 2 ]->SetScale( 1.0f );

						break;
					}
					case VS_TUTORIAL:
					{
						m_pVSMode[ 0 ]->SetScale( 1.0f );
						m_pVSMode[ 1 ]->SetScale( 1.0f );	
						m_pVSMode[ 2 ]->SetScale( 1.0f + m_fAddScale );

						break;
					}
				}
			}

#ifdef KEYBOARD_ENABLE

			if( ( pKeyboard->GetKeyboardPress( DIK_A ) ) &&
				m_nWaitTime == 0 )
			{
				m_nWaitTime = 15;

				//  フォーカスがあっているものによって大きさを変える
				switch( m_vs )
				{
					case VS_SOLO:
					{
						m_vs = VS_TUTORIAL;

						break;
					}
					case VS_PARTY:
					{
						m_vs = VS_SOLO;			

						break;
					}
					case VS_TUTORIAL:
					{
						m_vs = VS_PARTY;			

						break;
					}
				}
			}
			else if( ( pKeyboard->GetKeyboardPress( DIK_D ) ) &&
				m_nWaitTime == 0 )
			{
				m_nWaitTime = 15;

				//  フォーカスがあっているものによって大きさを変える
				switch( m_vs )
				{
					case VS_SOLO:
					{
						m_vs = VS_PARTY;

						break;
					}
					case VS_PARTY:
					{
						m_vs = VS_TUTORIAL;			

						break;
					}
					case VS_TUTORIAL:
					{
						m_vs = VS_SOLO;			

						break;
					}
				}
			}

			if( pKeyboard->GetKeyboardTrigger( DIK_RETURN ) || pKeyboard->GetKeyboardTrigger( DIK_SPACE ) )
			{
				m_nFase = 1;

				//  初期化
				m_fAddScale = 0.0f;
				m_bAdd = true;

				if( m_vs == VS_SOLO )
				{
					for( int nCntVS = 0; nCntVS < VS_MAX; nCntVS++ )
					{
						if( m_pVSMode[ nCntVS ] != NULL )
						{
							m_pVSMode[ nCntVS ]->Release( );
							m_pVSMode[ nCntVS ] = NULL;
						}
					}

					if( m_pCrystalMagic[ 0 ] == NULL )
					{
						m_pCrystalMagic[ 0 ] = CrystalMagic::Create( CrystalMagic::TYPE_FIRE ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_FIRE_POS_X , CRYSTAL_SOLO_MAGIC_FIRE_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_FIRE_SIZE_X  , CRYSTAL_SOLO_MAGIC_FIRE_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic[ 1 ] == NULL )
					{
						m_pCrystalMagic[ 1 ] = CrystalMagic::Create( CrystalMagic::TYPE_TORNADE ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_TORNADE_POS_X , CRYSTAL_SOLO_MAGIC_TORNADE_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_TORNADE_SIZE_X  , CRYSTAL_SOLO_MAGIC_TORNADE_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic[ 2 ] == NULL )
					{
						m_pCrystalMagic[ 2 ] = CrystalMagic::Create( CrystalMagic::TYPE_LIGHTNING ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_LIGHTNING_POS_X , CRYSTAL_SOLO_MAGIC_LIGHTNING_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_LIGHTNING_SIZE_X  , CRYSTAL_SOLO_MAGIC_LIGHTNING_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pPlayer[ 0 ] == NULL )
					{
						//  プレイヤーの生成
						m_pPlayer[ 0 ] = PlayerSelect::Create( D3DXVECTOR3( 0.0f , -1.3f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 5.0f ) , 
																D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
																D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );
					}
				}
				else if( m_vs == VS_PARTY )
				{
					for( int nCntVS = 0; nCntVS < VS_MAX; nCntVS++ )
					{
						if( m_pVSMode[ nCntVS ] != NULL )
						{
							m_pVSMode[ nCntVS ]->Release( );
							m_pVSMode[ nCntVS ] = NULL;
						}
					}

					if( m_pCrystalMagic[ 0 ] == NULL )
					{
						m_pCrystalMagic[ 0 ] = CrystalMagic::Create( CrystalMagic::TYPE_FIRE ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_FIRE_POS_X , CRYSTAL1_MAGIC_FIRE_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_FIRE_SIZE_X  , CRYSTAL1_MAGIC_FIRE_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic[ 1 ] == NULL )
					{
						m_pCrystalMagic[ 1 ] = CrystalMagic::Create( CrystalMagic::TYPE_TORNADE ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_TORNADE_POS_X , CRYSTAL1_MAGIC_TORNADE_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_TORNADE_SIZE_X  , CRYSTAL1_MAGIC_TORNADE_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic[ 2 ] == NULL )
					{
						m_pCrystalMagic[ 2 ] = CrystalMagic::Create( CrystalMagic::TYPE_LIGHTNING ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_LIGHTNING_POS_X , CRYSTAL1_MAGIC_LIGHTNING_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_LIGHTNING_SIZE_X  , CRYSTAL1_MAGIC_LIGHTNING_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic2[ 0 ] == NULL )
					{
						m_pCrystalMagic2[ 0 ] = CrystalMagic::Create( CrystalMagic::TYPE_FIRE ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_FIRE_POS_X , CRYSTAL2_MAGIC_FIRE_POS_Y , 0.0f ) ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_FIRE_SIZE_X  , CRYSTAL2_MAGIC_FIRE_SIZE_Y , 0.0f ) ,
																	   D3DXVECTOR2( 0.0f , 0.0f ) , 
																	   D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic2[ 1 ] == NULL )
					{
						m_pCrystalMagic2[ 1 ] = CrystalMagic::Create( CrystalMagic::TYPE_TORNADE ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_TORNADE_POS_X , CRYSTAL2_MAGIC_TORNADE_POS_Y , 0.0f ) ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_TORNADE_SIZE_X  , CRYSTAL2_MAGIC_TORNADE_SIZE_Y , 0.0f ) ,
																	   D3DXVECTOR2( 0.0f , 0.0f ) , 
																	   D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic2[ 2 ] == NULL )
					{
						m_pCrystalMagic2[ 2 ] = CrystalMagic::Create( CrystalMagic::TYPE_LIGHTNING ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_LIGHTNING_POS_X , CRYSTAL2_MAGIC_LIGHTNING_POS_Y , 0.0f ) ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_LIGHTNING_SIZE_X  , CRYSTAL2_MAGIC_LIGHTNING_SIZE_Y , 0.0f ) ,
																	   D3DXVECTOR2( 0.0f , 0.0f ) , 
																	   D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pPlayer[ 0 ] == NULL )
					{
						//  プレイヤーの生成
						m_pPlayer[ 0 ] = PlayerSelect::Create( D3DXVECTOR3( 10.0f , -1.3f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 15.0f ) ,
																D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
																D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );
					}

					if( m_pPlayer[ 1 ] == NULL )
					{
						//  プレイヤーの生成
						m_pPlayer[ 1 ] = PlayerSelect::Create( D3DXVECTOR3( -10.0f , -1.3f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 15.0f ) , 
																D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
																D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );
					}
				}
				else if( m_vs == VS_TUTORIAL )
				{
					m_nFase = 0;

					//  フェードの設定
					Fade::SetFade( Fade::FADE_OUT , Mode::MODE::TUTORIAL , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
				}
			}

#else

			POINT leftStick = pPS4Input->GetLeftStickDisposition( 0 );

			if( ( leftStick.x < -PS4_STICK_ENABLE ) &&
				m_nWaitTime == 0 )
			{
				m_nWaitTime = 15;

				//  フォーカスがあっているものによって大きさを変える
				switch( m_vs )
				{
					case VS_SOLO:
					{
						m_vs = VS_TUTORIAL;

						break;
					}
					case VS_PARTY:
					{
						m_vs = VS_SOLO;			

						break;
					}
					case VS_TUTORIAL:
					{
						m_vs = VS_PARTY;			

						break;
					}
				}
			}
			else if( ( leftStick.x > PS4_STICK_ENABLE ) && m_nWaitTime == 0 )
			{
				//  音の再生
				Xact::Play( Xact::XACT_LABEL_SE_SELECT );

				m_nWaitTime = 15;

				//  フォーカスがあっているものによって大きさを変える
				switch( m_vs )
				{
					case VS_SOLO:
					{
						m_vs = VS_PARTY;

						break;
					}
					case VS_PARTY:
					{
						m_vs = VS_TUTORIAL;			

						break;
					}
					case VS_TUTORIAL:
					{
						m_vs = VS_SOLO;			

						break;
					}
				}
			}

			if( pKeyboard->GetKeyboardTrigger( DIK_RETURN ) || 
				pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CIRCLE ) || pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_OPTIONS ) )
			{
				//  音の再生
				Xact::Play( Xact::XACT_LABEL_SE_ENTER );

				m_nFase = 1;

				//  初期化
				m_fAddScale = 0.0f;
				m_bAdd = true;

				if( m_vs == VS_SOLO )
				{
					for( int nCntVS = 0; nCntVS < VS_MAX; nCntVS++ )
					{
						if( m_pVSMode[ nCntVS ] != NULL )
						{
							m_pVSMode[ nCntVS ]->Release( );
							m_pVSMode[ nCntVS ] = NULL;
						}
					}

					if( m_pCrystalMagic[ 0 ] == NULL )
					{
						m_pCrystalMagic[ 0 ] = CrystalMagic::Create( CrystalMagic::TYPE_FIRE ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_FIRE_POS_X , CRYSTAL_SOLO_MAGIC_FIRE_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_FIRE_SIZE_X  , CRYSTAL_SOLO_MAGIC_FIRE_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic[ 1 ] == NULL )
					{
						m_pCrystalMagic[ 1 ] = CrystalMagic::Create( CrystalMagic::TYPE_TORNADE ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_TORNADE_POS_X , CRYSTAL_SOLO_MAGIC_TORNADE_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_TORNADE_SIZE_X  , CRYSTAL_SOLO_MAGIC_TORNADE_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic[ 2 ] == NULL )
					{
						m_pCrystalMagic[ 2 ] = CrystalMagic::Create( CrystalMagic::TYPE_LIGHTNING ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_LIGHTNING_POS_X , CRYSTAL_SOLO_MAGIC_LIGHTNING_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL_SOLO_MAGIC_LIGHTNING_SIZE_X  , CRYSTAL_SOLO_MAGIC_LIGHTNING_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pPlayer[ 0 ] == NULL )
					{
						//  プレイヤーの生成
						m_pPlayer[ 0 ] = PlayerSelect::Create( D3DXVECTOR3( 0.0f , -1.3f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 5.0f ) , 
																D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
																D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );
					}
				}
				else if( m_vs == VS_PARTY )
				{
					for( int nCntVS = 0; nCntVS < VS_MAX; nCntVS++ )
					{
						if( m_pVSMode[ nCntVS ] != NULL )
						{
							m_pVSMode[ nCntVS ]->Release( );
							m_pVSMode[ nCntVS ] = NULL;
						}
					}

					if( m_pCrystalMagic[ 0 ] == NULL )
					{
						m_pCrystalMagic[ 0 ] = CrystalMagic::Create( CrystalMagic::TYPE_FIRE ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_FIRE_POS_X , CRYSTAL1_MAGIC_FIRE_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_FIRE_SIZE_X  , CRYSTAL1_MAGIC_FIRE_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic[ 1 ] == NULL )
					{
						m_pCrystalMagic[ 1 ] = CrystalMagic::Create( CrystalMagic::TYPE_TORNADE ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_TORNADE_POS_X , CRYSTAL1_MAGIC_TORNADE_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_TORNADE_SIZE_X  , CRYSTAL1_MAGIC_TORNADE_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic[ 2 ] == NULL )
					{
						m_pCrystalMagic[ 2 ] = CrystalMagic::Create( CrystalMagic::TYPE_LIGHTNING ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_LIGHTNING_POS_X , CRYSTAL1_MAGIC_LIGHTNING_POS_Y , 0.0f ) ,
																	  D3DXVECTOR3( CRYSTAL1_MAGIC_LIGHTNING_SIZE_X  , CRYSTAL1_MAGIC_LIGHTNING_SIZE_Y , 0.0f ) ,
																	  D3DXVECTOR2( 0.0f , 0.0f ) , 
																	  D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic2[ 0 ] == NULL )
					{
						m_pCrystalMagic2[ 0 ] = CrystalMagic::Create( CrystalMagic::TYPE_FIRE ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_FIRE_POS_X , CRYSTAL2_MAGIC_FIRE_POS_Y , 0.0f ) ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_FIRE_SIZE_X  , CRYSTAL2_MAGIC_FIRE_SIZE_Y , 0.0f ) ,
																	   D3DXVECTOR2( 0.0f , 0.0f ) , 
																	   D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic2[ 1 ] == NULL )
					{
						m_pCrystalMagic2[ 1 ] = CrystalMagic::Create( CrystalMagic::TYPE_TORNADE ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_TORNADE_POS_X , CRYSTAL2_MAGIC_TORNADE_POS_Y , 0.0f ) ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_TORNADE_SIZE_X  , CRYSTAL2_MAGIC_TORNADE_SIZE_Y , 0.0f ) ,
																	   D3DXVECTOR2( 0.0f , 0.0f ) , 
																	   D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pCrystalMagic2[ 2 ] == NULL )
					{
						m_pCrystalMagic2[ 2 ] = CrystalMagic::Create( CrystalMagic::TYPE_LIGHTNING ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_LIGHTNING_POS_X , CRYSTAL2_MAGIC_LIGHTNING_POS_Y , 0.0f ) ,
																	   D3DXVECTOR3( CRYSTAL2_MAGIC_LIGHTNING_SIZE_X  , CRYSTAL2_MAGIC_LIGHTNING_SIZE_Y , 0.0f ) ,
																	   D3DXVECTOR2( 0.0f , 0.0f ) , 
																	   D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pPlayer[ 0 ] == NULL )
					{
						//  プレイヤーの生成
						m_pPlayer[ 0 ] = PlayerSelect::Create( D3DXVECTOR3( 10.0f , -1.3f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 15.0f ) ,
																D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
																D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );
					}

					if( m_pPlayer[ 1 ] == NULL )
					{
						//  プレイヤーの生成
						m_pPlayer[ 1 ] = PlayerSelect::Create( D3DXVECTOR3( -10.0f , -1.3f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 15.0f ) , 
																D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
																D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , 0.0f );
					}
				}
				else if( m_vs == VS_TUTORIAL )
				{
					m_nFase = 0;

					//  フェードの設定
					Fade::SetFade( Fade::FADE_OUT , Mode::MODE::TUTORIAL , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
				}
			}
#endif

			if( m_bAdd == true )
			{
				m_fAddScale += 0.004f;

				if( m_fAddScale > 0.15f )
				{
					m_fAddScale = 0.15f;

					m_bAdd = false;
				}
			}
			else
			{
				m_fAddScale -= 0.004f;

				if( m_fAddScale < 0.0f )
				{
					m_fAddScale = 0.0f;

					m_bAdd = true;
				}
			}
		}
		else if( m_nFase == 1 )
		{
			if( m_pCrystalMagic[ 0 ] != NULL && m_pCrystalMagic[ 1 ] != NULL && m_pCrystalMagic[ 2 ] != NULL )
			{
				//  フォーカスがあっているものによって大きさを変える
				switch( m_crystal )
				{
					case Game::CRYSTAL_FIRE:
					{
						m_pCrystalMagic[ 0 ]->SetScale( 1.0f + m_fAddScale );
						m_pCrystalMagic[ 1 ]->SetScale( 1.0f );
						m_pCrystalMagic[ 2 ]->SetScale( 1.0f );

						break;
					}
					case Game::CRYSTAL_TORNADE:
					{
						m_pCrystalMagic[ 0 ]->SetScale( 1.0f );
						m_pCrystalMagic[ 1 ]->SetScale( 1.0f + m_fAddScale );
						m_pCrystalMagic[ 2 ]->SetScale( 1.0f );

						break;
					}
					case Game::CRYSTAL_LIGHTNING:
					{
						m_pCrystalMagic[ 0 ]->SetScale( 1.0f );
						m_pCrystalMagic[ 1 ]->SetScale( 1.0f );
						m_pCrystalMagic[ 2 ]->SetScale( 1.0f + m_fAddScale );

						break;
					}
				}
			}

			if( m_pCrystalMagic2[ 0 ] != NULL && m_pCrystalMagic2[ 1 ] != NULL && m_pCrystalMagic2[ 2 ] != NULL )
			{
				//  フォーカスがあっているものによって大きさを変える
				switch( m_crystal2 )
				{
					case Game::CRYSTAL_FIRE:
					{
						m_pCrystalMagic2[ 0 ]->SetScale( 1.0f + m_fAddScale2 );
						m_pCrystalMagic2[ 1 ]->SetScale( 1.0f );
						m_pCrystalMagic2[ 2 ]->SetScale( 1.0f );

						break;
					}
					case Game::CRYSTAL_TORNADE:
					{
						m_pCrystalMagic2[ 0 ]->SetScale( 1.0f );
						m_pCrystalMagic2[ 1 ]->SetScale( 1.0f + m_fAddScale2 );
						m_pCrystalMagic2[ 2 ]->SetScale( 1.0f );

						break;
					}
					case Game::CRYSTAL_LIGHTNING:
					{
						m_pCrystalMagic2[ 0 ]->SetScale( 1.0f );
						m_pCrystalMagic2[ 1 ]->SetScale( 1.0f );
						m_pCrystalMagic2[ 2 ]->SetScale( 1.0f + m_fAddScale2 );

						break;
					}
				}
			}

			if( m_bCheckPlayer[ 0 ] == false )
			{

#ifdef KEYBOARD_ENABLE

				if( pKeyboard->GetKeyboardTrigger( DIK_A ) &&
					m_nWaitTime == 0 )
				{
					m_nWaitTime = 15;

					//  フォーカスがあっているものによって大きさを変える
					switch( m_crystal )
					{
						case Game::CRYSTAL_FIRE:
						{
							m_crystal = Game::CRYSTAL_LIGHTNING;

							break;
						}
						case Game::CRYSTAL_TORNADE:
						{
							m_crystal = Game::CRYSTAL_FIRE;			

							break;
						}
						case Game::CRYSTAL_LIGHTNING:
						{
							m_crystal = Game::CRYSTAL_TORNADE;			

							break;
						}
					}
				}

				if( pKeyboard->GetKeyboardTrigger( DIK_D ) &&
					m_nWaitTime == 0 )
				{
					m_nWaitTime = 15;

					//  フォーカスがあっているものによって大きさを変える
					switch( m_crystal )
					{
						case Game::CRYSTAL_FIRE:
						{
							m_crystal = Game::CRYSTAL_TORNADE;

							break;
						}
						case Game::CRYSTAL_TORNADE:
						{
							m_crystal = Game::CRYSTAL_LIGHTNING;			

							break;
						}
						case Game::CRYSTAL_LIGHTNING:
						{
							m_crystal = Game::CRYSTAL_FIRE;			

							break;
						}
					}
				}

				if( pKeyboard->GetKeyboardTrigger( DIK_BACKSPACE ) )
				{
					m_nFase = 0;

					//  初期化
					m_fAddScale = 0.0f;
					m_bAdd = true;
					m_fAddScale2  = 0.0f;
					m_bAdd2 = true;

					m_crystal = Game::CRYSTAL_FIRE;
					m_crystal2 = Game::CRYSTAL_FIRE;

					for( int nCntPlayer = 0; nCntPlayer < 2; nCntPlayer++ )
					{
						if( m_pPlayer[ nCntPlayer ] != NULL )
						{
							m_pPlayer[ nCntPlayer ]->Release( );
							m_pPlayer[ nCntPlayer ] = NULL;
						}
					}

					for( int nCntCrystal = 0; nCntCrystal < Game::CRYSTAL_MAX; nCntCrystal++ )
					{
						if( m_pCrystalMagic[ nCntCrystal ] != NULL )
						{
							m_pCrystalMagic[ nCntCrystal ]->Release( );
							m_pCrystalMagic[ nCntCrystal ] = NULL;
						}

						if( m_pCrystalMagic2[ nCntCrystal ] != NULL )
						{
							m_pCrystalMagic2[ nCntCrystal ]->Release( );
							m_pCrystalMagic2[ nCntCrystal ] = NULL;
						}
					}

					if( m_pVSMode[ 0 ] == NULL )
					{
						m_pVSMode[ 0 ] = SelectVSMode::Create( SelectVSMode::TYPE_SOLO , D3DXVECTOR3( VS_SOLO_POS_X , VS_SOLO_POS_Y , 0.0f ) ,
																D3DXVECTOR3( VS_SOLO_SIZE_X  , VS_SOLO_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
																D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pVSMode[ 1 ] == NULL )
					{
						m_pVSMode[ 1 ] = SelectVSMode::Create( SelectVSMode::TYPE_PARTY , D3DXVECTOR3( VS_PARTY_POS_X , VS_PARTY_POS_Y , 0.0f ) ,
																D3DXVECTOR3( VS_PARTY_SIZE_X  , VS_PARTY_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
																D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pVSMode[ 2 ] == NULL )
					{
						m_pVSMode[ 2 ] = SelectVSMode::Create( SelectVSMode::TYPE_TUTORIAL , D3DXVECTOR3( VS_TUTORIAL_POS_X , VS_TUTORIAL_POS_Y , 0.0f ) ,
																D3DXVECTOR3( VS_TUTORIAL_SIZE_X  , VS_TUTORIAL_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
																D3DXVECTOR2( 1.0f , 1.0f ) );
					}
				}
			}

			if( m_bCheckPlayer[ 1 ] == false )
			{
				if( pKeyboard->GetKeyboardTrigger( DIK_LEFTARROW ) &&
					m_nWaitTime2 == 0 )
				{
					m_nWaitTime2 = 15;

					//  フォーカスがあっているものによって大きさを変える
					switch( m_crystal2 )
					{
						case Game::CRYSTAL_FIRE:
						{
							m_crystal2 = Game::CRYSTAL_LIGHTNING;

							break;
						}
						case Game::CRYSTAL_TORNADE:
						{
							m_crystal2 = Game::CRYSTAL_FIRE;			

							break;
						}
						case Game::CRYSTAL_LIGHTNING:
						{
							m_crystal2 = Game::CRYSTAL_TORNADE;			

							break;
						}
					}
				}

				if( pKeyboard->GetKeyboardTrigger( DIK_RIGHTARROW ) &&
					m_nWaitTime2 == 0 )
				{
					m_nWaitTime2 = 15;

					//  フォーカスがあっているものによって大きさを変える
					switch( m_crystal2 )
					{
						case Game::CRYSTAL_FIRE:
						{
							m_crystal2 = Game::CRYSTAL_TORNADE;

							break;
						}
						case Game::CRYSTAL_TORNADE:
						{
							m_crystal2 = Game::CRYSTAL_LIGHTNING;			

							break;
						}
						case Game::CRYSTAL_LIGHTNING:
						{
							m_crystal2 = Game::CRYSTAL_FIRE;			

							break;
						}
					}
				}
			}

			if( pKeyboard->GetKeyboardTrigger( DIK_SPACE ) )
			{
				m_bCheckPlayer[ 0 ] = true;

				if( m_pPlayer[ 0 ] != NULL )
				{
					m_pPlayer[ 0 ]->Decide( );
				}
			}

			if( pKeyboard->GetKeyboardTrigger( DIK_B ) )
			{
				m_bCheckPlayer[ 0 ] = false;

				if( m_pPlayer[ 0 ] != NULL )
				{
					m_pPlayer[ 0 ]->Cancel( );
				}
			}

			if( pKeyboard->GetKeyboardTrigger( DIK_RETURN ) )
			{
				m_bCheckPlayer[ 1 ] = true;

				if( m_pPlayer[ 1 ] != NULL )
				{
					m_pPlayer[ 1 ]->Decide( );
				}
			}

			if( pKeyboard->GetKeyboardTrigger( DIK_BACKSPACE ) )
			{
				m_bCheckPlayer[ 1 ] = false;

				if( m_pPlayer[ 1 ] != NULL )
				{
					m_pPlayer[ 1 ]->Cancel( );
				}
			}

			if( m_vs == VS_SOLO && m_bCheckPlayer[ 0 ] == true )
			{
				m_nFase = 2;

				if( m_pReadySelect == NULL )
				{
					m_pReadySelect = ReadySelect::Create( D3DXVECTOR3( READY_POS_X , READY_POS_Y , 0.0f ) , 
															D3DXVECTOR3( READY_SIZE_X , READY_SIZE_Y , 0.0f ) ,
															D3DXVECTOR2( 0.0f, 0.0f ) , D3DXVECTOR2( 1.0f, 1.0f ) );
				}
			}

			if( m_vs == VS_PARTY && m_bCheckPlayer[ 0 ] == true && m_bCheckPlayer[ 1 ] == true )
			{
				m_nFase = 2;

				if( m_pReadySelect == NULL )
				{
					m_pReadySelect = ReadySelect::Create( D3DXVECTOR3( READY_POS_X , READY_POS_Y , 0.0f ) , 
															D3DXVECTOR3( READY_SIZE_X , READY_SIZE_Y , 0.0f ) ,
															D3DXVECTOR2( 0.0f, 0.0f ) , D3DXVECTOR2( 1.0f, 1.0f ) );
				}
			}

#else

				POINT leftStick = pPS4Input->GetLeftStickDisposition( 0 );

				if( ( leftStick.x < -PS4_STICK_ENABLE ) &&
					m_nWaitTime == 0 )
				{
					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_SELECT );

					m_nWaitTime = 15;

					//  フォーカスがあっているものによって大きさを変える
					switch( m_crystal )
					{
						case Game::CRYSTAL_FIRE:
						{
							m_crystal = Game::CRYSTAL_LIGHTNING;

							break;
						}
						case Game::CRYSTAL_TORNADE:
						{
							m_crystal = Game::CRYSTAL_FIRE;			

							break;
						}
						case Game::CRYSTAL_LIGHTNING:
						{
							m_crystal = Game::CRYSTAL_TORNADE;			

							break;
						}
					}
				}

				if( ( leftStick.x > PS4_STICK_ENABLE ) &&
					m_nWaitTime == 0 )
				{
					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_SELECT );

					m_nWaitTime = 15;

					//  フォーカスがあっているものによって大きさを変える
					switch( m_crystal )
					{
						case Game::CRYSTAL_FIRE:
						{
							m_crystal = Game::CRYSTAL_TORNADE;

							break;
						}
						case Game::CRYSTAL_TORNADE:
						{
							m_crystal = Game::CRYSTAL_LIGHTNING;			

							break;
						}
						case Game::CRYSTAL_LIGHTNING:
						{
							m_crystal = Game::CRYSTAL_FIRE;			

							break;
						}
					}
				}

				if( pKeyboard->GetKeyboardTrigger( DIK_BACKSPACE ) || 
					pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CROSS ) )
				{
					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_CANCEL );

					m_nFase = 0;

					//  初期化
					m_fAddScale = 0.0f;
					m_bAdd = true;
					m_fAddScale2  = 0.0f;
					m_bAdd2 = true;

					m_crystal = Game::CRYSTAL_FIRE;
					m_crystal2 = Game::CRYSTAL_FIRE;

					for( int nCntPlayer = 0; nCntPlayer < 2; nCntPlayer++ )
					{
						if( m_pPlayer[ nCntPlayer ] != NULL )
						{
							m_pPlayer[ nCntPlayer ]->Release( );
							m_pPlayer[ nCntPlayer ] = NULL;
						}
					}

					for( int nCntCrystal = 0; nCntCrystal < Game::CRYSTAL_MAX; nCntCrystal++ )
					{
						if( m_pCrystalMagic[ nCntCrystal ] != NULL )
						{
							m_pCrystalMagic[ nCntCrystal ]->Release( );
							m_pCrystalMagic[ nCntCrystal ] = NULL;
						}

						if( m_pCrystalMagic2[ nCntCrystal ] != NULL )
						{
							m_pCrystalMagic2[ nCntCrystal ]->Release( );
							m_pCrystalMagic2[ nCntCrystal ] = NULL;
						}
					}

					if( m_pVSMode[ 0 ] == NULL )
					{
						m_pVSMode[ 0 ] = SelectVSMode::Create( SelectVSMode::TYPE_SOLO , D3DXVECTOR3( VS_SOLO_POS_X , VS_SOLO_POS_Y , 0.0f ) ,
																D3DXVECTOR3( VS_SOLO_SIZE_X  , VS_SOLO_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
																D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pVSMode[ 1 ] == NULL )
					{
						m_pVSMode[ 1 ] = SelectVSMode::Create( SelectVSMode::TYPE_PARTY , D3DXVECTOR3( VS_PARTY_POS_X , VS_PARTY_POS_Y , 0.0f ) ,
																D3DXVECTOR3( VS_PARTY_SIZE_X  , VS_PARTY_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
																D3DXVECTOR2( 1.0f , 1.0f ) );
					}

					if( m_pVSMode[ 2 ] == NULL )
					{
						m_pVSMode[ 2 ] = SelectVSMode::Create( SelectVSMode::TYPE_TUTORIAL , D3DXVECTOR3( VS_TUTORIAL_POS_X , VS_TUTORIAL_POS_Y , 0.0f ) ,
																D3DXVECTOR3( VS_TUTORIAL_SIZE_X  , VS_TUTORIAL_SIZE_Y , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
																D3DXVECTOR2( 1.0f , 1.0f ) );
					}
				}
			}

			if( m_bCheckPlayer[ 1 ] == false )
			{
				POINT leftStick2 = pPS4Input->GetLeftStickDisposition( 1 );

				if( ( leftStick2.x < -PS4_STICK_ENABLE ) &&
					m_nWaitTime2 == 0 )
				{
					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_SELECT );

					m_nWaitTime2 = 15;

					//  フォーカスがあっているものによって大きさを変える
					switch( m_crystal2 )
					{
						case Game::CRYSTAL_FIRE:
						{
							m_crystal2 = Game::CRYSTAL_LIGHTNING;

							break;
						}
						case Game::CRYSTAL_TORNADE:
						{
							m_crystal2 = Game::CRYSTAL_FIRE;			

							break;
						}
						case Game::CRYSTAL_LIGHTNING:
						{
							m_crystal2 = Game::CRYSTAL_TORNADE;			

							break;
						}
					}
				}

				if( ( leftStick2.x > PS4_STICK_ENABLE ) &&
					m_nWaitTime2 == 0 )
				{
					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_SELECT );

					m_nWaitTime2 = 15;

					//  フォーカスがあっているものによって大きさを変える
					switch( m_crystal2 )
					{
						case Game::CRYSTAL_FIRE:
						{
							m_crystal2 = Game::CRYSTAL_TORNADE;

							break;
						}
						case Game::CRYSTAL_TORNADE:
						{
							m_crystal2 = Game::CRYSTAL_LIGHTNING;			

							break;
						}
						case Game::CRYSTAL_LIGHTNING:
						{
							m_crystal2 = Game::CRYSTAL_FIRE;			

							break;
						}
					}
				}
			}

			if( pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CIRCLE ) || pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_OPTIONS ) ||
				pKeyboard->GetKeyboardTrigger( DIK_RETURN ) )
			{
				if( m_bCheckPlayer[ 0 ] == false )  
				{
					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_VOICE0_READY00 );

					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_ENTER );

					m_bCheckPlayer[ 0 ] = true;
				}

				if( m_pPlayer[ 0 ] != NULL )
				{
					m_pPlayer[ 0 ]->Decide( );
				}
			}

			if( pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CROSS ) ||
				pKeyboard->GetKeyboardTrigger( DIK_BACKSPACE ) )
			{
				//  音の再生
				Xact::Play( Xact::XACT_LABEL_SE_CANCEL );

				m_bCheckPlayer[ 0 ] = false;

				if( m_pPlayer[ 0 ] != NULL )
				{
					m_pPlayer[ 0 ]->Cancel( );
				}
			}

			if( pPS4Input->GetTrigger( 1 , PS4Controller::DIJ_CIRCLE ) || pPS4Input->GetTrigger( 1 , PS4Controller::DIJ_OPTIONS ) ||
				pKeyboard->GetKeyboardTrigger( DIK_RETURN ) )
			{
				if( m_bCheckPlayer[ 1 ] == false )  
				{
					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_VOICE0_READY00 );

					//  音の再生
					Xact::Play( Xact::XACT_LABEL_SE_ENTER );

					m_bCheckPlayer[ 1 ] = true;
				}

				if( m_pPlayer[ 1 ] != NULL )
				{
					m_pPlayer[ 1 ]->Decide( );
				}
			}

			if( pPS4Input->GetTrigger( 1 , PS4Controller::DIJ_CROSS ) ||
				pKeyboard->GetKeyboardTrigger( DIK_BACKSPACE ) )
			{
				//  音の再生
				Xact::Play( Xact::XACT_LABEL_SE_CANCEL );

				m_bCheckPlayer[ 1 ] = false;

				if( m_pPlayer[ 1 ] != NULL )
				{
					m_pPlayer[ 1 ]->Cancel( );
				}
			}

			if( m_vs == VS_SOLO && m_bCheckPlayer[ 0 ] == true )
			{
				m_nFase = 2;

				if( m_pReadySelect == NULL )
				{
					m_pReadySelect = ReadySelect::Create( D3DXVECTOR3( READY_POS_X , READY_POS_Y , 0.0f ) , 
															D3DXVECTOR3( READY_SIZE_X , READY_SIZE_Y , 0.0f ) ,
															D3DXVECTOR2( 0.0f, 0.0f ) , D3DXVECTOR2( 1.0f, 1.0f ) );
				}
			}

			if( m_vs == VS_PARTY && m_bCheckPlayer[ 0 ] == true && m_bCheckPlayer[ 1 ] == true )
			{
				m_nFase = 2;

				if( m_pReadySelect == NULL )
				{
					m_pReadySelect = ReadySelect::Create( D3DXVECTOR3( READY_POS_X , READY_POS_Y , 0.0f ) , 
															D3DXVECTOR3( READY_SIZE_X , READY_SIZE_Y , 0.0f ) ,
															D3DXVECTOR2( 0.0f, 0.0f ) , D3DXVECTOR2( 1.0f, 1.0f ) );
				}
			}
#endif

		}
		else if( m_nFase == 2 )
		{

#ifdef KEYBOARD_ENABLE

			if( pKeyboard->GetKeyboardTrigger( DIK_RETURN ) || pKeyboard->GetKeyboardTrigger( DIK_SPACE ) )
			{
				if( m_vs == VS_SOLO && m_bCheckPlayer[ 0 ] == true )
				{
					//  魔法クリスタル情報の代入
					Game::SetCrystal( 0 , m_crystal );

					Game::SetModeVS( Game::MODE_VS_CPU );

					//  乱数の宣言
					std::random_device rd;

					//  乱数の設定
					std::uniform_int_distribution< int > RandomStage( 0 , 1 );

					//  ステージランダム選択
					int nStage = RandomStage( rd );

					if( nStage == 0 )
					{
						SceneManager::SetStage( SceneManager::STAGE1 );
					}
					else if( nStage == 1 )
					{
						SceneManager::SetStage( SceneManager::STAGE2 );
					}

					//  フェードの設定
					Fade::SetFade( Fade::FADE_OUT , Mode::MODE::GAME , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
				}

				if( m_vs == VS_PARTY && m_bCheckPlayer[ 0 ] == true && m_bCheckPlayer[ 1 ] == true )
				{
					//  魔法クリスタル情報の代入
					Game::SetCrystal( 0 , m_crystal );
					Game::SetCrystal( 1 , m_crystal2 );

					if( m_vs == VS_SOLO )
					{
						Game::SetModeVS( Game::MODE_VS_CPU );
					}
					else if( m_vs == VS_PARTY )
					{
						Game::SetModeVS( Game::MODE_VS_PLAYER );
					}

					//  乱数の宣言
					std::random_device rd;

					//  乱数の設定
					std::uniform_int_distribution< int > RandomStage( 0 , 1 );

					//  ステージランダム選択
					int nStage = RandomStage( rd );

					if( nStage == 0 )
					{
						SceneManager::SetStage( SceneManager::STAGE1 );
					}
					else if( nStage == 1 )
					{
						SceneManager::SetStage( SceneManager::STAGE2 );
					}

					//  フェードの設定
					Fade::SetFade( Fade::FADE_OUT , Mode::MODE::GAME , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
				}
			}

			if( pKeyboard->GetKeyboardTrigger( DIK_BACKSPACE ) )
			{
				m_nFase = 1;

				//  初期化
				m_fAddScale = 0.0f;
				m_bAdd = true;

				m_crystal = Game::CRYSTAL_FIRE;

				m_bCheckPlayer[ 0 ] = false;

				if( m_pPlayer[ 0 ] != NULL )
				{
					m_pPlayer[ 0 ]->Cancel( );
				}

				if( m_pReadySelect != NULL )
				{
					m_pReadySelect->Release( );
					m_pReadySelect = NULL;
				}
			}

			if( pKeyboard->GetKeyboardTrigger( DIK_B ) )
			{
				m_nFase = 1;

				//  初期化
				m_fAddScale2  = 0.0f;
				m_bAdd2 = true;

				m_crystal2 = Game::CRYSTAL_FIRE;

				m_bCheckPlayer[ 1 ] = false;

				if( m_pPlayer[ 1 ] != NULL )
				{
					m_pPlayer[ 1 ]->Cancel( );
				}

				if( m_pReadySelect != NULL )
				{
					m_pReadySelect->Release( );
					m_pReadySelect = NULL;
				}
			}
		}

#else

			if( pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CIRCLE ) || pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_OPTIONS ) )
			{
				//  音の再生
				Xact::Play( Xact::XACT_LABEL_SE_ENTER );

				if( m_vs == VS_SOLO && m_bCheckPlayer[ 0 ] == true )
				{
					//  魔法クリスタル情報の代入
					Game::SetCrystal( 0 , m_crystal );

					Game::SetModeVS( Game::MODE_VS_CPU );

					//  乱数の宣言
					std::random_device rd;

					//  乱数の設定
					std::uniform_int_distribution< int > RandomStage( 0 , 1 );

					//  ステージランダム選択
					int nStage = RandomStage( rd );

					if( nStage == 0 )
					{
						SceneManager::SetStage( SceneManager::STAGE1 );
					}
					else if( nStage == 1 )
					{
						SceneManager::SetStage( SceneManager::STAGE2 );
					}

					//  フェードの設定
					Fade::SetFade( Fade::FADE_OUT , Mode::MODE::GAME , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
				}

				if( m_vs == VS_PARTY && m_bCheckPlayer[ 0 ] == true && m_bCheckPlayer[ 1 ] == true )
				{
					//  魔法クリスタル情報の代入
					Game::SetCrystal( 0 , m_crystal );
					Game::SetCrystal( 1 , m_crystal2 );

					if( m_vs == VS_SOLO )
					{
						Game::SetModeVS( Game::MODE_VS_CPU );
					}
					else if( m_vs == VS_PARTY )
					{
						Game::SetModeVS( Game::MODE_VS_PLAYER );
					}

					//  乱数の宣言
					std::random_device rd;

					//  乱数の設定
					std::uniform_int_distribution< int > RandomStage( 0 , 1 );

					//  ステージランダム選択
					int nStage = RandomStage( rd );

					if( nStage == 0 )
					{
						SceneManager::SetStage( SceneManager::STAGE1 );
					}
					else if( nStage == 1 )
					{
						SceneManager::SetStage( SceneManager::STAGE2 );
					}

					//  フェードの設定
					Fade::SetFade( Fade::FADE_OUT , Mode::MODE::GAME , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
				}
			}

			if( pPS4Input->GetTrigger( 0 , PS4Controller::DIJ_CROSS ) )
			{
				//  音の再生
				Xact::Play( Xact::XACT_LABEL_SE_CANCEL );

				m_nFase = 1;

				//  初期化
				m_fAddScale = 0.0f;
				m_bAdd = true;

				m_crystal = Game::CRYSTAL_FIRE;

				m_bCheckPlayer[ 0 ] = false;

				if( m_pPlayer[ 0 ] != NULL )
				{
					m_pPlayer[ 0 ]->Cancel( );
				}

				if( m_pReadySelect != NULL )
				{
					m_pReadySelect->Release( );
					m_pReadySelect = NULL;
				}
			}

			if( pPS4Input->GetTrigger( 1 , PS4Controller::DIJ_CROSS ) )
			{
				//  音の再生
				Xact::Play( Xact::XACT_LABEL_SE_CANCEL );

				m_nFase = 1;

				//  初期化
				m_fAddScale2  = 0.0f;
				m_bAdd2 = true;

				m_crystal2 = Game::CRYSTAL_FIRE;

				m_bCheckPlayer[ 1 ] = false;

				if( m_pPlayer[ 1 ] != NULL )
				{
					m_pPlayer[ 1 ]->Cancel( );
				}

				if( m_pReadySelect != NULL )
				{
					m_pReadySelect->Release( );
					m_pReadySelect = NULL;
				}
			}
		}

#endif

		if( m_pCrystalMagic[ 0 ] != NULL && m_pCrystalMagic[ 1 ] != NULL && m_pCrystalMagic[ 2 ] != NULL )
		{
			//  フォーカスがあっているものによって大きさを変える
			switch( m_crystal )
			{
				case Game::CRYSTAL_FIRE:
				{
					m_pCrystalMagic[ 0 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
					m_pCrystalMagic[ 1 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );
					m_pCrystalMagic[ 2 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					m_pCrystalMagic[ 0 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );
					m_pCrystalMagic[ 1 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
					m_pCrystalMagic[ 2 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					m_pCrystalMagic[ 0 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );
					m_pCrystalMagic[ 1 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );
					m_pCrystalMagic[ 2 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

					break;
				}
			}
		}

		if( m_pCrystalMagic2[ 0 ] != NULL && m_pCrystalMagic2[ 1 ] != NULL && m_pCrystalMagic2[ 2 ] != NULL )
		{
			//  フォーカスがあっているものによって大きさを変える
			switch( m_crystal2 )
			{
				case Game::CRYSTAL_FIRE:
				{
					m_pCrystalMagic2[ 0 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
					m_pCrystalMagic2[ 1 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );
					m_pCrystalMagic2[ 2 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					m_pCrystalMagic2[ 0 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );
					m_pCrystalMagic2[ 1 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );
					m_pCrystalMagic2[ 2 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					m_pCrystalMagic2[ 0 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );
					m_pCrystalMagic2[ 1 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.3f ) );
					m_pCrystalMagic2[ 2 ]->SetColor( D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );

					break;
				}
			}
		}

		if( m_bCheckPlayer[ 0 ] == false )
		{
			if( m_bAdd == true )
			{
				m_fAddScale += 0.005f;

				if( m_fAddScale > 0.15f )
				{
					m_fAddScale = 0.15f;

					m_bAdd = false;
				}
			}
			else
			{
				m_fAddScale -= 0.005f;

				if( m_fAddScale < 0.0f )
				{
					m_fAddScale = 0.0f;

					m_bAdd = true;
				}
			}
		}
		else
		{
			m_fAddScale = 0.15f;
		}

		if( m_bCheckPlayer[ 1 ] == false )
		{
			if( m_bAdd2 == true )
			{
				m_fAddScale2 += 0.005f;

				if( m_fAddScale2 > 0.15f )
				{
					m_fAddScale2 = 0.15f;

					m_bAdd2 = false;
				}
			}
			else
			{
				m_fAddScale2 -= 0.005f;

				if( m_fAddScale2 < 0.0f )
				{
					m_fAddScale2 = 0.0f;

					m_bAdd2 = true;
				}
			}
		}
		else
		{
			m_fAddScale2 = 0.15f;
		}

		if( m_nWaitTime > 0 )
		{
			m_nWaitTime--;
		}
		else
		{
			m_nWaitTime = 0;
		}

		if( m_nWaitTime2 > 0 )
		{
			m_nWaitTime2--;
		}
		else
		{
			m_nWaitTime2 = 0;
		}
	}
}

//--------------------------------------------------------------------------------------
//  タイトルクラスの描画処理
//--------------------------------------------------------------------------------------
void StageSelect::Draw( void )
{
	Scene::DrawAll( );
}