//--------------------------------------------------------------------------------------
//  雷魔法  ( magicLightning.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _MAGIC_LIGHTNING_H_
#define _MAGIC_LIGHTNING_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "magic.h"

//--------------------------------------------------------------------------------------
//  雷魔法クラスの定義
//--------------------------------------------------------------------------------------
class MagicLightning : public Magic
{
public:
	MagicLightning( ){ }												//  コンストラクタ
	~MagicLightning( ){ }												//  デストラクタ

	HRESULT					Init( void );								//  初期化
	void					Uninit( void );								//  終了
	void					Update( void );								//  更新
	void					Draw( void );								//  描画
	void					DrawDepth( void ){ }						//  デプス値の書き込み

	OWNER					GetOwner( void );							//  所有者の確認
	void					Hit( void );								//  当たった時の処理
	void					SetActive( void );							//  アクティブ状態に
	void					Delete( void );								//  削除

	void					MoveMagicPos( D3DXVECTOR3 vecDirect ,		//  魔法陣の移動処理
										  float fSpeed = 4.0f );

	static MagicLightning*	Create(	OWNER owner,						//  生成
									D3DXVECTOR3 position ,		
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 scale ,
									D3DXVECTOR3 vecDirect ,
									int nPlayerNo = 0 );
private:
	D3DXVECTOR3				m_magicPos;									//  魔法発動座標
	D3DXVECTOR3				m_firstPos;									//  初期座標
	D3DXVECTOR3				m_beforePos;								//  前回座標
	D3DXVECTOR3				m_rot;										//  回転
	D3DXVECTOR3				m_vecDirect;								//  進行方向
	float					m_fScale;									//  大きさ
	float					m_fAngle;									//  角度
	float					m_fDisance;									//  距離
	int						m_nHitTime;									//  当たり判定時間のカウント
	int						m_nPlayerNo;								//  プレイヤー番号
	::Effekseer::Handle		m_handle;									//  エフェクシアハンドル
};

#endif

