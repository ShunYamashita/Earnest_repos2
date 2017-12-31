//--------------------------------------------------------------------------------------
//  PS4コントローラー   ( joystick.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "joystick.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define		JOYSTICK_BUFFER_SIZE			( 100 )			// ステック入力のバッファーサイズ
#define		NUM_MAX_JOYSTICK_BUTTON			( 128 )			// ジョイスティックボタンの最大数
#define		LIMIT_COUNT_REPEAT_JOYSTICK		( 20 )			// リピートまでの間隔（単位：フレーム）

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
LPDIRECTINPUTDEVICE8	PS4Controller::m_pDevJoystick[ MAX_JOYSTICK ] = { NULL };
DIDEVCAPS				PS4Controller::m_diJoystickCaps;
int						PS4Controller::m_nCntJoystick = 0;

//--------------------------------------------------------------------------------------
//  入力クラスのコンストラクタ
//--------------------------------------------------------------------------------------
PS4Controller::PS4Controller( )
{

}

//--------------------------------------------------------------------------------------
//  入力クラスのデストラクタ
//--------------------------------------------------------------------------------------
PS4Controller::~PS4Controller( )
{

}

//--------------------------------------------------------------------------------------
//  入力の初期化処理
//--------------------------------------------------------------------------------------
HRESULT PS4Controller::Init( HINSTANCE hInstance , HWND hWnd )
{
	for( m_nCntJoystick = 0; m_nCntJoystick < MAX_JOYSTICK; m_nCntJoystick++ )
	{
		m_bConnectJoystick[ m_nCntJoystick ] = false;

		// 各値0クリア
		ZeroMemory( &m_diJoystickState[ m_nCntJoystick ] , sizeof( m_diJoystickState[ m_nCntJoystick ] ) );
		ZeroMemory( &m_diJoystickStateTrigger[ m_nCntJoystick ] , sizeof( m_diJoystickStateTrigger[ m_nCntJoystick ] ) );
		ZeroMemory( &m_diJoystickStateRelease[ m_nCntJoystick ] , sizeof( m_diJoystickStateRelease[ m_nCntJoystick ] ) );
		ZeroMemory( &m_diJoystickStateRepeat[ m_nCntJoystick ] , sizeof( m_diJoystickStateRepeat[ m_nCntJoystick ] ) );

		for( int i = 0; i < NUM_MAX_JOYSTICK_BUTTON; i++ )
		{
			m_diJoystickStateRepeatCnt[ m_nCntJoystick ][ i ] = 0;
		}

		// 入力処理の初期化
		if( FAILED( Input::Init( hInstance , hWnd ) ) )
		{
			MessageBox( hWnd , "DirectInputオブジェクトが作れねぇ！" , "worning!!" , MB_ICONWARNING );
			return E_FAIL;
		}

		// デバイスの作成
		if( FAILED( m_pInput->EnumDevices( DI8DEVCLASS_GAMECTRL , EnumJoysticksCallback , NULL , DIEDFL_ATTACHEDONLY ) ) )
		{
			MessageBox( hWnd , "ステックの判別ができません" , "warning!!",  MB_ICONWARNING );
			return E_FAIL;
		}
	
		// 接続確認
		if( m_pDevJoystick[ m_nCntJoystick ] == NULL )
		{
			// ステックの接続フラグ設定
			m_bConnectJoystick[ m_nCntJoystick ] = false;
			//MessageBox( hWnd ,"Joystick is not connected\n"
			//				"Exit the game, please restart after connecting" , "warning!!" , MB_ICONWARNING );
			return E_FAIL;
		}
		else
		{
			// ステックの接続フラグ設定
			m_bConnectJoystick[ m_nCntJoystick ] = true;
		}

		// コールバック関数を使って各軸のモードを設定
		if( FAILED( m_pDevJoystick[ m_nCntJoystick ]->EnumObjects( EnumAxesCallback , NULL , DIDFT_AXIS ) ) )
		{
			MessageBox( hWnd , "ステックの入力値の設定ができません" , "warning!!" , MB_ICONWARNING );
			return E_FAIL;
		}

		// データフォーマットを設定
		if( FAILED( m_pDevJoystick[ m_nCntJoystick ]->SetDataFormat( &c_dfDIJoystick2 ) ) )
		{
			MessageBox( hWnd , "ステックのデータフォーマットを設定できません" , "warning!!" , MB_ICONWARNING );
			return E_FAIL;
		}

		// 協調モードを設定（フォアグラウンド＆非排他モード）
		if( FAILED( m_pDevJoystick[ m_nCntJoystick ]->SetCooperativeLevel( hWnd , ( DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) ) )
		{
			MessageBox( hWnd , "ステックの協調モードを設定できません" , "warning!!" , MB_ICONWARNING );
			return E_FAIL;
		}

		// バッファーサイズの設定
		DIPROPDWORD	dipdwJoystick;
		dipdwJoystick.diph.dwSize		= sizeof( dipdwJoystick );
		dipdwJoystick.diph.dwHeaderSize = sizeof( dipdwJoystick.diph );
		dipdwJoystick.diph.dwObj		= 0;
		dipdwJoystick.diph.dwHow		= DIPH_DEVICE;
		dipdwJoystick.dwData			= JOYSTICK_BUFFER_SIZE;

		if( FAILED( m_pDevJoystick[ m_nCntJoystick ]->SetProperty( DIPROP_BUFFERSIZE , &dipdwJoystick.diph ) ) )
		{
			MessageBox( hWnd , "ステックのバッファーサイズを設定できません" , "worning!!" , MB_ICONWARNING );
			return E_FAIL;
		}

		// ステックへのアクセス権を獲得(入力制御開始)
		m_pDevJoystick[ m_nCntJoystick ]->Acquire( );
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  入力の終了処理
//--------------------------------------------------------------------------------------
void PS4Controller::Uninit( void )
{
	for( int nCntJoystick = 0; nCntJoystick < MAX_JOYSTICK; nCntJoystick++ )
	{
		if( m_pDevJoystick[ nCntJoystick ] != NULL )
		{
			// 入力デバイスの開放
			// ステックへのアクセス権を開放(入力制御終了)
			m_pDevJoystick[ nCntJoystick ]->Unacquire();

			m_pDevJoystick[ nCntJoystick ]->Release();
			m_pDevJoystick[ nCntJoystick ] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------
//  入力の更新処理
//--------------------------------------------------------------------------------------
void PS4Controller::Update( void )
{
	for( int nCntJoystick = 0; nCntJoystick < MAX_JOYSTICK; nCntJoystick++ )
	{
		if(m_bConnectJoystick[ nCntJoystick ] == true)
		{
			DIJOYSTATE2 diJoystickState;

			// デバイスに値の取得準備を通知
			m_pDevJoystick[ nCntJoystick ]->Poll();

			// デバイスの値を取得
			if(SUCCEEDED(m_pDevJoystick[ nCntJoystick ]->GetDeviceState( sizeof(DIJOYSTATE2), &diJoystickState )))
			{
				for(int nCnKey = 0; nCnKey < NUM_MAX_JOYSTICK_BUTTON; nCnKey++)
				{
					// ボタンのトリガー情報の生成
					m_diJoystickStateTrigger[ nCntJoystick ].rgbButtons[nCnKey] = (m_diJoystickState[ nCntJoystick ].rgbButtons[nCnKey] ^ diJoystickState.rgbButtons[nCnKey]) & diJoystickState.rgbButtons[nCnKey];

					// ボタンのリリース情報の生成
					m_diJoystickStateRelease[ nCntJoystick ].rgbButtons[nCnKey] = (m_diJoystickState[ nCntJoystick ].rgbButtons[nCnKey] ^ diJoystickState.rgbButtons[nCnKey]) & m_diJoystickState[ nCntJoystick ].rgbButtons[nCnKey];

					// ボタンのリピート情報の生成
					if(diJoystickState.rgbButtons[nCnKey])
					{
						if(m_diJoystickStateRepeatCnt[ nCntJoystick ][nCnKey] < LIMIT_COUNT_REPEAT_JOYSTICK)
						{
							m_diJoystickStateRepeatCnt[ nCntJoystick ][nCnKey]++;

							if(m_diJoystickStateRepeatCnt[ nCntJoystick ][nCnKey] == 1 || m_diJoystickStateRepeatCnt[ nCntJoystick ][nCnKey] >= LIMIT_COUNT_REPEAT_JOYSTICK)
							{
								m_diJoystickStateRepeat[ nCntJoystick ].rgbButtons[nCnKey] = diJoystickState.rgbButtons[nCnKey];
							}
							else
							{
								m_diJoystickStateRepeat[ nCntJoystick ].rgbButtons[nCnKey] = 0;
							}
						}
					}
					else{
						m_diJoystickStateRepeat[ nCntJoystick ].rgbButtons[nCnKey] = 0;
						m_diJoystickStateRepeatCnt[ nCntJoystick ][nCnKey] = 0;
					}
		
					// ボタンのプレス情報を保存
					m_diJoystickState[ nCntJoystick ].rgbButtons[nCnKey] = diJoystickState.rgbButtons[nCnKey];
				}
			}
			else
			{
				// ステックへのアクセス権を取得
				m_pDevJoystick[ nCntJoystick ]->Acquire();
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  スティックを列挙して作成
//--------------------------------------------------------------------------------------
BOOL CALLBACK PS4Controller::EnumJoysticksCallback( const DIDEVICEINSTANCE* pDevInstance, LPVOID lpContext )
{
	UNREFERENCED_PARAMETER(pDevInstance);
	UNREFERENCED_PARAMETER(lpContext);

	static GUID pad_discrimination[MAX_JOYSTICK];	// 各デバイスの識別子を格納

	// 既に取得済みのデバイスだった場合はもう一回実行する
	for( int i = 0; i < m_nCntJoystick; i++ )
	{
		if(pad_discrimination[i] == pDevInstance->guidInstance)
		{
			return DIENUM_CONTINUE;
		}
	}

	// 列挙されたステックへのインターフェイスを取得
	if( FAILED ( m_pInput->CreateDevice( pDevInstance->guidInstance , &m_pDevJoystick[ m_nCntJoystick ], NULL ) ) )
	{
		MessageBox(NULL, "ステックの接続が確認できません", "worning!!", MB_ICONWARNING);
		return DIENUM_CONTINUE;
	}
	
	// ステックの能力を取得
	m_diJoystickCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(m_pDevJoystick[ m_nCntJoystick ]->GetCapabilities(&m_diJoystickCaps)))
	{
		// ステック能力の取得に失敗
		MessageBox(NULL, "ステックの能力の取得ができません", "worning!!", MB_ICONWARNING);

		for( int nCntJoystick = 0; nCntJoystick < MAX_JOYSTICK; nCntJoystick++ )
		{
			if( m_pDevJoystick[ nCntJoystick ] != NULL )
			{
				// 入力デバイスの開放
				// ステックへのアクセス権を開放(入力制御終了)
				m_pDevJoystick[ nCntJoystick ]->Unacquire();

				m_pDevJoystick[ nCntJoystick ]->Release();
				m_pDevJoystick[ nCntJoystick ] = NULL;
			}
		}

		return DIENUM_CONTINUE;
	}

	// デバイスの識別子を保存
	pad_discrimination[m_nCntJoystick] = pDevInstance->guidInstance;
	
	return DIENUM_STOP;
}

//--------------------------------------------------------------------------------------
//  スティックの軸を列挙して作成
//--------------------------------------------------------------------------------------
BOOL CALLBACK PS4Controller::EnumAxesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi , LPVOID lpvRef )
{
	// 軸の値の範囲を設定（-1000～1000）
	DIPROPRANGE diprg;
	ZeroMemory( &diprg, sizeof(diprg) );
	diprg.diph.dwSize       = sizeof(diprg); 
	diprg.diph.dwHeaderSize = sizeof(diprg.diph); 
	diprg.diph.dwObj        = lpddoi->dwType;
	diprg.diph.dwHow        = DIPH_BYID;
	diprg.lMin              = -1000;
	diprg.lMax              = +1000;

	// 設定値をジョイスティックデバイスへ反映
	if( FAILED( m_pDevJoystick[ m_nCntJoystick ]->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
	{
		MessageBox( NULL , "取得したステックに値を設定することができません" , "worning!!" , MB_ICONWARNING );
		return DIENUM_STOP;
	}
	
	return DIENUM_CONTINUE;
}

//--------------------------------------------------------------------------------------
//	左スティックの傾き値取得（上下左右共に-1000～+1000）
//--------------------------------------------------------------------------------------
POINT PS4Controller::GetLeftStickDisposition( int nIndex )
{
	POINT LeftStick = {NULL,NULL};

	if(m_bConnectJoystick[ nIndex ] == true)
	{
		DIJOYSTATE2 DevJoystick;

		// スティックの傾き値を取得
		m_pDevJoystick[ nIndex ]->GetDeviceState( sizeof(DIJOYSTATE2), &DevJoystick );

		LeftStick.x = DevJoystick.lX;
		LeftStick.y = DevJoystick.lY;
	}

	if( LeftStick.x >= -PS4_STICK_ENABLE && LeftStick.x <= PS4_STICK_ENABLE )
	{
		LeftStick.x = 0;
	}

	if( LeftStick.y >= -PS4_STICK_ENABLE && LeftStick.y <= PS4_STICK_ENABLE )
	{
		LeftStick.y = 0;
	}

	return LeftStick;
}

//--------------------------------------------------------------------------------------
//	右スティックの傾き値取得（上下左右共に-1000～+1000）
//--------------------------------------------------------------------------------------
POINT PS4Controller::GetRightStickDisposition( int nIndex )
{
	POINT RightStick = {NULL,NULL};

	if(m_bConnectJoystick[ nIndex ] == true)
	{
		DIJOYSTATE2 DevJoystick;

		// スティックの傾き値を取得
		m_pDevJoystick[ nIndex ]->GetDeviceState( sizeof(DIJOYSTATE2), &DevJoystick );

		RightStick.x = DevJoystick.lZ;
		RightStick.y = DevJoystick.lRz;

		if( RightStick.x >= -PS4_STICK_ENABLE && RightStick.x <= PS4_STICK_ENABLE )
		{
			RightStick.x = 0;
		}

		if( RightStick.y >= -PS4_STICK_ENABLE && RightStick.y <= PS4_STICK_ENABLE )
		{
			RightStick.y = 0;
		}
	}

	return RightStick;
}

/*==============================================================================
	【 ステックのプレス状態を取得 】
==============================================================================*/
bool PS4Controller::GetPress( int nIndex , int nKey)
{
	return (m_diJoystickState[ nIndex ].rgbButtons[nKey] & 0x80) ? true: false;
}

/*==============================================================================
	【 ステックのトリガー状態を取得 】
==============================================================================*/
bool PS4Controller::GetTrigger( int nIndex , int nKey)
{
	return (m_diJoystickStateTrigger[ nIndex ].rgbButtons[nKey] & 0x80) ? true: false;
}

/*==============================================================================
	【 ステックのリピート状態を取得 】
==============================================================================*/
bool PS4Controller::GetRepeat( int nIndex , int nKey)
{
	return (m_diJoystickStateRepeat[ nIndex ].rgbButtons[nKey] & 0x80) ? true: false;
}

/*==============================================================================
	【 ステックのリリ－ス状態を取得 】
==============================================================================*/
bool PS4Controller::GetRelease( int nIndex , int nKey)
{
	return (m_diJoystickStateRelease[ nIndex ].rgbButtons[nKey] & 0x80) ? true: false;
}