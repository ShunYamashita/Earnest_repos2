//--------------------------------------------------------------------------------------
//  プレイヤーステート( Brave攻撃000( 後 ) )  ( playerBraveAttack000After.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_BRAVE_ATTACK000_AFTER_H_
#define _PLAYER_BRAVE_ATTACK000_AFTER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( Brave攻撃000( 後 ) ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerBraveAttack000After : public PlayerState
{
public:
	PlayerBraveAttack000After( Player* player )
	{
		SetPlayer( player );
	};

	void								Init( void ) override;			//  初期化
	void								Uninit( void ) override;		//  終了
	void								Update( void ) override;		//  更新
	void								Draw( void ) override;			//  描画

	static PlayerBraveAttack000After*	Create( void );					//  生成

private:
	void								Action( void );					//  アクション

	float								m_speed;						//  速度
};

#endif
