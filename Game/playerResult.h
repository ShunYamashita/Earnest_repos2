//--------------------------------------------------------------------------------------
//  プレイヤー  ( player.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RESULT_H_
#define _PLAYER_RESULT_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModelAnim.h"
#include "utility.h"
#include "shadow.h"
#include "life.h"
#include "brave.h"
#include "stateAnimator.h"
#include "effekseerManager.h"
#include "magic.h"
#include "playerUI.h"

//--------------------------------------------------------------------------------------
//  プレイヤークラスの定義
//--------------------------------------------------------------------------------------
class PlayerResult : public SceneModelAnim
{
public:
	PlayerResult( int nPriority = 3 );								//  コンストラクタ
	~PlayerResult( );												//  デストラクタ

	HRESULT					Init( void );							//  初期化
	void					Uninit( void );							//  終了
	void					Update( void );							//  更新
	void					Draw( void );							//  描画

	void					Win( void );							//  勝利
	void					Lose( void );							//  敗北

	static PlayerResult*	Create( D3DXVECTOR3 position ,				//  生成
									D3DXVECTOR3 posAt ,				
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 scale ,
									float fSpeed ,
									int nPriority = 3 );

private:
	D3DXVECTOR3				m_firstPos;								//  最初の座標
	D3DXVECTOR3				m_beforePos;							//  前回の座標
	float					m_fDistance;							//  距離
	D3DXVECTOR3				m_vecDirect;							//  進行方向
	float					m_fSpeed;								//  速度

	StateAnimator*			m_pStateAnimator;						//  ステートマシン( アニメーター )クラスのポインタ
};

#endif

