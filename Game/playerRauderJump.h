//--------------------------------------------------------------------------------------
//  プレイヤーステート( ジャンプ )  ( playerRauderJump.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RAUDER_JUMP_H_
#define _PLAYER_RAUDER_JUMP_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerRauderState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class PlayerRauder;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( ジャンプ ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerRauderJump : public PlayerRauderState
{
public:
	PlayerRauderJump( PlayerRauder* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;			//  初期化
	void					Uninit( void ) override;		//  終了
	void					Update( void ) override;		//  更新
	void					Draw( void ) override;			//  描画

private:
	void					Action( void );					//  アクション

	float					m_jumpPower;					//  ジャンプ力
	int						m_countJump;					//  ジャンプ数
};

#endif
