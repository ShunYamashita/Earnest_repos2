﻿//--------------------------------------------------------------------------------------
//  入力処理   ( input.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "input.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
LPDIRECTINPUT8 Input::m_pInput = NULL;

//--------------------------------------------------------------------------------------
//  入力クラスのコンストラクタ
//--------------------------------------------------------------------------------------
Input::Input( )
{

}

//--------------------------------------------------------------------------------------
//  入力クラスのデストラクタ
//--------------------------------------------------------------------------------------
Input::~Input( )
{

}

//--------------------------------------------------------------------------------------
//  入力の初期化処理
//--------------------------------------------------------------------------------------
HRESULT Input::Init( HINSTANCE hInstance , HWND hWnd )
{
	if( m_pInput == NULL )
	{
		// DirectInputオブジェクトの作成
		if( FAILED( DirectInput8Create( hInstance , DIRECTINPUT_VERSION ,
										IID_IDirectInput8, ( void** )&m_pInput , NULL ) ) )
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  入力の終了処理
//--------------------------------------------------------------------------------------
void Input::Uninit( void )
{
	//  ポインタが空ではない場合
	if( m_pInput != NULL)
	{
		// DirectInputオブジェクトの開放
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  入力の更新処理
//--------------------------------------------------------------------------------------
void Input::Update( void )
{

}