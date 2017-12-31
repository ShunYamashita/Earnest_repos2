//--------------------------------------------------------------------------------------
//  体力ゲージ   ( life.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _TITLE_PRESS_H_
#define _TITLE_PRESS_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"
#include "number.h"

//--------------------------------------------------------------------------------------
//  体力クラスの定義
//--------------------------------------------------------------------------------------
class TitlePress : public Scene2D
{
public:
	TitlePress( );
	TitlePress( D3DXVECTOR3 position , D3DXVECTOR3 size ,
		   D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV ,
		   D3DXCOLOR color );								//  コンストラクタ
	~TitlePress( );												//  デストラクタ

	HRESULT					Init( void );					//  初期化
	void					Uninit( void );					//  終了
	void					Update( void );					//  更新
	void					Draw( void );					//  描画

	static TitlePress*		Create( D3DXVECTOR3 position ,					
									D3DXVECTOR3 size , 
									D3DXVECTOR2 posUV ,
									D3DXVECTOR2 divideUV );

private:
	const float				CHANGE_SCALL = 0.001f;

	int						m_nFase;
};

#endif
