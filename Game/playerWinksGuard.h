//--------------------------------------------------------------------------------------
//  プレイヤーステート( 防御 )  ( playerWinksGuard.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_GUARD_H_
#define _PLAYER_WINKS_GUARD_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( 防御 ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerWinksGuard : public PlayerWinksState
{
public:
	enum class MOTION_STATE
	{
		START = 0,
		GUARD ,
		END ,
	};

	PlayerWinksGuard( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;			//  初期化
	void					Uninit( void ) override;		//  終了
	void					Update( void ) override;		//  更新
	void					Draw( void ) override;			//  描画

private:
	void					Action( void );					//  アクション

	::Effekseer::Handle		m_effekseerHandle;				//  エフェクシア識別子
	MOTION_STATE			m_motionState;					//  モーション状態
	bool					m_ground;						//  地上にいるかどうか
	int						m_guardTime;					//  ガード時間
};

#endif
