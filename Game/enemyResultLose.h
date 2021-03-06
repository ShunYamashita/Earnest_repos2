//--------------------------------------------------------------------------------------
//  エネミーステート( リザルト時の敗北 )  ( enemyResultLose.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_RESULT_LOSE_H_
#define _ENEMY_RESULT_LOSE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Enemy;

//--------------------------------------------------------------------------------------
//  エネミーステート( リザルト時の敗北 ) クラスの定義
//--------------------------------------------------------------------------------------
class EnemyResultLose : public EnemyState
{
public:
	EnemyResultLose( Enemy* enemy )
	{
		SetEnemy( enemy );
	};

	void						Init( void ) override;			//  初期化
	void						Uninit( void ) override;		//  終了
	void						Update( void ) override;		//  更新
	void						Draw( void ) override;			//  描画

	static EnemyResultLose*		Create( void );					//  生成

private:
};

#endif
