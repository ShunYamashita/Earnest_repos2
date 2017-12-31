//--------------------------------------------------------------------------------------
//  Xboxコントローラー  ( xboxController.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include <Windows.h>
#include <XInput.h>

//--------------------------------------------------------------------------------------
//  ライブラリのリンク
//--------------------------------------------------------------------------------------
#pragma comment( lib , "xinput.lib" )

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MAX_XINPUT		( 1 )

//--------------------------------------------------------------------------------------
//  入力クラスの定義
//--------------------------------------------------------------------------------------
class XboxController
{
public:
	typedef enum
	{
		TYPE_STICK_LX = 0 ,
		TYPE_STICK_LY ,
		TYPE_STICK_RX ,
		TYPE_STICK_RY ,
	} TYPE_STICK;

	typedef enum
	{
		BUTTON_A = 0 ,
		BUTTON_B ,
		BUTTON_X ,
		BUTTON_Y ,
		BUTTON_RB ,
		BUTTON_RT ,
		BUTTON_LB ,
		BUTTON_LT ,
	} BUTTON;

	XboxController( );													//  コンストラクタ
	~XboxController( );												//  デストラクタ

	HRESULT					Init( void );								//  初期化
	void					Uninit( void );								//  終了
	void					Update( void );								//  更新

	bool					GetButtonPress( int nIndex ,				//  ボタンプレス情報の取得
											BUTTON nButton );

	bool					GetButtonTrigger( int nIndex ,				//  ボタントリガー情報の取得
											  BUTTON nButton );

	int						GetStick( int nIndex ,						//  スティック情報の取得
									  TYPE_STICK type );

protected:
	XINPUT_STATE			m_xInput[ MAX_XINPUT ];						//  XInput情報
	XINPUT_STATE			m_xInputTrigger[ MAX_XINPUT ];				//  XInput情報
	XINPUT_VIBRATION		m_xVibration[ MAX_XINPUT ];					//  Xboxコントローラーバイブレーション情報
};

#endif

