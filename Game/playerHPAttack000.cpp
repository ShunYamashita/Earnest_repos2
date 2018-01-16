//--------------------------------------------------------------------------------------
//  プレイヤーステート( ブレイブ攻撃000 )   ( playerHPAttack000.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerHPAttack000.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "utility.h"
#include "enemy.h"
#include "bullet.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const int	PLAYER_HP_DAMAGE				= 400;
static const float	PLAYER_HP_BLOW_POWER			= 8.0f;
static const float	PLAYER_HP_RANGE					= 30.0f;
static const float	PLAYER_HP_RANGE_HEIGHT			= 12.0f;

static const int	PLAYER_HEAL_LIFE				= 20;

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerHPAttack000::Init( void )
{
	//  親の初期化
	PlayerState::Init( );

	m_slashEffekseer = false;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerHPAttack000::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerHPAttack000::Update( void )
{
	//  カメラ制御
	ControlCamera( );

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
void PlayerHPAttack000::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerHPAttack000::Action( void )
{
	Utility::HIT_SPHERE	attackHitSphere;

	//  攻撃側の当たり判定の代入
	D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

	if( GetPlayer( )->GetAnimationKey( ) == 2 && m_slashEffekseer == false )
	{
		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								 D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_FLAME , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) , 1.0f );

		m_slashEffekseer = true;
	}

	Scene* pScene = nullptr;

	if( GetPlayer( )->GetAnimationKey( ) >= 3 && GetPlayer( )->GetAnimationKey( ) <= 5 )
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

				//  CPU対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  種類が敵の場合
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						//  ダウンキャスト
						Enemy* pEnemy = ( Enemy* )pScene;

						//  敵プレイヤーの座標の代入
						D3DXVECTOR2 enemyPos;
						enemyPos.x  = pEnemy->GetPos( ).x;
						enemyPos.y  = pEnemy->GetPos( ).z;

						Utility::HIT_CIRCLE hitCircle;
						hitCircle.position.x = GetPlayer( )->GetPos( ).x;
						hitCircle.position.y = GetPlayer( )->GetPos( ).z;
						hitCircle.fLength = PLAYER_HP_RANGE;

						//  敵プレイヤーの3D座標の代入
						D3DXVECTOR3 enemyPos3D;
						enemyPos3D = pEnemy->GetPos( );

						D3DXVECTOR3 blowVecDirect = enemyPos3D - GetPlayer( )->GetPos( );
						D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

						if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
						{
							if( GetPlayer( )->GetPos( ).y + PLAYER_HP_RANGE_HEIGHT >= pEnemy->GetPos( ).y )
							{
								//  相手が防御状態である場合
								if( pEnemy->GetGuard( ) )
								{
									//  のけぞり状態に
									GetPlayer( )->SetAnimation( StateAnimator::MOTION_BEND );
									GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::BEND ) );
								}
								else
								{
									pEnemy->Damage( blowVecDirect , PLAYER_HP_BLOW_POWER , PLAYER_HP_DAMAGE , true );
								}
							}
						}
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
						if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
						{
							//  敵プレイヤーの座標の代入
							D3DXVECTOR2 enemyPos;
							enemyPos.x  = pPlayer->GetPos( ).x;
							enemyPos.y  = pPlayer->GetPos( ).z;

							Utility::HIT_CIRCLE hitCircle;
							hitCircle.position.x = GetPlayer( )->GetPos( ).x;
							hitCircle.position.y = GetPlayer( )->GetPos( ).z;
							hitCircle.fLength = PLAYER_HP_RANGE;

							//  敵プレイヤーの3D座標の代入
							D3DXVECTOR3 enemyPos3D;
							enemyPos3D = pPlayer->GetPos( );

							D3DXVECTOR3 blowVecDirect = enemyPos3D - GetPlayer( )->GetPos( );
							D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

							if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
							{
								if( GetPlayer( )->GetPos( ).y + PLAYER_HP_RANGE_HEIGHT >= pPlayer->GetPos( ).y )
								{
									//  相手が防御状態である場合
									if( pPlayer->GetGuard( ) )
									{
										//  のけぞり状態に
										GetPlayer( )->SetAnimation( StateAnimator::MOTION_BEND );
										GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::BEND ) );
									}
									else
									{
										pPlayer->Damage( blowVecDirect , PLAYER_HP_BLOW_POWER , PLAYER_HP_DAMAGE , true );
									}
								}
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