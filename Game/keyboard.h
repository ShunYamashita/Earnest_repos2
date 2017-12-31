//--------------------------------------------------------------------------------------
//  キーボード処理  ( keyboard.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "input.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define	NUM_KEY_MAX				( 256 )				// キーの最大数
#define	LIMIT_COUNT_REPEAT		( 20 )				// リピートカウントリミッター

//--------------------------------------------------------------------------------------
//  入力クラスの定義
//--------------------------------------------------------------------------------------
class Keyboard : public Input
{
public:
	Keyboard( );														//  コンストラクタ
	~Keyboard( );														//  デストラクタ

	HRESULT					Init( HINSTANCE hInstance ,					//  初期化
								  HWND hWnd );
	void					Uninit( void );								//  終了
	void					Update( void );								//  更新
	void					Reset( void );								//  リセット

	bool					GetKeyboardPress( int nKey );				//  プレス情報取得
	bool					GetKeyboardTrigger( int nKey );				//  トリガー情報取得
	bool					GetKeyboardRepeat( int nKey );				//  リピート情報取得
	bool					GetKeyboardRelease( int nKey );				//  リリース情報取得

private:
	LPDIRECTINPUTDEVICE8	m_pDevKeyboard;								// 入力デバイス(キーボード)へのポインタ
	BYTE					m_aKeyState[ NUM_KEY_MAX ];					// キーボードの入力情報ワーク
	BYTE					m_aKeyStateTrigger[ NUM_KEY_MAX ];			// キーボードのトリガー情報ワーク
	BYTE					m_aKeyStateRelease[ NUM_KEY_MAX ];			// キーボードのリリース情報ワーク
	BYTE					m_aKeyStateRepeat[ NUM_KEY_MAX ];			// キーボードのリピート情報ワーク
	int						m_aKeyStateRepeatCnt[ NUM_KEY_MAX ];		// キーボードのリピートカウンタ
};

#endif

