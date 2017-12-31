//--------------------------------------------------------------------------------------
//  3Dレンダリング処理   ( scene3D.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_3D_H_
#define _SCENE_3D_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class Scene3D : public Scene
{
public:
	Scene3D( int nPriority = 4 );									//  デフォルトコンストラクタ
	Scene3D( D3DXVECTOR3 position ,										//  コンストラクタ
			  D3DXVECTOR3 normal ,
			  D3DXVECTOR3 size ,
			  D3DXVECTOR3 rot ,
			  D3DXCOLOR color );									
	~Scene3D( );													//  デストラクタ

	HRESULT					Init( void );							//  初期化
	void					Uninit( void );							//  終了
	void					Update( void );							//  更新
	void					Draw( void );							//  描画
	void					DrawDepth( void );						//  デプス値の書き込み

	static Scene3D*			Create( D3DXVECTOR3 position ,			//  生成
									D3DXVECTOR3 normal ,
									D3DXVECTOR3 size ,
									D3DXVECTOR3 rot ,
									D3DXCOLOR color , 
									char *pTextureName ,
									float fScale ,
									bool bInverse = false ,
									float fPosU = 0.0f ,
									float fPosV = 0.0f ,
									float fDivideU = 1.0f , 
									float fDivideV = 1.0f ,
									float fAddRot = 0.0f );		

	void					SetTextureName( char* pFileName );		//  テクスチャ名の設定
	void					SetSize( D3DXVECTOR3 size );			//  大きさの設定
	void					SetScale( float fScale );				//  大きさ比率の設定

protected:
	D3DXVECTOR3				GetSize( void);							//  大きさの取得

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;								//  頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;							//  インデックスバッファインターフェースへのポインタ

	D3DXVECTOR3				m_normal;								//  法線
	D3DXVECTOR3				m_rot;									//  回転角
	float					m_fRot;									//  回転
	float					m_fAddRot;								//  回転角増加分
	float					m_fScale;								//  大きさの倍率
	bool					m_bInverse;

	char					m_aFileName[ 256 ];						//  ファイル名

private:
	HRESULT					MakeVertex( void );						//  頂点の作成
	void					SetVertex( void );						//  頂点の設定

	D3DXVECTOR2				m_posUV;								//  UV座標
	D3DXVECTOR2				m_divideUV;								//  UV分割数
};

#endif
