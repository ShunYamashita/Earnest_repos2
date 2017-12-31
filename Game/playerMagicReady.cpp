//--------------------------------------------------------------------------------------
//  プレイヤーステート( 魔法準備 )   ( playerMagicReady.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerMagicReady.h"
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

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Init( void )
{
	//  親の初期化
	PlayerState::Init( );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Update( void )
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
void PlayerMagicReady::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Action( void )
{
	//  移動方向
	D3DXVECTOR3 move( 0.0f , 0.0f , 0.0f );

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4コントローラー情報の取得
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

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

	//  カメラ方向ベクトル
	D3DXVECTOR3 cameraVecDirect;

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	Magic* magic = GetPlayer( )->GetMagic( );

	if( magic != NULL )
	{
		Player::MAGIC magicType = GetPlayer( )->GetMagicType( );

		//  雷魔法を持っている場合
		if( magicType == Player::MAGIC::LIGHTNING )
		{
			D3DXVECTOR3 vecDirect( move.x , 0.0f , move.z );

			cameraVecDirect = pCamera->GetCameraVecDirect( );

			if( pCamera != NULL )
			{
				float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

				D3DXMATRIX mtxRot;
				D3DXMatrixIdentity( &mtxRot );
				D3DXMatrixRotationY( &mtxRot , fAngle );

				D3DXVec3TransformNormal( &vecDirect , &move , &mtxRot );
				D3DXVec3Normalize( &vecDirect , &vecDirect );
			}
			else
			{
				D3DXVec3Normalize( &vecDirect , &move );
			}

			//  魔法陣を動かす
			MagicLightning* pMagicLightning = ( MagicLightning* )GetPlayer( )->GetMagic( );
			pMagicLightning->MoveMagicPos( vecDirect );
		}	

		//  竜巻魔法を持っている倍
		if( magicType == Player::MAGIC::TORNADE )
		{
			D3DXVECTOR3 vecDirect( move.x , 0.0f , move.z );

			cameraVecDirect = pCamera->GetCameraVecDirect( );

			if( pCamera != NULL )
			{
				float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

				D3DXMATRIX mtxRot;
				D3DXMatrixIdentity( &mtxRot );
				D3DXMatrixRotationY( &mtxRot , fAngle );

				D3DXVec3TransformNormal( &vecDirect , &move , &mtxRot );
				D3DXVec3Normalize( &vecDirect , &vecDirect );
			}
			else
			{
				D3DXVec3Normalize( &vecDirect , &move );
			}

			//  移動方向ベクトルの設定
			MagicTornade* pMagicToranade = ( MagicTornade* )GetPlayer( )->GetMagic( );

			if( move.x == 0.0f && move.z == 0.0f )
			{
				vecDirect = GetPlayer( )->GetVecDirect( );
			}

			pMagicToranade->SetMoveVecDirect( vecDirect );
		}
				
		int nKey= 0;

		if( GetPlayer( )->GetPlayerNo( ) == 0 )
		{
			nKey = DIK_H;
		}
		else if( GetPlayer( )->GetPlayerNo( ) == 1 )
		{
			nKey = DIK_AT;
		}

#ifdef KEYBOARD_ENABLE

		if( pKeyboard->GetKeyboardRelease( nKey ) )
		{
			if( magicType == Player::MAGIC::FIRE )
			{
				//  魔法発動モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE );

				//  魔法発動状態に
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  実行状態に
				magic->SetActive( );
			}
			else if( magicType == Player::MAGIC::LIGHTNING )
			{
				//  魔法発動モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE2 );

				//  魔法発動状態に
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  実行状態に
				magic->SetActive( );
			}
			else if( magicType == Player::MAGIC::TORNADE )
			{
				//  魔法発動モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE3 );

				//  魔法発動状態に
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  実行状態に
				magic->SetActive( );
			}
		}

#else

		if( pPS4Input->GetRelease( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_R2 ) )
		{
			if( magicType == Player::MAGIC::FIRE )
			{
				//  魔法発動モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE );

				//  魔法発動状態に
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  実行状態に
				magic->SetActive( );
			}
			else if( magicType == Player::MAGIC::LIGHTNING )
			{
				//  魔法発動モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE2 );

				//  魔法発動状態に
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  実行状態に
				magic->SetActive( );
			}
			else if( magicType == Player::MAGIC::TORNADE )
			{
				//  魔法発動モーションの変更
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE3 );

				//  魔法発動状態に
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  エフェクトの停止
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  実行状態に
				magic->SetActive( );
			}
		}

#endif

	}
}