//--------------------------------------------------------------------------------------
//  メッシュウォール  ( meshCube.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _MEESH_CUBE_H_
#define _MEESH_CUBE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class MeshField;
class MeshWall;

//--------------------------------------------------------------------------------------
//  メッシュウォールクラスの定義
//--------------------------------------------------------------------------------------
class MeshCube : public Scene
{
public:
	typedef enum
	{
		TYPE_SIDE = 0 ,
		TYPE_VERTICAL ,
		TYPE_FIXED ,
		TYPE_HAVING
	} TYPE;

	MeshCube( );												//  コンストラクタ					
	~MeshCube( );												//  デストラクタ

	HRESULT					Init( void );						//  初期化
	void					Uninit( void );						//  終了
	void					Update( void );						//  更新
	void					Draw( void );						//  描画
	void					DrawDepth( void ){ }				//  デプス値の書き込み

	static MeshCube*		Create( TYPE type ,					//  生成
									D3DXVECTOR3 position ,	
									D3DXCOLOR color );					

private:
	MeshField*				m_pMeshField;						//  メッシュフィールド格納用
	MeshWall*				m_pMeshWall[ 4 ];					//  メッシュウォール格納用

	TYPE					m_type;								//  種類
};

#endif


