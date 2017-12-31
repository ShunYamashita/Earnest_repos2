//--------------------------------------------------------------------------------------
//  影   ( shadow.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADOW_H_
#define _SHADOW_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene3D.h"

//--------------------------------------------------------------------------------------
//  影クラスの定義
//--------------------------------------------------------------------------------------
class Shadow : public Scene3D
{
public:
	Shadow( int nPriority );								//  コンストラクタ
	~Shadow( ){ }											//  デストラクタ

	HRESULT				Init( void );
	void				Uninit( void );
	void				Update( void );
	void				Draw( void );

	static Shadow*		Create( D3DXVECTOR3 position ,			//  生成
								D3DXVECTOR3 size ,
								D3DXVECTOR3 normal ,
								D3DXVECTOR3 rot ,
								D3DXCOLOR color );							

private:
};

#endif