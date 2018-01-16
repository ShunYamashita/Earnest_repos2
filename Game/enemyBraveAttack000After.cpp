//--------------------------------------------------------------------------------------
//  エネミーステート( Brave攻撃000( 後 ) )   ( enemyBraveAttack000After.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyBraveAttack000After.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "player.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	ENEMY_ENABLE_BRAVE_ATTACK_DIST	= 150.0f;
static const float	ENEMY_BRAVE_ATTACK_SPEED		= 1.0f;
static const int	ENEMY_BRAVE_DAMAGE				= 150;
static const float	ENEMY_BRAVE_BLOW_POWER			= 3.0f;

static const float	ENEMY_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000After::Init( void )
{
	//  親の初期化
	EnemyState::Init( );

	m_speed = ENEMY_BRAVE_ATTACK_SPEED;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000After::Uninit( void )
{
	//  相手に当たる様に
	GetEnemy( )->SetJudgeHit( true );
}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000After::Update( void )
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
void EnemyBraveAttack000After::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000After::Action( void )
{
	Utility::HIT_SPHERE	attackHitSphere;		//  攻撃判定用の球体

	//  相手に当たらない様に
	GetEnemy( )->SetJudgeHit( false );

	//  座標の更新( 進行方向 × 速度 分の移動 )
	GetEnemy( )->MovePos( GetEnemy( )->GetVecDirect( ) * m_speed );

	//  注視点を向いている方向に
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );

	//  攻撃側の当たり判定の代入
	D3DXVECTOR3 attackPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.position.y += 2.0f;
	attackHitSphere.fLength = ENEMY_ATTACK_HIT_SPHERE_LENGTH;

	Scene* pScene = nullptr;

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
				Player* pPlayer = ( Player* )pScene;

				//  敵エネミーの3D座標の代入
				D3DXVECTOR3 playerPos3D;
				playerPos3D = pPlayer->GetPos( );

				D3DXVECTOR3 blowVecDirect = playerPos3D - GetEnemy( )->GetPos( );
				D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

				if( Utility::HitSphere( attackHitSphere , pPlayer->GetHitSphere( ) ) )
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
						pPlayer->Damage( blowVecDirect , ENEMY_BRAVE_BLOW_POWER , ENEMY_BRAVE_DAMAGE , true );
					}
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

	EffekseerManager::SetPosition( GetEnemy( ) ->GetEffekseerHandle( EffekseerManager::TYPE_SLASH000 ) ,
								   D3DXVECTOR3( GetEnemy( )->GetPos( ).x , attackHitSphere.position.y , GetEnemy( )->GetPos( ).z ) );
	EffekseerManager::SetRot( GetEnemy( ) ->GetEffekseerHandle( EffekseerManager::TYPE_SLASH000 ) ,
							  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) );
}