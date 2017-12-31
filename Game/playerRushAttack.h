//--------------------------------------------------------------------------------------
//  プレイヤーステート( 突進攻撃 )  ( playerRushAttack.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RUSH_ATTACK_H_
#define _PLAYER_RUSH_ATTACK_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerState.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  プレイヤーステート( 突進攻撃 ) クラスの定義
//--------------------------------------------------------------------------------------
class PlayerRushAttack : public PlayerState
{
public:
	PlayerRushAttack( Player* player )
	{
		SetPlayer( player );
	};

	void						Init( void ) override;			//  初期化
	void						Uninit( void ) override;		//  終了
	void						Update( void ) override;		//  更新
	void						Draw( void ) override;			//  描画

	static PlayerRushAttack*	Create( void );					//  生成

private:
	void						Action( void );					//  アクション
	void						ControlCameraExclusive( void );	//  専用のカメラ制御

	float						m_speed;
};

#endif
