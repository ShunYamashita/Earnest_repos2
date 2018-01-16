//--------------------------------------------------------------------------------------
//  エネミーステート( Brave攻撃000( 後 ) )  ( enemyBraveAttack000After.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_BRAVE_ATTACK000_AFTER_H_
#define _ENEMY_BRAVE_ATTACK000_AFTER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Enemy;

//--------------------------------------------------------------------------------------
//  エネミーステート( Brave攻撃000( 後 ) ) クラスの定義
//--------------------------------------------------------------------------------------
class EnemyBraveAttack000After : public EnemyState
{
public:
	EnemyBraveAttack000After( Enemy* enemy )
	{
		SetEnemy( enemy );
	};

	void								Init( void ) override;			//  初期化
	void								Uninit( void ) override;		//  終了
	void								Update( void ) override;		//  更新
	void								Draw( void ) override;			//  描画

	static EnemyBraveAttack000After*	Create( void );					//  生成

private:
	void								Action( void );					//  アクション

	float								m_speed;						//  速度
};

#endif
