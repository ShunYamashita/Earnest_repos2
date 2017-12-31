//--------------------------------------------------------------------------------------
//  プレイヤー   ( player.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "player.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "scene2D.h"
#include "meshField.h"
#include "enemy.h"
#include "bullet.h"
#include "fade.h"
#include "bullet.h"
#include "particle.h"
#include "hitField.h"
#include "result.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"
#include "meshTracing.h"
#include "stencilShadow.h"
#include "magic.h"
#include "playerUI.h"
#include "shadow.h"
#include "life.h"
#include "brave.h"
#include "stateAnimator.h"
#include "utility.h"
#include <random>
#include "Wwise.h"
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  ヘッダーファイル( ステート系 )
//--------------------------------------------------------------------------------------
#include "playerState.h"
#include "playerBend.h"
#include "playerBlow.h"
#include "playerHPAttack000.h"
#include "playerEscape.h"
#include "playerGuard.h"
#include "playerBraveAttack000After.h"
#include "playerBraveAttack000Before.h"
#include "playerJump.h"
#include "playerLanding.h"
#include "playerLockonDash.h"
#include "playerMagicActive.h"
#include "playerMagicReady.h"
#include "playerMove.h"
#include "playerNeutral.h"
#include "playerPassive.h"
#include "playerResultLose.h"
#include "playerResultWin.h"
#include "playerRushAttack.h"
#include "playerFreeDash.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define PLAYER_TO_CAMERA_DISTANCE_Y				( 9.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Y2			( 10.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Z				( 20.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Z2			( 23.0f )
#define PLAYER_TO_CUBE_DISTANCE					( 7.0f )
#define PLAYER_TO_CUBE_PUT_DISTANCE				( 2.5f )
#define PLAYER_SIZE								( 2.5f )
#define PLAYER_GOAL_SIZE						( 0.8f )
#define PLAYER_HIT_SIZE							( 2.0f )
#define PLAYER_WAIT_TIME						( 20 )
#define PLAYER_GRAVITY							( 0.1f )
#define PLAYER_GRAVITY2							( 0.2f )
#define PLAYER_ENABLE_NUM_JUMP					( 2 )
#define PLAYER_TO_CAMERA_RATE					( 0.5f )
#define PLAYER_LOCKON_DASH_SPEED				( 3.0f )
#define PLAYER_HIT_SPHERE_LENGTH				( 6.0f )
#define PLAYER_HEAL_LIFE						( 20 )

#define PLAYER_AVOID_SPEED						( 1.5f )

#define PLAYER_HIT_SPHERE_POS_Y					( 3.0f )

#define PLAYER_LIFE_POS_X						( 389.0f )
#define PLAYER_LIFE_POS_Y						( 661.0f )
#define PLAYER_LIFE_SIZE_X						( 350.0f )
#define PLAYER_LIFE_SIZE_Y						( 49.0f )

#define PLAYER_VS_LIFE_POS_X					( 125.0f )
#define PLAYER_VS_LIFE_POS_Y					( 646.0f )
#define PLAYER_VS_LIFE_SIZE_X					( 442.0f )
#define PLAYER_VS_LIFE_SIZE_Y					( 38.0f )

#define PLAYER_BRAVE_POS_X						( 440.0f )
#define PLAYER_BRAVE_POS_Y						( 570.0f )
#define PLAYER_BRAVE_SIZE_X						( 60.0f )
#define PLAYER_BRAVE_SIZE_Y						( 60.0f )

#define PLAYER_UI_POS_X							( SCREEN_WIDTH * 0.25f )
#define PLAYER_UI_POS_Y							( SCREEN_HEIGHT * 0.88f )
#define PLAYER_UI_SIZE_X						( 640.0f )
#define PLAYER_UI_SIZE_Y						( 200.0f )

#define PLAYER2_LIFE_POS_X						( 975.0f )
#define PLAYER2_LIFE_POS_Y						( 605.0f )
#define PLAYER2_LIFE_SIZE_X						( 255.0f )
#define PLAYER2_LIFE_SIZE_Y						( 30.0f )

#define PLAYER2_VS_LIFE_POS_X					( 715.0f )
#define PLAYER2_VS_LIFE_POS_Y					( 646.0f )
#define PLAYER2_VS_LIFE_SIZE_X					( 442.0f )
#define PLAYER2_VS_LIFE_SIZE_Y					( 38.0f )

#define PLAYER2_BRAVE_POS_X						( 990.0f )
#define PLAYER2_BRAVE_POS_Y						( 565.0f )
#define PLAYER2_BRAVE_SIZE_X					( 35.0f )
#define PLAYER2_BRAVE_SIZE_Y					( 35.0f )

#define PLAYER2_UI_POS_X						( SCREEN_WIDTH * 0.75f )
#define PLAYER2_UI_POS_Y						( SCREEN_HEIGHT * 0.88f )
#define PLAYER2_UI_SIZE_X						( 640.0f )
#define PLAYER2_UI_SIZE_Y						( 200.0f )

#define PLAYER_JUMP_							( 0.4f )

#define PLAYER_FINISH_SPEED						( 3.0f )

#define PLAYER_HOMIMG_TIME						( 90.0f )
#define PLAYER_ENABLE_HP_ATTACK_DIST			( 150.0f )
#define PLAYER_HP_ATTACK_SPEED					( 1.0f )

static const int	PLAYER_HP_DAMAGE			= 150;
static const int	PLAYER_FINISHER_DAMAGE		= 250;

static const float	PLAYER_HP_RANGE				= 30.0f;
static const float	PLAYER_HP_RANGE_HEIGHT		= 12.0f;

static const float	PLAYER_HP_BLOW_POWER		= 3.0f;
static const float	PLAYER_FINISHER_BLOW_POWER	= 7.0f;

static const int	PLAYER_GUARD_FRAME			= 5;

static const float	CAMERA_ROTATE_ANGLE			= 0.025f;

static const float	PLAYER_HEIGHT = 10.0f;

static const float	PLAYER_SHADOW_SCALE = 0.55f;

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  プレイヤークラスのコンストラクタ
//--------------------------------------------------------------------------------------
Player::Player( ) : SceneModelAnim( 3 )
{
	m_beforePos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	m_fSpeed = 0.0f;
	m_nWaitTime = 0;
	m_fCurrentJumpPower = 0.0f;
	m_nCntJump = 0;
	m_bLockOn = true;
	m_stencilShadow = nullptr;
	m_hitSphere.fLength = PLAYER_HIT_SPHERE_LENGTH * 0.5f;
	m_hitSphere.position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_pLife = NULL;
	m_pBrave = NULL;
	m_pStateAnimator = NULL;
	m_pPlayerUI = NULL;
	m_pMagic = NULL;
	m_nLife = BASE_LIFE;
	m_nBrave = BASE_BRAVE;
	m_bVoiceStart = false;
	m_bJudgeHit = true;
	m_bHomingLeft = false;
	m_bSlashEffekseer = false;
	m_fHomingTime = 0.0f;
	m_fBlowPower = 0.0f;
	m_fMoveY = 0.0f;
	m_nPressTime = 0;
}

//--------------------------------------------------------------------------------------
//  プレイヤークラスのデストラクタ
//--------------------------------------------------------------------------------------
Player::~Player( )
{

}

//--------------------------------------------------------------------------------------
//  プレイヤーの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Player::Init( void )
{
	m_drawDepth = true;

	//  シーンモデル
	SceneModelAnim::Init( );

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_PLAYER );

	////  ステンシルシャドウの生成
	//m_stencilShadow = StencilShadow::Create( StencilShadow::TYPE::SPHERE ,
	//										 m_position , 
	//										 D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
	//										 D3DXVECTOR3( PLAYER_SHADOW_SCALE , PLAYER_SHADOW_SCALE , PLAYER_SHADOW_SCALE ) );

	if( m_nPlayerNo == 0 )
	{
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			//  体力の生成
			m_pLife = Life::Create( Life::TYPE_LIFE000 ,
									 D3DXVECTOR3( PLAYER_LIFE_POS_X , PLAYER_LIFE_POS_Y , 0.0f ) ,
									 D3DXVECTOR3( PLAYER_LIFE_SIZE_X , PLAYER_LIFE_SIZE_Y , 0.0f ) ,
									 D3DXVECTOR2( 0.0f , 0.0f ) ,
									 D3DXVECTOR2( 1.0f , 1.0f ) ,
									 m_nLife );
			////  ブレイブの生成
			//m_pBrave = Brave::Create( D3DXVECTOR3( PLAYER_BRAVE_POS_X , PLAYER_BRAVE_POS_Y , 0.0f ) ,
			//						   D3DXVECTOR3( PLAYER_BRAVE_SIZE_X , PLAYER_BRAVE_SIZE_Y , 0.0f ) ,
			//						   1000 , 4 );
		}
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  プレイヤーUIの生成
			m_pPlayerUI = PlayerUI::Create( PlayerUI::TYPE_PLAYER1 ,
											 D3DXVECTOR3( PLAYER_UI_POS_X , PLAYER_UI_POS_Y , 0.0f ) ,
											 D3DXVECTOR3( PLAYER_UI_SIZE_X , PLAYER_UI_SIZE_Y , 0.0f ) );

			//  体力の生成
			m_pLife = Life::Create( Life::TYPE_LIFE002 ,
									 D3DXVECTOR3( PLAYER_VS_LIFE_POS_X , PLAYER_VS_LIFE_POS_Y , 0.0f ) ,
									 D3DXVECTOR3( PLAYER_VS_LIFE_SIZE_X , PLAYER_VS_LIFE_SIZE_Y , 0.0f ) ,
									 D3DXVECTOR2( 0.0f , 0.0f ) ,
									 D3DXVECTOR2( 1.0f , 1.0f ) ,
									 m_nLife );

			////  ブレイブの生成
			//m_pBrave = Brave::Create( D3DXVECTOR3( PLAYER_BRAVE_POS_X , PLAYER_BRAVE_POS_Y , 0.0f ) ,
			//						   D3DXVECTOR3( PLAYER_BRAVE_SIZE_X , PLAYER_BRAVE_SIZE_Y , 0.0f ) ,
			//						   1000 , 4 );
		}

		//  体力の設定
		m_pLife->SetEndLife( m_nLife );										 
	}
	else if( m_nPlayerNo == 1 )
	{
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			m_pLife = Life::Create( Life::TYPE_LIFE001 ,
									 D3DXVECTOR3( PLAYER2_LIFE_POS_X , PLAYER2_LIFE_POS_Y , 0.0f ) ,
									 D3DXVECTOR3( PLAYER2_LIFE_SIZE_X , PLAYER2_LIFE_SIZE_Y , 0.0f ) ,
									 D3DXVECTOR2( 0.0f , 0.0f ) ,
									 D3DXVECTOR2( 1.0f , 1.0f ) ,
									 m_nLife );

			////  ブレイブの生成
			//m_pBrave = Brave::Create( D3DXVECTOR3( PLAYER2_BRAVE_POS_X , PLAYER2_BRAVE_POS_Y , 0.0f ) ,
			//						   D3DXVECTOR3( PLAYER2_BRAVE_SIZE_X , PLAYER2_BRAVE_SIZE_Y , 0.0f ) ,
			//						   1000 , 4 );
		}
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  プレイヤーUIの生成
			m_pPlayerUI = PlayerUI::Create( PlayerUI::TYPE_PLAYER2 ,
											 D3DXVECTOR3( PLAYER2_UI_POS_X , PLAYER2_UI_POS_Y , 0.0f ) ,
											 D3DXVECTOR3( PLAYER2_UI_SIZE_X , PLAYER2_UI_SIZE_Y , 0.0f ) );

			m_pLife = Life::Create( Life::TYPE_LIFE003 ,
									 D3DXVECTOR3( PLAYER2_VS_LIFE_POS_X , PLAYER2_VS_LIFE_POS_Y , 0.0f ) ,
									 D3DXVECTOR3( PLAYER2_VS_LIFE_SIZE_X , PLAYER2_VS_LIFE_SIZE_Y , 0.0f ) ,
									 D3DXVECTOR2( 0.0f , 0.0f ) ,
									 D3DXVECTOR2( 1.0f , 1.0f ) ,
									 m_nLife );

			////  ブレイブの生成
			//m_pBrave = Brave::Create( D3DXVECTOR3( PLAYER2_BRAVE_POS_X , PLAYER2_BRAVE_POS_Y , 0.0f ) ,
			//						   D3DXVECTOR3( PLAYER2_BRAVE_SIZE_X , PLAYER2_BRAVE_SIZE_Y , 0.0f ) ,
			//						   1000 , 4 );
		}

		//  体力の設定
		m_pLife->SetEndLife( m_nLife );
	}

	//  ステートマシン( アニメーター )の生成
	m_pStateAnimator = StateAnimator::Create( StateAnimator::MOTION_NEUTRAL );

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( m_nPlayerNo );

	D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

	D3DXVECTOR3 workPosEye = m_position;

	//  CPU対戦の場合
	if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
	{
		workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y;
		workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
	}
	//  プレイヤー対戦の場合
	else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
	{
		workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
		workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
	}

	//  カメラの視点設定
	pCamera->SetCameraPosEyeDirect( workPosEye );

	D3DXVECTOR3 workPosAt = m_position;
	workPosAt.z += 5.0f;

	//  カメラの注視点設定
	pCamera->SetCameraPosAtDirect( workPosAt );

	//  プレイヤー注視点の設定
	m_posAt = m_position + cameraVecDirect * 5.0f;

	//  方向ベクトルの代入
	D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

	//  プレイヤーステートの生成
	m_allState[ static_cast< int >( Player::STATE::ATTACK_HP_ATTACK000 ) ] = new PlayerHPAttack000( this );
	m_allState[ static_cast< int >( Player::STATE::ATTACK_BRAVE_ATTACK000_BEFORE ) ] = new PlayerBraveAttack000Before( this );
	m_allState[ static_cast< int >( Player::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) ] = new PlayerBraveAttack000After( this );
	m_allState[ static_cast< int >( Player::STATE::BEND ) ] = new PlayerBend( this );
	m_allState[ static_cast< int >( Player::STATE::BLOW ) ] = new PlayerBlow( this );
	m_allState[ static_cast< int >( Player::STATE::ESCAPE ) ] = new PlayerEscape( this );
	m_allState[ static_cast< int >( Player::STATE::GUARD ) ] = new PlayerGuard( this );
	m_allState[ static_cast< int >( Player::STATE::JUMP ) ] = new PlayerJump( this );
	m_allState[ static_cast< int >( Player::STATE::LANDING ) ] = new PlayerLanding( this );
	m_allState[ static_cast< int >( Player::STATE::LOCKON_DASH ) ] = new PlayerLockonDash( this );
	m_allState[ static_cast< int >( Player::STATE::FREE_DASH ) ] = new PlayerFreeDash( this );
	m_allState[ static_cast< int >( Player::STATE::MAGIC_ACTIVE ) ] = new PlayerMagicActive( this );
	m_allState[ static_cast< int >( Player::STATE::MAGIC_READY ) ] = new PlayerMagicReady( this );
	m_allState[ static_cast< int >( Player::STATE::MOVE ) ] = new PlayerMove( this );
	m_allState[ static_cast< int >( Player::STATE::NEUTRAL ) ] = new PlayerNeutral( this );
	m_allState[ static_cast< int >( Player::STATE::PASSIVE ) ] = new PlayerPassive( this );
	m_allState[ static_cast< int >( Player::STATE::RESULT_LOSE ) ] = new PlayerResultLose( this );
	m_allState[ static_cast< int >( Player::STATE::RESULT_WIN ) ] = new PlayerResultWin( this );
	m_allState[ static_cast< int >( Player::STATE::RUSH_ATTACK ) ] = new PlayerRushAttack( this );

	//  ニュートラル状態で初期化
	m_playerState = m_allState[ static_cast< int >( Player::STATE::NEUTRAL ) ];
	SetAnimation( StateAnimator::MOTION_NEUTRAL );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  プレイヤーの終了処理
//--------------------------------------------------------------------------------------
void Player::Uninit( void )
{
	//  シーンモデル
	SceneModelAnim::Uninit( );

	if( m_pLife != NULL )
	{
		m_pLife = NULL;
	}

	if( m_pBrave != NULL )
	{
		m_pBrave = NULL;
	}

	if( m_pPlayerUI != NULL )
	{
		m_pPlayerUI = NULL;
	}

	if( m_pStateAnimator != NULL )
	{
		delete m_pStateAnimator;
		m_pStateAnimator = NULL;
	}

	if( m_stencilShadow != NULL )
	{
		m_stencilShadow = NULL;
	}

	//  プレイヤーステートの数分のループ
	for( int countState = 0; countState < static_cast< int >( Player::STATE::MAX ); ++countState )
	{
		m_allState[ countState ]->Uninit( );
		delete m_allState[ countState ];
		m_allState[ countState ] = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーの更新処理
//--------------------------------------------------------------------------------------
void Player::Update( void )
{
	//  開始ボイスをまだ再生していない場合
	if( m_bVoiceStart == false )
	{
		m_bVoiceStart = true;
	}

	//  受ける側の当たり判定の代入
	m_hitSphere.position = m_position;
	m_hitSphere.position.y += PLAYER_HIT_SPHERE_POS_Y;

	//  プレイヤーステートクラスの更新
	if( m_playerState != nullptr )
	{
		m_playerState->Update( );
	}

	//  敵との当たり判定
	JudgeHitEnemy( );

	//  壁との当たり判定
	JudgeHitWall( );

	//  地面との当たり判定
	JudgeHitGround( );

	//  シーンモデル
	SceneModelAnim::Update( );

	//  体力が上限を超えた場合
	if( m_nLife > BASE_LIFE )
	{
		m_nLife = BASE_LIFE;
	}

	//  体力ポインタがある場合
	if( m_pLife != NULL )
	{
		//  体力の設定
		m_pLife->SetLife( m_nLife );
	}

	//  無敵時間の場合
	if( m_nInvisibleTime > 0 )
	{
		//  無敵時間のカウント
		m_nInvisibleTime--;

		m_color.a = 0.5f;
	}
	else
	{
		m_color.a = 1.0f;
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーの描画処理
//--------------------------------------------------------------------------------------
void Player::Draw( void )
{
	Camera* camera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVectorDirect;
	D3DXVECTOR3 cameraToPlayer;

	if( camera != nullptr )
	{
		D3DXVECTOR3 cameraPosition = camera->GetCameraPosEye( );
		D3DXVECTOR3 playerPosition = m_position;
		cameraPosition.y = 0.0f;
		playerPosition.y = 0.0f;

		cameraToPlayer = playerPosition - cameraPosition;
		D3DXVec3Normalize( &cameraToPlayer , &cameraToPlayer );
		cameraVectorDirect = camera->GetCameraVecDirect( );
		cameraVectorDirect.y = 0.0f;
		D3DXVec3Normalize( &cameraVectorDirect , &cameraVectorDirect );
	}

	if( Utility::IntoView( cameraVectorDirect , cameraToPlayer , D3DX_PI * 0.5f ) )
	{
		//  シーンモデル
		SceneModelAnim::Draw( );
	}
}

//--------------------------------------------------------------------------------------
//  地面との当たり判定をする関数
//--------------------------------------------------------------------------------------
bool Player::JudgeHitGround( void )
{
	//  シーンクラスのポインタ
	Scene* pScene = NULL;											

	//  当たり判定用フィールドクラス
	HitField* pHitField = NULL;

#pragma omp parallel for
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		//  シーンの先頭アドレスを取得
		pScene = Scene::GetScene( nCntPriority );

		//  シーンが空ではない間ループ
		while( pScene != NULL )
		{
			Scene::OBJTYPE objType;						//  物体の種類

			//  物体の種類の取得
			objType = pScene->GetObjType( );

			//  種類が当たり判定用のフィールドの場合
			if( objType == Scene::OBJTYPE_HIT_FIELD )
			{
				//  当たり判定用フィールドクラスにダウンキャスト
				pHitField = ( HitField* )pScene;

				//  ステンシルシャドウが存在している場合している場合
				if( m_stencilShadow != nullptr )
				{
					float fScale = PLAYER_SHADOW_SCALE + ( m_position.y - pHitField->GetHeight( m_position )  ) * 0.01f;

					D3DXVECTOR3 position = m_position;
					position.y = pHitField->GetHeight( m_position );

					m_stencilShadow->SetScale( D3DXVECTOR3( fScale , fScale , fScale ) );
					m_stencilShadow->SetPosition( position );
				}

				float fHeight = pHitField->GetHeight( m_position );

				if( !( fHeight == -100000.0f ) )
				{	
					if( m_position.y <= fHeight )
					{
						//  Y座標フィールドに合わせる
						m_position.y = pHitField->GetHeight( m_position );

						return true;
					}
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  敵との当たり判定をする関数
//--------------------------------------------------------------------------------------
bool Player::JudgeHitEnemy( void )
{
	//  シーンクラスのポインタ
	Scene* pScene = NULL;	

	if( m_bJudgeHit )
	{
#pragma omp parallel for
		//  優先度の最大数分のループ
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  シーンの先頭アドレスを取得
			pScene = Scene::GetScene( nCntPriority );

			//  シーンが空ではない間ループ
			while( pScene != NULL )
			{
				Scene::OBJTYPE objType;						//  物体の種類

				//  物体の種類の取得
				objType = pScene->GetObjType( );

				//  CPU対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  種類が敵の場合
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						//  ダウンキャスト
						Enemy* pEnemy = ( Enemy* )pScene;

						float fDist = 0.0f;

						if( Utility::HitSphere( m_hitSphere , pEnemy->GetHitSphere( ) , &fDist ) )
						{
							//  速度をなくす
							m_fSpeed = 0.0f;

							//  反射するベクトルを求める
							D3DXVECTOR3 reflectVec = m_hitSphere.position - pEnemy->GetHitSphere( ).position;
							reflectVec.y = 0.0f;

							D3DXVec3Normalize( &reflectVec , &reflectVec );

							//  めり込んだ分座標を戻す
							m_position += reflectVec * fDist;

							return true;
						}
					}
				}
				//  プレイヤー対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					//  種類がプレイヤーの場合
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						Player* pPlayer = ( Player* )pScene;

						//  自分以外のプレイヤーでかつ、当たり判定をする場合
						if( m_nPlayerNo != pPlayer->GetPlayerNo( ) && pPlayer->GetJudgeHit( ) )
						{
							float fDist = 0.0f;

							if( Utility::HitSphere( m_hitSphere , pPlayer->GetHitSphere( ) , &fDist ) )
							{
								//  速度をなくす
								m_fSpeed = 0.0f;

								//  反射するベクトルを求める
								D3DXVECTOR3 reflectVec = m_hitSphere.position - pPlayer->GetHitSphere( ).position;
								reflectVec.y = 0.0f;

								D3DXVec3Normalize( &reflectVec , &reflectVec );

								//  めり込んだ分座標を戻す
								m_position += reflectVec * fDist;

								return true;
							}
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  壁との当たり判定をする関数
//--------------------------------------------------------------------------------------
bool Player::JudgeHitWall( void )
{
	bool hit = false;

	//  壁との当たり判定
	if( m_position.x > Game::GetFieldMax( ).x * 0.999f )
	{
		m_position.x = Game::GetFieldMax( ).x * 0.999f;

		m_fSpeed = 0.0f;

		hit = true;
	}
	if( m_position.x < Game::GetFieldMin( ).x * 0.999f )
	{
		m_position.x = Game::GetFieldMin( ).x * 0.999f;

		m_fSpeed = 0.0f;

		hit = true;
	}
	if( m_position.z > Game::GetFieldMax( ).z * 0.999f )
	{
		m_position.z = Game::GetFieldMax( ).z * 0.999f;

		m_fSpeed = 0.0f;

		hit = true;
	}
	if( m_position.z < Game::GetFieldMin( ).z * 0.999f )
	{
		m_position.z = Game::GetFieldMin( ).z * 0.999f;

		m_fSpeed = 0.0f;

		hit = true;
	}

	return hit;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Player* Player::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed , MAGIC magic , int nPlayerNo )
{
	Player* pPlayer;

	//  インスタンス生成
	pPlayer = new Player;

	//  モデルの種類の代入
	pPlayer->m_type = SceneModelAnim::TYPE_PLAYER;

	//  座標の代入
	pPlayer->m_position = position;

	//  最初の座標の代入
	pPlayer->m_firstPos = position;

	//  注視点座標の代入
	pPlayer->m_posAt = position;

	//  回転角の代入
	pPlayer->m_rot = rot;

	//  大きさ倍率の代入
	pPlayer->m_scale = scale;

	//  速度の代入
	pPlayer->m_fBaseSpeed = fSpeed;

	//  所持魔法の代入
	pPlayer->m_magic = magic;

	//  プレイヤー番号の代入
	pPlayer->m_nPlayerNo = nPlayerNo;

	//  初期化
	pPlayer->Init( );

	return pPlayer;
}

//--------------------------------------------------------------------------------------
//  プレイヤーのダメージ処理
//--------------------------------------------------------------------------------------
void Player::Damage( D3DXVECTOR3 blowVecDirect , float fBlowPower , int nDamage , bool bBlow )
{
	if( m_nInvisibleTime != 0 )
	{
		return;
	}

	//  吹っ飛び方向と力の代入
	m_blowVecDirect = blowVecDirect;
	m_fBlowPower = fBlowPower;

	//  ダメージ処理
	m_nLife -= nDamage;

	//  無敵時間の設定
	m_nInvisibleTime = 60;

	//  乱数の宣言
	std::random_device rd;

	//  乱数の設定
	std::uniform_int_distribution< int > RandomSeekPos( 5 , 20 );

	float fSeekPosX = ( float )RandomSeekPos( rd ) * 0.1f;
	float fSeekPosZ = ( float )RandomSeekPos( rd ) * 0.1f;

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( m_nPlayerNo );

	if( pCamera != NULL )
	{
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.0000006f;

		//  縦揺れ分の代入
		pCamera->SetSwayVertical( fSwayVertical );
	}

	if( m_pPlayerUI != NULL )
	{
		float fSwaySide = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00003f;
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00001f;

		if( m_nPlayerNo == 0 )
		{
			m_pPlayerUI->SetSwaySide( fSwaySide );
		}
		else if( m_nPlayerNo == 1 )
		{
			m_pPlayerUI->SetSwaySide( -fSwaySide );
		}
		
		m_pPlayerUI->SetSwayVertical( fSwayVertical );
	}

	if( m_pLife != NULL )
	{
		float fSwaySide = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00003f;
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00001f;

		m_pLife->SetSwaySide( fSwaySide );
		m_pLife->SetSwayVertical( fSwayVertical );
	}

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x + fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z + fSeekPosZ ) ,
							  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x - fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z - fSeekPosZ ) ,
							  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  エフェクトの停止
	EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

	//  エフェクトの停止
	EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

	//  魔法を所持している場合
	if( m_pMagic != NULL )
	{
		//  魔法の削除
		m_pMagic->Delete( );
		m_pMagic = NULL;
	}

	//  体力が0以下になった場合
	if( m_nLife <= 0 )
	{
		m_nLife = 0;

		m_pLife->SetLife( m_nLife );

		//  自分自身の削除
		Release( );

		//  CPU対戦の場合
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			//  ゲームオーバー画面へ
			Result::SetResult( Result::GAME_OVER );
		}
		//  プレイヤー対戦の場合
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			if( m_nPlayerNo == 0 )
			{
				//  プレイヤー2WIN画面へ
				Result::SetResult( Result::GAME_PLAYER2_WIN );
			}
			else if( m_nPlayerNo == 1 )
			{
				//  プレイヤー1WIN画面へ
				Result::SetResult( Result::GAME_PLAYER1_WIN );
			}
		}		

		//  結果画面への移行フラグをたてる
		Game::SetNextMode( Mode::MODE::RESULT );	
	}
	else
	{
		if( bBlow == true )
		{
			//  状態の変化
			SetAnimation( StateAnimator::MOTION_BLOW );
			ChangeState( m_allState[ static_cast< int >( Player::STATE::BLOW ) ] );
		}
		else
		{
			//  状態の変化
			SetAnimation( StateAnimator::MOTION_BEND );
			ChangeState( m_allState[ static_cast< int >( Player::STATE::BEND ) ] );
		}
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーのブレイブダメージ処理
//--------------------------------------------------------------------------------------
int Player::BraveDamage( int nDamage )
{
	if( m_nInvisibleTime != 0 )
	{
		return 0;
	}

	int nRealDamage = nDamage;	//  実際のダメージ格納
	int nOverDamage = 0;		//  オーバーした分のダメージ格納

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( m_nPlayerNo );

	if( pCamera != NULL )
	{
		float fSwayVertical = ( float )nDamage * ( float )nDamage * 0.0006f;

		//  縦揺れ分の代入
		pCamera->SetSwayVertical( fSwayVertical );
	}

	//  ブレイブダメージ処理
	m_nBrave -= nDamage;

	//  乱数の宣言
	std::random_device rd;

	//  乱数の設定
	std::uniform_int_distribution< int > RandomSeekPos( 5 , 20 );

	float fSeekPosX = ( float )RandomSeekPos( rd ) * 0.1f;
	float fSeekPosZ = ( float )RandomSeekPos( rd ) * 0.1f;

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x + fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z + fSeekPosZ ) ,
							  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x - fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z - fSeekPosZ ) ,
							  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  体力が0以下になった場合
	if( m_nBrave <= 0 )
	{
		//  オーバーした分の代入
		nOverDamage = abs( 0 - m_nBrave );

		m_nBrave = 0;
	}

	//  オーバーした分を実際のダメージから引く
	nRealDamage -= nOverDamage;

	return nRealDamage;
}

//--------------------------------------------------------------------------------------
//  プレイヤー全てのエフェクシアの再生ストップ処理
//--------------------------------------------------------------------------------------
void Player::StopAllEffekseer( void )
{
	for( int i = 0; i < EffekseerManager::TYPE_MAX; i++ )
	{
		if( m_handle[ i ] != -1 )
		{
			EffekseerManager::Stop( m_handle[ i ] );
		}
	}
}

//--------------------------------------------------------------------------------------
//  ロックオン状態の変更
//--------------------------------------------------------------------------------------
void Player::ChangeLockon( void )
{
	m_bLockOn = !m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  現在のジャンプ力の設定
//--------------------------------------------------------------------------------------
void Player::SetCurrentJumpPower( float jumpPower )
{
	m_fCurrentJumpPower = jumpPower;
}

//--------------------------------------------------------------------------------------
//  方向ベクトルの設定
//--------------------------------------------------------------------------------------
void Player::SetVecDirect( D3DXVECTOR3 vecDirect )
{
	m_vecDirect = vecDirect;
}

//--------------------------------------------------------------------------------------
//  注視点の設定
//--------------------------------------------------------------------------------------
void Player::SetPositionAt( D3DXVECTOR3 positionAt )
{
	m_posAt = positionAt;
}

//--------------------------------------------------------------------------------------
//  速度の設定
//--------------------------------------------------------------------------------------
void Player::SetSpeed( float speed )
{
	m_fSpeed = speed;
}

//--------------------------------------------------------------------------------------
//  移動力の設定
//--------------------------------------------------------------------------------------
void Player::SetMovePower( float movePower )
{
	m_fMovePower = movePower;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び力の設定
//--------------------------------------------------------------------------------------
void Player::SetBlowPower( float blowPower )
{
	m_fBlowPower = blowPower;
}

//--------------------------------------------------------------------------------------
//  無敵時間の設定
//--------------------------------------------------------------------------------------
void Player::SetInvisibleTime( int invisibleTime )
{
	m_nInvisibleTime = invisibleTime;
}

//--------------------------------------------------------------------------------------
//  追尾方向の設定
//--------------------------------------------------------------------------------------
void Player::SetHomingDirect( bool homingDirect )
{
	m_bHomingLeft = homingDirect;
}

//--------------------------------------------------------------------------------------
//  指定した種類のエフェクシア識別子の設定
//--------------------------------------------------------------------------------------
void Player::SetEffekseerHandle( EffekseerManager::TYPE type , ::Effekseer::Handle handle )
{
	m_handle[ static_cast< int >( type ) ] = handle;
}

//--------------------------------------------------------------------------------------
//  敵と当たり判定するかのフラグ設定
//--------------------------------------------------------------------------------------
void Player::SetJudgeHit( bool judgeHit )
{
	m_bJudgeHit = judgeHit;
}

//--------------------------------------------------------------------------------------
//  防御フラグの設定
//--------------------------------------------------------------------------------------
void Player::SetGuardFlag( bool guardFlag )
{
	m_bGuard = guardFlag;
}


//--------------------------------------------------------------------------------------
//  魔法クラスのポインタの設定
//--------------------------------------------------------------------------------------
void Player::SetMagic( Magic* magic )
{
	m_pMagic = magic;
}

//--------------------------------------------------------------------------------------
//  体力の増加
//--------------------------------------------------------------------------------------
void Player::AddLife( int addLife )
{
	m_nLife += addLife;
}

//--------------------------------------------------------------------------------------
//  魔法の削除
//--------------------------------------------------------------------------------------
void Player::DeleteMagic( void )
{
	//  魔法を所持している場合
	if( m_pMagic != NULL )
	{
		//  魔法の削除
		m_pMagic->Delete( );
		m_pMagic = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  方向ベクトルの取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	Player::GetVecDirect( void )
{
	return m_vecDirect;
}

//--------------------------------------------------------------------------------------
//  注視点の取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	Player::GetPositionAt( void )
{
	return m_posAt;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び方向ベクトルの取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	Player::GetBlowVecDirect( void )
{
	return m_blowVecDirect;
}

//--------------------------------------------------------------------------------------
//  状態の変化
//--------------------------------------------------------------------------------------
void Player::ChangeState( PlayerState* playerState )
{
	//  同じの場合リターン
	if( m_playerState == playerState )
	{
		return;
	}

	if( m_playerState != nullptr )
	{
		//  前回のプレイヤーステートの終了処理
		m_playerState->Uninit( );

		//  現在のプレイヤーステートの初期化処理
		m_playerState = playerState;
		m_playerState->Init( );
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤー番号の取得
//--------------------------------------------------------------------------------------
int	Player::GetPlayerNo( void )
{
	return m_nPlayerNo;
}

//--------------------------------------------------------------------------------------
//  無敵時間の取得
//--------------------------------------------------------------------------------------
int	Player::GetInvisibleTime( void )
{
	return m_nInvisibleTime;
}

//--------------------------------------------------------------------------------------
//  魔法クラスのポインタの取得
//--------------------------------------------------------------------------------------
Magic* Player::GetMagic( void )
{
	return m_pMagic;
}

//--------------------------------------------------------------------------------------
//  魔法の種類取得
//--------------------------------------------------------------------------------------
Player::MAGIC Player::GetMagicType( void )
{
	return m_magic;
}

//--------------------------------------------------------------------------------------
//  速度の取得
//--------------------------------------------------------------------------------------
float Player::GetSpeed( void )
{
	return m_fSpeed;
}

//--------------------------------------------------------------------------------------
//  基本速度の取得
//--------------------------------------------------------------------------------------
float Player::GetBaseSpeed( void )
{
	return m_fBaseSpeed;
}

//--------------------------------------------------------------------------------------
//  ジャンプ力の取得
//--------------------------------------------------------------------------------------
float Player::GetJumpPower( void )
{
	return m_fJumpPower;
}

//--------------------------------------------------------------------------------------
//  現在のジャンプ力の取得
//--------------------------------------------------------------------------------------
float Player::GetCurrentJumpPower( void )
{
	return m_fCurrentJumpPower;
}

//--------------------------------------------------------------------------------------
//  目標までの距離の取得
//--------------------------------------------------------------------------------------
float Player::GetTargetDistance( void )
{
	//  シーンクラスのポインタ
	Scene* pScene = NULL;											

	//  当たり判定用フィールドクラス
	HitField* pHitField = NULL;

	float targetDistance = -1.0f;

#pragma omp parallel for
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		//  シーンの先頭アドレスを取得
		pScene = Scene::GetScene( nCntPriority );

		//  シーンが空ではない間ループ
		while( pScene != NULL )
		{
			Scene::OBJTYPE objType;						//  物体の種類

			//  物体の種類の取得
			objType = pScene->GetObjType( );

			//  CPU対戦モードの場合
			if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
			{
				//  種類が敵の場合
				if( objType == Scene::OBJTYPE_ENEMY )
				{
					//  ダウンキャスト
					Enemy* pEnemy = ( Enemy* )pScene;

					//  敵の座標の代入
					D3DXVECTOR3 posEnemy = pEnemy->GetPos( );

					//  距離の算出
					targetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
											( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
											( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

					return targetDistance;
				}
			}
			//  プレイヤー対戦モードの場合
			else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
			{
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  ダウンキャスト
					Player* pPlayer = ( Player* )pScene;

					//  自分以外のプレイヤー番号の場合
					if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
					{
						//  敵の座標の代入
						D3DXVECTOR3 posEnemy = pPlayer->GetPos( );

						//  距離の算出
						targetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
												( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
												( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

						return targetDistance;
					}
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	return targetDistance;
}

//--------------------------------------------------------------------------------------
//  移動力の取得
//--------------------------------------------------------------------------------------
float Player::GetMovePower( void )
{
	return m_fMovePower;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び力の取得
//--------------------------------------------------------------------------------------
float Player::GetBlowPower( void )
{
	return m_fBlowPower;
}

//--------------------------------------------------------------------------------------
//  体力の取得
//--------------------------------------------------------------------------------------
int Player::GetLife( void )
{
	return m_nLife;
}

//--------------------------------------------------------------------------------------
//  プレイヤーが当たり判定をするかどうかの取得処理
//--------------------------------------------------------------------------------------
bool Player::GetJudgeHit( void )
{
	return m_bJudgeHit;
}

//--------------------------------------------------------------------------------------
//  プレイヤーが防御しているかどうかの取得処理
//--------------------------------------------------------------------------------------
bool Player::GetGuard( void )
{
	return m_bGuard;
}

//--------------------------------------------------------------------------------------
//  ロックオン情報の取得
//--------------------------------------------------------------------------------------
bool Player::GetLockon( void )
{
	return m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  追尾する向きの取得( false : 右 , true : 左 )
//--------------------------------------------------------------------------------------
bool Player::GetHomingDirect( void )
{
	return m_bHomingLeft;
}

//--------------------------------------------------------------------------------------
//  状態の取得
//--------------------------------------------------------------------------------------
PlayerState* Player::GetPlayerState( Player::STATE state )
{
	return m_allState[ static_cast< int >( state ) ];
}

//--------------------------------------------------------------------------------------
//  エフェクシア識別子の取得
//--------------------------------------------------------------------------------------
::Effekseer::Handle Player::GetEffekseerHandle( EffekseerManager::TYPE type )
{
	return m_handle[ static_cast< int >( type ) ];
}

//--------------------------------------------------------------------------------------
//  アニメーションキーの取得処理
//--------------------------------------------------------------------------------------
int	Player::GetAnimationKey( void )
{
	return m_nKey;
}

//--------------------------------------------------------------------------------------
//  アニメーションキーの取得処理
//--------------------------------------------------------------------------------------
float Player::GetAnimationFrame( void )
{
	return m_fFrame;
}

//--------------------------------------------------------------------------------------
//  ジャンプでの移動
//--------------------------------------------------------------------------------------
void Player::MoveHeight( float addMove )
{
	m_position.y += addMove;
}

//--------------------------------------------------------------------------------------
//  ジャンプ力を下げる
//--------------------------------------------------------------------------------------
void Player::DownCurrentJumpPower( float downPower )
{
	m_fCurrentJumpPower -= downPower;
}