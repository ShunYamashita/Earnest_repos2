////--------------------------------------------------------------------------------------
////  メッシュバウンディングボックス  ( meshBoundingBox.h )
////
////  Author : SHUN YAMASHITA
////--------------------------------------------------------------------------------------
//#ifndef _MEESH_BOUNDING_BOX_H_
//#define _MEESH_BOUNDING_BOX_H_
//
////--------------------------------------------------------------------------------------
////  ヘッダーファイル
////--------------------------------------------------------------------------------------
//#include "main.h"
//#include "scene.h"
//
////--------------------------------------------------------------------------------------
////  メッシュバウンディングボックスクラスの定義
////--------------------------------------------------------------------------------------
//class MeshBoundingBox : public Scene
//{
//public:
//	MeshBoundingBox( );											//  コンストラクタ					
//	~MeshBoundingBox( );										//  デストラクタ
//
//	HRESULT					Init( void );						//  初期化
//	void					Uninit( void );						//  終了
//	void					Update( void );						//  更新
//	void					Draw( void );						//  描画
//	void					DrawDepth( void );					//  デプス値の書き込み
//
//	static MeshBoundingBox*	Create( const D3DXVECTOR3& min ,
//									const D3DXVECTOR3& max );		
//
//private:
//	void					MakeVertex( void );					//  頂点の作成
//	void					SetVertex( void );					//  頂点の設定
//
//	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							//  頂点バッファへのポインタ
//	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;						//  インデックスバッファインターフェースへのポインタ
//	D3DXVECTOR3				m_corners[ 8 ];						//  角の座標
//};
//
//#endif
