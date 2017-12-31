//--------------------------------------------------------------------------------------
//  DirectX   ( main.h )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2016/05/17
//--------------------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include	<Windows.h>

#define		_CRTDBG_MAP_ALLOC
#include	<crtdbg.h>

#include	<d3dx9.h>

//#ifdef	_DEBUG
//#ifndef	DBG_NEW
//#define	DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define	new DBG_NEW
//#endif
//#endif	// DEBUG

#define		DIRECTINPUT_VERSION ( 0x0800 )
#include	<dinput.h>
#include	<XAudio2.h>

//--------------------------------------------------------------------------------------
//  ライブラリのリンク
//--------------------------------------------------------------------------------------
#pragma comment( lib , "d3d9.lib" )
#pragma comment( lib , "d3dx9.lib" )
#pragma comment( lib , "dxguid.lib" )
#pragma comment( lib , "dinput8.lib" )
#pragma comment( lib , "winmm.lib" )						//  システム時刻取得に必要

#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <EffekseerSoundXAudio2.h>

//--------------------------------------------------------------------------------------
//  プラグマコメント
//--------------------------------------------------------------------------------------
#if _DEBUG
#pragma comment(lib, "VS2015/Debug/Effekseer.lib" )
#pragma comment(lib, "VS2015/Debug/EffekseerRendererDX9.lib" )
#pragma comment(lib, "VS2015/Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "VS2015/Release/Effekseer.lib" )
#pragma comment(lib, "VS2015/Release/EffekseerRendererDX9.lib" )
#pragma comment(lib, "VS2015/Release/EffekseerSoundXAudio2.lib" )
#endif

//--------------------------------------------------------------------------------------
//	クラスの前方宣言
//--------------------------------------------------------------------------------------
class Texture;
class Camera;
class Keyboard;

//--------------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------------
int		GetFPS( void );
HWND	GetWindow( void );

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define SCREEN_WIDTH	( 1280 )							//  ウィンドウの幅
#define SCREEN_HEIGHT	( 720 )								//  ウィンドウの高さ

#define NUM_VERTEX		( 4 )								//  頂点数
#define NUM_VERTEX2		( 2 )								//  頂点数
#define NUM_POLYGON		( 2 )								//  ポリゴン数

#define SAFE_RELEASE( pointer )			{ if( pointer ) { ( pointer )->Release( ); ( pointer ) = NULL; } }
#define SAFE_DELETE_ARRAY( pointer )	{ if ( pointer ) { delete[ ] ( pointer ); ( pointer ) = NULL; } }

//--------------------------------------------------------------------------------------
//  頂点フォーマットの定義
//--------------------------------------------------------------------------------------
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//--------------------------------------------------------------------------------------
//  頂点情報構造体の宣言( 上の頂点フォーマットと構造を合わせること )
//--------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 position;									//  XYZ座標
	float		rhw;										//  rhw
	D3DCOLOR	color;										//  色情報
	D3DXVECTOR2 texcoord;									//  テクスチャ座標 
} VERTEX_2D;												//  2D頂点

typedef struct
{
	D3DXVECTOR3 position;									//  XYZ座標
	D3DXVECTOR3 normal;										//  法線
	D3DCOLOR	color;										//  色情報
	D3DXVECTOR2 texcoord;									//  テクスチャ座標 
} VERTEX_3D;												//  3D頂点

typedef struct
{
	D3DXVECTOR3 position;									//  XYZ座標
	D3DXVECTOR3 normal;										//  法線
	D3DXVECTOR2 texcoord;									//  テクスチャ座標 
} VERTEX_3D_ANOTHER;	

#endif