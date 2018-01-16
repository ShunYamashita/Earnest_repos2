//--------------------------------------------------------------------------------------
//  プレイヤーステート( 防御 )   ( playerWinksGuard.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksGuard.h"
#include "playerWinks.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------------
static const int GUARD_TIME = 30;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Init( void )
{
	//  親の初期化
	PlayerWinksState::Init( );

	//  地面にいるかどうかでモーション変更
	if( GetPlayer( )->JudgeHitGround( ) )
	{
		//  モーションの変更
		GetPlayer( )->SetMotion( FBXLoader::MOTION::GROUND_GUARD_START );
		m_ground = true;
	}
	else
	{
		//  モーションの変更
		GetPlayer( )->SetMotion( FBXLoader::MOTION::AIR_GUARD_START );
		m_ground = false;
	}

	float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

	//  攻撃側の当たり判定の代入
	D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f;
	effekseerPosition.y += 6.0f;

	//  エフェクトの生成
	m_effekseerHandle = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
												  D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) ,
												  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.5f );

	//  ガード時間の初期化
	m_guardTime = GUARD_TIME;

	//  モーション状態の初期化
	m_motionState = MOTION_STATE::START;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Uninit( void )
{
	//  防御解除状態に
	GetPlayer( )->SetGuardFlag( false );
}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Update( void )
{
	//  カメラ制御
	ControlCamera( );

	//  アクション
	Action( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Action( void )
{
	switch( m_motionState )
	{
		case MOTION_STATE::START:
		{
			//  防御状態に
			GetPlayer( )->SetGuardFlag( true );

			//  モーションが終了している場合
			if( GetPlayer( )->GetMotionFinish( ) )
			{
				m_motionState = MOTION_STATE::GUARD;
			}

			break;
		}
		case MOTION_STATE::GUARD:
		{
			//  防御状態に
			GetPlayer( )->SetGuardFlag( true );

			//  ガード時間中の場合
			if( m_guardTime > 0 )
			{
				m_guardTime--;
			}
			else
			{
				if( m_ground )
				{
					//  モーションの変更
					GetPlayer( )->SetMotion( FBXLoader::MOTION::GROUND_GUARD_END );
				}
				else
				{
					//  モーションの変更
					GetPlayer( )->SetMotion( FBXLoader::MOTION::AIR_GUARD_END );
				}

				m_motionState = MOTION_STATE::END;
			}

			break;
		}
		case MOTION_STATE::END:
		{
			//  防御解除状態に
			GetPlayer( )->SetGuardFlag( false );

			//  エフェクシアの再生ストップ
			EffekseerManager::Stop( m_effekseerHandle );

			//  モーションが終了した時は待機状態に変更
			JudgeChangeIdle( );

			break;
		}
	}

	//  エフェクシア座標の設定
	D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f;
	effekseerPosition.y += 6.0f;
	EffekseerManager::SetPosition( m_effekseerHandle , D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) );
}