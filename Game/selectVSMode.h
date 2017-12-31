//--------------------------------------------------------------------------------------
//  対戦モード選択   ( selectVSMode.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SELECT_VS_MODE_H_
#define _SELECT_VS_MODE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class SelectVSMode : public Scene2D
{
public:
	typedef enum
	{
		TYPE_SOLO = 0 ,
		TYPE_PARTY ,
		TYPE_TUTORIAL ,
		TYPE_MAX
	} TYPE;

	SelectVSMode( );														//  デフォルトコンストラクタ
	SelectVSMode( TYPE type ,
				   D3DXVECTOR3 position ,
				   D3DXVECTOR3 size ,
				   D3DXVECTOR2 divideUV ,
				   D3DXVECTOR2 posUV ,
				   D3DXCOLOR color );										//  コンストラクタ
	~SelectVSMode( );														//  デストラクタ

	HRESULT						Init( void );								//  初期化
	void						Uninit( void );								//  終了
	void						Update( void );								//  更新
	void						Draw( void );								//  描画

	static SelectVSMode*		Create( TYPE type ,							//  生成
										D3DXVECTOR3 position,					
										D3DXVECTOR3 size , 
										D3DXVECTOR2 posUV ,
										D3DXVECTOR2 divideUV );

private:
	TYPE						m_type;										//  種類
};

#endif
