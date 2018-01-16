//--------------------------------------------------------------------------------------
//  プレイヤーステート  ( playerState.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/28
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
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	PLAYER_TO_CAMERA_DISTANCE_Y		= 9.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Y2	= 10.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z		= 20.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z2	= 23.0f;

static const float	PLAYER_GRAVITY2					= 0.2f;
static const float	CAMERA_ROTATE_ANGLE				= 0.025f;
static const float	ESCAPE_POWER					= 1.0f;

static const float	PLAYER_ENABLE_BRAVE_ATTACK_DIST	= 80.0f;

static const int	PLAYER_GUARD_FRAME				= 2;

static const float	PLAYER_HIT_SPHERE_POS_Y			= 3.0f;

//--------------------------------------------------------------------------------------
//  重力をかける関数
//--------------------------------------------------------------------------------------
void PlayerState::Init( void )
{
	m_moveHeight = 0.0f;
	m_pressTime = 0;
}

//--------------------------------------------------------------------------------------
//  プレイヤーデータの設定をする関数
//--------------------------------------------------------------------------------------
void PlayerState::SetPlayer( Player* player )
{
	if( player == nullptr )
	{
		return;
	}

	m_player = player;
}

//--------------------------------------------------------------------------------------
//  プレイヤーデータの取得をする関数
//--------------------------------------------------------------------------------------
Player* PlayerState::GetPlayer( void )
{
	if( m_player == nullptr )
	{
		return nullptr;
	}

	return m_player;
}

//--------------------------------------------------------------------------------------
//  重力をかける関数
//--------------------------------------------------------------------------------------
void PlayerState::AddGravity( void )
{
	//  地面との当たっていない場合
	if( GetPlayer( )->JudgeHitGround( ) == false )
	{
		//  下方向に力をかける
		m_moveHeight -= PLAYER_GRAVITY2;

		//  Y軸移動
		GetPlayer( )->MoveHeight( m_moveHeight );
	}
	else
	{
		//  下方向の力初期化
		m_moveHeight = 0.0f;
	}
}

//--------------------------------------------------------------------------------------
//  カメラ制御をする関数
//--------------------------------------------------------------------------------------
void PlayerState::ControlCamera( void )
{
	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	//  シーンクラスのポインタ
	Scene* pScene = NULL;			

	//  カメラ方向ベクトル
	D3DXVECTOR3 cameraVecDirect;

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	if( GetPlayer( )->GetLockon( ) == false )
	{
		D3DXVECTOR3 workPosEye = GetPlayer( )->GetPos( );

		//  CPU対戦の場合
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y;
			workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
		}
		//  プレイヤー対戦の場合
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
			workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
		}

		//  カメラの視点設定
		pCamera->SetCameraPosEye( workPosEye , 1.0f );

		//  注視点座標の代入
		GetPlayer( )->GetPositionAt( ) = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f;

		D3DXVECTOR3 workPosAt = GetPlayer( )->GetPos( );

		//  CPU対戦の場合
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			workPosAt.y += PLAYER_TO_CAMERA_DISTANCE_Y * 0.7f;
			workPosAt += cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
		}
		//  プレイヤー対戦の場合
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			workPosAt.y += PLAYER_TO_CAMERA_DISTANCE_Y2 * 0.7f;
			workPosAt += cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
		}

		//  カメラの注視点設定
		pCamera->SetCameraPosAt( workPosAt , 1.0f );
	}
	else
	{
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
						D3DXVECTOR3 posEnemy;						//  敵の座標

						//  敵情報の取得
						posEnemy = pScene->GetPos( );

						float fDistance = sqrtf( ( posEnemy.x - GetPlayer( )->GetPos( ).x ) * ( posEnemy.x - GetPlayer( )->GetPos( ).x ) +
												 ( posEnemy.z - GetPlayer( )->GetPos( ).z ) * ( posEnemy.z - GetPlayer( )->GetPos( ).z ) );

						//if( fDistance > 10.0f )
						{
							//  カメラの注視点設定
							pCamera->SetCameraPosAt( posEnemy , 0.1f );
						}

						D3DXVECTOR3 workPos;
						D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
						workVec.y = 0.0f;

						D3DXVec3Normalize( &workVec , &workVec );

						workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
						workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;

						//  カメラの視点設定
						pCamera->SetCameraPosEye( workPos , 0.1f );

						break;
					}
				}
				//  プレイヤー対戦モードの場合
				if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					//  種類がプレイヤーの場合
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						Player* pPlayer = ( Player* )pScene;

						if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
						{
							D3DXVECTOR3 posEnemy;						//  敵の座標

							//  敵情報の取得
							posEnemy = pScene->GetPos( );

							float fDistance = sqrtf( ( posEnemy.x - GetPlayer( )->GetPos( ).x ) * ( posEnemy.x - GetPlayer( )->GetPos( ).x ) +
													 ( posEnemy.z - GetPlayer( )->GetPos( ).z ) * ( posEnemy.z - GetPlayer( )->GetPos( ).z ) );

							//  カメラの注視点設定
							pCamera->SetCameraPosAt( posEnemy , 0.05f );

							D3DXVECTOR3 workPos;
							D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
							workVec.y = 0.0f;

							D3DXVec3Normalize( &workVec , &workVec );

							workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;

							//  カメラの視点設定
							pCamera->SetCameraPosEye( workPos , 0.1f );

							break;
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}

	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = NULL;

	pPS4Input = SceneManager::GetPS4Input( );

	//  ロックオン状態ではない場合
	if( GetPlayer( )->GetLockon( ) == false )
	{
		//  カメラ注視点切り替え処理
		if( pPS4Input->GetRightStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x > 300 ||
			pPS4Input->GetRightStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x < -300 ||
			pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) || pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
		{
			if( pPS4Input->GetRightStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x < 0 ||
				pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) )
			{
				D3DXVECTOR3 cameraPosAt = pCamera->GetCameraBasePosAt( );
				D3DXVECTOR3 cameraPosEye = pCamera->GetCameraBasePosEye( );
				D3DXMATRIX mtxRot;
				float fAngle = atan2f( cameraPosAt.z - cameraPosEye.z , cameraPosAt.x - cameraPosEye.x );
				float fLength = sqrtf( ( cameraPosAt.z - cameraPosEye.z ) * ( cameraPosAt.z - cameraPosEye.z ) +
										( cameraPosAt.x - cameraPosEye.x ) * ( cameraPosAt.x - cameraPosEye.x ) );

				fAngle += CAMERA_ROTATE_ANGLE;

				cameraPosAt.x = cosf( fAngle ) * fLength + cameraPosEye.x;
				cameraPosAt.z = sinf( fAngle ) * fLength + cameraPosEye.z;

				//  カメラの注視点設定
				pCamera->SetCameraPosAt( cameraPosAt , 1.0f );
			}
			else if( pPS4Input->GetRightStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x > 0 ||
					 pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
			{
				D3DXVECTOR3 cameraPosAt = pCamera->GetCameraBasePosAt( );
				D3DXVECTOR3 cameraPosEye = pCamera->GetCameraBasePosEye( );
				D3DXMATRIX mtxRot;
				float fAngle = atan2f( cameraPosAt.z - cameraPosEye.z , cameraPosAt.x - cameraPosEye.x );
				float fLength = sqrtf( ( cameraPosAt.z - cameraPosEye.z ) * ( cameraPosAt.z - cameraPosEye.z ) +
										( cameraPosAt.x - cameraPosEye.x ) * ( cameraPosAt.x - cameraPosEye.x ) );

				fAngle -= CAMERA_ROTATE_ANGLE;

				cameraPosAt.x = cosf( fAngle ) * fLength + cameraPosEye.x;
				cameraPosAt.z = sinf( fAngle ) * fLength + cameraPosEye.z;

				//  カメラの注視点設定
				pCamera->SetCameraPosAt( cameraPosAt , 1.0f );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  ロックオン状態の変更をする関数
//--------------------------------------------------------------------------------------
void PlayerState::ChangeLockon( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_R;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_RETURN;
	}

#ifdef KEYBOARD_ENABLE

	//  ENTERキーまたは、L1ボタンを押した場合
	if( ( pKeyboard->GetKeyboardTrigger( nKey ) ) )
	{
		//  ロックオンの切り替え
		GetPlayer( )->ChangeLockon( );
	}

#else

	//  ENTERキーまたは、L1ボタンを押した場合
	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_L1 ) )
	{
		//  ロックオンの切り替え
		GetPlayer( )->ChangeLockon( );
	}

#endif

}

//--------------------------------------------------------------------------------------
//  ニュートラル状態に変更するかの判断をする関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeNeutral( void )
{
	//  アニメーションが終了している場合
	if( GetPlayer( )->GetAnimationFinish( ) )
	{
		//  ニュートラルモーションの変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_NEUTRAL );

		//  ニュートラル状態に
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::NEUTRAL ) );
	}
}

//--------------------------------------------------------------------------------------
//  移動状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeMove( void )
{
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

	if( move.x != 0 ||
		move.z != 0 )
	{
		//  モーションの変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_MOVE );

		//  ステートの変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MOVE ) );
	}
}

//--------------------------------------------------------------------------------------
//  ジャンプ状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeJump( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

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

		//  ジャンプ状態に
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::JUMP ) );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CROSS ) )
	{
		//  ジャンプアニメーションに
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_JUMP );

		//  ジャンプ状態に
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::JUMP ) );
	}

#endif

}

//--------------------------------------------------------------------------------------
//  HP攻撃000状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeHPAttack000( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_B;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_SEMICOLON;
	}

#ifdef KEYBOARD_ENABLE

	if( pKeyboard->GetKeyboardTrigger( nKey ) )
	{
		//  モーションの変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP001 );

		//  ブレイブ攻撃に変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_HP_ATTACK000 ) );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_SQUARE ) )
	{
		//  モーションの変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP001 );

		//  ブレイブ攻撃に変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_HP_ATTACK000 ) );
	}

#endif
}

//--------------------------------------------------------------------------------------
//  ブレイブ攻撃000状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeBraveAttack000( void )
{
	int nKey = 0;

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

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_C;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_K;
	}

#ifdef KEYBOARD_ENABLE

	if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_BRAVE_ATTACK_DIST )
	{ 
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  ブレイブ攻撃000状態に変更
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_BEFORE );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_BEFORE ) );

			if( move.x >= 0.0f )
			{
				//  右回り追尾状態に
				GetPlayer( )->SetHomingDirect( false );
			}
			else
			{
				//  左回り追尾状態に
				GetPlayer( )->SetHomingDirect( true );
			}
		}
	}
	else
	{
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  ブレイブ攻撃000状態に変更
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  攻撃側の当たり判定の代入
			D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;

			//  エフェクトの生成
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
																   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , effekseerPosition.y , GetPlayer( )->GetPos( ).z ) ,
																   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );

		}
	}

#else

	if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_BRAVE_ATTACK_DIST )
	{
		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
		{
			//  ブレイブ攻撃000状態に変更
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_BEFORE );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_BEFORE ) );

			if( move.x >= 0.0f )
			{
				//  右回り追尾状態に
				GetPlayer( )->SetHomingDirect( false );
			}
			else
			{
				//  左回り追尾状態に
				GetPlayer( )->SetHomingDirect( true );
			}
		}
	}
	else
	{
		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
		{
			//  ブレイブ攻撃000状態に変更
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  攻撃側の当たり判定の代入
			D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;

			//  エフェクトの生成
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
																   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , effekseerPosition.y , GetPlayer( )->GetPos( ).z ) ,
																   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );
		}
	}

#endif
}

//--------------------------------------------------------------------------------------
//  回避状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeEscape( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_G;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_P;
	}

	//  移動方向
	D3DXVECTOR3 move( 0.0f , 0.0f , 0.0f );

#ifdef KEYBOARD_ENABLE

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

#ifdef KEYBOARD_ENABLE

	if( pKeyboard->GetKeyboardTrigger( nKey ) && ( move.x != 0.0f || move.z != 0.0f ) )
	{
		//  エフェクトの停止
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

		//  エフェクトの停止
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

		//  魔法の削除
		GetPlayer( )->DeleteMagic( );

		D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

		//  速度の代入
		GetPlayer( )->SetSpeed( GetPlayer( )->GetBaseSpeed( ) );

		D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

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

		//  移動力の計算
		GetPlayer( )->SetMovePower( ESCAPE_POWER );

		//  モーションの変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ESCAPE );

		//  状態の変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ESCAPE ) );
	}
#else

	if( pPS4Input->GetPress( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CROSS ) && pPS4Input->GetPress( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_R1 ) &&
		( ( float )pPS4Input->GetLeftStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x != 0.0f || ( float )pPS4Input->GetLeftStickDisposition( GetPlayer( )->GetPlayerNo( ) ).y != 0.0f ) )
	{
		//  エフェクトの停止
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

		//  エフェクトの停止
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

		//  魔法の削除
		GetPlayer( )->DeleteMagic( );

		D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

		//  速度の代入
		GetPlayer( )->SetSpeed( GetPlayer( )->GetBaseSpeed( ) );

		D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

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

		//  移動力の計算
		GetPlayer( )->SetMovePower( ESCAPE_POWER );

		//  モーションの変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ESCAPE );

		//  状態の変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ESCAPE ) );
	}

#endif
}

//--------------------------------------------------------------------------------------
//  防御状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeGuard( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_F;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_O;
	}

#ifdef KEYBOARD_ENABLE

	//  R1を押した場合間
	if( pKeyboard->GetKeyboardPress( nKey ) )
	{
		//  一定フレーム以上になった場合
		if( m_pressTime >= PLAYER_GUARD_FRAME )
		{
			//  モーションの変更
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_GUARD );

			//  状態の変更
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::GUARD ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  攻撃側の当たり判定の代入
			D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
			effekseerPosition.y += 3.0f;

			//  エフェクトの生成
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
																	D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) ,
																	D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SHIELD , handle );
		}

		//  押す時間のカウント
		m_pressTime++;
	}
	else
	{
		//  押す時間の初期化
		m_pressTime = 0;
	}

#else

	//  R1を押した場合間
	if( ( pPS4Input->GetPress( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_R1 ) ) )
	{
		//  一定フレーム以上になった場合
		if( m_pressTime >= PLAYER_GUARD_FRAME )
		{
			//  モーションの変更
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_GUARD );

			//  状態の変更
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::GUARD ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  攻撃側の当たり判定の代入
			D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
			effekseerPosition.y += 3.0f;

			//  エフェクトの生成
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
																	D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) ,
																	D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SHIELD , handle );
		}

		//  押す時間のカウント
		m_pressTime++;
	}
	else
	{
		//  押す時間の初期化
		m_pressTime = 0;
	}

#endif
}

//--------------------------------------------------------------------------------------
//  ロックオンダッシュに変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeLockonDash( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	//  ロックオン状態の場合
	if( GetPlayer( )->GetLockon( ) == true )
	{
		if( GetPlayer( )->GetPlayerNo( ) == 0 )
		{
			nKey = DIK_V;
		}
		else if( GetPlayer( )->GetPlayerNo( ) == 1 )
		{
			nKey = DIK_L;
		}

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

#ifdef KEYBOARD_ENABLE

		//  YとRBを長押ししている間
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			if( move.x != 0.0f || move.z != 0.0f )
			{
				//  モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

				//  状態の変更
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::FREE_DASH ) );
			}
			else
			{
				//  モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

				//  状態の変更
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::LOCKON_DASH ) );
			}
		}

#else

		//  YとRBを長押ししている間
		if( ( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_TRIANGLE ) ) )
		{
			if( move.x != 0.0f || move.z != 0.0f )
			{
				//  モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

				//  状態の変更
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::FREE_DASH ) );
			}
			else
			{
				//  モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

				//  状態の変更
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::LOCKON_DASH ) );
			}
		}

#endif
	}
}

//--------------------------------------------------------------------------------------
//  突進攻撃に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeRushAttack( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_B;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_SEMICOLON;
	}

#ifdef KEYBOARD_ENABLE

	if( pKeyboard->GetKeyboardTrigger( nKey ) )
	{
		//  モーションの変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_RUSH_ATTACK );

		//  状態の変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::RUSH_ATTACK ) );

		//  角度の算出
		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  エフェクトの生成
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_LANCE ,
																D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z ) ,
																D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.5f , 1.5f , 1.5f ) );
		GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_LANCE , handle );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_SQUARE ) )
	{
		//  モーションの変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_RUSH_ATTACK );

		//  状態の変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::RUSH_ATTACK ) );

		//  角度の算出
		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  エフェクトの生成
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_LANCE ,
																D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z ) ,
																D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.5f , 1.5f , 1.5f ) );
		GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_LANCE , handle );
	}

#endif
}

//--------------------------------------------------------------------------------------
//  魔法準備状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeMagicReady( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetMagic( ) == NULL )
	{
		if( GetPlayer( )->GetPlayerNo( ) == 0 )
		{
			nKey = DIK_H;
		}
		else if( GetPlayer( )->GetPlayerNo( ) == 1 )
		{
			nKey = DIK_AT;
		}

#ifdef KEYBOARD_ENABLE

		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  モーションの変更
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_READY );

			//  状態の変更
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_READY ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  エフェクトの生成
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_AURA ,
																   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z ) ,
																   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.0f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_AURA , handle );

			//  エフェクトの生成
			handle = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE ,
											   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y , GetPlayer( )->GetPos( ).z ) ,
											   D3DXVECTOR3( -D3DX_PI * 0.5f , fAngle , 0.0f ) , D3DXVECTOR3( 5.0f , 5.0f , 5.0f ) , 1.0f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE , handle );

			D3DXVECTOR3 magicPos = GetPlayer( )->GetPos( );
			magicPos.y += 3.0f;

			Magic* magic = nullptr;

			if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::FIRE )
			{
				magic = MagicFire::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
										   D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}
			else if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::LIGHTNING )
			{
				magic = MagicLightning::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
												D3DXVECTOR3( 0.6f , 0.6f , 0.6f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}
			else if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::TORNADE )
			{
				magic = MagicTornade::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
											  D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}

			//  魔法の設定
			GetPlayer( )->SetMagic( magic );
		}

#else

		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_R2 ) )
		{
			//  モーションの変更
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_READY );

			//  状態の変更
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_READY ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  エフェクトの生成
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_AURA ,
																   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z ) ,
																   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.0f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_AURA , handle );

			//  エフェクトの生成
			handle = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE ,
											   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y , GetPlayer( )->GetPos( ).z ) ,
											   D3DXVECTOR3( -D3DX_PI * 0.5f , fAngle , 0.0f ) , D3DXVECTOR3( 5.0f , 5.0f , 5.0f ) , 1.0f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE , handle );

			D3DXVECTOR3 magicPos = GetPlayer( )->GetPos( );
			magicPos.y += 3.0f;

			Magic* magic = nullptr;

			if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::FIRE )
			{
				magic = MagicFire::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
										   D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}
			else if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::LIGHTNING )
			{
				magic = MagicLightning::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
												D3DXVECTOR3( 0.6f , 0.6f , 0.6f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}
			else if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::TORNADE )
			{
				magic = MagicTornade::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
											  D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}

			//  魔法の設定
			GetPlayer( )->SetMagic( magic );
		}

#endif

	}
}