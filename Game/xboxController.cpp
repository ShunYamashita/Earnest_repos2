//--------------------------------------------------------------------------------------
//  Xboxコントローラー   ( xboxController.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "xboxController.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define XINPUT_STICK_RANGE_ENABLE		( 10000 )		//  XInputのスティック反応範囲

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  入力クラスのコンストラクタ
//--------------------------------------------------------------------------------------
XboxController::XboxController( )
{

}

//--------------------------------------------------------------------------------------
//  入力クラスのデストラクタ
//--------------------------------------------------------------------------------------
XboxController::~XboxController( )
{

}

//--------------------------------------------------------------------------------------
//  入力の初期化処理
//--------------------------------------------------------------------------------------
HRESULT XboxController::Init( void )
{
	for( int nCntXInput = 0; nCntXInput < MAX_XINPUT; nCntXInput++ )
	{
		if( XInputGetState( 0 , &m_xInput[ nCntXInput ] ) != ERROR_SUCCESS )
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  入力の終了処理
//--------------------------------------------------------------------------------------
void XboxController::Uninit( void )
{
	//  XInputを無効に
	XInputEnable( false );
}

//--------------------------------------------------------------------------------------
//  入力の更新処理
//--------------------------------------------------------------------------------------
void XboxController::Update( void )
{
	XINPUT_STATE	xInput[ MAX_XINPUT ];						//  XInput情報

	for( int nCntXInput = 0; nCntXInput < MAX_XINPUT; nCntXInput++ )
	{
		if( XInputGetState( 0 , &xInput[ nCntXInput ] ) != ERROR_SUCCESS )
		{
			return;
		}

		// キートリガー情報を保存
		m_xInputTrigger[ nCntXInput ].Gamepad.wButtons = ( m_xInput[ nCntXInput ].Gamepad.wButtons ^ xInput[ nCntXInput ].Gamepad.wButtons ) & xInput[ nCntXInput ].Gamepad.wButtons;

		// キープレス情報を保存
		m_xInput[ nCntXInput ] = xInput[ nCntXInput ];
	}
}

//--------------------------------------------------------------------------------------
//  入力ボタンプレスフラグ確認処理
//--------------------------------------------------------------------------------------
bool XboxController::GetButtonPress( int nIndex , BUTTON nButton )
{
	if( m_xInput[ nIndex ].Gamepad.wButtons & nButton )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  入力ボタントリガーフラグ確認処理
//--------------------------------------------------------------------------------------
bool XboxController::GetButtonTrigger( int nIndex , BUTTON nButton )
{
	if( m_xInputTrigger[ nIndex ].Gamepad.wButtons & nButton )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  入力スティック確認処理
//--------------------------------------------------------------------------------------
int XboxController::GetStick( int nIndex , XboxController::TYPE_STICK type )
{
	//  種類によっての場合分け
	switch( type )
	{
		case TYPE_STICK_LX:
		{
			if( m_xInput[ nIndex ].Gamepad.sThumbLX > XINPUT_STICK_RANGE_ENABLE || m_xInput[ nIndex ].Gamepad.sThumbLX < -XINPUT_STICK_RANGE_ENABLE )
			{
				return m_xInput[ nIndex ].Gamepad.sThumbLX;
			}
			else
			{
				return 0;
			}

			break;
		}
		case TYPE_STICK_LY:
		{
			if( m_xInput[ nIndex ].Gamepad.sThumbLY > XINPUT_STICK_RANGE_ENABLE || m_xInput[ nIndex ].Gamepad.sThumbLY < -XINPUT_STICK_RANGE_ENABLE )
			{
				return m_xInput[ nIndex ].Gamepad.sThumbLY;
			}
			else
			{
				return 0;
			}

			break;
		}
		case TYPE_STICK_RX:
		{
			if( m_xInput[ nIndex ].Gamepad.sThumbRX > XINPUT_STICK_RANGE_ENABLE || m_xInput[ nIndex ].Gamepad.sThumbRX < -XINPUT_STICK_RANGE_ENABLE )
			{
				return m_xInput[ nIndex ].Gamepad.sThumbRX;
			}
			else
			{
				return 0;
			}

			break;
		}
		case TYPE_STICK_RY:
		{
			if( m_xInput[ nIndex ].Gamepad.sThumbRY > XINPUT_STICK_RANGE_ENABLE || m_xInput[ nIndex ].Gamepad.sThumbRY < -XINPUT_STICK_RANGE_ENABLE )
			{
				return m_xInput[ nIndex ].Gamepad.sThumbRY;
			}
			else
			{
				return 0;
			}

			break;
		}
	}

	return 0;
}