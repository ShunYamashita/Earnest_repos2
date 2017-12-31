//--------------------------------------------------------------------------------------
//  キーボード処理   ( keyboard.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  キーボードクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Keyboard::Keyboard( )
{
	m_pDevKeyboard = NULL;

	//  キーの最大数分のループ
	for( int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++ )
	{
		m_aKeyState[ nCntKey ] = 0;
		m_aKeyStateTrigger[ nCntKey ] = 0;
		m_aKeyStateRelease[ nCntKey ] = 0;
		m_aKeyStateRepeat[ nCntKey ] = 0;
		m_aKeyStateRepeatCnt[ nCntKey ] = 0;
	}
}

//--------------------------------------------------------------------------------------
//  キーボードクラスのデストラクタ
//--------------------------------------------------------------------------------------
Keyboard::~Keyboard( )
{

}

//--------------------------------------------------------------------------------------
//  キーボードの初期化
//--------------------------------------------------------------------------------------
HRESULT Keyboard::Init( HINSTANCE hInstance , HWND hWnd )
{
	// 入力処理の初期化
	if( FAILED( Input::Init( hInstance , hWnd ) ) )
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの作成
	if( FAILED( m_pInput->CreateDevice( GUID_SysKeyboard , &m_pDevKeyboard , NULL ) ) )
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if( FAILED( m_pDevKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if( FAILED( m_pDevKeyboard->SetCooperativeLevel ( hWnd , ( DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) ) )
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevKeyboard->Acquire();

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  キーボードの終了処理
//--------------------------------------------------------------------------------------
void Keyboard::Uninit( void )
{
	//  ポインタが空ではない場合
	if( m_pDevKeyboard != NULL )
	{
		// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		m_pDevKeyboard->Unacquire( );

		m_pDevKeyboard->Release( );
		m_pDevKeyboard = NULL;
	}

	Input::Uninit( );
}

//--------------------------------------------------------------------------------------
//  キーボードの更新処理
//--------------------------------------------------------------------------------------
void Keyboard::Update( void )
{
	BYTE aKeyState[ NUM_KEY_MAX ];

	// デバイスからデータを取得
	if( SUCCEEDED( m_pDevKeyboard->GetDeviceState( sizeof( aKeyState ), aKeyState ) ) )
	{
		for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// キートリガー情報を保存
			m_aKeyStateTrigger[ nCntKey ] = ( m_aKeyState[ nCntKey ] ^ aKeyState[ nCntKey ] ) & aKeyState[ nCntKey ];

			//  キーリリース情報を保存
			m_aKeyStateRelease[ nCntKey ] = ( m_aKeyState[ nCntKey ] ^ aKeyState[ nCntKey ] ) & m_aKeyState[ nCntKey ];

			/*  キーリピート処理  */

			//  もしキーが押されている場合
			if( aKeyState[ nCntKey ] & 0x80 )
			{
				//  リピート回数が限度をまだ超えていない場合
				if( m_aKeyStateRepeatCnt[ nCntKey ] < LIMIT_COUNT_REPEAT )
				{
					//  カウント+1する
					m_aKeyStateRepeatCnt[ nCntKey ]++;

					//  押し始め、またはカウントが限界を超えた場合
					if( m_aKeyStateRepeatCnt[ nCntKey ] == 1 ||
						m_aKeyStateRepeatCnt[ nCntKey ] >= LIMIT_COUNT_REPEAT )
					{
						//  現在の状態を入れる
						m_aKeyStateRepeat[ nCntKey ] = aKeyState[ nCntKey ];
					}
					else
					{
						//  何も押されていない状態に
						m_aKeyStateRepeat[ nCntKey ] = 0;
					}
				}
				else
				{
					//  リピートとリピートカウントの初期化
					m_aKeyStateRepeat[ nCntKey ] = 0;
					m_aKeyStateRepeatCnt[ nCntKey ] = 0;
				}
			}

			// キープレス情報を保存
			m_aKeyState[ nCntKey ] = aKeyState[ nCntKey ];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		m_pDevKeyboard->Acquire();
	}
}

//--------------------------------------------------------------------------------------
//  キーボードのプレス状態を取得
//--------------------------------------------------------------------------------------
bool Keyboard::GetKeyboardPress( int nKey )
{
	return ( m_aKeyState[ nKey ] & 0x80 ) ? true: false;
}

//--------------------------------------------------------------------------------------
//  キーボードのトリガー状態を取得
//--------------------------------------------------------------------------------------
bool Keyboard::GetKeyboardTrigger( int nKey )
{
	return ( m_aKeyStateTrigger[ nKey ] & 0x80 ) ? true: false;
}

//--------------------------------------------------------------------------------------
//  キーボードのリピート状態を取得
//--------------------------------------------------------------------------------------
bool Keyboard::GetKeyboardRepeat( int nKey )
{
	return ( m_aKeyStateRepeat[ nKey ] & 0x80 ) ? true: false;
}

//--------------------------------------------------------------------------------------
//  キーボードのリリ－ス状態を取得
//--------------------------------------------------------------------------------------
bool Keyboard::GetKeyboardRelease( int nKey )
{
	return ( m_aKeyStateRelease[ nKey ] & 0x80 ) ? true: false;
}

//--------------------------------------------------------------------------------------
//  キーボードのリリ－ス状態を取得
//--------------------------------------------------------------------------------------
void Keyboard::Reset( void )
{
	//  キーの最大数分のループ
	for( int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++ )
	{
		m_aKeyState[ nCntKey ] = 0;
		m_aKeyStateTrigger[ nCntKey ] = 0;
		m_aKeyStateRelease[ nCntKey ] = 0;
		m_aKeyStateRepeat[ nCntKey ] = 0;
		m_aKeyStateRepeatCnt[ nCntKey ] = 0;
	}
}