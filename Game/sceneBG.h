//--------------------------------------------------------------------------------------
//  背景レンダリング処理   ( sceneBG.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_BG_H_
#define _SCENE_BG_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class SceneBG : public Scene
{
public:
	typedef enum
	{
		TYPE_TITLE = 0 ,
		TYPE_GAME ,
		TYPE_STAGE_SELECT ,
		TYPE_RESULT ,
		TYPE_RESULT2 ,
		TYPE_RESULT3 ,
		TYPE_RESULT4 ,
		TYPE_TEAM_LOGO ,
		TYPE_TUTORIAL ,
	} TYPE;

	SceneBG( );															//  デフォルトコンストラクタ
	SceneBG( D3DXVECTOR3 position ,
			 D3DXVECTOR3 size ,
			 D3DXVECTOR2 divideUV ,
			 D3DXVECTOR2 posUV ,
			 D3DXCOLOR color );											//  コンストラクタ
	~SceneBG( );														//  デストラクタ

	HRESULT					Init( void );								//  初期化
	void					Uninit( void );								//  終了
	void					Update( void );								//  更新
	void					Draw( void );								//  描画
	void					DrawDepth( void ){ }						//  デプス値の書き込み

	static SceneBG*			Create( TYPE type ,							//  生成
									D3DXVECTOR3 position ,
									D3DXVECTOR3 size , 
									D3DXVECTOR2 posUV ,
									D3DXVECTOR2 divideUV );			

private:
	void					SetPositionUV( D3DXVECTOR2 size );				//  UV座標の設定
	D3DXVECTOR2				GetPosUV( void);							//  UV座標の取得

	void					SetDivideUV( D3DXVECTOR2 size );			//  UV分割数の設定
	D3DXVECTOR2				GetDivideUV( void);							//  UV分割数の取得

	HRESULT					MakeVertex( void );							//  頂点の作成
	void					SetVertex( void );							//  頂点の設定

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;									//  頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;								//  インデックスバッファインターフェースへのポインタ

	D3DXVECTOR2				m_posUV;									//  UV座標
	D3DXVECTOR2				m_divideUV;									//  UV分割数

	TYPE					m_type;										//  種類

	int						m_nAnimationShift;							//  アニメーション移行フレーム	

	float					m_fRot;										//  傾き
};

#endif
