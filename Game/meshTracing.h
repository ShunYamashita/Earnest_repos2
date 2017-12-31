//--------------------------------------------------------------------------------------
//  軌跡メッシュ ( meshTracing.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _MEESH_TRACING_H_
#define _MEESH_TRACING_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include <vector>

//--------------------------------------------------------------------------------------
//  メッシュフィールドクラスの定義
//--------------------------------------------------------------------------------------
class MeshTracing : public Scene
{
public:
	MeshTracing( );																	//  コンストラクタ					
	~MeshTracing( );																//  デストラクタ

	HRESULT						Init( void );										//  初期化
	void						Uninit( void );										//  終了
	void						Update( void );										//  更新
	void						Draw( void );										//  描画

	void						SetNewVertexPositionUp( D3DXVECTOR3 position );		//  新しい頂点座標のトップ座標の設定
	void						SetNewVertexPositionDown( D3DXVECTOR3 position );	//  新しい頂点座標のトップ座標の設定

	static MeshTracing*			Create( D3DXCOLOR color ,							//  生成 	
										D3DXVECTOR3 positionUp ,
										D3DXVECTOR3 positionDown );								

private:
	void						MakeVertex( void );									//  頂点の作成
	void						SetVertex( void );									//  頂点の設定

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;											//  頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIndexBuff;										//  インデックスバッファインターフェースへのポインタ

	D3DXVECTOR3					m_rot;

	std::vector< D3DXVECTOR3 >	m_vertex;											//  頂点座標

	D3DXVECTOR3					m_vertexPosition[ 10 ];								//  座標
	D3DXVECTOR3					m_newVertexPositionUp;								//  新しい頂点座標のトップ座標
	D3DXVECTOR3					m_newVertexPositionDown;							//  新しい頂点座標のダウン座標

	int							m_nDivideSide;										//  横分割数
	int							m_nDivideVertical;									//  縦分割数

	int							m_countFrame;										//  フレームカウント
};

#endif
