//--------------------------------------------------------------------------------------
//  数字   ( number.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _NUMBER_H_
#define _NUMBER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2DL.h"

//--------------------------------------------------------------------------------------
//  数字クラスの定義
//--------------------------------------------------------------------------------------
class Number : public Scene2DL
{
public:
	Number( );
	Number( D3DXVECTOR3 position , D3DXVECTOR3 size ,
		     D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV ,
		     D3DXCOLOR color );								//  コンストラクタ
	~Number( );											//  デストラクタ

	HRESULT				Init( void );						//  初期化
	void				Uninit( void );						//  終了
	void				Update( void );						//  更新
	void				Draw( void );						//  描画

	static Number*		Create( D3DXVECTOR3 position ,					
								D3DXVECTOR3 size , 
								D3DXVECTOR2 posUV ,
								D3DXVECTOR2 divideUV ,
								int nNumber );

	void				SetNumber( int nNumber );				//  ブレイブの設定

private:
	int					m_nNumber;							//  ブレイブ
};

#endif
