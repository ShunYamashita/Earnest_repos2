//--------------------------------------------------------------------------------------
//  シーンクラス   ( scene.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_H_
#define _SCENE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include <list>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define		MAX_NUM_PRIORITY	( 8 )						//  優先順位の最大数
#define		MAX_MATERIAL		( 16 )						//  マテリアル最大数

//--------------------------------------------------------------------------------------
//  シーンクラスの定義
//--------------------------------------------------------------------------------------
class Scene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0 ,
		OBJTYPE_PLAYER ,
		OBJTYPE_ENEMY ,
		OBJTYPE_BULLET ,
		OBJTYPE_FIELD ,
		OBJTYPE_HIT_FIELD ,
		OBJTYPE_WALL ,
		OBJTYPE_BUILDING ,
		OBJTYPE_MAGIC_FIRE ,
		OBJTYPE_MAGIC_LIGHTNING ,
		OBJTYPE_MAGIC_TORNADE ,
		OBJTPE_MAX
	} OBJTYPE;

	Scene( int nPriority = 3 );													//  コンストラクタ
	virtual	~Scene( );															//  デストラクタ
	virtual HRESULT				Init( void ) = 0;								//  初期化
	virtual void				Uninit( void ) = 0;								//  終了
	virtual void				Update( void ) = 0;								//  更新
	virtual void				Draw( void ) = 0;								//  描画
	virtual void				DrawDepth( void ) = 0;;							//  デプス値書き込み

	static void					UpdateAll( void );								//  全ての更新
	static void					DrawAll( void );								//  全ての描画
	static void					DrawDepthAll( void );							//  全てのデプス値書き込み
	static void					ReleaseAll( void );								//  全ての解放

	static void					UpdateAllDelete( void );						//  全ての更新時の削除処理

	void						Release( void );								//  解放

	static Scene*				GetScene( int nIdxPriority , int nIdxScene );	//  シーンの情報を取得  
	void						SetObjType( OBJTYPE objType );					//  種類設定
	OBJTYPE						GetObjType( void );								//  種類情報の取得

	void						SetPosition( D3DXVECTOR3 position );			//  座標の設定
	void						MovePos( D3DXVECTOR3 velocity );				//  座標の移動
	D3DXVECTOR3					GetPos( void);									//  座標の取得

	void						SetSize( D3DXVECTOR3 size );					//  大きさの設定
	D3DXVECTOR3					GetSize( void);									//  大きさの取得

	void						SetColor( D3DXCOLOR color );					//  色の設定

	Scene*						GetNextScene( Scene* pScene );					//  シーン情報次のアドレスの取得
	static Scene*				GetScene( int nCntPriority );					//  シーン情報先頭アドレスの取得

protected:
	D3DXVECTOR3					m_position;										//  座標
	D3DXVECTOR3					m_size;											//  大きさ
	D3DXCOLOR					m_color;										//  色情報

	Scene*						m_pNext;										//  シーンの次のポインタ
	static Scene*				m_pTop[ MAX_PRIORITY ];							//  シーンの先頭ポインタ
	bool						m_bDelete;										//  削除フラグ
	bool						m_drawDepth;									//  深度バッファに書き込むかのフラグ
	int							m_nPriority;									//  優先順位

	static int					m_nNumScene;									//	オブジェクトの総数
	OBJTYPE						m_objType;										//  種類
};

#endif

