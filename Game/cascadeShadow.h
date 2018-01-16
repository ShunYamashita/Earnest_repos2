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
	struct AABB
	{
		D3DXVECTOR3 min;
		D3DXVECTOR3 max;
	};

	CascadeShadow(  );																//  デフォルトコンストラクタ		
	~CascadeShadow( );																//  デストラクタ

	static HRESULT				Init( void );										//  初期化
	static void					Uninit( void );										//  終了		
	static void					DrawClearBuffer( void );							//  バッファのクリア
	static void					SetCascadeShadowInfo( const D3DXVECTOR3& min ,		//  カスケードシャドウ情報の設定
													  const D3DXVECTOR3& max );				
	static void					SetRendererTarget( int index );						//  レンダーターゲットの設定
	static void					SetDepthSerface( void );							//  深度バッファの設定
	static void					SetBias( float bias );								//  バイアス値の設定
	static float				GetBias( void );									//  バイアス値の取得
	static LPDIRECT3DTEXTURE9	GetRendereTargetTexture( int index );				//  レンダーターゲットテクスチャの取得
	static const D3DXMATRIX&	GetShadowMatrix( int index );						//  シャドウ行列の取得
	static const float&			GetSplitPosition( int index );						//  分割ごとの座標の取得

	static const UINT			TEXTURE_WIDTH;										//  カスケードシャドウマップの幅
	static const UINT			TEXTURE_HEIGHT;										//  カスケードシャドウマップの高さ

private:
	static void					SetSplitShadowPosition( int numberSplit ,			//  平行分割シャドウマップの分割位置の計算
														float lamda , 
														float nearValue ,
														float farValue ,
														float* positions );
	static void					AdjustClipPlane( const AABB& aabb ,					//  クリップ平面の距離を調整
												 const D3DXVECTOR3& position ,
												 const D3DXVECTOR3& vecDirect ,
												 float& nearValue , 
												 float& farValue );
	static const AABB&			CalculateAABB( const float nearValue ,				//  角からAABBを求める
											   const float farValue ,
											   const D3DXMATRIX& viewProjection );
	static const D3DXMATRIX&	CreateCropMatrix( const AABB& aabb );				//  クロップ行列の作成

	static LPDIRECT3DTEXTURE9	m_cascadeShadowTexture[ MAX_CASCADE ];				//  テクスチャ
	static LPDIRECT3DSURFACE9	m_cascadeShadowSurface[ MAX_CASCADE ];				//  レンダーターゲット用サーフェイス
	static LPDIRECT3DSURFACE9	m_shadowMap;										//  テクスチャ用の深度バッファー
	static float				m_bias;												//  バイアス値
	static D3DXMATRIX			m_shadowMatrix[ MAX_CASCADE ];						//  シャドウ行列
	static float				m_splitPosition[ MAX_CASCADE ];						//  分割ごとの座標
};

#endif