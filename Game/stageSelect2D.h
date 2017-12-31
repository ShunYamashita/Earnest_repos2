//--------------------------------------------------------------------------------------
//  レンダリング処理   ( stageSelect2D.h )
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
class StageSelect2D : public Scene2D
{
public:
	typedef enum
	{
		TYPE_STAGE1 = 0 ,
		TYPE_STAGE2 ,
		TYPE_STAGE3 ,
		TYPE_MAX
	} TYPE;

	StageSelect2D( );														//  デフォルトコンストラクタ
	StageSelect2D( TYPE type ,
					D3DXVECTOR3 position ,
				    D3DXVECTOR3 size ,
				    D3DXVECTOR2 divideUV ,
				    D3DXVECTOR2 posUV ,
				    D3DXCOLOR color );										//  コンストラクタ
	~StageSelect2D( );														//  デストラクタ

	HRESULT						Init( void );								//  初期化
	void						Uninit( void );								//  終了
	void						Update( void );								//  更新
	void						Draw( void );								//  描画

	static StageSelect2D*		Create( TYPE type ,							//  生成
										D3DXVECTOR3 position,					
										D3DXVECTOR3 size , 
										D3DXVECTOR2 posUV ,
										D3DXVECTOR2 divideUV );

private:
	TYPE						m_type;										//  種類
};

#endif
