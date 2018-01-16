//--------------------------------------------------------------------------------------
//  テクスチャ   ( texture.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  テクスチャのコンストラクタ
//--------------------------------------------------------------------------------------
Texture::Texture( )
{
	//  テクスチャの最大数分のループ
	for( int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++ )
	{
		//  値のクリア
		m_pTexture[ nCntTexture ] = NULL;
		strcpy( m_aFileName[ nCntTexture ] , "NULL" );
	}
}

//--------------------------------------------------------------------------------------
//  テクスチャのデストラクタ
//--------------------------------------------------------------------------------------
Texture::~Texture( )
{

}

//--------------------------------------------------------------------------------------
//  テクスチャの初期化
//--------------------------------------------------------------------------------------
void Texture::Init( void )
{

}

//--------------------------------------------------------------------------------------
//  テクスチャの終了
//--------------------------------------------------------------------------------------
void Texture::Uninit( void )
{
	//  テクスチャの最大数分のループ
	for( int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++ )
	{
		//  テクスチャ情報の解放
		SAFE_RELEASE( m_pTexture[ nCntTexture ] );
	}
}

//--------------------------------------------------------------------------------------
//  テクスチャの設定をする関数
//--------------------------------------------------------------------------------------
void Texture::SetTextureImage( const char *aFileName )
{
	int nCntTexture;

	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャ最大数分のループ
	for( nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++ )
	{
		//  テクスチャポインタが空の場合
		if( m_pTexture[ nCntTexture ] != NULL )
		{
			//  文字列が一致している場合
			if( strcmp( aFileName , m_aFileName[ nCntTexture ] ) == 0 )
			{
				return;
			}
		}
	}

	//  テクスチャ最大数分のループ
	for( nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++ )
	{
		//  テクスチャポインタが空の場合
		if( m_pTexture[ nCntTexture ] == NULL )
		{
			//  文字列のコピー
			strcpy( m_aFileName[ nCntTexture ] , aFileName );

			//  テクスチャの読み込み( ハードディスクから )
			if( FAILED( D3DXCreateTextureFromFile( pDevice ,								//  デバイス
												   aFileName ,								//  テクスチャ名
												   &m_pTexture[ nCntTexture ] ) ) )			//  呼び込む場所
			{
				std::string message = aFileName;

				//  エラーだった場合
				//MessageBox( NULL , message.c_str( ) , "エラーメッセージ" , MB_OK | MB_ICONWARNING );
			}

			break;
		}
	}
}

//--------------------------------------------------------------------------------------
//  テクスチャの取得をする関数
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 Texture::GetTextureImage( const char *aFileName )
{
	//  テクスチャ最大数分のループ
	for( int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++ )
	{
		//  テクスチャポインタが空の場合
		if( m_pTexture[ nCntTexture ] != NULL )
		{
			//  文字列が一致している場合
			if( strcmp( aFileName , m_aFileName[ nCntTexture ] ) == 0 )
			{
				return m_pTexture[ nCntTexture ];
			}
		}
	}

	return NULL;
}