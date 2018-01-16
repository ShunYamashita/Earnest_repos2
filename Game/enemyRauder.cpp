//--------------------------------------------------------------------------------------
//  敵   ( enemyWinks.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyRauder.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "scene2D.h"
#include "meshField.h"
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
#include "playerWinks.h"
#include "imgui_impl_dx9.h"
#include "AIManager.h"

//--------------------------------------------------------------------------------------
//  ヘッダーファイル( ステート系 )
//--------------------------------------------------------------------------------------
#include "EnemyRauderState.h"
#include "enemyRauderIdle.h"
#include "enemyRauderMove.h"
#include "enemyRauderJump.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define ENEMY_TO_CAMERA_DISTANCE_Y				( 11.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Y2				( 15.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Z				( 25.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Z2				( 35.0f )
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

#define ENEMY_LIFE_POS_X						( 389.0f )
#define ENEMY_LIFE_POS_Y						( 661.0f )
#define ENEMY_LIFE_SIZE_X						( 350.0f )
#define ENEMY_LIFE_SIZE_Y						( 49.0f )

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

static const float	ENEMY_JUMP_POWER			= 2.0f;

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  エネミークラスのコンストラクタ
//--------------------------------------------------------------------------------------
EnemyRauder::EnemyRauder( ) : Scene( 3 )
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
	m_pLife = nullptr;
	m_pEnemyRauderUI = nullptr;
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
	m_fJumpPower = ENEMY_JUMP_POWER;
}

//--------------------------------------------------------------------------------------
//  エネミークラスのデストラクタ
//--------------------------------------------------------------------------------------
EnemyRauder::~EnemyRauder( )
{

}

//--------------------------------------------------------------------------------------
//  エネミーの初期化処理
//--------------------------------------------------------------------------------------
HRESULT EnemyRauder::Init( void )
{
	m_drawDepth = true;

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_ENEMY );

	//  体力の生成
	m_pLife = Life::Create( Life::TYPE_LIFE000 ,
							D3DXVECTOR3( ENEMY_LIFE_POS_X , ENEMY_LIFE_POS_Y , 0.0f ) ,
							D3DXVECTOR3( ENEMY_LIFE_SIZE_X , ENEMY_LIFE_SIZE_Y , 0.0f ) ,
							D3DXVECTOR2( 0.0f , 0.0f ) ,
							D3DXVECTOR2( 1.0f , 1.0f ) ,
							D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) ,
							m_nLife );

	//  エネミー注視点の設定
	m_posAt = m_position - m_vecDirect * 5.0f;

	//  方向ベクトルの代入
	D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

	//  エネミーステートの生成
	m_allState[ static_cast< int >( EnemyRauder::STATE::IDLE ) ] = new EnemyRauderIdle( this );
	m_allState[ static_cast< int >( EnemyRauder::STATE::MOVE ) ] = new EnemyRauderMove( this );
	m_allState[ static_cast< int >( EnemyRauder::STATE::JUMP ) ] = new EnemyRauderJump( this );

	//  ニュートラル状態で初期化
	m_enemyState = m_allState[ static_cast< int >( EnemyRauder::STATE::IDLE ) ];

	//  FBXデータの作成
	m_fbx = FBXLoader::Create( FBXLoader::TYPE::RAUDER , m_position , m_posAt , 0.07f );
	m_fbx->SetMotion( FBXLoader::MOTION::IDLE );

	//  AI管理クラスにキャラクターAIとして追加
	m_characterAIIndex = AIManager::AddCharacterAI( "enemyRauder" , this );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  エネミーの終了処理
//--------------------------------------------------------------------------------------
void EnemyRauder::Uninit( void )
{
	if( m_pLife != nullptr )
	{
		m_pLife = nullptr;
	}

	if( m_pEnemyRauderUI != nullptr )
	{
		m_pEnemyRauderUI = nullptr;
	}

	if( m_fbx != nullptr )
	{
		m_fbx->Uninit( );
		m_fbx = nullptr;
	}

	//  エネミーステートの数分のループ
	for( int countState = 0; countState < static_cast< int >( EnemyRauder::STATE::MAX ) - 1; ++countState )
	{
		m_allState[ countState ]->Uninit( );
		delete m_allState[ countState ];
		m_allState[ countState ] = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  エネミーの更新処理
//--------------------------------------------------------------------------------------
void EnemyRauder::Update( void )
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
//  エネミーの描画処理
//--------------------------------------------------------------------------------------
void EnemyRauder::Draw( void )
{
	Camera* camera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVectorDirect;
	D3DXVECTOR3 cameraToEnemyRauder;

	if( camera != nullptr )
	{
		D3DXVECTOR3 cameraPosition = camera->GetCameraPosEye( );
		D3DXVECTOR3 enemyWinksPosition = m_position;
		cameraPosition.y = 0.0f;
		enemyWinksPosition.y = 0.0f;

		cameraToEnemyRauder = enemyWinksPosition - cameraPosition;
		D3DXVec3Normalize( &cameraToEnemyRauder , &cameraToEnemyRauder );
		cameraVectorDirect = camera->GetCameraVecDirect( );
		cameraVectorDirect.y = 0.0f;
		D3DXVec3Normalize( &cameraVectorDirect , &cameraVectorDirect );
	}

	if( Utility::IntoView( cameraVectorDirect , cameraToEnemyRauder , D3DX_PI * 0.5f ) )
	{
		if( m_fbx != nullptr )
		{
			m_fbx->SetDrawInfo( m_position , m_posAt );
			m_fbx->Draw( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  エネミーの描画処理
//--------------------------------------------------------------------------------------
void EnemyRauder::DrawDepth( void )
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
bool EnemyRauder::JudgeHitGround( void )
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
bool EnemyRauder::JudgeHitEnemy( void )
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

				//  種類がプレイヤーの場合
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  ダウンキャスト
					PlayerWinks* pPlayer = ( PlayerWinks* )pScene;

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
bool EnemyRauder::JudgeHitWall( void )
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
EnemyRauder* EnemyRauder::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed , MAGIC magic )
{
	EnemyRauder* pEnemyRauder;

	//  インスタンス生成
	pEnemyRauder = new EnemyRauder;

	//  座標の代入
	pEnemyRauder->m_position = position;

	//  最初の座標の代入
	pEnemyRauder->m_firstPos = position;

	//  注視点座標の代入
	pEnemyRauder->m_posAt = position;

	//  回転角の代入
	pEnemyRauder->m_rot = rot;

	//  大きさ倍率の代入
	pEnemyRauder->m_scale = scale;

	//  速度の代入
	pEnemyRauder->m_fBaseSpeed = fSpeed;

	//  所持魔法の代入
	pEnemyRauder->m_magic = magic;

	//  初期化
	pEnemyRauder->Init( );

	return pEnemyRauder;
}

//--------------------------------------------------------------------------------------
//  エネミーのダメージ処理
//--------------------------------------------------------------------------------------
void EnemyRauder::Damage( D3DXVECTOR3 blowVecDirect , float fBlowPower , int nDamage , bool bBlow )
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
			Result::SetResult( Result::GAME_CLEAR );
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
//  エネミーのブレイブダメージ処理
//--------------------------------------------------------------------------------------
int EnemyRauder::BraveDamage( int nDamage )
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
void EnemyRauder::StopAllEffekseer( void )
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
void EnemyRauder::ChangeLockon( void )
{
	m_bLockOn = !m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  現在のジャンプ力の設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetCurrentJumpPower( float jumpPower )
{
	m_fCurrentJumpPower = jumpPower;
}

//--------------------------------------------------------------------------------------
//  方向ベクトルの設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetVecDirect( D3DXVECTOR3 vecDirect )
{
	m_vecDirect = vecDirect;
}

//--------------------------------------------------------------------------------------
//  注視点の設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetPositionAt( D3DXVECTOR3 positionAt )
{
	m_posAt = positionAt;
}

//--------------------------------------------------------------------------------------
//  速度の設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetSpeed( float speed )
{
	m_fSpeed = speed;
}

//--------------------------------------------------------------------------------------
//  移動力の設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetMovePower( float movePower )
{
	m_fMovePower = movePower;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び力の設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetBlowPower( float blowPower )
{
	m_fBlowPower = blowPower;
}

//--------------------------------------------------------------------------------------
//  無敵時間の設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetInvisibleTime( int invisibleTime )
{
	m_nInvisibleTime = invisibleTime;
}

//--------------------------------------------------------------------------------------
//  追尾方向の設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetHomingDirect( bool homingDirect )
{
	m_bHomingLeft = homingDirect;
}

//--------------------------------------------------------------------------------------
//  指定した種類のエフェクシア識別子の設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetEffekseerHandle( EffekseerManager::TYPE type , ::Effekseer::Handle handle )
{
	m_handle[ static_cast< int >( type ) ] = handle;
}

//--------------------------------------------------------------------------------------
//  敵と当たり判定するかのフラグ設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetJudgeHit( bool judgeHit )
{
	m_bJudgeHit = judgeHit;
}

//--------------------------------------------------------------------------------------
//  防御フラグの設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetGuardFlag( bool guardFlag )
{
	m_bGuard = guardFlag;
}

//--------------------------------------------------------------------------------------
//  魔法クラスのポインタの設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetMagic( Magic* magic )
{
	m_pMagic = magic;
}

//--------------------------------------------------------------------------------------
//  モーションの設定
//--------------------------------------------------------------------------------------
void EnemyRauder::SetMotion( FBXLoader::MOTION motion )
{
	if( m_fbx != nullptr )
	{
		m_fbx->SetMotion( motion );
	}
}

//--------------------------------------------------------------------------------------
//  体力の増加
//--------------------------------------------------------------------------------------
void EnemyRauder::AddLife( int addLife )
{
	m_nLife += addLife;
}

//--------------------------------------------------------------------------------------
//  魔法の削除
//--------------------------------------------------------------------------------------
void EnemyRauder::DeleteMagic( void )
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
D3DXVECTOR3	EnemyRauder::GetVecDirect( void )
{
	return m_vecDirect;
}

//--------------------------------------------------------------------------------------
//  注視点の取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	EnemyRauder::GetPositionAt( void )
{
	return m_posAt;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び方向ベクトルの取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3	EnemyRauder::GetBlowVecDirect( void )
{
	return m_blowVecDirect;
}

//--------------------------------------------------------------------------------------
//  状態の変化
//--------------------------------------------------------------------------------------
void EnemyRauder::ChangeState( EnemyRauderState* enemyState )
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
//  状態の変化
//--------------------------------------------------------------------------------------
void EnemyRauder::ChangeState( EnemyRauder::STATE state )
{
	//  同じの場合リターン
	if( m_enemyState == m_allState[ ( int )state ] )
	{
		return;
	}

	if( m_enemyState != nullptr )
	{
		//  前回のエネミーステートの終了処理
		m_enemyState->Uninit( );

		//  現在のエネミーステートの初期化処理
		m_enemyState = m_allState[ ( int )state ];
		m_enemyState->Init( );
	}
}

//--------------------------------------------------------------------------------------
//  無敵時間の取得
//--------------------------------------------------------------------------------------
int	EnemyRauder::GetInvisibleTime( void )
{
	return m_nInvisibleTime;
}

//--------------------------------------------------------------------------------------
//  魔法クラスのポインタの取得
//--------------------------------------------------------------------------------------
Magic* EnemyRauder::GetMagic( void )
{
	return m_pMagic;
}

//--------------------------------------------------------------------------------------
//  魔法の種類取得
//--------------------------------------------------------------------------------------
EnemyRauder::MAGIC EnemyRauder::GetMagicType( void )
{
	return m_magic;
}

//--------------------------------------------------------------------------------------
//  速度の取得
//--------------------------------------------------------------------------------------
float EnemyRauder::GetSpeed( void )
{
	return m_fSpeed;
}

//--------------------------------------------------------------------------------------
//  基本速度の取得
//--------------------------------------------------------------------------------------
float EnemyRauder::GetBaseSpeed( void )
{
	return m_fBaseSpeed;
}

//--------------------------------------------------------------------------------------
//  ジャンプ力の取得
//--------------------------------------------------------------------------------------
float EnemyRauder::GetJumpPower( void )
{
	return m_fJumpPower;
}

//--------------------------------------------------------------------------------------
//  現在のジャンプ力の取得
//--------------------------------------------------------------------------------------
float EnemyRauder::GetCurrentJumpPower( void )
{
	return m_fCurrentJumpPower;
}

//--------------------------------------------------------------------------------------
//  目標までの距離の取得
//--------------------------------------------------------------------------------------
float EnemyRauder::GetTargetDistance( void )
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

			//  種類がプレイヤーの場合
			if( objType == Scene::OBJTYPE_PLAYER )
			{
				//  ダウンキャスト
				PlayerWinks* pPlayer = ( PlayerWinks* )pScene;

				//  敵の座標の代入
				D3DXVECTOR3 posEnemy = pPlayer->GetPos( );

				//  距離の算出
				targetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
										( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
										( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

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
float EnemyRauder::GetMovePower( void )
{
	return m_fMovePower;
}

//--------------------------------------------------------------------------------------
//  吹っ飛び力の取得
//--------------------------------------------------------------------------------------
float EnemyRauder::GetBlowPower( void )
{
	return m_fBlowPower;
}

//--------------------------------------------------------------------------------------
//  体力の取得
//--------------------------------------------------------------------------------------
int EnemyRauder::GetLife( void )
{
	return m_nLife;
}

//--------------------------------------------------------------------------------------
//  エネミーが当たり判定をするかどうかの取得処理
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetJudgeHit( void )
{
	return m_bJudgeHit;
}

//--------------------------------------------------------------------------------------
//  エネミーが防御しているかどうかの取得処理
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetGuard( void )
{
	return m_bGuard;
}

//--------------------------------------------------------------------------------------
//  ロックオン情報の取得
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetLockon( void )
{
	return m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  追尾する向きの取得( false : 右 , true : 左 )
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetHomingDirect( void )
{
	return m_bHomingLeft;
}

//--------------------------------------------------------------------------------------
//  モーションが終了したかの取得
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetMotionFinish( void )
{
	if( m_fbx != nullptr )
	{
		return m_fbx->GetMotionFinish( );
	}
}

//--------------------------------------------------------------------------------------
//  状態の取得
//--------------------------------------------------------------------------------------
EnemyRauderState* EnemyRauder::GetEnemyState( EnemyRauder::STATE state )
{
	return m_allState[ static_cast< int >( state ) ];
}

//--------------------------------------------------------------------------------------
//  現在の状態の取得
//--------------------------------------------------------------------------------------
EnemyRauderState* EnemyRauder::GetCurrentEnemyState( void )
{
	return m_allState[ static_cast< int >( m_state ) ];
}

//--------------------------------------------------------------------------------------
//  エフェクシア識別子の取得
//--------------------------------------------------------------------------------------
::Effekseer::Handle EnemyRauder::GetEffekseerHandle( EffekseerManager::TYPE type )
{
	return m_handle[ static_cast< int >( type ) ];
}

//--------------------------------------------------------------------------------------
//  ジャンプでの移動
//--------------------------------------------------------------------------------------
void EnemyRauder::MoveHeight( float addMove )
{
	m_position.y += addMove;
}

//--------------------------------------------------------------------------------------
//  ジャンプ力を下げる
//--------------------------------------------------------------------------------------
void EnemyRauder::DownCurrentJumpPower( float downPower )
{
	m_fCurrentJumpPower -= downPower;
}