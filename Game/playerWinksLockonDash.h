//--------------------------------------------------------------------------------------
//  プレイヤーステート( ロックオンダッシュ )  ( playerWinksLockonDash.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_LOCKON_DASH_H_
#define _PLAYER_WINKS_LOCKON_DASH_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( ロックオンダッシュ ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerWinksLockonDash : public PlayerWinksState
{
public:
	enum class MOTION_STATE
	{
		START = 0 ,
		END ,
	};

	PlayerWinksLockonDash( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void						Init( void ) override;			//  初期化
	void						Uninit( void ) override;		//  終了
	void						Update( void ) override;		//  更新
	void						Draw( void ) override;			//  描画

private:
	void						Action( void );					//  アクション

	float						m_speed;						//  速度
	MOTION_STATE				m_motionState;					//  モーション状態
};

#endif
