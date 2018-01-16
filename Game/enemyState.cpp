//--------------------------------------------------------------------------------------
//  エネミーステート  ( enemyState.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/28
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyJump.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	ENEMY_TO_CAMERA_DISTANCE_Y		= 9.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Y2		= 10.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z		= 20.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z2		= 23.0f;

static const float	ENEMY_GRAVITY2					= 0.2f;
static const float	CAMERA_ROTATE_ANGLE				= 0.025f;
static const float	ESCAPE_POWER					= 1.0f;

static const float	ENEMY_ENABLE_BRAVE_ATTACK_DIST	= 80.0f;

static const int	ENEMY_GUARD_FRAME				= 2;

static const float	ENEMY_HIT_SPHERE_POS_Y			= 3.0f;

//--------------------------------------------------------------------------------------
//  重力をかける関数
//--------------------------------------------------------------------------------------
void EnemyState::Init( void )
{
	m_moveHeight = 0.0f;
	m_pressTime = 0;
}

//--------------------------------------------------------------------------------------
//  エネミーデータの設定をする関数
//--------------------------------------------------------------------------------------
void EnemyState::SetEnemy( Enemy* enemy )
{
	if( enemy == nullptr )
	{
		return;
	}

	m_enemy = enemy;
}

//--------------------------------------------------------------------------------------
//  エネミーデータの取得をする関数
//--------------------------------------------------------------------------------------
Enemy* EnemyState::GetEnemy( void )
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
void EnemyState::AddGravity( void )
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
void EnemyState::ChangeLockon( void )
{
	//  ロックオンの切り替え
	GetEnemy( )->ChangeLockon( );
}

//--------------------------------------------------------------------------------------
//  ニュートラル状態に変更するかの判断をする関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeNeutral( void )
{
	//  アニメーションが終了している場合
	if( GetEnemy( )->GetAnimationFinish( ) )
	{
		//  ニュートラルモーションの変更
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_NEUTRAL );

		//  ニュートラル状態に
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::NEUTRAL ) );
	}
}

//--------------------------------------------------------------------------------------
//  移動状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeMove( const D3DXVECTOR3& vecDirect )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	//  進行方向ベクトルの代入
	GetEnemy( )->SetVecDirect( vecDirect );

	//  モーションの変更
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_MOVE );

	//  ステートの変更
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MOVE ) );
}

//--------------------------------------------------------------------------------------
//  ジャンプ状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeJump( void )
{
	//  ジャンプアニメーションに
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_JUMP );

	//  ジャンプ状態に
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::JUMP ) );
}

//--------------------------------------------------------------------------------------
//  HP攻撃000状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeHPAttack000( void )
{
	//  モーションの変更
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP001 );

	//  ブレイブ攻撃に変更
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ATTACK_HP_ATTACK000 ) );
}

//--------------------------------------------------------------------------------------
//  ブレイブ攻撃000状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeBraveAttack000( bool left )
{
	if( GetEnemy( )->GetTargetDistance( ) <= ENEMY_ENABLE_BRAVE_ATTACK_DIST )
	{ 
		//  ブレイブ攻撃000状態に変更
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_BEFORE );
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ATTACK_BRAVE_ATTACK000_BEFORE ) );

		//  どっち周りで追尾するかの設定
		GetEnemy( )->SetHomingDirect( left );
	}
	else
	{
		//  ブレイブ攻撃000状態に変更
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

		float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

		//  攻撃側の当たり判定の代入
		D3DXVECTOR3 effekseerPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;

		//  エフェクトの生成
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
																D3DXVECTOR3( GetEnemy( )->GetPos( ).x , effekseerPosition.y , GetEnemy( )->GetPos( ).z ) ,
																D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
		GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );
	}
}

//--------------------------------------------------------------------------------------
//  回避状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeEscape( const D3DXVECTOR3& vecDirect )
{
	//  エフェクトの停止
	EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

	//  エフェクトの停止
	EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

	//  魔法の削除
	GetEnemy( )->DeleteMagic( );

	//  速度の代入
	GetEnemy( )->SetSpeed( GetEnemy( )->GetBaseSpeed( ) );

	//  方向ベクトルの設定
	GetEnemy( )->SetVecDirect( vecDirect );

	//  移動力の計算
	GetEnemy( )->SetMovePower( ESCAPE_POWER );

	//  モーションの変更
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_ESCAPE );

	//  状態の変更
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ESCAPE ) );
}

//--------------------------------------------------------------------------------------
//  防御状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeGuard( void )
{
	//  モーションの変更
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_GUARD );

	//  状態の変更
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::GUARD ) );

	float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

	//  攻撃側の当たり判定の代入
	D3DXVECTOR3 effekseerPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
	effekseerPosition.y += 3.0f;

	//  エフェクトの生成
	::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
															D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) ,
															D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
	GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_SHIELD , handle );
}


//--------------------------------------------------------------------------------------
//  ロックオンダッシュに変更するかの判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeLockonDash( const D3DXVECTOR3& vecDirect )
{
	if( vecDirect.x != 0.0f || vecDirect.z != 0.0f )
	{
		//  方向ベクトルの設定
		GetEnemy( )->SetVecDirect( vecDirect );

		//  モーションの変更
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

		//  状態の変更
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::FREE_DASH ) );
	}
	else
	{
		//  モーションの変更
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

		//  状態の変更
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::LOCKON_DASH ) );
	}
}

//--------------------------------------------------------------------------------------
//  突進攻撃に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeRushAttack( void )
{
	//  モーションの変更
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_RUSH_ATTACK );

	//  状態の変更
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::RUSH_ATTACK ) );

	//  角度の算出
	float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

	//  エフェクトの生成
	::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_LANCE ,
															D3DXVECTOR3( GetEnemy( )->GetPos( ).x , GetEnemy( )->GetPos( ).y + ENEMY_HIT_SPHERE_POS_Y , GetEnemy( )->GetPos( ).z ) ,
															D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.5f , 1.5f , 1.5f ) );
	GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_LANCE , handle );
}

//--------------------------------------------------------------------------------------
//  魔法準備状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeMagicReady( void )
{
	//  モーションの変更
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_MAGIC_READY );

	//  状態の変更
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MAGIC_READY ) );

	float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

	//  エフェクトの生成
	::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_AURA ,
															D3DXVECTOR3( GetEnemy( )->GetPos( ).x , GetEnemy( )->GetPos( ).y + ENEMY_HIT_SPHERE_POS_Y , GetEnemy( )->GetPos( ).z ) ,
															D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.0f );
	GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_AURA , handle );

	//  エフェクトの生成
	handle = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE ,
										D3DXVECTOR3( GetEnemy( )->GetPos( ).x , GetEnemy( )->GetPos( ).y , GetEnemy( )->GetPos( ).z ) ,
										D3DXVECTOR3( -D3DX_PI * 0.5f , fAngle , 0.0f ) , D3DXVECTOR3( 5.0f , 5.0f , 5.0f ) , 1.0f );
	GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE , handle );

	D3DXVECTOR3 magicPos = GetEnemy( )->GetPos( );
	magicPos.y += 3.0f;

	Magic* magic = nullptr;

	if( GetEnemy( )->GetMagicType( ) == Enemy::MAGIC::FIRE )
	{
		magic = MagicFire::Create( Magic::OWNER_ENEMY , magicPos , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
									D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetEnemy( )->GetVecDirect( ) );
	}
	else if( GetEnemy( )->GetMagicType( ) == Enemy::MAGIC::LIGHTNING )
	{
		magic = MagicLightning::Create( Magic::OWNER_ENEMY , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
										D3DXVECTOR3( 0.6f , 0.6f , 0.6f ) , GetEnemy( )->GetVecDirect( ) );
	}
	else if( GetEnemy( )->GetMagicType( ) == Enemy::MAGIC::TORNADE )
	{
		magic = MagicTornade::Create( Magic::OWNER_ENEMY , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
										D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetEnemy( )->GetVecDirect( ) );
	}

	//  魔法の設定
	GetEnemy( )->SetMagic( magic );
}

//--------------------------------------------------------------------------------------
//  魔法アクティブ状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeMagicActive( void )
{
	Magic* magic = GetEnemy( )->GetMagic( );

	if( magic != NULL )
	{
		Enemy::MAGIC magicType = GetEnemy( )->GetMagicType( );

		if( magicType == Enemy::MAGIC::FIRE )
		{
			//  魔法発動モーションの変更
			GetEnemy( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE );

			//  魔法発動状態に
			GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MAGIC_ACTIVE ) );

			//  エフェクトの停止
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

			//  エフェクトの停止
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

			//  実行状態に
			magic->SetActive( );
		}
		else if( magicType == Enemy::MAGIC::LIGHTNING )
		{
			//  魔法発動モーションの変更
			GetEnemy( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE2 );

			//  魔法発動状態に
			GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MAGIC_ACTIVE ) );

			//  エフェクトの停止
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

			//  エフェクトの停止
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

			//  実行状態に
			magic->SetActive( );
		}
		else if( magicType == Enemy::MAGIC::TORNADE )
		{
			//  魔法発動モーションの変更
			GetEnemy( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE3 );

			//  魔法発動状態に
			GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MAGIC_ACTIVE ) );

			//  エフェクトの停止
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

			//  エフェクトの停止
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

			//  実行状態に
			magic->SetActive( );
		}
	}
}