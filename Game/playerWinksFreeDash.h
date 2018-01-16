//--------------------------------------------------------------------------------------
//  プレイヤーステート( フリーダッシュ )  ( playerWinksFreeDash.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_FREE_DASH_H_
#define _PLAYER_WINKS_FREE_DASH_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( フリーダッシュ ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerWinksFreeDash : public PlayerWinksState
{
public:
	enum class MOTION_STATE
	{
		START = 0 ,
		END ,
	};

	PlayerWinksFreeDash( PlayerWinks* player )
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
