//--------------------------------------------------------------------------------------
//  プレイヤーステート( 移動 )  ( playerMove.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_MOVE_H_
#define _PLAYER_MOVE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( 移動 ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerMove : public PlayerState
{
public:
	PlayerMove( Player* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;			//  初期化
	void					Uninit( void ) override;		//  終了
	void					Update( void ) override;		//  更新
	void					Draw( void ) override;			//  描画

	static PlayerMove*		Create( void );					//  生成

private:
	/*--------------  
	    判断処理
	 -------------*/
	void					JudgeChangeNeutral( void );		//  ニュートラル状態に変更するかの判断

	void					Action( void );					//  アクション
};

#endif
