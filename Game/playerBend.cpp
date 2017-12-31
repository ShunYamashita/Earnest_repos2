//--------------------------------------------------------------------------------------
//  プレイヤーステート( やられ )   ( playerBend.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerBend.h"
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
void PlayerBend::Init( void )
{
	//  親の初期化
	PlayerState::Init( );

	//  プレイヤーが持っているエフェクシア全て停止
	GetPlayer( )->StopAllEffekseer( );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBend::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBend::Update( void )
{
	//  カメラ制御
	ControlCamera( );

	//  ニュートラル状態に変更するかの判断
	JudgeChangeNeutral( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBend::Draw( void )
{

}	

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerBend::Action( void )
{

}	