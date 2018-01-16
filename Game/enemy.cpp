//--------------------------------------------------------------------------------------
//  エネミー   ( enemy.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemy.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "scene2D.h"
#include "meshField.h"
#include "player.h"
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
#include "shadow.h"
#include "life.h"
#include "brave.h"
#include "stateAnimator.h"
#include "utility.h"
#include <random>
#include "Wwise.h"
#include "imgui_impl_dx9.h"
#include "AIManager.h"

//--------------------------------------------------------------------------------------
//  ヘッダーファイル( ステート系 )
//--------------------------------------------------------------------------------------
#include "enemyState.h"
#include "enemyBend.h"
#include "enemyBlow.h"
#include "enemyHPAttack000.h"
#include "enemyEscape.h"
#include "enemyGuard.h"
#include "enemyBraveAttack000After.h"
#include "enemyBraveAttack000Before.h"
#include "enemyJump.h"
#include "enemyLanding.h"
#include "enemyLockonDash.h"
#include "enemyMagicActive.h"
#include "enemyMagicReady.h"
#include "enemyMove.h"
#include "enemyNeutral.h"
#include "enemyPassive.h"
#include "enemyResultLose.h"
#include "enemyResultWin.h"
#include "enemyRushAttack.h"
#include "enemyFreeDash.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define ENEMY_TO_CAMERA_DISTANCE_Y				( 9.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Y2				( 10.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Z				( 20.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Z2				( 23.0f )
#define ENEMY_TO_CUBE_DISTANCE					( 7.0f )
#define ENEMY_TO_CUBE_PUT_DISTANCE				( 2.5f )
#define ENEMY_SIZE								( 2.5f )
#define ENEMY_GOAL_SIZE							( 0.8f )
#define ENEMY_HIT_SIZE							( 2.0f )
#define ENEMY_WAIT_TIME							( 20 )
#define ENEMY_GRAVITY							( 0.1f )
#define ENEMY_GRAVITY2							( 0.2f )
#define ENEMY_ENABLE_NUM_JUMP					( 2 )
#define ENEMY_TO_CAMERA_RATE					( 0.5f )
#define ENEMY_LOCKON_DASH_SPEED					( 3.0f )
#define ENEMY_HIT_SPHERE_LENGTH					( 6.0f )
#define ENEMY_HEAL_LIFE							( 20 )

#define ENEMY_AVOID_SPEED						( 1.5f )

#define ENEMY_HIT_SPHERE_POS_Y					( 3.0f )

#define ENEMY_LIFE_POS_X						( 975.0f )
#define ENEMY_LIFE_POS_Y						( 605.0f )
#define ENEMY_LIFE_SIZE_X						( 255.0f )
#define ENEMY_LIFE_SIZE_Y						( 30.0f )

#define ENEMY_VS_LIFE_POS_X						( 125.0f )
#define ENEMY_VS_LIFE_POS_Y						( 646.0f )
#define ENEMY_VS_LIFE_SIZE_X					( 442.0f )
#define ENEMY_VS_LIFE_SIZE_Y					( 38.0f )

#define ENEMY_BRAVE_POS_X						( 440.0f )
#define ENEMY_BRAVE_POS_Y						( 570.0f )
#define ENEMY_BRAVE_SIZE_X						( 60.0f )
#define ENEMY_BRAVE_SIZE_Y						( 60.0f )

#define ENEMY_UI_POS_X							( SCREEN_WIDTH * 0.25f )
#define ENEMY_UI_POS_Y							( SCREEN_HEIGHT * 0.88f )
#define ENEMY_UI_SIZE_X							( 640.0f )
#define ENEMY_UI_SIZE_Y							( 200.0f )

#define ENEMY2_LIFE_POS_X						( 975.0f )
#define ENEMY2_LIFE_POS_Y						( 605.0f )
#define ENEMY2_LIFE_SIZE_X						( 255.0f )
#define ENEMY2_LIFE_SIZE_Y						( 30.0f )

#define ENEMY2_VS_LIFE_POS_X					( 715.0f )
#define ENEMY2_VS_LIFE_POS_Y					( 646.0f )
#define ENEMY2_VS_LIFE_SIZE_X					( 442.0f )
#define ENEMY2_VS_LIFE_SIZE_Y					( 38.0f )

#define ENEMY2_BRAVE_POS_X						( 990.0f )
#define ENEMY2_BRAVE_POS_Y						( 565.0f )
#define ENEMY2_BRAVE_SIZE_X						( 35.0f )
#define ENEMY2_BRAVE_SIZE_Y						( 35.0f )

#define ENEMY2_UI_POS_X							( SCREEN_WIDTH * 0.75f )
#define ENEMY2_UI_POS_Y							( SCREEN_HEIGHT * 0.88f )
#define ENEMY2_UI_SIZE_X						( 640.0f )
#define ENEMY2_UI_SIZE_Y						( 200.0f )

#define ENEMY_JUMP_								( 0.4f )

#define ENEMY_FINISH_SPEED						( 3.0f )

#define ENEMY_HOMIMG_TIME						( 90.0f )
#define ENEMY_ENABLE_HP_ATTACK_DIST				( 150.0f )
#define ENEMY_HP_ATTACK_SPEED					( 1.0f )

static const int	ENEMY_HP_DAMAGE				= 150;
static const int	ENEMY_FINISHER_DAMAGE		= 250;

static const float	ENEMY_HP_RANGE				= 30.0f;
static const float	ENEMY_HP_RANGE_HEIGHT		= 12.0f;

static const float	ENEMY_HP_BLOW_POWER			= 3.0f;
static const float	ENEMY_FINISHER_BLOW_POWER	= 7.0f;

static const int	ENEMY_GUARD_FRAME			= 5;

static const float	CAMERA_ROTATE_ANGLE			= 0.025f;

static const float	ENEMY_HEIGHT				= 10.0f;

static const float	ENEMY_SHADOW_SCALE			= 0.55f;

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  エネミークラスのコンストラクタ
//--------------------------------------------------------------------------------------
Enemy::Enemy( ) : SceneModelAnim( 3 )
{
	m_beforePos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	m_fSpeed = 0.0f;
	m_nWaitTime = 0;
	m_fCurrentJumpPower = 0.0f;
	m_nCntJump = 0;
	m_bLockOn = true;
	m_hitSphere.fLength = ENEMY_HIT_SPHERE_LENGTH * 0.5f;
	m_hitSphere.position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_pLife = NULL;
	m_pBrave = NULL;
	m_pStateAnimator = NULL;
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
	m_characterAIIndex = -1;
}

//--------------------------------------------------------------------------------------
//  エネミークラスのデストラクタ
//--------------------------------------------------------------------------------------
Enemy::~Enemy( )
{

}

//--------------------------------------------------------------------------------------
//  エネミーの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Enemy::Init( void )
{
	m_drawDepth = true;

	//  シーンモデル
	SceneModelAnim::Init( );

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_ENEMY );

	//  体力の生成
	m_pLife = Life::Create( Life::TYPE_LIFE001 ,
							D3DXVECTOR3( ENEMY_LIFE_POS_X , ENEMY_LIFE_POS_Y , 0.0f ) ,
							D3DXVECTOR3( ENEMY_LIFE_SIZE_X , ENEMY_LIFE_SIZE_Y , 0.0f ) ,
							D3DXVECTOR2( 0.0f , 0.0f ) ,
							D3DXVECTOR2( 1.0f , 1.0f ) ,
							D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) ,
							m_nLife );

	//  体力の設定
	m_pLife->SetEndLife( m_nLife );

	//  ステートマシン( アニメーター )の生成
	m_pStateAnimator = StateAnimator::Create( StateAnimator::MOTION_NEUTRAL );

	//  エネミー注視点の設定
	m_posAt = m_position - m_vecDirect * 5.0f;

	//  方向ベクトルの代入
	D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

	//  エネミーステートの生成
	m_allState[ static_cast< int >( Enemy::STATE::ATTACK_HP_ATTACK000 ) ] = new EnemyHPAttack000( this );
	m_allState[ static_cast< int >( Enemy::STATE::ATTACK_BRAVE_ATTACK000_BEFORE ) ] = new EnemyBraveAttack000Before( this );
	m_allState[ static_cast< int >( Enemy::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) ] = new EnemyBraveAttack000After( this );
	m_allState[ static_cast< int >( Enemy::STATE::BEND ) ] = new EnemyBend( this );
	m_allState[ static_cast< int >( Enemy::STATE::BLOW ) ] = new EnemyBlow( this );
	m_allState[ static_cast< int >( Enemy::STATE::ESCAPE ) ] = new EnemyEscape( this );
	m_allState[ static_cast< int >( Enemy::STATE::GUARD ) ] = new EnemyGuard( this );
	m_allState[ static_cast< int >( Enemy::STATE::JUMP ) ] = new EnemyJump( this );
	m_allState[ static_cast< int >( Enemy::STATE::LANDING ) ] = new EnemyLanding( this );
	m_allState[ static_cast< int >( Enemy::STATE::LOCKON_DASH ) ] = new EnemyLockonDash( this );
	m_allState[ static_cast< int >( Enemy::STATE::FREE_DASH ) ] = new EnemyFreeDash( this );
	m_allState[ static_cast< int >( Enemy::STATE::MAGIC_ACTIVE ) ] = new EnemyMagicActive( this );
	m_allState[ static_cast< int >( Enemy::STATE::MAGIC_READY ) ] = new EnemyMagicReady( this );
	m_allState[ static_cast< int >( Enemy::STATE::MOVE ) ] = new EnemyMove( this );
	m_allState[ static_cast< int >( Enemy::STATE::NEUTRAL ) ] = new EnemyNeutral( this );
	m_allState[ static_cast< int >( Enemy::STATE::PASSIVE ) ] = new EnemyPassive( this );
	m_allState[ static_cast< int >( Enemy::STATE::RESULT_LOSE ) ] = new EnemyResultLose( this );
	m_allState[ static_cast< int >( Enemy::STATE::RESULT_WIN ) ] = new EnemyResultWin( this );
	m_allState[ static_cast< int >( Enemy::STATE::RUSH_ATTACK ) ] = new EnemyRushAttack( this );

	//  ニュートラル状態で初期化
	m_enemyState = m_allState[ static_cast< int >( Enemy::STATE::NEUTRAL ) ];
	SetAnimation( StateAnimator::MOTION_NEUTRAL );

	//  AI管理クラスにキャラクターAIとして追加
	m_characterAIIndex = AIManager::AddCharacterAI( "enemy" , this );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  エネミーの終了処理
//--------------------------------------------------------------------------------------
void Enemy::Uninit( void )
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

	if( m_pStateAnimator != NULL )
	{
		delete m_pStateAnimator;
		m_pStateAnimator = NULL;
	}

	//  エネミーステートの数分のループ
	for( int countState = 0; countState < static_cast< int >( Enemy::STATE::MAX ); ++countState )
	{
		m_allState[ countState ]->Uninit( );
		delete m_allState[ countState ];
		m_allState[ countState ] = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  エネミーの更新処理
//--------------------------------------------------------------------------------------
void Enemy::Update( void )
{
	//  開始ボイスをまだ再生していない場合
	if( m_bVoiceStart == false )
	{
		m_bVoiceStart = true;
	}

	//  受ける側の当たり判定の代入
	m_hitSphere.position = m_position;
	m_hitSphere.position.y += ENEMY_HIT_SPHERE_POS_Y;

	//  エネミーステートクラスの更新
	if( m_enemyState != nullptr )
	{
		m_enemyState->Update( );
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
//  エネミーの描画処理
//--------------------------------------------------------------------------------------
void Enemy::Draw( void )
{
	Camera* camera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVectorDirect;
	D3DXVECTOR3 cameraToEnemy;

	if( camera != nullptr )
	{
		D3DXVECTOR3 cameraPosition = camera->GetCameraPosEye( );
		D3DXVECTOR3 enemyPosition = m_position;
		cameraPosition.y = 0.0f;
		enemyPosition.y = 0.0f;

		cameraToEnemy = enemyPosition - cameraPosition;
		D3DXVec3Normalize( &cameraToEnemy , &cameraToEnemy );
		cameraVectorDirect = camera->GetCameraVecDirect( );
		cameraVectorDirect.y = 0.0f;
		D3DXVec3Normalize( &cameraVectorDirect , &cameraVectorDirect );
	}

	if( Utility::IntoView( cameraVectorDirect , cameraToEnemy , D3DX_PI * 0.5f ) )
	{
		//  シーンモデル
		SceneModelAnim::Draw( );
	}
}

//--------------------------------------------------------------------------------------
//  地面との当たり判定をする関数
//--------------------------------------------------------------------------------------
bool Enemy::JudgeHitGround( void )
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
bool Enemy::JudgeHitEnemy( void )
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

				//  種類がプレイヤーの場合
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  ダウンキャスト
					Player* pPlayer = ( Player* )pScene;

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
bool Enemy::JudgeHitWall( void )
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
Enemy* Enemy::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed , MAGIC magic )
{
	Enemy* pEnemy;

	//  インスタンス生成
	pEnemy = new Enemy;

	//  モデルの種類の代入
	pEnemy->m_type = SceneModelAnim::TYPE_ENEMY;

	//  座標の代入
	pEnemy->m_position = position;

	//  最初の座標の代入
	pEnemy->m_firstPos = position;

	//  注視点座標の代入
	pEnemy->m_posAt = position;

	//  回転角の代入
	pEnemy->m_rot = rot;

	//  大きさ倍率の代入
	pEnemy->m_scale = scale;

	//  速度の代入
	pEnemy->m_fBaseSpeed = fSpeed;

	//  所持魔法の代入
	pEnemy->m_magic = magic;

	//  初期化
	pEnemy->Init( );

	return pEnemy;
}

//--------------------------------------------------------------------------------------
//  エネミーのダメージ処理
//--------------------------------------------------------------------------------------
void Enemy::Damage( D3DXVECTOR3 blowVecDirect , float fBlowPower , int nDamage , bool bBlow )
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
	}
	else
	{
		if( bBlow == true )
		{
			//  状態の変化
			SetAnimation( StateAnimator::MOTION_BLOW );
			ChangeState( m_allState[ static_cast< int >( Enemy::STATE::BLOW ) ] );
		}
		else
		{
			//  状態の変化
			SetAnimation( StateAnimator::MOTION_BEND );
			ChangeState( m_allState[ static_cast< int >( Enemy::STATE::BEND ) ] );
		}
	}
}

//--------------------------------------------------------------------------------------
//  エネミーのブレイブダメージ処理
//--------------------------------------------------------------------------------------
int Enemy::BraveDamage( int nDamage )
{
	if( m_nInvisibleTime != 0 )
	{
		return 0;
	}

	int nRealDamage = nDamage;	//  実際のダメージ格納
	int nOverDamage = 0;		//  オーバーした分のダメージ格納

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
//  エネミー全てのエフェクシアの再生ストップ処理
//--------------------------------------------------------------------------------------
void Enemy::StopAllEffekseer( void )
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
void Enemy::ChangeLockon( void )
{
	m_bLockOn = !m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  現在のジャンプ力の設定
//--------------------------------------------------------------------------------------
void Enemy::SetCurrentJumpPower( float jumpPower )
{
	m_fCurrentJumpPower = jumpPower;
}

//--------------------------------------------------------------------------------------
//  方向ベクトルの設定
//--------------------------------------------------------------------------------------
void Enemy::SetVecDirect( D3DXVECTOR3 vecDirect )
{
	m_vecDirect = vecDirect;
}

//--------------------------------------------------------------------------------------
//  注視点の設定
//--------------------------------------------------------------------------------------
void Enemy::SetPositionAt( D3DXVECTOR3 positionAt )
{
	m_posAt = positionAt;
}

//--------------------------------------------------------------------------------------
//  速度の設定
//--------------------------------------------------------------------------------------
void Enemy::SetSpeed( float speed )
{
	m_fSpeed = speed;
}

//--------------------------------------------------------------------------------------
//  移動力の設定
//--------------------------------------------------------------------------------------
void Enemy::SetMovePower( float movePower )
{
	m_fMovePower = movePower;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び力の設定
//--------------------------------------------------------------------------------------
void Enemy::SetBlowPower( float blowPower )
{
	m_fBlowPower = blowPower;
}

//--------------------------------------------------------------------------------------
//  無敵時間の設定
//--------------------------------------------------------------------------------------
void Enemy::SetInvisibleTime( int invisibleTime )
{
	m_nInvisibleTime = invisibleTime;
}

//--------------------------------------------------------------------------------------
//  追尾方向の設定
//--------------------------------------------------------------------------------------
void Enemy::SetHomingDirect( bool homingDirect )
{
	m_bHomingLeft = homingDirect;
}

//--------------------------------------------------------------------------------------
//  指定した種類のエフェクシア識別子の設定
//--------------------------------------------------------------------------------------
void Enemy::SetEffekseerHandle( EffekseerManager::TYPE type , ::Effekseer::Handle handle )
{
	m_handle[ static_cast< int >( type ) ] = handle;
}

//--------------------------------------------------------------------------------------
//  敵と当たり判定するかのフラグ設定
//--------------------------------------------------------------------------------------
void Enemy::SetJudgeHit( bool judgeHit )
{
	m_bJudgeHit = judgeHit;
}

//--------------------------------------------------------------------------------------
//  防御フラグの設定
//--------------------------------------------------------------------------------------
void Enemy::SetGuardFlag( bool guardFlag )
{
	m_bGuard = guardFlag;
}

//--------------------------------------------------------------------------------------
//  魔法クラスのポインタの設定
//--------------------------------------------------------------------------------------
void Enemy::SetMagic( Magic* magic )
{
	m_pMagic = magic;
}

//--------------------------------------------------------------------------------------
//  体力の増加
//--------------------------------------------------------------------------------------
void Enemy::AddLife( int addLife )
{
	m_nLife += addLife;
}

//--------------------------------------------------------------------------------------
//  魔法の削除
//--------------------------------------------------------------------------------------
void Enemy::DeleteMagic( void )
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
D3DXVECTOR3	Enemy::GetVecDirect( void )
{
	return m_vecDirect;
}

//--------------------------------------------------------------------------------------
//  注視点の取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	Enemy::GetPositionAt( void )
{
	return m_posAt;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び方向ベクトルの取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	Enemy::GetBlowVecDirect( void )
{
	return m_blowVecDirect;
}

//--------------------------------------------------------------------------------------
//  状態の変化
//--------------------------------------------------------------------------------------
void Enemy::ChangeState( EnemyState* enemyState )
{
	//  同じの場合リターン
	if( m_enemyState == enemyState )
	{
		return;
	}

	if( m_enemyState != nullptr )
	{
		//  前回のエネミーステートの終了処理
		m_enemyState->Uninit( );

		//  現在のエネミーステートの初期化処理
		m_enemyState = enemyState;
		m_enemyState->Init( );
	}
}

//--------------------------------------------------------------------------------------
//  無敵時間の取得
//--------------------------------------------------------------------------------------
int	Enemy::GetInvisibleTime( void )
{
	return m_nInvisibleTime;
}

//--------------------------------------------------------------------------------------
//  魔法クラスのポインタの取得
//--------------------------------------------------------------------------------------
Magic* Enemy::GetMagic( void )
{
	return m_pMagic;
}

//--------------------------------------------------------------------------------------
//  魔法の種類取得
//--------------------------------------------------------------------------------------
Enemy::MAGIC Enemy::GetMagicType( void )
{
	return m_magic;
}

//--------------------------------------------------------------------------------------
//  速度の取得
//--------------------------------------------------------------------------------------
float Enemy::GetSpeed( void )
{
	return m_fSpeed;
}

//--------------------------------------------------------------------------------------
//  基本速度の取得
//--------------------------------------------------------------------------------------
float Enemy::GetBaseSpeed( void )
{
	return m_fBaseSpeed;
}

//--------------------------------------------------------------------------------------
//  ジャンプ力の取得
//--------------------------------------------------------------------------------------
float Enemy::GetJumpPower( void )
{
	return m_fJumpPower;
}

//--------------------------------------------------------------------------------------
//  現在のジャンプ力の取得
//--------------------------------------------------------------------------------------
float Enemy::GetCurrentJumpPower( void )
{
	return m_fCurrentJumpPower;
}

//--------------------------------------------------------------------------------------
//  目標までの距離の取得
//--------------------------------------------------------------------------------------
float Enemy::GetTargetDistance( void )
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

			//  種類がプレイヤーの場合
			if( objType == Scene::OBJTYPE_PLAYER )
			{
				//  ダウンキャスト
				Player* pPlayer = ( Player* )pScene;

				//  プレイヤーの座標の代入
				D3DXVECTOR3 posPlayer = pPlayer->GetPos( );

				//  距離の算出
				targetDistance = sqrtf( ( posPlayer.x - m_position.x ) * ( posPlayer.x - m_position.x ) +
										( posPlayer.y - m_position.y ) * ( posPlayer.y - m_position.y ) + 
										( posPlayer.z - m_position.z ) * ( posPlayer.z - m_position.z ) );

				return targetDistance;
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
float Enemy::GetMovePower( void )
{
	return m_fMovePower;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び力の取得
//--------------------------------------------------------------------------------------
float Enemy::GetBlowPower( void )
{
	return m_fBlowPower;
}

//--------------------------------------------------------------------------------------
//  体力の取得
//--------------------------------------------------------------------------------------
int Enemy::GetLife( void )
{
	return m_nLife;
}

//--------------------------------------------------------------------------------------
//  エネミーが当たり判定をするかどうかの取得処理
//--------------------------------------------------------------------------------------
bool Enemy::GetJudgeHit( void )
{
	return m_bJudgeHit;
}

//--------------------------------------------------------------------------------------
//  エネミーが防御しているかどうかの取得処理
//--------------------------------------------------------------------------------------
bool Enemy::GetGuard( void )
{
	return m_bGuard;
}

//--------------------------------------------------------------------------------------
//  ロックオン情報の取得
//--------------------------------------------------------------------------------------
bool Enemy::GetLockon( void )
{
	return m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  追尾する向きの取得( false : 右 , true : 左 )
//--------------------------------------------------------------------------------------
bool Enemy::GetHomingDirect( void )
{
	return m_bHomingLeft;
}

//--------------------------------------------------------------------------------------
//  状態の取得
//--------------------------------------------------------------------------------------
EnemyState* Enemy::GetEnemyState( Enemy::STATE state )
{
	return m_allState[ static_cast< int >( state ) ];
}

//--------------------------------------------------------------------------------------
//  現在の状態の取得
//--------------------------------------------------------------------------------------
EnemyState* Enemy::GetCurrentEnemyState( void )
{
	return m_enemyState;
}

//--------------------------------------------------------------------------------------
//  エフェクシア識別子の取得
//--------------------------------------------------------------------------------------
::Effekseer::Handle Enemy::GetEffekseerHandle( EffekseerManager::TYPE type )
{
	return m_handle[ static_cast< int >( type ) ];
}

//--------------------------------------------------------------------------------------
//  アニメーションキーの取得処理
//--------------------------------------------------------------------------------------
int	Enemy::GetAnimationKey( void )
{
	return m_nKey;
}

//--------------------------------------------------------------------------------------
//  アニメーションキーの取得処理
//--------------------------------------------------------------------------------------
float Enemy::GetAnimationFrame( void )
{
	return m_fFrame;
}

//--------------------------------------------------------------------------------------
//  ジャンプでの移動
//--------------------------------------------------------------------------------------
void Enemy::MoveHeight( float addMove )
{
	m_position.y += addMove;
}

//--------------------------------------------------------------------------------------
//  ジャンプ力を下げる
//--------------------------------------------------------------------------------------
void Enemy::DownCurrentJumpPower( float downPower )
{
	m_fCurrentJumpPower -= downPower;
}