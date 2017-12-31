//--------------------------------------------------------------------------------------
//  レンダリング処理   ( scene2D.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _GOAL_SCENE_2D_H_
#define _GOAL_SCENE_2D_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class CStageSelect1 : public CScene2D
{
public:
	CStageSelect1( );														//  デフォルトコンストラクタ
	CStageSelect1( D3DXVECTOR3 pos ,
				   D3DXVECTOR3 size ,
				   D3DXVECTOR2 divideUV ,
				   D3DXVECTOR2 posUV ,
				   D3DXCOLOR color );										//  コンストラクタ
	~CStageSelect1( );														//  デストラクタ

	HRESULT						Init( void );								//  初期化
	void						Uninit( void );								//  終了
	void						Update( void );								//  更新
	void						Draw( void );								//  描画

	static CStageSelect1*		Create( D3DXVECTOR3 pos,					//  生成
										D3DXVECTOR3 size , 
										D3DXVECTOR2 posUV ,
										D3DXVECTOR2 divideUV );
};

#endif
