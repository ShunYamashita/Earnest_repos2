//--------------------------------------------------------------------------------------
//  エネミーステート( Brave攻撃000( 後 ) )  ( enemyBraveAttack000Before.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_BRAVE_ATTACK000_BEFORE_H_
#define _ENEMY_BRAVE_ATTACK000_BEFORE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyState.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Enemy;

//--------------------------------------------------------------------------------------
//  エネミーステート( Brave攻撃000( 前 ) ) クラスの定義
//--------------------------------------------------------------------------------------
class EnemyBraveAttack000Before : public EnemyState
{
public:
	EnemyBraveAttack000Before( Enemy* enemy )
	{
		SetEnemy( enemy );
	};

	void								Init( void ) override;			//  初期化
	void								Uninit( void ) override;		//  終了
	void								Update( void ) override;		//  更新
	void								Draw( void ) override;			//  描画

	static EnemyBraveAttack000Before*	Create( void );					//  生成

private:
	void								Action( void );					//  アクション

	D3DXVECTOR3							m_beforePosition;				//  前回の座標
	float								m_homingTime;					//  追尾時間
	float								m_speed;						//  速度
};

#endif
