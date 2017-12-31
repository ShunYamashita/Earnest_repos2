//--------------------------------------------------------------------------------------
//  エネミー  ( enemyr.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModelAnim.h"
#include "shadow.h"
#include "life.h"
#include "brave.h"

//--------------------------------------------------------------------------------------
//  エネミークラスの定義
//--------------------------------------------------------------------------------------
class Enemy : public SceneModelAnim
{
public:
	typedef enum
	{
		STATE_NEUTRAL = 0 ,									//  ニュートラル
		STATE_MOVE ,										//  移動
		STATE_FOLLOW ,										//  追従
		STATE_ATTACK_NORMAL ,								//  通常攻撃
		STATE_ATTACK_FIRE ,									//  炎の攻撃
		STATE_JUMP ,										//  ジャンプ
		STATE_MAX											//  最大数
	} STATE;												//  状態

	Enemy( );												//  コンストラクタ
	~Enemy( );												//  デストラクタ

	HRESULT				Init( void );						//  初期化
	void				Uninit( void );						//  終了
	void				Update( void );						//  更新
	void				Draw( void );						//  描画

	static Enemy*		Create( D3DXVECTOR3 position ,			//  生成
								D3DXVECTOR3 rot ,
								D3DXVECTOR3 scale ,
								float fSpeed );

	void				Damage( int nDamage = 250 );		//  ダメージ処理
	int					BraveDamage( int nDamage = 100 );	//  ブレイブダメージ処理

private:
	D3DXVECTOR3			m_firstPos;							//  最初の座標
	D3DXVECTOR3			m_vecDirect;						//  進行方向
	float				m_fSpeed;							//  速度
	float				m_fCurrentJumpPower;				//  現在のジャンプ力
	int					m_nWaitTime;						//  待ち時間
	int					m_nCntJump;							//  ジャンプ回数カウント用
	int					m_nThinkTime;						//  思考時間
	int					m_nInvisibleTime;					//  無敵時間
	int					m_nLife;							//  体力
	int					m_nBrave;							//  ブレイブ

	Shadow*			m_pShadow;							//  影のクラスのポインタ
	Life*				m_pLife;							//  体力クラスのポインタ
	Brave*				m_pBrave;							//  ブレイブクラスのポインタ
	StateAnimator*		m_pStateAnimator;					//  ステートマシン( アニメーター )クラスのポインタ

	STATE				m_state;							//  状態

	const int			BASE_LIFE = 3000;						//  体力初期値
	const int			BASE_BRAVE = 1000;					//  ブレイブ初期値
};

#endif

