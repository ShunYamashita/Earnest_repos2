//--------------------------------------------------------------------------------------
//  プレイヤーステート  ( playerWinksState.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/28
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinks.h"
#include "playerWinksState.h"
#include "fbxLoader.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"
#include "enemyRauder.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	PLAYER_TO_CAMERA_DISTANCE_Y		= 15.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Y2	= 17.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z		= 30.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z2	= 35.0f;

static const float	PLAYER_GRAVITY2					= 0.2f;
static const float	CAMERA_ROTATE_ANGLE				= 0.025f;
static const float	ESCAPE_POWER					= 1.0f;

static const float	PLAYER_ENABLE_BRAVE_ATTACK_DIST	= 80.0f;

static const int	PLAYER_GUARD_FRAME				= 2;

static const float	PLAYER_HIT_SPHERE_POS_Y			= 3.0f;

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  初期化する関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::Init( void )
{
	m_moveHeight = 0.0f;
	m_pressTime = 0;
}

//--------------------------------------------------------------------------------------
//  プレイヤーデータの設定をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::SetPlayer( PlayerWinks* playerWinks )
{
	if( playerWinks == nullptr )
	{
		return;
	}

	m_player = playerWinks;
}

//--------------------------------------------------------------------------------------
//  プレイヤーデータの取得をする関数
//--------------------------------------------------------------------------------------
PlayerWinks* PlayerWinksState::GetPlayer( void )
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
void PlayerWinksState::AddGravity( void )
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
void PlayerWinksState::ControlCamera( void )
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
							pCamera->SetCameraPosAt( posEnemy , 0.3f );
						}

						D3DXVECTOR3 workPos;
						D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
						workVec.y = 0.0f;

						D3DXVec3Normalize( &workVec , &workVec );

						workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
						workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;

						//  カメラの視点設定
						pCamera->SetCameraPosEye( workPos , 0.3f );

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
						PlayerWinks* pPlayer = ( PlayerWinks* )pScene;

						if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
						{
							D3DXVECTOR3 posEnemy;						//  敵の座標

							//  敵情報の取得
							posEnemy = pScene->GetPos( );

							float fDistance = sqrtf( ( posEnemy.x - GetPlayer( )->GetPos( ).x ) * ( posEnemy.x - GetPlayer( )->GetPos( ).x ) +
													 ( posEnemy.z - GetPlayer( )->GetPos( ).z ) * ( posEnemy.z - GetPlayer( )->GetPos( ).z ) );

							//  カメラの注視点設定
							pCamera->SetCameraPosAt( posEnemy , 0.3f );

							D3DXVECTOR3 workPos;
							D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
							workVec.y = 0.0f;

							D3DXVec3Normalize( &workVec , &workVec );

							workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;

							//  カメラの視点設定
							pCamera->SetCameraPosEye( workPos , 0.3f );

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
void PlayerWinksState::ChangeLockon( void )
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
//  ターゲット方向に向く関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::LookTarget( void )
{
	//  ターゲットの方向に進行方向ベクトルを変更
	D3DXVECTOR3 workVecDirect = GetTargetPosition( ) - GetPlayer( )->GetPos( );
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetPlayer( )->SetVecDirect( workVecDirect );
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
}

//--------------------------------------------------------------------------------------
//  ターゲット方向( Y無視 )に向く関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::LookTargetXZ( void )
{
	//  ターゲットの方向に進行方向ベクトルを変更
	D3DXVECTOR3 workVecDirect = GetTargetPosition( ) - GetPlayer( )->GetPos( );
	workVecDirect.y = 0.0f;
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetPlayer( )->SetVecDirect( workVecDirect );
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
}

//--------------------------------------------------------------------------------------
//  ターゲット座標の取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3	PlayerWinksState::GetTargetPosition( void )
{
	//  シーンクラスのポインタ
	Scene* pScene = NULL;

	//  ターゲットの座標
	D3DXVECTOR3 targetPosition;

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
					PlayerWinks* player = ( PlayerWinks* )pScene;

					//  自分以外のプレイヤー番号の場合
					if( m_player->GetPlayerNo( ) != player->GetPlayerNo( ) )
					{
						D3DXVECTOR3 targetPosition = player->GetPos( );
					}
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	return targetPosition;
}

//--------------------------------------------------------------------------------------
//  待機状態に変更するかの判断をする関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeIdle( void )
{
	//  モーションが終了している場合
	if( GetPlayer( )->GetMotionFinish( ) )
	{
		//  待機状態に
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::IDLE ) );
	}
}

//--------------------------------------------------------------------------------------
//  移動状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeMove( void )
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
		//  ステートの変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::MOVE ) );
	}
}

//--------------------------------------------------------------------------------------
//  ジャンプ状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeJump( void )
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
		//  ジャンプ状態に
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::JUMP ) );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CROSS ) )
	{
		//  ジャンプ状態に
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::JUMP ) );
	}

#endif

}

//--------------------------------------------------------------------------------------
//  攻撃状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeAttack( void )
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

	D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	//  カメラ方向ベクトル
	D3DXVECTOR3 cameraVecDirect;

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	//  方向スティックを進行方向ベクトルに
	if( move.x != 0.0f || move.z != 0.0f )
	{
		D3DXVec3Normalize( &move , &move );

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

		GetPlayer( )->SetVecDirect( workVecDirect );
	}

#ifdef KEYBOARD_ENABLE

	if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_BRAVE_ATTACK_DIST )
	{ 
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  ターゲット方向に向かせる
			LookTargetXZ( );

			//  攻撃状態に変更
			GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK01 );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::ATTACK ) );

			//  追尾状態に
			GetPlayer( )->SetHomingDirect( true );
		}
	}
	else
	{
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  攻撃状態に変更
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::ATTACK ) );

			//  追尾状態に
			GetPlayer( )->SetHomingDirect( false );
		}
	}

#else

	if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_BRAVE_ATTACK_DIST )
	{
		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
		{
			//  ターゲット方向に向かせる
			LookTargetXZ( );

			//  攻撃状態に変更
			GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK01 );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::ATTACK ) );

			//  追尾状態に
			GetPlayer( )->SetHomingDirect( true );
		}
	}
	else
	{
		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
		{
			//  攻撃状態に変更
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::ATTACK ) );

			//  追尾状態に
			GetPlayer( )->SetHomingDirect( false );
		}
	}

#endif

}

//--------------------------------------------------------------------------------------
//  ダッシュ状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeDash( void )
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
				//  状態の変更
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::FREE_DASH ) );
			}
			else
			{
				//  状態の変更
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::LOCKON_DASH ) );
			}
		}

#else

		//  YとRBを長押ししている間
		if( ( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_TRIANGLE ) ) )
		{
			if( move.x != 0.0f || move.z != 0.0f )
			{
				//  状態の変更
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::FREE_DASH ) );
			}
			else
			{
				//  状態の変更
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::LOCKON_DASH ) );
			}
		}

#endif
	}
}

//--------------------------------------------------------------------------------------
//  防御状態に変更するかの判断する関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeGuard( void )
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
			//  状態の変更
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::GUARD ) );
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
			//  状態の変更
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::GUARD ) );
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
//  HP攻撃状態に変更するか判断する関数
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeHPAttack( void )
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
		//  HP攻撃に変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::HP_ATTACK ) );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_SQUARE ) )
	{
		//  HP攻撃に変更
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::HP_ATTACK ) );
	}

#endif
}