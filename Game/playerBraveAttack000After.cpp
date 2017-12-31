//--------------------------------------------------------------------------------------
//  プレイヤーステート( Brave攻撃000( 後 ) )   ( playerBraveAttack000After.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerBraveAttack000After.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "enemy.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	PLAYER_ENABLE_BRAVE_ATTACK_DIST	= 150.0f;
static const float	PLAYER_BRAVE_ATTACK_SPEED		= 1.0f;
static const int	PLAYER_BRAVE_DAMAGE				= 150;
static const float	PLAYER_BRAVE_BLOW_POWER			= 3.0f;

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000After::Init( void )
{
	//  親の初期化
	PlayerState::Init( );

	m_speed = PLAYER_BRAVE_ATTACK_SPEED;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000After::Uninit( void )
{
	//  相手に当たる様に
	GetPlayer( )->SetJudgeHit( true );
}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000After::Update( void )
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
void PlayerBraveAttack000After::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000After::Action( void )
{
	Utility::HIT_SPHERE	attackHitSphere;		//  攻撃判定用の球体

	//  相手に当たらない様に
	GetPlayer( )->SetJudgeHit( false );

	//  座標の更新( 進行方向 × 速度 分の移動 )
	GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * m_speed );

	//  注視点を向いている方向に
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  攻撃側の当たり判定の代入
	D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.position.y += 2.0f;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

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

			//  CPU対戦モードの場合
			if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
			{
				//  種類が敵の場合
				if( objType == Scene::OBJTYPE_ENEMY )
				{
					//  ダウンキャスト
					Enemy* pEnemy = ( Enemy* )pScene;

					if( Utility::HitSphere( attackHitSphere , pEnemy->GetHitSphere( ) ) )
					{
						pEnemy->Damage( PLAYER_BRAVE_DAMAGE );
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
						//  敵プレイヤーの3D座標の代入
						D3DXVECTOR3 enemyPos3D;
						enemyPos3D = pPlayer->GetPos( );

						D3DXVECTOR3 blowVecDirect = enemyPos3D - GetPlayer( )->GetPos( );
						D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

						if( Utility::HitSphere( attackHitSphere , pPlayer->GetHitSphere( ) ) )
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
								pPlayer->Damage( blowVecDirect , PLAYER_BRAVE_BLOW_POWER , PLAYER_BRAVE_DAMAGE , true );
							}
						}
					}
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

	EffekseerManager::SetPosition( GetPlayer( ) ->GetEffekseerHandle( EffekseerManager::TYPE_SLASH000 ) ,
								   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , attackHitSphere.position.y , GetPlayer( )->GetPos( ).z ) );
	EffekseerManager::SetRot( GetPlayer( ) ->GetEffekseerHandle( EffekseerManager::TYPE_SLASH000 ) ,
							  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) );
}