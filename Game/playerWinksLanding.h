//--------------------------------------------------------------------------------------
//  プレイヤーステート( 着地 )  ( playerWinksLanding.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_LANDING_H_
#define _PLAYER_LANDING_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( 着地 ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerWinksLanding : public PlayerWinksState
{
public:
	enum class RIGID_TIME												//  硬直時間
	{
		SHORT = 0 ,														//  短い
		LONG ,															//  長い
	};

	PlayerWinksLanding( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;						//  初期化
	void					Uninit( void ) override;					//  終了
	void					Update( void ) override;					//  更新
	void					Draw( void ) override;						//  描画
																		
private:					
	void					Action( void );								//  アクション
																		
	RIGID_TIME				m_rigidTimeType;							//  硬直時間の種類
};

#endif
