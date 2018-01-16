//--------------------------------------------------------------------------------------
//  プレイヤーステート( 攻撃 )  ( playerRauderAttack.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RAUDER_ATTACK_H_
#define _PLAYER_RAUDER_ATTACK_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerRauderState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class PlayerRauder;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( 攻撃 ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerRauderAttack : public PlayerRauderState
{
public:
	PlayerRauderAttack( PlayerRauder* player )
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

	int						m_rigidTime;					//  硬直時間
	int						m_attackNo;						//  攻撃番号
};

#endif
