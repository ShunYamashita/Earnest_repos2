//--------------------------------------------------------------------------------------
//  メッシュウォール  ( meshWall.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _MEESH_WALL_H_
#define _MEESH_WALL_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  メッシュウォールクラスの定義
//--------------------------------------------------------------------------------------
class MeshWall : public Scene
{
public:
	typedef enum
	{
		TYPE_WALL = 0 ,
	} TYPE;

	MeshWall( int nPriority );									//  コンストラクタ					
	~MeshWall( );												//  デストラクタ

	HRESULT					Init( void );						//  初期化
	void					Uninit( void );						//  終了
	void					Update( void );						//  更新
	void					Draw( void );						//  描画
	void					DrawDepth( void ){ }				//  デプス値の書き込み

	static MeshWall*		Create( TYPE type ,					//  生成
									D3DXVECTOR3 position ,			
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 size ,
									D3DXCOLOR color ,
									int nDivedeSide ,
									int nDivideVertical ,
									int nPriority = 4 );			

private:
	void					MakeVertex( void );					//  頂点の作成
	void					SetVertex( void );					//  頂点の設定

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							//  頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;						//  インデックスバッファインターフェースへのポインタ

	D3DXVECTOR3				m_normal;							//  法線
	D3DXVECTOR3				m_rot;								//  回転角

	TYPE					m_type;								//  種類

	int						m_nDivideSide;						//  横分割数
	int						m_nDivideVertical;					//  縦分割数
};

#endif

