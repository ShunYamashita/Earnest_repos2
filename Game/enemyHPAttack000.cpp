//--------------------------------------------------------------------------------------
//  エネミーステート( ブレイブ攻撃000 )   ( enemyHPAttack000.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyHPAttack000.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "utility.h"
#include "player.h"
#include "bullet.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const int	ENEMY_HP_DAMAGE				= 400;
static const float	ENEMY_HP_BLOW_POWER			= 8.0f;
static const float	ENEMY_HP_RANGE					= 30.0f;
static const float	ENEMY_HP_RANGE_HEIGHT			= 12.0f;

static const int	ENEMY_HEAL_LIFE				= 20;

static const float	ENEMY_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Init( void )
{
	//  親の初期化
	EnemyState::Init( );

	m_slashEffekseer = false;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Update( void )
{
	//  アクション
	Action( );

	//  ニュートラル状態に変更するかの判断
	JudgeChangeNeutral( );

	//  重力をかける
	AddGravity( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Action( void )
{
	Utility::HIT_SPHERE	attackHitSphere;

	//  攻撃側の当たり判定の代入
	D3DXVECTOR3 attackPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.fLength = ENEMY_ATTACK_HIT_SPHERE_LENGTH;

	if( GetEnemy( )->GetAnimationKey( ) == 2 && m_slashEffekseer == false )
	{
		float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( attackHitSphere.position.x , GetEnemy( )->GetPos( ).y , attackHitSphere.position.z ) ,
								 D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( attackHitSphere.position.x , GetEnemy( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_FLAME , D3DXVECTOR3( attackHitSphere.position.x , GetEnemy( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) , 1.0f );

		m_slashEffekseer = true;
	}

	Scene* pScene = nullptr;

	if( GetEnemy( )->GetAnimationKey( ) >= 3 && GetEnemy( )->GetAnimationKey( ) <= 5 )
	{
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

				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  ダウンキャスト
					Player* pPlayer= ( Player* )pScene;

					//  プレイヤーの座標の代入
					D3DXVECTOR2 enemyPos;
					enemyPos.x  = pPlayer->GetPos( ).x;
					enemyPos.y  = pPlayer->GetPos( ).z;

					Utility::HIT_CIRCLE hitCircle;
					hitCircle.position.x = GetEnemy( )->GetPos( ).x;
					hitCircle.position.y = GetEnemy( )->GetPos( ).z;
					hitCircle.fLength = ENEMY_HP_RANGE;

					//  プレイヤーの3D座標の代入
					D3DXVECTOR3 playerPos3D;
					playerPos3D = pPlayer->GetPos( );

					D3DXVECTOR3 blowVecDirect = playerPos3D - GetEnemy( )->GetPos( );
					D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

					if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
					{
						if( GetEnemy( )->GetPos( ).y + ENEMY_HP_RANGE_HEIGHT >= pPlayer->GetPos( ).y )
						{
							//  相手が防御状態である場合
							if( pPlayer->GetGuard( ) )
							{
								//  のけぞり状態に
								GetEnemy( )->SetAnimation( StateAnimator::MOTION_BEND );
								GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::BEND ) );
							}
							else
							{
								pPlayer->Damage( blowVecDirect , ENEMY_HP_BLOW_POWER , ENEMY_HP_DAMAGE , true );
							}
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}
}