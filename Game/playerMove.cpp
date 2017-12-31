//--------------------------------------------------------------------------------------
//  プレイヤーステート( 移動 )   ( playerMove.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerMove.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerMove::Init( void )
{
	//  親の初期化
	PlayerState::Init( );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerMove::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerMove::Update( void )
{
	//  カメラ制御
	ControlCamera( );

	//  アクション
	Action( );

	//  重力をかける
	AddGravity( );

	//  ニュートラル状態に変更するかの判断
	JudgeChangeNeutral( );

	//  ジャンプ状態に変更するか判断
	JudgeChangeJump( );

	//  回避状態に変更するかの判断
	JudgeChangeEscape( );

	//  HP攻撃状態に変更するかの判断
	JudgeChangeHPAttack000( );

	//  ブレイブ攻撃000状態に変更するかの判定
	JudgeChangeBraveAttack000( );

	//  防御状態に変更するかの判断
	JudgeChangeGuard( );

	//  ロックオンダッシュ状態に変更するかの判断
	JudgeChangeLockonDash( );

	//  魔法準備状態に変更するかの判断
	JudgeChangeMagicReady( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerMove::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerMove::Action( void )
{
	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	//  シーンクラスのポインタ
	Scene* pScene = NULL;			

	//  カメラ方向ベクトル
	D3DXVECTOR3 cameraVecDirect;

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	//  移動方向
	D3DXVECTOR3 move( 0.0f , 0.0f , 0.0f );

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
	move.x = ( float )pPS4Input->GetLeftStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x;
	move.z = -( float )pPS4Input->GetLeftStickDisposition( GetPlayer( )->GetPlayerNo( ) ).y;

#endif 

	D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

	if( move.x != 0 ||
		move.z != 0 )
	{
		//  速度の代入
		GetPlayer( )->SetSpeed( GetPlayer( )->GetBaseSpeed( ) );

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

		//  方向ベクトルの設定
		GetPlayer( )->SetVecDirect( workVecDirect );

		//  座標の更新( 進行方向 × 速度 分の移動 )
		GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * GetPlayer( )->GetSpeed( ) );
	}
	else
	{
		//  ニュートラル状態に
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_NEUTRAL );
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::NEUTRAL ) );
	}

	//  注視点を向いている方向に
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
}

//--------------------------------------------------------------------------------------
//  ニュートラル状態に変更するかの判断をする関数
//--------------------------------------------------------------------------------------
void PlayerMove::JudgeChangeNeutral( void )
{

}