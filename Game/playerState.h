//--------------------------------------------------------------------------------------
//  プレイヤーステート  ( playerState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  プレイヤーステートクラスの定義
//--------------------------------------------------------------------------------------
class PlayerState
{
public:
	PlayerState( ){ }											//  デフォルトコンストラクタ
	PlayerState( Player* player ) : m_player( player ){ }		//  コンストラクタ

	virtual void	Init( void );								//  初期化
	virtual void	Uninit( void ) = 0;							//  終了
	virtual void	Update( void ) = 0;							//  更新
	virtual void	Draw( void ) = 0;							//  描画

protected:
	void			SetPlayer( Player* player );				//  プレイヤークラスの設定
	Player*			GetPlayer( void );							//  プレイヤークラスの取得

	/*----------------------------
		継承先で使える汎用関数
	----------------------------*/
	void			AddGravity( void );							//  重力をかける
	void			ControlCamera( void );						//  カメラの制御
	void			ChangeLockon( void );						//  ロックオン状態の変更

	/*  判断系の処理  */
	void			JudgeChangeNeutral( void );					//  ニュートラル状態に変更するかの判断
	void			JudgeChangeMove( void );					//  移動状態に変更するかの判断
	void			JudgeChangeJump( void );					//  ジャンプ状態に変更するかの判断
	void			JudgeChangeHPAttack000( void );				//  HP攻撃000に変更するかの判断
	void			JudgeChangeBraveAttack000( void );			//  ブレイブ攻撃000に変更するかの判断
	void			JudgeChangeEscape( void );					//  回避状態に変更するかの判断
	void			JudgeChangeGuard( void );					//  防御状態に変更するかの判断
	void			JudgeChangeLockonDash( void );				//  ロックオンダッシュ状態に変更するかの判断
	void			JudgeChangeFreeDash( void );				//  フリーダッシュ状態に変更するかの判断
	void			JudgeChangeRushAttack( void );				//  突進攻撃状態に変更するかの判断
	void			JudgeChangeMagicReady( void );				//  魔法準備状態に変更するかの判断

	/*----------------------------
		継承先で使える汎用変数
	----------------------------*/
	float			m_moveHeight;								//  高さ移動量

private:
	Player*			m_player;									//  プレイヤークラスのポインタ
	int				m_pressTime;								//  押している時間
};

#endif

