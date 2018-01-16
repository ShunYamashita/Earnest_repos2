//--------------------------------------------------------------------------------------
//  プレイヤーステート( 攻撃 )   ( playerRauderAttack.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerRauderAttack.h"
#include "playerRauder.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "fbxLoader.h"
#include "enemyRauder.h"

//--------------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------------
static const int	PLAYER_WINKS_ATTACK01_RIGIDTIME = 10;		//  攻撃01硬直時間
static const int	PLAYER_WINKS_ATTACK02_RIGIDTIME = 20;		//  攻撃02硬直時間
static const int	PLAYER_WINKS_ATTACK03_RIGIDTIME = 40;		//  攻撃03硬直時間
static const int	PLAYER_WINKS_ATTACK_NUMBER = 3;				//  攻撃数

static const float	PLAYER_ENABLE_ATTACK_DIST = 80.0f;			//  追尾可能距離

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Init( void )
{
	//  親の初期化
	PlayerRauderState::Init( );

	//  硬直時間の初期化
	m_rigidTime = PLAYER_WINKS_ATTACK01_RIGIDTIME;

	//  攻撃番号の初期化
	m_attackNo = 0;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Update( void )
{
	//  カメラ制御
	ControlCamera( );

	//  アクション
	Action( );

	//  重力をかける
	AddGravity( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Action( void )
{
	//  ターゲットに徐々に近づける
	MoveTarget( );

	//  次のモーションに移行するかの判断
	JudgeNextMotion( );
}

//--------------------------------------------------------------------------------------
//  ターゲットに徐々に近づく関数
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::MoveTarget( void )
{
	//  追尾状態の場合
	if( GetPlayer( )->GetHomingDirect( ) )
	{
		//  座標の更新( 進行方向 × 速度 分の移動 )
		D3DXVECTOR3 playerPosition = GetPlayer( )->GetPos( );

		//  ターゲットの座標
		D3DXVECTOR3 targetPosition;

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
						EnemyRauder* enemy = ( EnemyRauder* )pScene;

						targetPosition = enemy->GetPos( );
					}
				}
				//  プレイヤー対戦モードの場合
				else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						PlayerRauder* player = ( PlayerRauder* )pScene;

						//  自分以外のプレイヤー番号の場合
						if( GetPlayer( )->GetPlayerNo( ) != player->GetPlayerNo( ) )
						{
							D3DXVECTOR3 targetPosition = player->GetPos( );
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}

		//  座標を徐々に近づける
		playerPosition.x += ( targetPosition.x - playerPosition.x ) * 0.01f;
		playerPosition.y += ( targetPosition.y - playerPosition.y ) * 0.003f;
		playerPosition.z += ( targetPosition.z - playerPosition.z ) * 0.01f;

		//  プレイヤー座標と注視点の設定
		GetPlayer( )->SetPosition( playerPosition );
		GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
	}
}

//--------------------------------------------------------------------------------------
//  次のモーションに移行するかの判断をする関数
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::JudgeNextMotion( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	//  モーションが終わった場合
	if( GetPlayer( )->GetMotionFinish( ) )
	{
		//  硬直時間がある場合
		if( m_rigidTime > 0 )
		{

#ifdef KEYBOARD_ENABLE

			int nKey = 0;

			if( GetPlayer( )->GetPlayerNo( ) == 0 )
			{
				nKey = DIK_C;
			}
			else if( GetPlayer( )->GetPlayerNo( ) == 1 )
			{
				nKey = DIK_K;
			}

			if( m_attackNo < PLAYER_WINKS_ATTACK_NUMBER - 1 )
			{
				if( pKeyboard->GetKeyboardTrigger( nKey ) )
				{
					if( m_attackNo == 0 )
					{
						//  次の攻撃状態に変更
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK02 );

						m_rigidTime = PLAYER_WINKS_ATTACK02_RIGIDTIME;
					}
					else if( m_attackNo == 1 )
					{
						//  次の攻撃状態に変更
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK03 );

						m_rigidTime = PLAYER_WINKS_ATTACK03_RIGIDTIME;
					}

					if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_ATTACK_DIST )
					{
						//  追尾状態に
						GetPlayer( )->SetHomingDirect( true );
					}
					else
					{
						//  追尾しない状態に
						GetPlayer( )->SetHomingDirect( false );
					}

					//  攻撃番号を進める
					m_attackNo++;
				}
			}

#else

			if( m_attackNo < PLAYER_WINKS_ATTACK_NUMBER - 1 )
			{
				if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
				{
					if( m_attackNo == 0 )
					{
						//  次の攻撃状態に変更
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK02 );

						m_rigidTime = PLAYER_WINKS_ATTACK02_RIGIDTIME;
					}
					else if( m_attackNo == 1 )
					{
						//  次の攻撃状態に変更
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK03 );

						m_rigidTime = PLAYER_WINKS_ATTACK03_RIGIDTIME;
					}

					if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_ATTACK_DIST )
					{
						//  追尾状態に
						GetPlayer( )->SetHomingDirect( true );
					}
					else
					{
						//  追尾しない状態に
						GetPlayer( )->SetHomingDirect( false );
					}

					//  攻撃番号を進める
					m_attackNo++;
				}
			}

#endif

			//  硬直時間のカウント
			m_rigidTime--;
		}
		else
		{
			//  待機状態に
			JudgeChangeIdle( );
		}
	}
}