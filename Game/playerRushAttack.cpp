//--------------------------------------------------------------------------------------
//  プレイヤーステート( 突進攻撃 )   ( playerRushAttack.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerRushAttack.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "effekseerManager.h"
#include "enemy.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	PLAYER_TO_CAMERA_DISTANCE_Y		= 9.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Y2	= 10.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z		= 20.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z2	= 23.0f;

static const float	CAMERA_ROTATE_ANGLE				= 0.025f;

static const float	PLAYER_RUSH_ATTACK_SPEED		= 3.0f;
static const int	PLAYER_RUSH_ATTACK_DAMAGE		= 250;
static const float	PLAYER_RUSH_ATTACK_BLOW_POWER	= 7.0f;

static const float	PLAYER_HIT_SPHERE_POS_Y			= 3.0f;

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Init( void )
{
	//  親の初期化
	PlayerState::Init( );

	//  速度の初期化
	m_speed = PLAYER_RUSH_ATTACK_SPEED;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Uninit( void )
{
	m_speed = 0.0f;
}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Update( void )
{
	//  専用のカメラ制御
	ControlCameraExclusive( );

	//  アクション
	Action( );

	//  重力をかける
	AddGravity( );

	//  ニュートラル状態に変更するかの判断
	JudgeChangeNeutral( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Action( void )
{
	//  座標の更新( 進行方向 × 速度 分の移動 )
	GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * m_speed );

	//  注視点を向いている方向に
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  エフェクシア座標の設定
	D3DXVECTOR3 effekseerPos( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z );
	effekseerPos += GetPlayer( )->GetVecDirect( ) * 8.0f;
	EffekseerManager::SetPosition( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_LANCE ) , effekseerPos );

	//  キーがある程度までいった場合
	if( GetPlayer( )->GetAnimationKey( ) >= 11 )
	{
		//  速度の減衰
		m_speed += ( 0.0f - m_speed ) * 0.06f;

		//  エフェクトの停止
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_LANCE ) );
	}

	//  攻撃側の当たり判定の代入
	Utility::HIT_SPHERE attackHitSphere;
	attackHitSphere.position = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position.y += 2.0f;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

	//  シーンクラスのポインタ
	Scene* pScene = NULL;

	//  キーがこの範囲内にいる場合
	if( GetPlayer( )->GetAnimationKey( ) >= 1 && GetPlayer( )->GetAnimationKey( ) <= 10 )
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
						//  ダウンキャスト
						Enemy* pEnemy = ( Enemy* )pScene;

						if( Utility::HitSphere( attackHitSphere , pEnemy->GetHitSphere( ) ) )
						{
							pEnemy->Damage( PLAYER_RUSH_ATTACK_DAMAGE );
						}
					}
				}
				//  プレイヤー対戦モードの場合
				else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  ダウンキャスト
						Player* pPlayer = ( Player* )pScene;

						//  自分以外のプレイヤー番号の場合
						if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
						{
							if( Utility::HitSphere( attackHitSphere , pPlayer->GetHitSphere( ) ) )
							{
								//  吹っ飛び方向のベクトル算出
								D3DXVECTOR3 blowVecDirect = GetPlayer( )->GetVecDirect( );
								D3DXMATRIX mtxRot;
								D3DXMatrixIdentity( &mtxRot );
								D3DXMatrixRotationY( &mtxRot , D3DX_PI * 0.1f );
								D3DXVec3TransformNormal( &blowVecDirect , &blowVecDirect , &mtxRot );
								D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

								pPlayer->Damage( blowVecDirect , PLAYER_RUSH_ATTACK_BLOW_POWER , PLAYER_RUSH_ATTACK_DAMAGE );
							}
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  専用のカメラ制御をする関数
//--------------------------------------------------------------------------------------
void PlayerRushAttack::ControlCameraExclusive( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = NULL;

	pPS4Input = SceneManager::GetPS4Input( );

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	//  シーンクラスのポインタ
	Scene* pScene = NULL;			

	//  シーンクラスのポインタ
	Scene *pScene2 =NULL;										

	//  カメラ方向ベクトル
	D3DXVECTOR3 cameraVecDirect;

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

					//  カメラの注視点設定
					pCamera->SetCameraPosAt( posEnemy , 0.1f );

					D3DXVECTOR3 workPos;
					D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
					workVec.y = 0.0f;

					D3DXVec3Normalize( &workVec , &workVec );

					//  CPU対戦の場合
					if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
					{
						workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
						workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
					}
					//  プレイヤー対戦の場合
					else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
					{
						workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
						workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
					}

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

						//if( fDistance > 8.0f )
						{
							//  カメラの注視点設定
							pCamera->SetCameraPosAt( posEnemy , 0.8f );
						}

						D3DXVECTOR3 workPos;
						D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
						workVec.y = 0.0f;

						D3DXVec3Normalize( &workVec , &workVec );

						//  CPU対戦の場合
						if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
						{
							workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
						}
						//  プレイヤー対戦の場合
						else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
						{
							workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
						}

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