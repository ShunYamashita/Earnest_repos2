//--------------------------------------------------------------------------------------
//  エネミーステート  ( enemyState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_STATE_H_
#define _ENEMY_STATE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class Enemy;

//--------------------------------------------------------------------------------------
//  エネミーステートクラスの定義
//--------------------------------------------------------------------------------------
class EnemyState
{
public:
	EnemyState( ){ }														//  デフォルトコンストラクタ
	EnemyState( Enemy* enemy ) : m_enemy( enemy ){ }						//  コンストラクタ

	virtual void	Init( void );											//  初期化
	virtual void	Uninit( void ) = 0;										//  終了
	virtual void	Update( void ) = 0;										//  更新
	virtual void	Draw( void ) = 0;										//  描画

	/*  判断系の処理  */
	void			JudgeChangeNeutral( void );								//  ニュートラル状態に変更するかの判断
	void			JudgeChangeMove( const D3DXVECTOR3& vecDirect );		//  移動状態に変更するかの判断
	void			JudgeChangeJump( void );								//  ジャンプ状態に変更するかの判断
	void			JudgeChangeHPAttack000( void );							//  HP攻撃000に変更するかの判断
	void			JudgeChangeBraveAttack000( bool left = true );			//  ブレイブ攻撃000に変更するかの判断
	void			JudgeChangeEscape( const D3DXVECTOR3& vecDirect );		//  回避状態に変更するかの判断
	void			JudgeChangeGuard( void );								//  防御状態に変更するかの判断
	void			JudgeChangeLockonDash( const D3DXVECTOR3& vecDirect );	//  ロックオンダッシュ状態に変更するかの判断
	void			JudgeChangeFreeDash( const D3DXVECTOR3& vecDirect );	//  フリーダッシュ状態に変更するかの判断
	void			JudgeChangeRushAttack( void );							//  突進攻撃状態に変更するかの判断
	void			JudgeChangeMagicReady( void );							//  魔法準備状態に変更するかの判断
	void			JudgeChangeMagicActive( void );							//  魔法アクティブ状態に変更するかの判断

protected:
	void			SetEnemy( Enemy* enemy );								//  エネミークラスの設定
	Enemy*			GetEnemy( void );										//  エネミークラスの取得

	/*----------------------------
		継承先で使える汎用関数
	----------------------------*/
	void			AddGravity( void );										//  重力をかける
	void			ChangeLockon( void );									//  ロックオン状態の変更

	/*----------------------------
		継承先で使える汎用変数
	----------------------------*/
	float			m_moveHeight;										//  高さ移動量

private:
	Enemy*			m_enemy;											//  エネミークラスのポインタ
	int				m_pressTime;										//  押している時間
};

#endif

