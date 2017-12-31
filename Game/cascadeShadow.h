//--------------------------------------------------------------------------------------
//  カスケードシャドウ   ( cascadeShadow.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _CASCADE_SHADOW_H_
#define _CASCADE_SHADOW_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------------
static const int MAX_CASCADE = 4;

//--------------------------------------------------------------------------------------
//  カスケードシャドウクラスの定義
//--------------------------------------------------------------------------------------
class CascadeShadow
{
public:
	CascadeShadow(  );														//  デフォルトコンストラクタ		
	~CascadeShadow( );														//  デストラクタ

	static HRESULT				Init( void );								//  初期化
	static void					Uninit( void );								//  終了
	static void					DrawClearBuffer( void );					//  バッファのクリア
	static void					SetRendererTarget( int index );				//  レンダーターゲットの設定
	static void					SetDepthSerface( void );					//  深度バッファの設定
	static void					SetBias( float bias );						//  バイアス値の設定
	static float				GetBias( void );							//  バイアス値の取得
	static LPDIRECT3DTEXTURE9	GetRendereTargetTexture( int index );		//  レンダーターゲットテクスチャの取得
	static void					SetSplitShadowPosition( int numberSplit ,	//  平行分割シャドウマップの分割位置の計算
														float lamda , 
														float nearValue ,
														float farValue ,
														float* positions );

	static const UINT			TEXTURE_WIDTH;								//  カスケードシャドウマップの幅
	static const UINT			TEXTURE_HEIGHT;								//  カスケードシャドウマップの高さ

private:
	static LPDIRECT3DTEXTURE9	m_cascadeShadowTexture[ MAX_CASCADE ];		//  テクスチャ
	static LPDIRECT3DSURFACE9	m_cascadeShadowSurface[ MAX_CASCADE ];		//  レンダーターゲット用サーフェイス
	static LPDIRECT3DSURFACE9	m_shadowMap;								//  テクスチャ用の深度バッファー
	static float				m_bias;										//  バイアス値
};

#endif