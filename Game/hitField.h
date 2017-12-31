//--------------------------------------------------------------------------------------
//  当たり判定用フィールド  ( hitField.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _HIT_FIELD_H_
#define _HIT_FIELD_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModel.h"
#include "utility.h"
#include "shadow.h"
#include "life.h"
#include <vector>

static const int MAX_INDEX = 10000;
static const int MAX_VTX = 10000;

//--------------------------------------------------------------------------------------
//  当たり判定用フィールドクラスの定義
//--------------------------------------------------------------------------------------
class HitField : public SceneModel
{
public:
	typedef enum
	{
		TYPE_HIT_STAGE000 = 0 ,
		TYPE_HIT_STAGE001 ,
		TYPE_MAX ,
	} TYPE;

	HitField( );													//  コンストラクタ
	~HitField( );													//  デストラクタ

	HRESULT						Init( void );						//  初期化
	void						Uninit( void );						//  終了
	void						Update( void );						//  更新
	void						Draw( void );						//  描画

	static HitField*			Create( HitField::TYPE type ,		//  生成
										D3DXVECTOR3 position ,			
										D3DXVECTOR3 rot ,
										D3DXVECTOR3 scale );

	float						GetHeight( D3DXVECTOR3 position );		//  座標の取得

private:
	DWORD						m_vtxIndex[ MAX_INDEX ];			//  インデックス番号			
	D3DXVECTOR3					m_vtxPos[ MAX_VTX ];				//  座標
};

#endif