//--------------------------------------------------------------------------------------
//  エネミーステート  ( enemyRauderState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_RAUDER_STATE_H_
#define _ENEMY_RAUDER_STATE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class EnemyRauder;

//--------------------------------------------------------------------------------------
//  エネミーステートクラスの定義
//--------------------------------------------------------------------------------------
class EnemyRauderState
{
public:
	enum class MOVE_TYPE
	{
		TARGET_CHASE = 0 ,
		TARGET_CHASE_CURVE ,
		ESCAPE ,
	};

	EnemyRauderState( ){ }												//  デフォルトコンストラクタ
	EnemyRauderState( EnemyRauder* enemy ) : m_enemy( enemy ){ }		//  コンストラクタ

	virtual void	Init( void );										//  初期化
	virtual void	Uninit( void ) = 0;									//  終了
	virtual void	Update( void ) = 0;									//  更新
	virtual void	Draw( void ) = 0;									//  描画

	/*  判断系の処理  */
	void			ChangeIdle( void );									//  待機状態に変更する
	void			ChangeMove( MOVE_TYPE moveType );					//  移動状態に変更する
	void			ChangeJump( void );									//  ジャンプ状態に変更する

protected:
	void			SetEnemy( EnemyRauder* player );					//  エネミークラスの設定
	EnemyRauder*	GetEnemy( void );									//  エネミークラスの取得

	/*----------------------------
		継承先で使える汎用関数
	----------------------------*/
	void			AddGravity( void );									//  重力をかける
	void			ChangeLockon( void );								//  ロックオン状態の変更
	void			LookTargetXZ( void );									//  ターゲット方向に向く
	D3DXVECTOR3		GetTargetPosition( void );							//  ターゲット座標の取得

	/*----------------------------
		継承先で使える汎用変数
	----------------------------*/
	float			m_moveHeight;										//  高さ移動量
	MOVE_TYPE		m_moveType;											//  移動タイプ

private:
	EnemyRauder*	m_enemy;											//  エネミークラスのポインタ
	int				m_pressTime;										//  押している時間
};

#endif

