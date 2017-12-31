//--------------------------------------------------------------------------------------
//  レンダリング処理   ( renderer.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _RENDERER_H_
#define _RENDERER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "Effekseer.h"
#include "EffekseerRendererDX9.h"
#include "EffekseerSoundXAudio2.h"

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class Fade;

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class Renderer
{
public:
	enum class RENDERE_TARGET
	{
		BACKBUFFER = 0 ,
		SURFACE ,
	};

	Renderer( );																//  コンストラクタ
	~Renderer( );																//  デストラクタ
	HRESULT						Init( HWND hWnd, bool bWindow );				//  初期化
	void						Uninit( void );									//  終了
	void						Update( bool bUpdate );							//  更新
	void						Draw( void );									//  描画
	void						DrawBegin( void );								//  描画開始
	void						DrawClearBuffer( void );						//  バッファのクリア
	void						DrawEnd( void );								//  描画終了
	void						DrawEndPresent( void );							//  描画終了
	void						SetRendererTarget( RENDERE_TARGET target );		//  レンダーターゲットの設定
	void						SetBackBufferDepth( void );						//  バックバッファの深度バッファ設定
	LPDIRECT3DTEXTURE9			GetRendereTargetTexture( void );				//  レンダーターゲットテクスチャの取得
	void						ChangeFillMode( void );							//  描画形式の変更

	LPDIRECT3DDEVICE9			GetDevice( void );								//  デバイスの取得

private:

#ifdef _DEBUG

	void						DrawFPS(void);									//  FPS描画

	void						DrawField( void );								//  ツール時のフィールド情報描画

#endif

	LPDIRECT3D9					m_pD3D;											//  Direct3Dオブジェクト
	static LPDIRECT3DDEVICE9	m_pD3DDevice;									//  Deviceオブジェクト(描画に必要)

	LPDIRECT3DTEXTURE9			m_rendererTargetTexture;						//  テクスチャ
	LPDIRECT3DSURFACE9			m_rendererTargetSurface;						//  レンダーターゲット用サーフェイス
	LPDIRECT3DSURFACE9			m_backBufferSurface;							//  バックバッファ
	LPDIRECT3DSURFACE9			m_backBufferDepth;								//  バックバッファ用の深度バッファー
	bool						m_wireFrame;									//  ワイヤーフレーム描画のフラグ

	static Fade*				m_pFade;										//  フェードクラスのポインタ

#ifdef _DEBUG

	LPD3DXFONT					m_pFont;										//  フォントへのポインタ
	int							m_nCountFPS;									//  FPSカウンタ

#endif

};

#endif
