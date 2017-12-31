//--------------------------------------------------------------------------------------
//  PS4コントローラー  ( joystick.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "input.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define		NUM_MAX_JOYSTICK_BUTTON			( 128 )			// ジョイスティックボタンの最大数
#define		MAX_JOYSTICK					( 2 )			// 最大プレイヤー人数
#define		PS4_STICK_ENABLE				( 300 )			// ジョイスティックボタンの最大数

//--------------------------------------------------------------------------------------
//  入力クラスの定義
//--------------------------------------------------------------------------------------
class PS4Controller : Input
{
public:
	typedef enum
	{
		DIJ_SQUARE = 0,
		DIJ_CROSS,
		DIJ_CIRCLE,
		DIJ_TRIANGLE,
		DIJ_L1,
		DIJ_R1,
		DIJ_L2,
		DIJ_R2,
		DIJ_SHARE,
		DIJ_OPTIONS,
		DIJ_L3,
		DIJ_R3,
		DIJ_PS,
		DIJ_TOUCHPAD,
		DIJ_PS4,
		DIJ_MAX
	} DIJ;

	PS4Controller( );												//  コンストラクタ
	~PS4Controller( );												//  デストラクタ
		
	HRESULT			Init( HINSTANCE hInstance,						//  初期化
						  HWND hWnd );										
	void			Uninit( void );									//  終了
	void			Update( void );									//  更新

	POINT			GetLeftStickDisposition(int nIndex);			// 左スティックの傾き値取得
	POINT			GetRightStickDisposition(int nIndex);			// 右スティックの傾き値取得
	bool			GetPress(int nIndex, int nKey);					// ボタンを押している間
	bool			GetTrigger(int nIndex, int nKey);				// ボタンを押された瞬間のみ
	bool			GetRepeat(int nIndex, int nKey);				// ボタンを徐々に連打？
	bool			GetRelease(int nIndex, int nKey);				// ボタンを離された瞬間のみ

private:
	static BOOL CALLBACK	EnumJoysticksCallback( const DIDEVICEINSTANCE* pDevInstance ,			// 接続されたステックの種類を判別するコールバック関数
												   LPVOID lpContext );										
	static BOOL CALLBACK	EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi ,					// ステックの各軸を設定するコールバク関数
											  LPVOID lpvRef);				

	DIJOYSTATE2				m_diJoystickState[ MAX_JOYSTICK ];										// ステックのプレス情報ワーク
	DIJOYSTATE2				m_diJoystickStateTrigger[ MAX_JOYSTICK ];								// ステックのトリガー情報ワーク
	DIJOYSTATE2				m_diJoystickStateRelease[ MAX_JOYSTICK ];								// ステックのリリース情報ワーク
	DIJOYSTATE2				m_diJoystickStateRepeat[ MAX_JOYSTICK ];								// ステックのリピート情報ワーク
	int						m_diJoystickStateRepeatCnt[ MAX_JOYSTICK ][ NUM_MAX_JOYSTICK_BUTTON ];	// ステックのリピートカウンタ
	bool					m_bConnectJoystick[ MAX_JOYSTICK ];										// trueの場合、ステック接続有り

	static LPDIRECTINPUTDEVICE8	m_pDevJoystick[ MAX_JOYSTICK ];										// 入力デバイス(ステック)へのポインタ
	static DIDEVCAPS			m_diJoystickCaps;													// ステックの能力
	static int					m_nCntJoystick;

};

#endif
