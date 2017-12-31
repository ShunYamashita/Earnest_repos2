//--------------------------------------------------------------------------------------
//  プレイヤーステート( ロックオンダッシュ )   ( playerLockonDash.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerLockonDash.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "particle.h"
#include "enemy.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float PLAYER_LOCKON_DASH_SPEED = 2.5f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Init( void )
{
	//  親の初期化
	PlayerState::Init( );

	//  速度の初期化
	m_speed = PLAYER_LOCKON_DASH_SPEED;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Uninit( void )
{
	m_speed = 0.0f;
}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Update( void )
{
	//  カメラ制御
	ControlCamera( );

	//  アクション
	Action( );

	//  速度がある一定を下回った場合
	if( m_speed <= 0.9f )
	{
		//  ニュートラル状態に変更するかの判断
		JudgeChangeNeutral( );
	}

	//  突進攻撃状態に変更するかの判断
	JudgeChangeRushAttack( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Action( void )
{
	D3DXVECTOR3 posEnemy( 0.0f , 0.0f , 0.0f );

	//  シーンクラスのポインタ
	Scene* pScene = NULL;

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
							
					//  敵の座標代入
					posEnemy = pEnemy->GetPos( );
				}
			}
			//  プレイヤー対戦モードの場合
			if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
			{
				//  種類が敵の場合
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  ダウンキャスト
					Player* pPlayer = ( Player* )pScene;
							
					//  自分以外のプレイヤー番号の場合
					if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
					{
						//  敵の座標代入
						posEnemy = pPlayer->GetPos( );
					}
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  敵からプレイヤーまでの方向ベクトルを求める
	D3DXVECTOR3 workVecDirect = posEnemy - GetPlayer( )->GetPos( );
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetPlayer( )->SetVecDirect( workVecDirect );

	//  座標の更新( 進行方向 × 速度 分の移動 )
	GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * m_speed );

	//  注視点を向いている方向に
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  速度を徐々に減衰させていく
	m_speed += ( 0.0f - m_speed ) * 0.03f;

	//  パーティクルの生成
	Particle::Create( Particle::TYPE_NORMAL , GetPlayer( )->GetPos( ) , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
					  D3DXCOLOR( 0.2f , 0.5f , 0.8f , 1.0f ) , 4.0f , 0.1f , 0.01f , 0.0f , 0.0f , 20 );
}