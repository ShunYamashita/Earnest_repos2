//--------------------------------------------------------------------------------------
//  フェード処理  ( fade.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _FADE_H_
#define _FADE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "mode.h"

//--------------------------------------------------------------------------------------
//  フェードクラスの定義
//--------------------------------------------------------------------------------------
class Fade
{
public:
	typedef enum
	{
		FADE_NONE = 0 ,
		FADE_IN ,
		FADE_OUT ,
		FADE_MAX
	} STATE;

	Fade( );															//  デフォルトコンストラクタ										
	~Fade( );															//  デストラクタ

	HRESULT					Init( void );								//  初期化
	static void				Uninit( void );								//  終了
	static void				Update( void );								//  更新
	static void				Draw( void );								//  描画

	static void				SetFade( STATE state ,						//  フェードの設定
									 Mode::MODE modeNext ,
									 D3DXCOLOR color ,
									 float fFadeSpeed  );

	static STATE			GetFade( void );							//  フェード状態の取得

	static Fade*			Create( void );								//  生成		

private:
	static HRESULT					MakeVertex( void );					//  頂点の作成
	static void						SetVertex( void );					//  頂点の設定

	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							//  頂点バッファへのポインタ
	static LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;						//  インデックスバッファインターフェースへのポインタ

	static D3DXCOLOR				m_color;							//  フェード色格納用
	static STATE					m_state;							//  フェード状態格納用
	static Mode::MODE				m_modeNext;							//  次のモード格納用
	static float					m_fFadeSpeed;						//  フェード速度
};

#endif