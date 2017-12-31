//--------------------------------------------------------------------------------------
//  デプスシャドウ   ( depthShadow.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _DEPTH_SHADOW_H_
#define _DEPTH_SHADOW_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  デプスシャドウクラスの定義
//--------------------------------------------------------------------------------------
class DepthShadow
{
public:
	DepthShadow(  );													//  デフォルトコンストラクタ		
	~DepthShadow( );													//  デストラクタ

	static HRESULT				Init( void );							//  初期化
	static void					Uninit( void );							//  終了
	static void					DrawClearBuffer( void );				//  バッファのクリア
	static void					SetRendererTarget( void );				//  レンダーターゲットの設定
	static void					SetDepthSerface( void );				//  深度バッファの設定
	static void					SetBias( float bias );					//  バイアス値の設定
	static float				GetBias( void );						//  バイアス値の取得
	static LPDIRECT3DTEXTURE9	GetRendereTargetTexture( void );		//  レンダーターゲットテクスチャの取得

	static DepthShadow*			Create( void );							//  生成

	static const UINT			TEXTURE_WIDTH;							//  デプスシャドウの幅
	static const UINT			TEXTURE_HEIGHT;							//  デプスシャドウの高さ

private:
	static LPDIRECT3DTEXTURE9	m_depthShadowTexture;					//  テクスチャ
	static LPDIRECT3DSURFACE9	m_depthShadowSurface;					//  レンダーターゲット用サーフェイス
	static LPDIRECT3DSURFACE9	m_shadowMap;							//  テクスチャ用の深度バッファー
	static float				m_bias;									//  バイアス値
};

#endif