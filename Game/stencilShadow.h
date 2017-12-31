//--------------------------------------------------------------------------------------
//  ステンシルシャドウ  ( sceneModel.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _STENCIL_SHADOW_H_
#define _STENCIL_SHADOW_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  ステンシルシャドウクラスの定義
//--------------------------------------------------------------------------------------
class StencilShadow : public Scene
{
public:
	enum class TYPE
	{
		SPHERE = 0,
		PILLAR ,
		THIEF ,
	};

	StencilShadow(  );													//  デフォルトコンストラクタ
	StencilShadow( D3DXVECTOR3 position ,								//  コンストラクタ
				   D3DXVECTOR3 rot ,
				   D3DXVECTOR3 scale );				
	~StencilShadow( );													//  デストラクタ

	HRESULT					Init( void );								//  初期化
	void					Uninit( void );								//  終了
	void					Update( void );								//  更新
	void					Draw( void );								//  描画
	void					DrawShadow( void );							//  影の描画
	void					DrawDepth( void ){ }						//  デプス値の書き込み

	static StencilShadow*	Create( TYPE type ,							//  生成
									D3DXVECTOR3 position ,					
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 scale );

	void					SetScale( D3DXVECTOR3 scale );				//  大きさの設定

protected:
	D3DXVECTOR3				GetScale( void);							//  大きさの取得

	D3DXVECTOR3				m_scale;									//  大きさの倍率
	D3DXVECTOR3				m_rot;										//  回転

	TYPE					m_type;										//  種類

	LPD3DXMESH				m_pMeshModel;								//  メッシュ情報

private:
	LPD3DXBUFFER			m_pBuffMatModel;							//  マテリアル情報
	DWORD					m_nNumMatModel;								//  マテリアル数

	char					m_aTextureName[ MAX_MATERIAL ][ 256 ];		//  テクスチャ名保存用

	LPDIRECT3DVERTEXBUFFER9	m_shadowVtxBuff;							//  頂点バッファへのポインタ
};

#endif