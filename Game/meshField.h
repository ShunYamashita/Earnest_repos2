//--------------------------------------------------------------------------------------
//  メッシュフィールド  ( meshField.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _MEESH_FIELD_H_
#define _MEESH_FIELD_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  メッシュフィールドクラスの定義
//--------------------------------------------------------------------------------------
class MeshField : public Scene
{
public:
	typedef enum
	{
		TYPE_FIELD = 0 ,
		TYPE_SEA ,
		TYPE_VERTICAL ,
		TYPE_GOAL ,
		TYPE_FIXED ,
		TYPE_HAVING
	} TYPE;

	MeshField( );												//  コンストラクタ					
	~MeshField( );												//  デストラクタ

	HRESULT					Init( void );						//  初期化
	void					Uninit( void );						//  終了
	void					Update( void );						//  更新
	void					Draw( void );						//  描画
	void					DrawDepth( void );					//  デプス値の書き込み

	static MeshField*		Create( TYPE type ,					//  生成
									D3DXVECTOR3 position ,			
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 size ,
									D3DXCOLOR color ,
									int nDivedeSide ,
									int nDivideVertical ,
									D3DXVECTOR2 changeScroll );		

	float					GetHeight( D3DXVECTOR3 position );		//  対象の座標がフィールドのどこにいる検索してそのY座標を返す

	int						GetFocusSide( void );				//  横フォーカス番号の取得
	int						GetFocusVertical( void );			//  縦フォーカス番号の取得
	float					GetFocusHeight( int nVertical ,		//  フォーカス番号の高さ取得
											int nSide );				

	void					ChangeHeight( void );				//  高さの変化

	void					SaveField( void );

private:
	void					MakeVertex( void );					//  頂点の作成
	void					SetVertex( void );					//  頂点の設定

	void					SaveFile( void );					//  ファイルの保存

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							//  頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;						//  インデックスバッファインターフェースへのポインタ

	D3DXVECTOR3				m_fieldPos[ 21 ][ 21 ];
	D3DXVECTOR3				m_normal;							//  法線
	D3DXVECTOR3				m_rot;								//  回転角
	D3DXVECTOR2				m_scroll;							//  スクロール
	D3DXVECTOR2				m_changeScroll;						//  スクロール

	TYPE					m_type;								//  種類

	int						m_nDivideSide;						//  横分割数
	int						m_nDivideVertical;					//  縦分割数

	int						m_nFocusSide;						//  フォーカスがあっている横配列番号
	int						m_nFocusVertical;					//  フォーカスがあっている縦配列番号
};

#endif
