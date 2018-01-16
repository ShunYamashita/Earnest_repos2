//--------------------------------------------------------------------------------------
//  プレイヤーステート( フリーダッシュ )   ( playerWinksFreeDash.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksFreeDash.h"
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
static const float PLAYER_FREE_DASH_SPEED = 2.5f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksFreeDash::Init( void )
{
	//  親の初期化
	PlayerWinksState::Init( );

	//  速度の初期化
	m_speed = PLAYER_FREE_DASH_SPEED;

	//  モーション状態の初期化
	m_motionState = MOTION_STATE::START;

	//  モーションの変更
	GetPlayer( )->SetMotion( FBXLoader::MOTION::DASH_START );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksFreeDash::Uninit( void )
{
	m_speed = 0.0f;
}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksFreeDash::Update( void )
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
void PlayerWinksFreeDash::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerWinksFreeDash::Action( void )
{
	D3DXVECTOR3 posEnemy( 0.0f , 0.0f , 0.0f );

	//  移動方向
	D3DXVECTOR3 move( 0.0f , 0.0f , 0.0f );

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	//  カメラ方向ベクトル
	D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

#ifdef KEYBOARD_ENABLE

	//  移動量の初期化
	move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		if( pKeyboard->GetKeyboardPress( DIK_W ) )
		{
			move.z += 1.0f;
		}
		else if( pKeyboard->GetKeyboardPress( DIK_S ) )
		{
			move.z -= 1.0f;
		}

		if( pKeyboard->GetKeyboardPress( DIK_A ) )
		{
			move.x -= 1.0f;
		}
		else if( pKeyboard->GetKeyboardPress( DIK_D ) )
		{
			move.x += 1.0f;
		}
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		if( pKeyboard->GetKeyboardPress( DIK_UPARROW ) )
		{
			move.z += 1.0f;
		}
		else if( pKeyboard->GetKeyboardPress( DIK_DOWNARROW ) )
		{
			move.z -= 1.0f;
		}

		if( pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) )
		{
			move.x -= 1.0f;
		}
		else if( pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
		{
			move.x += 1.0f;
		}
	}

#else

	//  左スティック情報の取得
	move.x = ( float )pPS4Input->GetLeftStickDisposition( GetPlayerWinks( )->GetPlayerWinksNo( ) ).x;
	move.z = -( float )pPS4Input->GetLeftStickDisposition( GetPlayerWinks( )->GetPlayerWinksNo( ) ).y;

#endif 

	D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

	if( move.x != 0 ||
		move.z != 0 )
	{
		if( pCamera != NULL )
		{
			float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

			D3DXMATRIX mtxRot;
			D3DXMatrixIdentity( &mtxRot );
			D3DXMatrixRotationY( &mtxRot , fAngle );

			D3DXVec3TransformNormal( &workVecDirect , &move , &mtxRot );
			workVecDirect.y = 0.0f;
			D3DXVec3Normalize( &workVecDirect , &workVecDirect );
		}
		else
		{
			workVecDirect.y = 0.0f;
			D3DXVec3Normalize( &workVecDirect , &move );
		}
	}

	//  現在の方向ベクトル
	D3DXVECTOR3 currentVecDirect = GetPlayer( )->GetVecDirect( );

	//  現在の方向ベクトルを入力方向ベクトルに徐々に近づける
	currentVecDirect += ( workVecDirect - currentVecDirect ) * 0.05f;

	//  敵からプレイヤーまでの方向ベクトルを求める
	D3DXVec3Normalize( &currentVecDirect , &currentVecDirect );
	GetPlayer( )->SetVecDirect( currentVecDirect );

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