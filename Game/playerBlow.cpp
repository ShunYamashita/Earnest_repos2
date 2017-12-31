//--------------------------------------------------------------------------------------
//  プレイヤーステート( 吹っ飛び )   ( playerBlow.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerBlow.h"
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
void PlayerBlow::Init( void )
{
	//  親の初期化
	PlayerState::Init( );

	//  プレイヤーが持っているエフェクシア全て停止
	GetPlayer( )->StopAllEffekseer( );

	//  吹っ飛び力の取得
	m_blowPower = GetPlayer( )->GetBlowPower( );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBlow::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBlow::Update( void )
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
void PlayerBlow::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerBlow::Action( void )
{
	//  無敵時間の設定
	GetPlayer( )->SetInvisibleTime( 2 );

	//  速度の減衰
	m_blowPower += ( 0.0f - m_blowPower ) * 0.04f;

	//  吹っ飛び力分の移動
	GetPlayer( )->MovePos( GetPlayer( )->GetBlowVecDirect( ) * m_blowPower );

	//  プレイヤー方向ベクトルの設定
	D3DXVECTOR3 playerVecDirect;
	D3DXVec3Normalize( &playerVecDirect , &( GetPlayer( )->GetPositionAt( ) - GetPlayer( )->GetPos( ) ) );
	GetPlayer( )->SetVecDirect( playerVecDirect );

	//  注視点の設定
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) - GetPlayer( )->GetBlowVecDirect( ) * 5.0f );

	//  アニメーションが終了するか、吹っ飛び力がある程度まで落ちた場合
	if( GetPlayer( )->GetAnimationFinish( ) == true || m_blowPower < 1.0f )
	{
		//  現在の吹っ飛び力の設定
		GetPlayer( )->SetBlowPower( m_blowPower );

		//  受け身の状態に
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_PASSIVE );
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::PASSIVE ) );
	}
}