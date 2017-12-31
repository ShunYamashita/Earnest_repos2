//--------------------------------------------------------------------------------------
//  弾   ( bullet.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene3D.h"
#include "utility.h"
#include "shadow.h"

//--------------------------------------------------------------------------------------
//  説明文クラスの定義
//--------------------------------------------------------------------------------------
class Bullet : public Scene3D
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0 ,
		TYPE_WIDE ,
		TYPE_MAX
	} TYPE;

	typedef enum
	{
		OWNER_NONE = 0 ,
		OWNER_PLAYER ,
		OWNER_ENEMY ,
	} OWNER;

	Bullet( int nPriority );									//  コンストラクタ
	~Bullet( ){ }												//  デストラクタ

	HRESULT					Init( void );
	void					Uninit( void );
	void					Update( void );
	void					Draw( void );

	static Bullet*			Create( TYPE type ,
									OWNER owner ,
									D3DXVECTOR3 position ,			
									D3DXVECTOR3 size ,
									D3DXVECTOR3 normal ,
									D3DXVECTOR3 rot ,
									D3DXCOLOR color ,
									D3DXVECTOR3 vecDirect ,
									float fSpeed ,
									int nLife = 180 );			//  生成

	Utility::HIT_SPHERE		GetHitSphere( void );				//  球体の当たり判定の取得
	TYPE					GetType( void );					//  種類の取得
	OWNER					GetOwner( void );					//  所有者の確認
	void					Hit( D3DXVECTOR3 vecDirect );		//  当たった時の処理

private:
	TYPE					m_type;								//  種類
	OWNER					m_owner;							//  所有者
	Shadow*					m_pShadow;							//  影のクラスのポインタ
	D3DXVECTOR3				m_vecDirect;						//  進行方向
	float					m_fSpeed;							//  速度
	bool					m_bHoming;							//  追尾するかどうか
	int						m_nLife;							//  体力
	Utility::HIT_SPHERE		m_hitSphere;						//  当たり判定
};

#endif