﻿//--------------------------------------------------------------------------------------
//  テクスチャ管理   ( texture.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MAX_TEXTURE	( 256 )

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class Texture
{
public:
	Texture( );															//  デフォルトコンストラクタ
	~Texture( );														//  デストラクタ

	void Init( void );													//  初期化
	void Uninit( void );												//  終了

	void					SetTextureImage( const char *aFileName );	//  テクスチャの設定
	LPDIRECT3DTEXTURE9		GetTextureImage( const char *aFileName );	//  テクスチャの取得

private:
	LPDIRECT3DTEXTURE9		m_pTexture[ MAX_TEXTURE ];					//  テクスチャへのポインタ
	char					m_aFileName[ MAX_TEXTURE ][ 512 ];			//  テクスチャ名
};

#endif

