//--------------------------------------------------------------------------------------
//  プレイヤーステート( ジャンプ )   ( playerJump.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerJump.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	PLAYER_GRAVITY = 0.1f;
static const int	PLAYER_ENABLE_NUM_JUMP = 2;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerJump::Init( void )
{
	//  親の初期化
	PlayerState::Init( );

	//  メンバ変数の初期化
	m_countJump = 1;
	m_jumpPower = GetPlayer( )->GetJumpPower( );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerJump::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerJump::Update( void )
{
	//  カメラ制御
	ControlCamera( );

	//  アクション
	Action( );

	//  回避状態に変更するかの判断
	JudgeChangeEscape( );

	//  HP攻撃000状態に変更するかの判定
	JudgeChangeHPAttack000( );

	//  ブレイブ攻撃000状態に変更するかの判定
	JudgeChangeBraveAttack000( );

	//  防御状態に変更するかの判断
	JudgeChangeGuard( );

	//  ロックオンダッシュ状態に変更するかの判断
	JudgeChangeLockonDash( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerJump::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerJump::Action( void )
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

	//  PLAYER_ENABLE_NUM_JUMP( 2 )段ジャンプ用処理
	if( m_countJump < PLAYER_ENABLE_NUM_JUMP )
	{
		int nKey = 0;

		if( GetPlayer( )->GetPlayerNo( ) == 0 )
		{
			nKey = DIK_SPACE;
		}
		else if( GetPlayer( )->GetPlayerNo( ) == 1 )
		{
			nKey = DIK_RCONTROL;
		}

#ifdef KEYBOARD_ENABLE

		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  ジャンプアニメーションに
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_JUMP );

			//  ジャンプ力の初期化
			m_jumpPower = GetPlayer( )->GetJumpPower( );

			//  ジャンプカウント
			m_countJump++;
		}

#else

		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CROSS ) )
		{
			//  ジャンプアニメーションに
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_JUMP );

			//  ジャンプ力の初期化
			m_jumpPower = GetPlayer( )->GetJumpPower( );

			//  ジャンプカウント
			m_countJump++;
		}

#endif

	}

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
			D3DXVec3Normalize( &workVecDirect , &workVecDirect );
		}
		else
		{
			D3DXVec3Normalize( &workVecDirect , &move );
		}

		//  方向ベクトルの設定
		GetPlayer( )->SetVecDirect( workVecDirect );

		//  座標の更新( 進行方向 × 速度 分の移動 )
		GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * GetPlayer( )->GetSpeed( ) );
	}

	//  注視点を向いている方向に
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  ジャンプ力分上に上がるのとジャンプ力を重力分下げる
	GetPlayer( )->MoveHeight( m_jumpPower );
	m_jumpPower -= PLAYER_GRAVITY;

	//  地面についた場合
	if( GetPlayer( )->JudgeHitGround( ) )
	{
		//  着地アニメーションに
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_LANDING_SHORT );
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::LANDING ) );
	}
}