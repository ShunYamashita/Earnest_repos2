//--------------------------------------------------------------------------------------
//  プレイヤー   ( playerWinks.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinks.h"
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
#include "magic.h"
#include "life.h"
#include "utility.h"
#include "fbxLoader.h"
#include <random>
#include "Wwise.h"
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  ヘッダーファイル( ステート系 )
//--------------------------------------------------------------------------------------
#include "playerWinksIdle.h"
#include "playerWinksMove.h"
#include "playerWinksJump.h"
#include "playerWinksLanding.h"
#include "playerWinksAttack.h"
#include "playerWinksHPAttack.h"
#include "playerWinksFreeDash.h"
#include "playerWinksLockonDash.h"
#include "playerWinksGuard.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define PLAYER_TO_CAMERA_DISTANCE_Y				( 11.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Y2			( 15.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Z				( 25.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Z2			( 35.0f )
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

static const float	PLAYER_HEIGHT				= 10.0f;

static const float	PLAYER_SHADOW_SCALE			= 0.55f;

static const float	PLAYER_JUMP_POWER			= 2.0f;

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  プレイヤークラスのコンストラクタ
//--------------------------------------------------------------------------------------
PlayerWinks::PlayerWinks( ) : Scene( 3 )
{
	m_beforePos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	m_fSpeed = 0.0f;
	m_nWaitTime = 0;
	m_fCurrentJumpPower = 0.0f;
	m_nCntJump = 0;
	m_bLockOn = true;
	m_hitSphere.fLength = PLAYER_HIT_SPHERE_LENGTH * 0.5f;
	m_hitSphere.position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_pLife = nullptr;
	m_pPlayerWinksUI = nullptr;
	m_pMagic = nullptr;
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
	m_fJumpPower = PLAYER_JUMP_POWER;
}

//--------------------------------------------------------------------------------------
//  プレイヤークラスのデストラクタ
//--------------------------------------------------------------------------------------
PlayerWinks::~PlayerWinks( )
{

}

//--------------------------------------------------------------------------------------
//  プレイヤーの初期化処理
//--------------------------------------------------------------------------------------
HRESULT PlayerWinks::Init( void )
{
	m_drawDepth = true;

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_PLAYER );

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
									D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) ,
									m_nLife );
		}
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  プレイヤーUIの生成
			m_pPlayerWinksUI = PlayerUI::Create( PlayerUI::TYPE_PLAYER1 ,
												 D3DXVECTOR3( PLAYER_UI_POS_X , PLAYER_UI_POS_Y , 0.0f ) ,
												 D3DXVECTOR3( PLAYER_UI_SIZE_X , PLAYER_UI_SIZE_Y , 0.0f ) );

			//  体力の生成
			m_pLife = Life::Create( Life::TYPE_LIFE002 ,
									D3DXVECTOR3( PLAYER_VS_LIFE_POS_X , PLAYER_VS_LIFE_POS_Y , 0.0f ) ,
									D3DXVECTOR3( PLAYER_VS_LIFE_SIZE_X , PLAYER_VS_LIFE_SIZE_Y , 0.0f ) ,
									D3DXVECTOR2( 0.0f , 0.0f ) ,
									D3DXVECTOR2( 1.0f , 1.0f ) ,
									D3DXCOLOR( 0.1f , 0.9f , 1.0f , 1.0f ) ,
									m_nLife );
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
									D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) ,
									m_nLife );
		}
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  プレイヤーUIの生成
			m_pPlayerWinksUI = PlayerUI::Create( PlayerUI::TYPE_PLAYER2 ,
												 D3DXVECTOR3( PLAYER2_UI_POS_X , PLAYER2_UI_POS_Y , 0.0f ) ,
												 D3DXVECTOR3( PLAYER2_UI_SIZE_X , PLAYER2_UI_SIZE_Y , 0.0f ) );

			m_pLife = Life::Create( Life::TYPE_LIFE003 ,
									D3DXVECTOR3( PLAYER2_VS_LIFE_POS_X , PLAYER2_VS_LIFE_POS_Y , 0.0f ) ,
									D3DXVECTOR3( PLAYER2_VS_LIFE_SIZE_X , PLAYER2_VS_LIFE_SIZE_Y , 0.0f ) ,
									D3DXVECTOR2( 0.0f , 0.0f ) ,
									D3DXVECTOR2( 1.0f , 1.0f ) ,
									D3DXCOLOR( 0.1f , 0.9f , 1.0f , 1.0f ) ,
									m_nLife );
		}

		//  体力の設定
		m_pLife->SetEndLife( m_nLife );
	}

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
	m_allState[ static_cast< int >( PlayerWinks::STATE::IDLE ) ] = new PlayerWinksIdle( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::MOVE ) ] = new PlayerWinksMove( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::JUMP ) ] = new PlayerWinksJump( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::LANDING ) ] = new PlayerWinksLanding( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::ATTACK ) ] = new PlayerWinksAttack( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::HP_ATTACK ) ] = new PlayerWinksHPAttack( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::FREE_DASH ) ] = new PlayerWinksFreeDash( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::LOCKON_DASH ) ] = new PlayerWinksLockonDash( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::GUARD ) ] = new PlayerWinksGuard( this );

	//  ニュートラル状態で初期化
	m_playerState = m_allState[ static_cast< int >( PlayerWinks::STATE::IDLE ) ];

	//  FBXデータの作成
	m_fbx = FBXLoader::Create( FBXLoader::TYPE::WINKS , m_position , m_posAt , 0.06f );
	m_fbx->SetMotion( FBXLoader::MOTION::IDLE );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  プレイヤーの終了処理
//--------------------------------------------------------------------------------------
void PlayerWinks::Uninit( void )
{
	if( m_pLife != nullptr )
	{
		m_pLife = nullptr;
	}

	if( m_pPlayerWinksUI != nullptr )
	{
		m_pPlayerWinksUI = nullptr;
	}

	if( m_fbx != nullptr )
	{
		m_fbx->Uninit( );
		m_fbx = nullptr;
	}

	//  プレイヤーステートの数分のループ
	for( int countState = 0; countState < static_cast< int >( PlayerWinks::STATE::MAX ) - 1; ++countState )
	{
		m_allState[ countState ]->Uninit( );
		delete m_allState[ countState ];
		m_allState[ countState ] = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーの更新処理
//--------------------------------------------------------------------------------------
void PlayerWinks::Update( void )
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

	//  体力が上限を超えた場合
	if( m_nLife > BASE_LIFE )
	{
		m_nLife = BASE_LIFE;
	}

	//  体力ポインタがある場合
	if( m_pLife != nullptr )
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

	if( m_fbx != nullptr )
	{
		m_fbx->Update( );
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーの描画処理
//--------------------------------------------------------------------------------------
void PlayerWinks::Draw( void )
{
	Camera* camera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVectorDirect;
	D3DXVECTOR3 cameraToPlayerWinks;

	if( camera != nullptr )
	{
		D3DXVECTOR3 cameraPosition = camera->GetCameraPosEye( );
		D3DXVECTOR3 playerWinksPosition = m_position;
		cameraPosition.y = 0.0f;
		playerWinksPosition.y = 0.0f;

		cameraToPlayerWinks = playerWinksPosition - cameraPosition;
		D3DXVec3Normalize( &cameraToPlayerWinks , &cameraToPlayerWinks );
		cameraVectorDirect = camera->GetCameraVecDirect( );
		cameraVectorDirect.y = 0.0f;
		D3DXVec3Normalize( &cameraVectorDirect , &cameraVectorDirect );
	}

	if( Utility::IntoView( cameraVectorDirect , cameraToPlayerWinks , D3DX_PI * 0.5f ) )
	{
		if( m_fbx != nullptr )
		{
			m_fbx->SetDrawInfo( m_position , m_posAt );
			m_fbx->Draw( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーの描画処理
//--------------------------------------------------------------------------------------
void PlayerWinks::DrawDepth( void )
{
	if( m_fbx != nullptr )
	{
		m_fbx->SetDrawInfo( m_position , m_posAt );
		m_fbx->DrawDepth( );
	}
}

//--------------------------------------------------------------------------------------
//  地面との当たり判定をする関数
//--------------------------------------------------------------------------------------
bool PlayerWinks::JudgeHitGround( void )
{
	//  シーンクラスのポインタ
	Scene* pScene = nullptr;											

	//  当たり判定用フィールドクラス
	HitField* pHitField = nullptr;

#pragma omp parallel for
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		//  シーンの先頭アドレスを取得
		pScene = Scene::GetScene( nCntPriority );

		//  シーンが空ではない間ループ
		while( pScene != nullptr )
		{
			Scene::OBJTYPE objType;						//  物体の種類

			//  物体の種類の取得
			objType = pScene->GetObjType( );

			//  種類が当たり判定用のフィールドの場合
			if( objType == Scene::OBJTYPE_HIT_FIELD )
			{
				//  当たり判定用フィールドクラスにダウンキャスト
				pHitField = ( HitField* )pScene;

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
bool PlayerWinks::JudgeHitEnemy( void )
{
	//  シーンクラスのポインタ
	Scene* pScene = nullptr;	

	if( m_bJudgeHit )
	{
#pragma omp parallel for
		//  優先度の最大数分のループ
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  シーンの先頭アドレスを取得
			pScene = Scene::GetScene( nCntPriority );

			//  シーンが空ではない間ループ
			while( pScene != nullptr )
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
						PlayerWinks* pPlayerWinks = ( PlayerWinks* )pScene;

						//  自分以外のプレイヤーでかつ、当たり判定をする場合
						if( m_nPlayerNo != pPlayerWinks->GetPlayerNo( ) && pPlayerWinks->GetJudgeHit( ) )
						{
							float fDist = 0.0f;

							if( Utility::HitSphere( m_hitSphere , pPlayerWinks->GetHitSphere( ) , &fDist ) )
							{
								//  速度をなくす
								m_fSpeed = 0.0f;

								//  反射するベクトルを求める
								D3DXVECTOR3 reflectVec = m_hitSphere.position - pPlayerWinks->GetHitSphere( ).position;
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
bool PlayerWinks::JudgeHitWall( void )
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
PlayerWinks* PlayerWinks::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed , MAGIC magic , int nPlayerWinksNo )
{
	PlayerWinks* pPlayerWinks;

	//  インスタンス生成
	pPlayerWinks = new PlayerWinks;

	//  座標の代入
	pPlayerWinks->m_position = position;

	//  最初の座標の代入
	pPlayerWinks->m_firstPos = position;

	//  注視点座標の代入
	pPlayerWinks->m_posAt = position;

	//  回転角の代入
	pPlayerWinks->m_rot = rot;

	//  大きさ倍率の代入
	pPlayerWinks->m_scale = scale;

	//  速度の代入
	pPlayerWinks->m_fBaseSpeed = fSpeed;

	//  所持魔法の代入
	pPlayerWinks->m_magic = magic;

	//  プレイヤー番号の代入
	pPlayerWinks->m_nPlayerNo = nPlayerWinksNo;

	//  初期化
	pPlayerWinks->Init( );

	return pPlayerWinks;
}

//--------------------------------------------------------------------------------------
//  プレイヤーのダメージ処理
//--------------------------------------------------------------------------------------
void PlayerWinks::Damage( D3DXVECTOR3 blowVecDirect , float fBlowPower , int nDamage , bool bBlow )
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

	if( pCamera != nullptr )
	{
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.0000006f;

		//  縦揺れ分の代入
		pCamera->SetSwayVertical( fSwayVertical );
	}

	if( m_pPlayerWinksUI != nullptr )
	{
		float fSwaySide = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00003f;
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00001f;

		if( m_nPlayerNo == 0 )
		{
			m_pPlayerWinksUI->SetSwaySide( fSwaySide );
		}
		else if( m_nPlayerNo == 1 )
		{
			m_pPlayerWinksUI->SetSwaySide( -fSwaySide );
		}
		
		m_pPlayerWinksUI->SetSwayVertical( fSwayVertical );
	}

	if( m_pLife != nullptr )
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
	if( m_pMagic != nullptr )
	{
		//  魔法の削除
		m_pMagic->Delete( );
		m_pMagic = nullptr;
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

		}
		else
		{

		}
	}
}

//--------------------------------------------------------------------------------------
//  プレイヤーのブレイブダメージ処理
//--------------------------------------------------------------------------------------
int PlayerWinks::BraveDamage( int nDamage )
{
	if( m_nInvisibleTime != 0 )
	{
		return 0;
	}

	int nRealDamage = nDamage;	//  実際のダメージ格納
	int nOverDamage = 0;		//  オーバーした分のダメージ格納

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( m_nPlayerNo );

	if( pCamera != nullptr )
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
void PlayerWinks::StopAllEffekseer( void )
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
void PlayerWinks::ChangeLockon( void )
{
	m_bLockOn = !m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  現在のジャンプ力の設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetCurrentJumpPower( float jumpPower )
{
	m_fCurrentJumpPower = jumpPower;
}

//--------------------------------------------------------------------------------------
//  方向ベクトルの設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetVecDirect( D3DXVECTOR3 vecDirect )
{
	m_vecDirect = vecDirect;
}

//--------------------------------------------------------------------------------------
//  注視点の設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetPositionAt( D3DXVECTOR3 positionAt )
{
	m_posAt = positionAt;
}

//--------------------------------------------------------------------------------------
//  速度の設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetSpeed( float speed )
{
	m_fSpeed = speed;
}

//--------------------------------------------------------------------------------------
//  移動力の設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetMovePower( float movePower )
{
	m_fMovePower = movePower;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び力の設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetBlowPower( float blowPower )
{
	m_fBlowPower = blowPower;
}

//--------------------------------------------------------------------------------------
//  無敵時間の設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetInvisibleTime( int invisibleTime )
{
	m_nInvisibleTime = invisibleTime;
}

//--------------------------------------------------------------------------------------
//  追尾方向の設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetHomingDirect( bool homingDirect )
{
	m_bHomingLeft = homingDirect;
}

//--------------------------------------------------------------------------------------
//  指定した種類のエフェクシア識別子の設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetEffekseerHandle( EffekseerManager::TYPE type , ::Effekseer::Handle handle )
{
	m_handle[ static_cast< int >( type ) ] = handle;
}

//--------------------------------------------------------------------------------------
//  敵と当たり判定するかのフラグ設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetJudgeHit( bool judgeHit )
{
	m_bJudgeHit = judgeHit;
}

//--------------------------------------------------------------------------------------
//  防御フラグの設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetGuardFlag( bool guardFlag )
{
	m_bGuard = guardFlag;
}

//--------------------------------------------------------------------------------------
//  魔法クラスのポインタの設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetMagic( Magic* magic )
{
	m_pMagic = magic;
}

//--------------------------------------------------------------------------------------
//  モーションの設定
//--------------------------------------------------------------------------------------
void PlayerWinks::SetMotion( FBXLoader::MOTION motion )
{
	if( m_fbx != nullptr )
	{
		m_fbx->SetMotion( motion );
	}
}

//--------------------------------------------------------------------------------------
//  体力の増加
//--------------------------------------------------------------------------------------
void PlayerWinks::AddLife( int addLife )
{
	m_nLife += addLife;
}

//--------------------------------------------------------------------------------------
//  魔法の削除
//--------------------------------------------------------------------------------------
void PlayerWinks::DeleteMagic( void )
{
	//  魔法を所持している場合
	if( m_pMagic != nullptr )
	{
		//  魔法の削除
		m_pMagic->Delete( );
		m_pMagic = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  方向ベクトルの取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	PlayerWinks::GetVecDirect( void )
{
	return m_vecDirect;
}

//--------------------------------------------------------------------------------------
//  注視点の取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	PlayerWinks::GetPositionAt( void )
{
	return m_posAt;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び方向ベクトルの取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	PlayerWinks::GetBlowVecDirect( void )
{
	return m_blowVecDirect;
}

//--------------------------------------------------------------------------------------
//  状態の変化
//--------------------------------------------------------------------------------------
void PlayerWinks::ChangeState( PlayerWinksState* playerState )
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
int	PlayerWinks::GetPlayerNo( void )
{
	return m_nPlayerNo;
}

//--------------------------------------------------------------------------------------
//  無敵時間の取得
//--------------------------------------------------------------------------------------
int	PlayerWinks::GetInvisibleTime( void )
{
	return m_nInvisibleTime;
}

//--------------------------------------------------------------------------------------
//  魔法クラスのポインタの取得
//--------------------------------------------------------------------------------------
Magic* PlayerWinks::GetMagic( void )
{
	return m_pMagic;
}

//--------------------------------------------------------------------------------------
//  魔法の種類取得
//--------------------------------------------------------------------------------------
PlayerWinks::MAGIC PlayerWinks::GetMagicType( void )
{
	return m_magic;
}

//--------------------------------------------------------------------------------------
//  速度の取得
//--------------------------------------------------------------------------------------
float PlayerWinks::GetSpeed( void )
{
	return m_fSpeed;
}

//--------------------------------------------------------------------------------------
//  基本速度の取得
//--------------------------------------------------------------------------------------
float PlayerWinks::GetBaseSpeed( void )
{
	return m_fBaseSpeed;
}

//--------------------------------------------------------------------------------------
//  ジャンプ力の取得
//--------------------------------------------------------------------------------------
float PlayerWinks::GetJumpPower( void )
{
	return m_fJumpPower;
}

//--------------------------------------------------------------------------------------
//  現在のジャンプ力の取得
//--------------------------------------------------------------------------------------
float PlayerWinks::GetCurrentJumpPower( void )
{
	return m_fCurrentJumpPower;
}

//--------------------------------------------------------------------------------------
//  目標までの距離の取得
//--------------------------------------------------------------------------------------
float PlayerWinks::GetTargetDistance( void )
{
	//  シーンクラスのポインタ
	Scene* pScene = nullptr;											

	//  当たり判定用フィールドクラス
	HitField* pHitField = nullptr;

	float targetDistance = -1.0f;

#pragma omp parallel for
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		//  シーンの先頭アドレスを取得
		pScene = Scene::GetScene( nCntPriority );

		//  シーンが空ではない間ループ
		while( pScene != nullptr )
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
					PlayerWinks* pPlayerWinks = ( PlayerWinks* )pScene;

					//  自分以外のプレイヤー番号の場合
					if( m_nPlayerNo != pPlayerWinks->GetPlayerNo( ) )
					{
						//  敵の座標の代入
						D3DXVECTOR3 posEnemy = pPlayerWinks->GetPos( );

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
float PlayerWinks::GetMovePower( void )
{
	return m_fMovePower;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び力の取得
//--------------------------------------------------------------------------------------
float PlayerWinks::GetBlowPower( void )
{
	return m_fBlowPower;
}

//--------------------------------------------------------------------------------------
//  体力の取得
//--------------------------------------------------------------------------------------
int PlayerWinks::GetLife( void )
{
	return m_nLife;
}

//--------------------------------------------------------------------------------------
//  プレイヤーが当たり判定をするかどうかの取得処理
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetJudgeHit( void )
{
	return m_bJudgeHit;
}

//--------------------------------------------------------------------------------------
//  プレイヤーが防御しているかどうかの取得処理
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetGuard( void )
{
	return m_bGuard;
}

//--------------------------------------------------------------------------------------
//  ロックオン情報の取得
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetLockon( void )
{
	return m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  追尾する向きの取得( false : 右 , true : 左 )
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetHomingDirect( void )
{
	return m_bHomingLeft;
}

//--------------------------------------------------------------------------------------
//  モーションが終了したかの取得
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetMotionFinish( void )
{
	if( m_fbx != nullptr )
	{
		return m_fbx->GetMotionFinish( );
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  状態の取得
//--------------------------------------------------------------------------------------
PlayerWinksState* PlayerWinks::GetPlayerState( PlayerWinks::STATE state )
{
	return m_allState[ static_cast< int >( state ) ];
}

//--------------------------------------------------------------------------------------
//  エフェクシア識別子の取得
//--------------------------------------------------------------------------------------
::Effekseer::Handle PlayerWinks::GetEffekseerHandle( EffekseerManager::TYPE type )
{
	return m_handle[ static_cast< int >( type ) ];
}

//--------------------------------------------------------------------------------------
//  ジャンプでの移動
//--------------------------------------------------------------------------------------
void PlayerWinks::MoveHeight( float addMove )
{
	m_position.y += addMove;
}

//--------------------------------------------------------------------------------------
//  ジャンプ力を下げる
//--------------------------------------------------------------------------------------
void PlayerWinks::DownCurrentJumpPower( float downPower )
{
	m_fCurrentJumpPower -= downPower;
}

//--------------------------------------------------------------------------------------
//  モーションフレーム取得
//--------------------------------------------------------------------------------------
int PlayerWinks::GetMotionFrame( void )
{
	if( m_fbx != nullptr )
	{
		return m_fbx->GetMotionFrame( );
	}
}

//--------------------------------------------------------------------------------------
//  モーション全体からフレーム割合取得
//--------------------------------------------------------------------------------------
float PlayerWinks::GetMotionFrameRate( void )
{
	if( m_fbx != nullptr )
	{
		return m_fbx->GetMotionFrameRate( );
	}
}