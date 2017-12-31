//--------------------------------------------------------------------------------------
//  DirectX   ( main.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2016/05/17
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include	<stdio.h>

#include	"main.h"
#include	"game.h"
#include	"manager.h"
#include	"imgui_impl_dx9.h"
#include	"renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define CLASS_NAME				"就職作品用ゲーム" 							//  ウィンドウクラスの名前
#define WINDOW_NAME				"DISSIDIA ENCAMPMENT"						//  ウィンドウ( タイトル )の名前

//--------------------------------------------------------------------------------------
//  プロトタイプ宣言
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam );

//extern LRESULT ImGui_ImplDX9_WndProcHandler( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );

//--------------------------------------------------------------------------------------
//  グローバル変数の宣言
//--------------------------------------------------------------------------------------
HWND	g_hWnd;

#ifdef _DEBUG

	int	g_nCountFPS;							// FPSカウンタ

#endif

//--------------------------------------------------------------------------------------
//  メイン関数
//--------------------------------------------------------------------------------------
int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	//  
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	//  注意文の表示を消す
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	//  ウィンドウの構造体の宣言
	WNDCLASSEX wcex;

	//  ウィンドウの構造体の設定
	wcex.cbSize = sizeof( WNDCLASSEX );								//  ウィンドウ構造体のサイズ( バイト数 )
	wcex.style = CS_CLASSDC;										//  ◎表示するウィンドウスタイルの設定
	wcex.lpfnWndProc = WndProc;										//  引数( () )が書いていない関数は関数ポインタであり、アドレスを返す
	wcex.cbClsExtra = 0;											//  通常は使用しないので"0"を設定
	wcex.cbWndExtra = 0;											//  通常は使用しないので"0"を設定
	wcex.hInstance = hInstance;										//  WinMainのパラメータのインスタンスハンドルを設定
	wcex.hIcon = NULL;												//  アイコンの種類の設定( リソースに設定して、ロードで変えることが出来る )
	wcex.hCursor = LoadCursor( NULL , IDC_ARROW );					//  カーソルの種類の設定( リソースに設定して、ロードで変えることが出来る )
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );			//  ウィンドウのクライアント領域の背景色を設定
	wcex.lpszMenuName = NULL;										//  windowにつけるメニューを設定( つけると処理が重くなる )
	wcex.lpszClassName = CLASS_NAME;								//  ウィンドウクラスの名前の設定
	wcex.hIconSm = NULL;											//  ミニアイコンが設定された場合の情報を記述

	//  ウィンドウクラスの登録
	RegisterClassEx( &wcex );

	//  ローカル変数の宣言
	//HWND hWnd;														//  ウィンドウハンドル( 構造体のポインタ )
	MSG msg;														//  メッセージ

	//  ウィンドウサイズをスクリーンに合わせる
	RECT cr = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	DWORD style = WS_OVERLAPPEDWINDOW ^ ( WS_MAXIMIZEBOX | WS_THICKFRAME );
	AdjustWindowRect( &cr , style , FALSE );
	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	//  デスクトップのサイズをとる
	RECT dr;
	GetWindowRect( GetDesktopWindow() , &dr );
	int dw = dr.right - dr.left;
	int dh = dr.bottom - dr.top;

	//  デスクトップ上の真ん中にウィンドウがでる処理
	int wx = ww > dw ? 0 : ( dw - ww ) / 2;
	int wy = wh > dh ? 0 : ( dh - wh ) / 2;

	//  ウィンドウを作成
	g_hWnd = CreateWindowEx( 0 ,									//  拡張ウィンドウスタイル( 基本 0 )
							 CLASS_NAME ,							//  ウィンドウクラスの名前( カスタムウィンドウの結び付ける )
							 WINDOW_NAME ,							//  ウィンドウの名前( タイトル )
							 style ,								//  ◎ウィンドウスタイル( ウィンドウ右上の×や最小化がない形式や、拡大縮小ができない形式に設定できる )
							 wx ,									//  ウィンドウの左上のX座標
							 wy ,									//  ウィンドウの左上のY座標
							 ww ,									//  ウィンドウの幅( フレームを含めた )
							 wh ,									//  ウィンドウの高さ( フレームを含めた )
							 NULL ,									//  親ウィンドウのハンドル
							 NULL ,									//  メニューハンドルまたは子ウィンドウID
							 hInstance ,							//  インスタンスハンドル
							 NULL );								//  ウィンドウ作成データ

	//  ウィンドウの表示
	ShowWindow( g_hWnd  , nCmdShow );								//  指定されたウィンドウの表示( nCmdShow → ウィンドウ情報のフラグ )
	UpdateWindow( g_hWnd );											//  ウィンドウの更新

	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// 分解能を設定
	timeBeginPeriod( 1 );

	// フレームカウント初期化
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime( );

	SceneManager *pManager = NULL;						//  マネージャークラスのポインタ

	//  マネージャーの生成
	pManager = new SceneManager;

	//  初期化
	if( FAILED( pManager->Init( hInstance , g_hWnd , TRUE ) ) )
	{
		MessageBox( g_hWnd , "初期化に失敗しました。" , "エラーメッセージ" , MB_OK );
	}
	else
	{
		//  メッセージループ( while( 1 )と同じ )
		for( ;; )
		{
			if( PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ) != 0 )
			{
				//  windowsの処理
				if( msg.message == WM_QUIT )
				{
					break;
				}
				else
				{
					//  呼び出し側スレッドのメッセージキューブにメッセージを取得
					TranslateMessage( &msg );									//  仮想キーメッセージを文字メッセージに変換

					//  メッセージの翻訳と送出
					DispatchMessage( &msg );									//  ウィンドウプロシージャへメッセージを送出
				}
			}
			else
			{
				dwCurrentTime = timeGetTime( );

				if( ( dwCurrentTime - dwFPSLastTime ) >= 500 )	// 0.5秒ごとに実行
				{

#ifdef _DEBUG

					g_nCountFPS = dwFrameCount * 1000 / ( dwCurrentTime - dwFPSLastTime );

#endif
					dwFPSLastTime = dwCurrentTime;
					dwFrameCount = 0;
				}

				if( ( dwCurrentTime - dwExecLastTime ) >= ( 1000 / 60 ) )
				{
					dwExecLastTime = dwCurrentTime;

					//  マネージャーの更新
					pManager->Update( );

					//  マネージャーの描画
					pManager->Draw( );

					dwFrameCount++;
				}
			}
		}

		//  ウィンドウクラスの登録を解除
		//  第1引数 → メッセージクラス名
		//  第2引数 → アプリケーションインスタンス
		UnregisterClass( CLASS_NAME , wcex.hInstance );

		//  マネージャーが生成されている場合
		if( pManager != NULL )
		{
			//  マネージャーの解放
			pManager->Uninit( );
			delete pManager;
			pManager = NULL;
		}
	}

	getchar( );

	_CrtDumpMemoryLeaks( );

	return ( int )msg.wParam;
}

//--------------------------------------------------------------------------------------
//  ウィンドウプロージャ( ウィンドウにメッセージが帰ってきた場合反応、そして奪い取る )
//  第1引数 → ウィンドウのハンドル
//  第2引数 → メッセージの識別子( 割り当てられた番号 )
//  第3引数 → メッセージの最初のパラメータ( 本来はポインタだが、値を入れても大丈夫 )
//  第4引数 → メッセージの2番目のパラメータ( 本来はポインタだが、値を入れても大丈夫 )
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
{
	//  ローカル変数
	const RECT rect = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	UINT nID = 0;

	ImGui_ImplDX9_WndProcHandler( hWnd , uMsg, wParam , lParam );

	//  メッセージによっての場合分け
	switch( uMsg )
	{
		//  ウィンドウが破壊される場合
		case WM_DESTROY:
		{
			//  終了( "WM_QUIT" )メッセージを返す
			PostQuitMessage( 0 );

			break;
		}
		//  親ウィンドウが出来た場合
		case WM_CREATE:
		{
			break;
		}
		//  コマンドが押された場合
		case WM_COMMAND:
		{
			break;
		}
		case WM_PAINT:
		{
			break;
		}
		//  キーが押された場合
		case WM_KEYDOWN:
		{
			//  ESCキーが押された場合
			if( wParam == VK_ESCAPE )
			{
				//  メッセージの表示
				//nID = MessageBox( NULL , "本当に終了してもよろしいですか？" , "終了メッセージ" , MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );

				//  はいと答えた場合
				//if( nID == IDYES )
				{
					//  ウィンドウの削除
					DestroyWindow( hWnd );
				}
			}

			break;
		}
		//  閉じるボタンが押された場合
		case WM_CLOSE:
		{
			//  メッセージの表示
			nID = MessageBox( NULL , "本当に終了してもよろしいですか？" , "終了メッセージ" , MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );

			//  はいと答えた場合
			if( nID == IDYES )
			{
				//  ウィンドウの削除
				DestroyWindow( hWnd );
			}
			
			return 0;

			break;
		}
		default:
		{
			break;
		}
	}

	//  セーブされていない場合、セーブしなくても大丈夫かどうか聞けるようにここで終了メッセージを貰って判定する

	//  決まった動きをする関数( WM_DESTROYは通らない )
	return DefWindowProc( hWnd , uMsg , wParam , lParam );
}

//--------------------------------------------------------------------------------------
//  Window情報を渡す関数
//--------------------------------------------------------------------------------------
HWND GetWindow( void )
{
	return g_hWnd;
}

#ifdef _DEBUG

//--------------------------------------------------------------------------------------
//  FPS情報を渡す関数
//--------------------------------------------------------------------------------------
int GetFPS( void )
{
	return g_nCountFPS;
}

#endif