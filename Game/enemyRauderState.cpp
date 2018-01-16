//--------------------------------------------------------------------------------------
//  エネミーステート  ( playerState.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/28
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyRauder.h"
#include "enemyRauderState.h"
#include "fbxLoader.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"
#include "playerWinks.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	ENEMY_TO_CAMERA_DISTANCE_Y		= 10.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Y2	= 13.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z		= 25.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z2	= 30.0f;

static const float	ENEMY_GRAVITY2					= 0.2f;
static const float	CAMERA_ROTATE_ANGLE				= 0.025f;
static const float	ESCAPE_POWER					= 1.0f;

static const float	ENEMY_ENABLE_BRAVE_ATTACK_DIST	= 80.0f;

static const int	ENEMY_GUARD_FRAME				= 2;

static const float	ENEMY_HIT_SPHERE_POS_Y			= 3.0f;

//--------------------------------------------------------------------------------------
//  重力をかける関数
//--------------------------------------------------------------------------------------
void EnemyRauderState::Init( void )
{
	m_moveHeight = 0.0f;
	m_pressTime = 0;
}

//--------------------------------------------------------------------------------------
//  エネミーデータの設定をする関数
//--------------------------------------------------------------------------------------
void EnemyRauderState::SetEnemy( EnemyRauder* enemyRauder )
{
	if( enemyRauder == nullptr )
	{
		return;
	}

	m_enemy = enemyRauder;
}

//--------------------------------------------------------------------------------------
//  エネミーデータの取得をする関数
//--------------------------------------------------------------------------------------
EnemyRauder* EnemyRauderState::GetEnemy( void )
{
	if( m_enemy == nullptr )
	{
		return nullptr;
	}

	return m_enemy;
}

//--------------------------------------------------------------------------------------
//  重力をかける関数
//--------------------------------------------------------------------------------------
void EnemyRauderState::AddGravity( void )
{
	//  地面との当たっていない場合
	if( GetEnemy( )->JudgeHitGround( ) == false )
	{
		//  下方向に力をかける
		m_moveHeight -= ENEMY_GRAVITY2;

		//  Y軸移動
		GetEnemy( )->MoveHeight( m_moveHeight );
	}
	else
	{
		//  下方向の力初期化
		m_moveHeight = 0.0f;
	}
}

//--------------------------------------------------------------------------------------
//  ロックオン状態の変更をする関数
//--------------------------------------------------------------------------------------
void EnemyRauderState::ChangeLockon( void )
{
	//  ロックオンの切り替え
	GetEnemy( )->ChangeLockon( );
}

//--------------------------------------------------------------------------------------
//  ターゲット方向に向く関数
//--------------------------------------------------------------------------------------
void EnemyRauderState::LookTargetXZ( void )
{
	//  ターゲットの方向に進行方向ベクトルを変更
	D3DXVECTOR3 workVecDirect = GetTargetPosition( ) - GetEnemy( )->GetPos( );
	workVecDirect.y = 0.0f;
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetEnemy( )->SetVecDirect( workVecDirect );
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );
}

//--------------------------------------------------------------------------------------
//  ターゲット座標の取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3	EnemyRauderState::GetTargetPosition( void )
{
	//  シーンクラスのポインタ
	Scene* pScene = NULL;

	//  ターゲットの座標
	D3DXVECTOR3 targetPosition;

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

			//  種類が敵の場合
			if( objType == Scene::OBJTYPE_PLAYER )
			{
				//  ダウンキャスト
				PlayerWinks* player = ( PlayerWinks* )pScene;

				targetPosition = player->GetPos( );
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	return targetPosition;
}

//--------------------------------------------------------------------------------------
//  待機状態に変更する関数
//--------------------------------------------------------------------------------------
void EnemyRauderState::ChangeIdle( void )
{
	//  モーションが終了している場合
	if( GetEnemy( )->GetMotionFinish( ) )
	{
		//  待機モーションに変更
		GetEnemy( )->SetMotion( FBXLoader::MOTION::IDLE );

		//  ニュートラル状態に
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( EnemyRauder::STATE::IDLE ) );
	}
}

//--------------------------------------------------------------------------------------
//  移動状態に変更する関数
//--------------------------------------------------------------------------------------
void EnemyRauderState::ChangeMove( MOVE_TYPE moveType )
{
	m_moveType = moveType;

	//  移動モーションに変更
	GetEnemy( )->SetMotion( FBXLoader::MOTION::RUN );

	//  ステートの変更
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( EnemyRauder::STATE::MOVE ) );
}

//--------------------------------------------------------------------------------------
//  ジャンプ状態に変更する関数
//--------------------------------------------------------------------------------------
void EnemyRauderState::ChangeJump( void )
{
	//  ジャンプアニメーションに
	GetEnemy( )->SetMotion( FBXLoader::MOTION::JUMP );

	//  ジャンプ状態に
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( EnemyRauder::STATE::JUMP ) );
}