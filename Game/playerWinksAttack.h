//--------------------------------------------------------------------------------------
//  プレイヤーステート( 攻撃 )  ( playerWinksAttack.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_ATTACK_H_
#define _PLAYER_WINKS_ATTACK_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( 攻撃 ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerWinksAttack : public PlayerWinksState
{
public:
	enum class MOTION_STATE
	{
		ATTACK01 = 0 ,
		ATTACK02 ,
		ATTACK03 ,
		MAX ,
	};

	PlayerWinksAttack( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;			//  初期化
	void					Uninit( void ) override;		//  終了
	void					Update( void ) override;		//  更新
	void					Draw( void ) override;			//  描画

private:
	void					Action( void );					//  アクション
	void					MoveTarget( void );				//  ターゲットに徐々に近づく
	void					JudgeNextMotion( void );		//  次のモーションに移行するかの判断

	MOTION_STATE			m_motionState;					//  モーション状態
	int						m_rigidTime;					//  硬直時間
	bool					m_effekseerFrame;				//  フレームを発生したかどうかのフラグ
	Effekseer::Handle		m_effekseerHandle[ 2 ];			//  エフェクシア識別子
};

#endif
