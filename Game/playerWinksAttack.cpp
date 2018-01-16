//--------------------------------------------------------------------------------------
//  プレイヤーステート( 攻撃 )   ( playerWinksAttack.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksAttack.h"
#include "playerWinks.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "fbxLoader.h"
#include "enemyRauder.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------------
static const int	PLAYER_WINKS_ATTACK01_RIGIDTIME = 20;		//  攻撃01硬直時間
static const int	PLAYER_WINKS_ATTACK02_RIGIDTIME = 30;		//  攻撃02硬直時間
static const int	PLAYER_WINKS_ATTACK03_RIGIDTIME = 40;		//  攻撃03硬直時間
static const int	PLAYER_WINKS_ATTACK_NUMBER = 3;				//  攻撃数

static const float	PLAYER_ENABLE_ATTACK_DIST = 80.0f;			//  追尾可能距離

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;		//  攻撃当たり判定

static const float	PLAYER_ATTACK_MOVE_RATE_XZ = 0.03f;			//  攻撃追尾時の移動割合( XZ座標 )
static const float	PLAYER_ATTACK_MOVE_RATE_Y = 0.001f;			//  攻撃追尾時の移動割合( Y座標 )

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::Init( void )
{
	//  親の初期化
	PlayerWinksState::Init( );

	//  硬直時間の初期化
	m_rigidTime = PLAYER_WINKS_ATTACK01_RIGIDTIME;

	//  攻撃番号の初期化
	m_motionState = MOTION_STATE::ATTACK01;

	//  フレームを発生したかどうかのフラグの初期化
	m_effekseerFrame = false;

	//  攻撃モーションに変更
	GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK01 );

	Utility::HIT_SPHERE	attackHitSphere;

	//  攻撃側の当たり判定の代入
	D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

	float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

	//  エフェクトの生成
	m_effekseerHandle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
													   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );

	//  エフェクトの生成
	m_effekseerHandle[ 1 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
													   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::Update( void )
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
void PlayerWinksAttack::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::Action( void )
{
	if( GetPlayer( )->GetMotionFrameRate( ) < 0.3f )
	{
		//  ターゲットに徐々に近づける
		MoveTarget( );
	}

	//  次のモーションに移行するかの判断
	JudgeNextMotion( );

	if( m_motionState == MOTION_STATE::ATTACK03 && GetPlayer( )->GetMotionFrame( ) == 20 )
	{
		Utility::HIT_SPHERE	attackHitSphere;

		//  攻撃側の当たり判定の代入
		D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 10.0f;
		attackHitSphere.position = attackPosition;
		attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  エフェクトの生成
		m_effekseerHandle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH003 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
															D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );
	}

	if( m_motionState == MOTION_STATE::ATTACK03 && GetPlayer( )->GetMotionFrameRate( ) > 0.8f && m_effekseerFrame == false )
	{
		Utility::HIT_SPHERE	attackHitSphere;

		//  攻撃側の当たり判定の代入
		D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 10.0f;
		attackHitSphere.position = attackPosition;
		attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_THUNDER , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) , 1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_SYLPH , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , 1.0f );

		//  エフェクト発生状態に
		m_effekseerFrame = true;
	}
}

//--------------------------------------------------------------------------------------
//  ターゲットに徐々に近づく関数
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::MoveTarget( void )
{
	//  追尾状態の場合
	if( GetPlayer( )->GetHomingDirect( ) )
	{
		//  座標の更新( 進行方向 × 速度 分の移動 )
		D3DXVECTOR3 playerPosition = GetPlayer( )->GetPos( );

		//  ターゲットの座標
		D3DXVECTOR3 targetPosition = GetTargetPosition( );

		//  座標を徐々に近づける
		playerPosition.x += ( targetPosition.x - playerPosition.x ) * PLAYER_ATTACK_MOVE_RATE_XZ;
		playerPosition.y += ( targetPosition.y - playerPosition.y ) * PLAYER_ATTACK_MOVE_RATE_Y;
		playerPosition.z += ( targetPosition.z - playerPosition.z ) * PLAYER_ATTACK_MOVE_RATE_XZ;

		//  プレイヤー座標と注視点の設定
		GetPlayer( )->SetPosition( playerPosition );
		GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
	}
}

//--------------------------------------------------------------------------------------
//  次のモーションに移行するかの判断をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::JudgeNextMotion( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	Utility::HIT_SPHERE	attackHitSphere;

	//  攻撃側の当たり判定の代入
	D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

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

			if( ( int )m_motionState < ( int )MOTION_STATE::MAX - 1 )
			{
				if( pKeyboard->GetKeyboardTrigger( nKey ) )
				{
					if( m_motionState == MOTION_STATE::ATTACK01 )
					{
						//  次の攻撃状態に変更
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK02 );
						m_motionState = MOTION_STATE::ATTACK02;

						m_rigidTime = PLAYER_WINKS_ATTACK02_RIGIDTIME;

						float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

						//  エフェクトの生成
						m_effekseerHandle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
																		   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );

						//  エフェクトの生成
						m_effekseerHandle[ 1 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
																		   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );
					}
					else if( m_motionState == MOTION_STATE::ATTACK02 )
					{
						//  次の攻撃状態に変更
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK03 );
						m_motionState = MOTION_STATE::ATTACK03;

						m_rigidTime = PLAYER_WINKS_ATTACK03_RIGIDTIME;
					}

					if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_ATTACK_DIST )
					{
						//  ターゲット方向に向かせる
						LookTargetXZ( );

						//  追尾状態に
						GetPlayer( )->SetHomingDirect( true );
					}
					else
					{
						//  追尾しない状態に
						GetPlayer( )->SetHomingDirect( false );
					}
				}
			}

#else

			if( ( int )m_motionState < ( int )MOTION_STATE::MAX - 1 )
			{
				if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
				{
					if( m_motionState == MOTION_STATE::ATTACK01 )
					{
						//  次の攻撃状態に変更
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK02 );
						m_motionState = MOTION_STATE::ATTACK02;

						m_rigidTime = PLAYER_WINKS_ATTACK02_RIGIDTIME;

						float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

						//  エフェクトの生成
						m_effekseerHandle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
																		   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );

						//  エフェクトの生成
						m_effekseerHandle[ 1 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
																		   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );
					}
					else if( m_motionState == MOTION_STATE::ATTACK02 )
					{
						//  次の攻撃状態に変更
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK03 );
						m_motionState = MOTION_STATE::ATTACK03;

						m_rigidTime = PLAYER_WINKS_ATTACK03_RIGIDTIME;
					}

					if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_ATTACK_DIST )
					{
						//  ターゲット方向に向かせる
						LookTargetXZ( );

						//  追尾状態に
						GetPlayer( )->SetHomingDirect( true );
					}
					else
					{
						//  追尾しない状態に
						GetPlayer( )->SetHomingDirect( false );
					}
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

	//  エフェクシアの座標決定
	EffekseerManager::SetPosition( m_effekseerHandle[ 0 ] , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) );
	EffekseerManager::SetPosition( m_effekseerHandle[ 1 ] , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) );
}