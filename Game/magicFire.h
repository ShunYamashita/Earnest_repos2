//--------------------------------------------------------------------------------------
//  炎魔法  ( magicFire.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _MAGIC_FIRE_H_
#define _MAGIC_FIRE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "magic.h"
#include "utility.h"
#include "effekseerManager.h"

//--------------------------------------------------------------------------------------
//  炎魔法クラスの定義
//--------------------------------------------------------------------------------------
class MagicFire : public Magic
{
public:
	MagicFire( ){ }											//  コンストラクタ
	~MagicFire( ){ }										//  デストラクタ

	HRESULT					Init( void );					//  初期化
	void					Uninit( void );					//  終了
	void					Update( void );					//  更新
	void					Draw( void );					//  描画
	void					DrawDepth( void ){ }			//  デプス値の書き込み

	Utility::HIT_SPHERE		GetHitSphere( void );			//  球体の当たり判定の取得
	OWNER					GetOwner( void );				//  所有者の確認
	void					Hit( void );					//  当たった時の処理
	void					SetActive( void );				//  アクティブ状態に
	void					Delete( void );					//  削除

	static MagicFire*		Create(	OWNER owner ,
									D3DXVECTOR3 position ,		
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 scale ,
									D3DXVECTOR3 vecDirect ,
									int nPlayerNo = 0 ,
									float fSpeed = 1.5f ,
									int nLife = 180 );

private:
	D3DXVECTOR3				m_firstPos;						//  初期座標
	D3DXVECTOR3				m_beforePos;					//  前回座標
	D3DXVECTOR3				m_rot;							//  回転
	D3DXVECTOR3				m_vecDirect;					//  進行方向
	float					m_fSpeed;						//  速度
	float					m_fScale;						//  大きさ
	float					m_fAngle;						//  角度
	float					m_fDisance;						//  距離
	bool					m_bHoming;						//  追尾するかどうか
	int						m_nLife;						//  体力
	int						m_nPlayerNo;					//  プレイヤー番号
	Utility::HIT_SPHERE		m_hitSphere;					//  当たり判定
	::Effekseer::Handle		m_handle;						//  エフェクシアハンドル
};

#endif

