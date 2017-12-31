//--------------------------------------------------------------------------------------
//  XACT   ( xact.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define		_WIN32_DCOM
#define		XACT_FILENAME				"data/XACT/WIN/SSP.xgs"
#define		XACT_SE_FILENAME			"data/XACT/WIN/SE.xwb"
#define		XACT_BGM_FILENAME			"data/XACT/WIN/BGM.xwb"
#define		XACT_SOUNDBANK_FILENAME		"data/XACT/WIN/SoundBank.xsb"

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "xact.h"

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
char Xact::m_aFileName[ XACT_LABEL_MAX ][ 256 ] = {
														"title" ,
														"stageSelect" ,
														"battle000" ,
														"battle001" ,
														"result" ,
														"result2" ,
														"enter" ,
														"select" ,
														"cancel" ,
														"walk" ,
														"jump" ,
														"landing" ,
														"swordBlow" ,
														"swordClash" ,
														"swordSlash" ,
														"damage" ,
														"lockon" ,
														"fire" ,
														"flame" ,
														"tornade" ,
														"lightning" ,
														"dash" ,
														"guard" ,
														"aura" ,
														"voice0_ready00" ,
														"voice0_start00" ,
														"voice0_attack00" ,
														"voice0_attack01" ,
														"voice0_attack02" ,
														"voice0_attack03" ,
														"voice0_fire" ,
														"voice0_lightning" ,
														"voice0_damage00" ,
														"voice0_damage01" ,
														"voice0_jump00" ,
												   };

//--------------------------------------------------------------------------------------
//  サウンドクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Xact::Xact( )
{
	m_pXACT3Engine = NULL;				
	m_pInMemoryWaveBank = NULL;			
	m_pStreamingWaveBank = NULL;			
	m_pSoundBank = NULL;					
	m_hStreamingWaveBankFile = NULL;		
	m_pBufInMemoryWaveBank = NULL;			
	m_pBufSoundBank = NULL;

	for( int nCntLabel = 0; nCntLabel < Xact::XACT_LABEL_MAX; nCntLabel++ )
	{
		m_pCue[ nCntLabel ] = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  サウンドクラスのデストラクタ
//--------------------------------------------------------------------------------------
Xact::~Xact( )
{

}

//--------------------------------------------------------------------------------------
//  サウンドの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Xact::Init( HWND hWnd )
{
	//  COMの初期化
	if( FAILED( CoInitializeEx( NULL , COINIT_MULTITHREADED ) ) )
	{
		MessageBox( NULL , "xact.cpp : COMの初期化に失敗しました。" , "< エラーメッセージ >" , MB_OK );

		return E_FAIL;
	}

	//  XACTエンジンの生成
	if( FAILED(  XACT3CreateEngine( 0 , &m_pXACT3Engine ) ) )
	{
		MessageBox( NULL , "xact.cpp : XACTエンジンの生成に失敗しました。" , "< エラーメッセージ >" , MB_OK );

		return E_FAIL;
	}

	XACT_RUNTIME_PARAMETERS				xrParams = { 0 };						//  
	XACT_WAVEBANK_STREAMING_PARAMETERS	xwsParams ={ 0 };						//  

	if( FAILED( m_pXACT3Engine->Initialize( &xrParams ) ) )
	{
		MessageBox( NULL , "xact.cpp : XACTエンジンの初期化に失敗しました。" , "< エラーメッセージ >" , MB_OK );

		return E_FAIL;
	}

	//  SEファイルの読み込み
	HANDLE hFile = CreateFile( XACT_SE_FILENAME , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL );

	//  読み込めた場合
	if( hFile != INVALID_HANDLE_VALUE )
	{
		//  ファイルサイズの取得
		DWORD fileSize = GetFileSize( hFile , NULL );

		//  ファイルにデータが入っている場合
		if( fileSize != -1 )
		{
			//  マップファイルの作成?
			HANDLE hMapFile = CreateFileMapping( hFile , NULL , PAGE_READONLY , 0 , fileSize , NULL );

			//  マップファイルが存在する場合
			if( hMapFile ) 
			{
				//  マップファイルの作成
				m_pBufInMemoryWaveBank = MapViewOfFile( hMapFile , FILE_MAP_READ , 0 , 0 , 0 );

				//  マップファイルが空ではない場合
				if( m_pBufInMemoryWaveBank ) 
				{                               
					//  InMemory用のWaveBankの作成
					if( FAILED( m_pXACT3Engine->CreateInMemoryWaveBank( m_pBufInMemoryWaveBank , fileSize , 0 , 0 , &m_pInMemoryWaveBank ) ) )
					{
						MessageBox( NULL , "xact.cpp : InMemoryWaveバンクの作成に失敗" , "< エラーメッセージ >" , MB_OK );

						return E_FAIL;
					}
				}

				//  マップファイルを閉じる
				CloseHandle( hMapFile );        
			}
		}

		//  ファイルを閉じる
		CloseHandle( hFile ); 
	}

	//  ストリーミング再生のWaveデータの取得
	m_hStreamingWaveBankFile = CreateFile( XACT_BGM_FILENAME , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING , NULL );

	if( m_hStreamingWaveBankFile != INVALID_HANDLE_VALUE ) 
	{           
		xwsParams.file = m_hStreamingWaveBankFile;
		xwsParams.packetSize = 64;

		//  ストリーミング再生のWaveデータの取得
		if( FAILED( m_pXACT3Engine->CreateStreamingWaveBank( &xwsParams , &m_pStreamingWaveBank ) ) )
		{
			MessageBox( NULL , "xact.cpp : ストリーミング用Wave バンクの作成に失敗" , "< エラーメッセージ >" , MB_OK );

			return E_FAIL;
		}
	}

	//  サウンドバンクファイルの読み込み
	hFile = CreateFile( XACT_SOUNDBANK_FILENAME , GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL );

	if( hFile != INVALID_HANDLE_VALUE ) 
	{        
		DWORD fileSize = GetFileSize( hFile , NULL );

		if( fileSize != -1 ) 
		{
			m_pBufSoundBank = new BYTE[ fileSize ];

			if( m_pBufSoundBank ) 
			{
				DWORD byteRead;

				if( 0 != ReadFile( hFile , m_pBufSoundBank , fileSize , &byteRead , NULL ) ) 
				{
					//  サウンドバンクファイルの作成
					if( FAILED( m_pXACT3Engine->CreateSoundBank( m_pBufSoundBank , fileSize , 0 , 0 , &m_pSoundBank ) ) )
					{
						MessageBox( NULL , "xact.cpp : サウンドバンクの作成に失敗" , "< エラーメッセージ >" , MB_OK );

						return E_FAIL;
					}
				}       
			}
		}

		//  ファイルを閉じる
		CloseHandle( hFile );
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  サウンドの終了処理
//--------------------------------------------------------------------------------------
void Xact::Uninit( void )
{
	//  XACTエンジンが存在する場合
	if( m_pXACT3Engine != NULL )
	{
		//  メモリの解放
		m_pXACT3Engine->ShutDown( );
		m_pXACT3Engine->Release( );
	}

	SAFE_DELETE_ARRAY( m_pBufSoundBank );

	if( m_hStreamingWaveBankFile != INVALID_HANDLE_VALUE ) 
	{    
		CloseHandle( m_hStreamingWaveBankFile ); 
	}

	if( m_pBufInMemoryWaveBank ) 
	{    
		UnmapViewOfFile( m_pBufInMemoryWaveBank ); 
	}

	//  COMの終了処理
	CoUninitialize( );
}

//--------------------------------------------------------------------------------------
//  サウンドの更新処理
//--------------------------------------------------------------------------------------
void Xact::Update( void )
{
	//  XACTエンジンの更新
	m_pXACT3Engine->DoWork( );
}

//--------------------------------------------------------------------------------------
//  サウンドの再生処理
//--------------------------------------------------------------------------------------
void Xact::Play( XACT_LABEL xactLabel )
{
	//  XACTエンジンの更新
	m_pXACT3Engine->DoWork( );

	XACTINDEX index = m_pSoundBank->GetCueIndex( m_aFileName[ xactLabel ] );

	DWORD stateFlag = 0;

	if( ( xactLabel == Xact::XACT_LABEL_SE_WALK ) &&
		m_pCue[ index ] != NULL )
	{
		m_pCue[ index ]->GetState( &stateFlag );

		if( !( stateFlag & XACT_CUESTATE_PLAYING ) )
		{ 
			//  再生
			m_pSoundBank->Play( index , 0 , 0 , &m_pCue[ index ] );
		}
	}
	else
	{
		//  再生
		m_pSoundBank->Play( index , 0 , 0 , &m_pCue[ index ] );
	}
}

//--------------------------------------------------------------------------------------
//  サウンドの停止処理
//--------------------------------------------------------------------------------------
void Xact::Stop( XACT_LABEL xactLabel )
{
	XACTINDEX index = m_pSoundBank->GetCueIndex( m_aFileName[ xactLabel ] );

	m_pSoundBank->Stop( index , 0 );
}

//--------------------------------------------------------------------------------------
//  サウンドの停止処理
//--------------------------------------------------------------------------------------
void Xact::StopAll( void )
{
	for( int nCntLabel = 0; nCntLabel < Xact::XACT_LABEL_MAX; nCntLabel++ )
	{
		XACTINDEX index = m_pSoundBank->GetCueIndex( m_aFileName[ nCntLabel ] );

		m_pSoundBank->Stop( index , 0 );

		m_pCue[ nCntLabel ] = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  サウンドのピッチ変更処理
//--------------------------------------------------------------------------------------
void Xact::ChangePitch( XACT_LABEL xactLabel , int nPitch )
{
	XACTINDEX index = m_pSoundBank->GetCueIndex( m_aFileName[ xactLabel ] );

	XACTVARIABLEINDEX variableIndex = m_pCue[ index ]->GetVariableIndex( "Pitch" );  

	//  XACTエンジンの更新
	m_pXACT3Engine->DoWork( );
	//m_pCue[ index ]->SetVariable( variableIndex , nPitch );
	m_pCue[ index ]->Play( );
}