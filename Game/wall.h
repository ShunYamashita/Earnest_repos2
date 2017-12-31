//--------------------------------------------------------------------------------------
//  壁  ( wall.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModel.h"
#include "utility.h"
#include "shadow.h"
#include "life.h"

//--------------------------------------------------------------------------------------
//  壁クラスの定義
//--------------------------------------------------------------------------------------
class Wall : public SceneModel
{
public:
	Wall( );											//  コンストラクタ
	~Wall( );											//  デストラクタ

	HRESULT				Init( void );					//  初期化
	void				Uninit( void );					//  終了
	void				Update( void );					//  更新
	void				Draw( void );					//  描画

	static Wall*		Create(	D3DXVECTOR3 position ,	//  生成	
								D3DXVECTOR3 rot ,
								D3DXVECTOR3 scale );

private:

};

#endif

#pragma once
