//--------------------------------------------------------------------------------------
//  選択画面準備   ( readySelect.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _READY_SELECT_H_
#define _READY_SELECT_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------------
//  選択画面準備クラスの定義
//--------------------------------------------------------------------------------------
class ReadySelect : public Scene2D
{
public:
	ReadySelect( );														//  デフォルトコンストラクタ
	~ReadySelect( );														//  デストラクタ

	HRESULT						Init( void );								//  初期化
	void						Uninit( void );								//  終了
	void						Update( void );								//  更新
	void						Draw( void );								//  描画

	static ReadySelect*		Create( D3DXVECTOR3 position ,					//  生成			
										D3DXVECTOR3 size , 
										D3DXVECTOR2 posUV ,
										D3DXVECTOR2 divideUV );

private:

};

#endif
