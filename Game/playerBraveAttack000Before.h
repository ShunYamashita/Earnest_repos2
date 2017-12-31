//--------------------------------------------------------------------------------------
//  プレイヤーステート( Brave攻撃000( 後 ) )  ( playerBraveAttack000Before.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_BRAVE_ATTACK000_BEFORE_H_
#define _PLAYER_BRAVE_ATTACK000_BEFORE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerState.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( Brave攻撃000( 前 ) ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerBraveAttack000Before : public PlayerState
{
public:
	PlayerBraveAttack000Before( Player* player )
	{
		SetPlayer( player );
	};

	void								Init( void ) override;			//  初期化
	void								Uninit( void ) override;		//  終了
	void								Update( void ) override;		//  更新
	void								Draw( void ) override;			//  描画

	static PlayerBraveAttack000Before*	Create( void );					//  生成

private:
	void								Action( void );					//  アクション

	D3DXVECTOR3							m_beforePosition;				//  前回の座標
	float								m_homingTime;					//  追尾時間
	float								m_speed;						//  速度
};

#endif
