//--------------------------------------------------------------------------------------
//  プレイヤーステート( ロックオンダッシュ )   ( playerWinksLockonDash.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksLockonDash.h"
#include "playerWinks.h"
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
static const float PLAYER_LOCKON_DASH_SPEED = 3.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Init( void )
{
	//  親の初期化
	PlayerWinksState::Init( );

	//  速度の初期化
	m_speed = PLAYER_LOCKON_DASH_SPEED;

	//  モーション状態の初期化
	m_motionState = MOTION_STATE::START;

	//  モーションの変更
	GetPlayer( )->SetMotion( FBXLoader::MOTION::DASH_START );

	//  姿勢を固定ではない状態に
	FBXLoader* fbx = GetPlayer( )->GetFBXLoader( );

	if( fbx != nullptr )
	{
		fbx->SetFixedPose( false );
	}
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Uninit( void )
{
	m_speed = 0.0f;

	//  姿勢を固定状態に
	FBXLoader* fbx = GetPlayer( )->GetFBXLoader( );

	if( fbx != nullptr )
	{
		fbx->SetFixedPose( true );
	}
}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Update( void )
{
	//  カメラ制御
	ControlCamera( );

	//  アクション
	Action( );

	//  ダッシュ開始状態の場合
	if( m_motionState == MOTION_STATE::START )
	{
		//  速度がある一定を下回った場合
		if( m_speed <= 0.9f )
		{
			//  モーション変更
			GetPlayer( )->SetMotion( FBXLoader::MOTION::DASH_END );
			m_motionState = MOTION_STATE::END;
		}
	}
	//  ダッシュ終了状態の場合
	else if( m_motionState == MOTION_STATE::END )
	{
		//  速度がある一定を下回ってかつ、モーションが終了した場合
		if( m_speed <= 0.3f && GetPlayer( )->GetMotionFinish( ) )
		{
			//  待機モーションに変更
			JudgeChangeIdle( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Action( void )
{
	//  ターゲット方向に進行方向ベクトルを変更
	LookTarget( );

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