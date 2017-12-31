//--------------------------------------------------------------------------------------
//  魔法クリスタル   ( crystalMagic.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _CRYSTAL_MAGIC_H_
#define _CRYSTAL_MAGIC_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------------
//  魔法クリスタルクラスの定義
//--------------------------------------------------------------------------------------
class CrystalMagic : public Scene2D
{
public:
	typedef enum
	{
		TYPE_FIRE = 0 ,
		TYPE_TORNADE ,
		TYPE_LIGHTNING ,
		TYPE_MAX
	} TYPE;

	CrystalMagic( );														//  デフォルトコンストラクタ
	CrystalMagic( TYPE type ,
				   D3DXVECTOR3 position ,
				   D3DXVECTOR3 size ,
				   D3DXVECTOR2 divideUV ,
				   D3DXVECTOR2 posUV ,
				   D3DXCOLOR color );										//  コンストラクタ
	~CrystalMagic( );														//  デストラクタ

	HRESULT						Init( void );								//  初期化
	void						Uninit( void );								//  終了
	void						Update( void );								//  更新
	void						Draw( void );								//  描画

	static CrystalMagic*		Create( TYPE type ,							//  生成
										D3DXVECTOR3 position,					
										D3DXVECTOR3 size , 
										D3DXVECTOR2 posUV ,
										D3DXVECTOR2 divideUV );

private:
	TYPE						m_type;										//  種類
};

#endif
