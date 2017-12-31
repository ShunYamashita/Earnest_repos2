//--------------------------------------------------------------------------------------
//  サウンド  ( sound.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  サウンドクラスの定義
//--------------------------------------------------------------------------------------
class Sound
{
public:
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0 ,									//  タイトル
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	typedef struct
	{
		char	*pFilename;											//  ファイル名
		int		nCntLoop;											//  ループカウント
		double	dNumLoop;											//  ループ数
	} SOUNDPARAM;

	Sound( );														//  コンストラクタ
	~Sound( );														//  デストラクタ

	HRESULT					Init( HWND hWnd );						//  初期化
	void					Uninit( void );							//  終了
	void					Update( void );							//  更新

	HRESULT					Play( SOUND_LABEL label ,
								  int nSeekTime = 0 );				//  再生
	void					Stop( SOUND_LABEL label );				//  停止
	void					Stop( void);							//  全ての停止

	int						GetAudioLoop( SOUND_LABEL label );		//  サウンドループ回数の取得
	IXAudio2*				GetXAudio( void );						//  XAudio2オブジェクトへのインターフェイスの取得

private:

	HRESULT					CheckChunk( HANDLE hFile ,
										DWORD format ,
										DWORD *pChunkSize ,
										DWORD *pChunkDataPosition );

	HRESULT					ReadChunkData( HANDLE hFile ,
										   void *pBuffer ,
										   DWORD dwBuffersize ,
										   DWORD dwBufferoffset );

	IXAudio2				*m_pXAudio2;							//  XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice	*m_pMasteringVoice;						//  マスターボイス
	IXAudio2SourceVoice		*m_apSourceVoice[ SOUND_LABEL_MAX ];	//  ソースボイス
	BYTE					*m_apDataAudio[ SOUND_LABEL_MAX ];		//  オーディオデータ
	DWORD					m_aSizeAudio[ SOUND_LABEL_MAX ];		//  オーディオデータサイズ
	SOUNDPARAM				m_aParam [SOUND_LABEL_MAX ];			//  各音のパラメーター
};

#endif

