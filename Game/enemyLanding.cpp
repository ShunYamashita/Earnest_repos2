//--------------------------------------------------------------------------------------
//  エネミーステート( 着地 )   ( enemyLanding.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyLanding.h"
#include "enemy.h"
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
void EnemyLanding::Init( void )
{
	//  親の初期化
	EnemyState::Init( );
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void EnemyLanding::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void EnemyLanding::Update( void )
{
	//  ニュートラル状態に変更するかの判断
	JudgeChangeNeutral( );

	//  重力をかける
	AddGravity( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void EnemyLanding::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void EnemyLanding::Action( void )
{

}