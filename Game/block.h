//--------------------------------------------------------------------------------------
//  ブロック  ( block.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _BLOCK_H_
#define _BLOCK_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModel.h"
#include "utility.h"
#include "shadow.h"
#include "life.h"

//--------------------------------------------------------------------------------------
//  ブロッククラスの定義
//--------------------------------------------------------------------------------------
class Block : public SceneModel
{
public:
	Block( );											//  コンストラクタ
	~Block( );											//  デストラクタ

	HRESULT				Init( void );					//  初期化
	void				Uninit( void );					//  終了
	void				Update( void );					//  更新
	void				Draw( void );					//  描画

	static Block*		Create(	D3DXVECTOR3 position ,	//  生成	
								D3DXVECTOR3 rot ,
								D3DXVECTOR3 scale );

private:
};

#endif

