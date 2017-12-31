//--------------------------------------------------------------------------------------
//  パーティクル   ( particle.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene3D.h"

//--------------------------------------------------------------------------------------
//  パーティクルクラスの定義
//--------------------------------------------------------------------------------------
class Particle : public Scene3D
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0 ,
		TYPE_WIDE ,
		TYPE_MAX ,
	} TYPE;

	Particle( int nPriority );								//  コンストラクタ
	~Particle( ){ }										//  デストラクタ

	HRESULT				Init( void );
	void				Uninit( void );
	void				Update( void );
	void				Draw( void );

	static Particle*	Create( TYPE type ,					//  生成
								D3DXVECTOR3 position ,		
								D3DXVECTOR3 size ,
								D3DXCOLOR color ,
								float fScale ,
								float fVelocity ,
								float fAccel ,
								float fSeekRot ,
								float fRangeRot ,
								float fLife ,
								float fVecDirectX = 0.0f , 
								float fVecDirectY = 0.0f , 
								float fVecDirectZ = 0.0f );

private:
	TYPE				m_type;								//  種類
	D3DXVECTOR3			m_vecDirect;						//  方向	
	float				m_fVelocity;						//  ポリゴンの斜辺の長さ
	float				m_fAccel;							//  加速度
	float				m_fLife;							//  寿命
	float				m_fAddScale;
};

#endif