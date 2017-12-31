//--------------------------------------------------------------------------------------
//  フィールド  ( field.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _FIELD_H_
#define _FIELD_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModel.h"
#include "utility.h"
#include "shadow.h"
#include "life.h"

//--------------------------------------------------------------------------------------
//  フィールドクラスの定義
//--------------------------------------------------------------------------------------
class Field : public SceneModel
{
public:
	typedef enum
	{
		TYPE_STAGE000 = 0 ,
		TYPE_STAGE001 ,
		TYPE_MAX ,
	} TYPE;

	Field( );											//  コンストラクタ
	~Field( );											//  デストラクタ

	HRESULT				Init( void );					//  初期化
	void				Uninit( void );					//  終了
	void				Update( void );					//  更新
	void				Draw( void );					//  描画

	static Field*		Create(	Field::TYPE type ,		//  生成
								D3DXVECTOR3 position ,		
								D3DXVECTOR3 rot ,
								D3DXVECTOR3 scale );

private:
	TYPE				m_modelType;					//  種類
};

#endif

