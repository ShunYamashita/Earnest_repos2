//--------------------------------------------------------------------------------------
//  プレイヤーステート  ( playerRauderState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RAUDER_STATE_H_
#define _PLAYER_RAUDER_STATE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class PlayerRauder;

//--------------------------------------------------------------------------------------
//  プレイヤーステートクラスの定義
//--------------------------------------------------------------------------------------
class PlayerRauderState
{
public:
	PlayerRauderState( ){ }												//  デフォルトコンストラクタ
	PlayerRauderState( PlayerRauder* player ) : m_player( player ){ }		//  コンストラクタ

	virtual void	Init( void );										//  初期化
	virtual void	Uninit( void ) = 0;									//  終了
	virtual void	Update( void ) = 0;									//  更新
	virtual void	Draw( void ) = 0;									//  描画

protected:
	void			SetPlayer( PlayerRauder* player );						//  プレイヤークラスの設定
	PlayerRauder*	GetPlayer( void );									//  プレイヤークラスの取得

	/*----------------------------
		継承先で使える汎用関数
	----------------------------*/
	void			AddGravity( void );									//  重力をかける
	void			ControlCamera( void );								//  カメラの制御
	void			ChangeLockon( void );								//  ロックオン状態の変更

	/*  判断系の処理  */
	void			JudgeChangeIdle( void );							//  待機状態に変更するかの判断
	void			JudgeChangeMove( void );							//  移動状態に変更するかの判断
	void			JudgeChangeJump( void );							//  ジャンプ状態に変更するかの判断
	void			JudgeChangeAttack( void );							//  攻撃に変更するかの判断

	/*----------------------------
		継承先で使える汎用変数
	----------------------------*/
	float			m_moveHeight;										//  高さ移動量

private:
	PlayerRauder*	m_player;											//  プレイヤークラスのポインタ
	int				m_pressTime;										//  押している時間
};

#endif

