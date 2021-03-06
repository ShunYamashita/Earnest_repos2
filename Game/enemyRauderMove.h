//--------------------------------------------------------------------------------------
//  エネミーステート( 移動 )  ( enemyRauderMove.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_RAUDER_MOVE_H_
#define _ENEMY_RAUDER_MOVE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyRauderState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class EnemyRauder;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( 移動 ) クラスの定義
//--------------------------------------------------------------------------------------
class EnemyRauderMove : public EnemyRauderState
{
public:
	EnemyRauderMove( EnemyRauder* enemy )
	{
		SetEnemy( enemy );
	};

	void					Init( void ) override;			//  初期化
	void					Uninit( void ) override;		//  終了
	void					Update( void ) override;		//  更新
	void					Draw( void ) override;			//  描画

private:
	void					Action( void );					//  アクション
};

#endif
