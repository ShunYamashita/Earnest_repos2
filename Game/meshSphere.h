//--------------------------------------------------------------------------------------
//  メッシュスフィア  ( meshSphere.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _MESH_SPHERE_H_
#define _MESH_SPHERE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  メッシュスフィアクラスの定義
//--------------------------------------------------------------------------------------
class MeshSphere : public Scene
{
public:
	MeshSphere( );												//  コンストラクタ					
	~MeshSphere( );												//  デストラクタ

	HRESULT					Init( void );						//  初期化
	void					Uninit( void );						//  終了
	void					Update( void );						//  更新
	void					Draw( void );						//  描画
	void					DrawDepth( void ){ }				//  デプス値の書き込み

	static MeshSphere*		Create( D3DXVECTOR3 position ,		//  生成
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 size ,
									D3DXVECTOR2 scroll ,
									int nDivedeSide ,
									int nDivideVertical );			

private:
	void					MakeVertex( void );					//  頂点の作成
	void					SetTexture( void );					//  テクスチャの設定

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							//  頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;						//  インデックスバッファインターフェースへのポインタ

	D3DXVECTOR3				m_size;								//  大きさ
	D3DXVECTOR3				m_rot;								//  回転角
	D3DXVECTOR2				m_scroll;							//  スクロール

	float					m_fDistance;						//  距離
	int						m_nDivideSide;						//  横分割数
	int						m_nDivideVertical;					//  縦分割数
	int						m_nNumVertex;						//  頂点数
	int						m_nNumPolygon;						//  ポリゴン数
};

#endif
#pragma once
