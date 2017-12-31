//--------------------------------------------------------------------------------------
//  XACT  ( xact.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _XACT_H_
#define _XACT_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include <Windows.h>
#include <xact3.h>

//--------------------------------------------------------------------------------------
//  サウンドクラスの定義
//--------------------------------------------------------------------------------------
class Xact
{
public:
	typedef enum
	{
		XACT_LABEL_BGM_TITLE = 0 ,									//  タイトル
		XACT_LABEL_BGM_STAGESELECT ,								//  ステージセレクト
		XACT_LABEL_BGM_BATTLE000 ,									//  ゲーム( 戦闘画面000 )
		XACT_LABEL_BGM_BATTLE001 ,									//  ゲーム( 戦闘画面001 )
		XACT_LABEL_BGM_RESULT ,										//  リザルト
		XACT_LABEL_BGM_RESULT2 ,									//  リザルト2
		XACT_LABEL_SE_ENTER ,										//  決定
		XACT_LABEL_SE_SELECT ,										//  選択
		XACT_LABEL_SE_CANCEL ,										//  キャンセル
		XACT_LABEL_SE_WALK ,										//  歩き 
		XACT_LABEL_SE_JUMP ,										//  ジャンプ
		XACT_LABEL_SE_LANDING ,										//  着地
		XACT_LABEL_SE_SWORD_BLOW ,									//  剣を振る
		XACT_LABEL_SE_SWORD_CLASH ,									//  剣がかちあう
		XACT_LABEL_SE_SWORD_SLASH ,									//  剣が当たる
		XACT_LABEL_SE_DAMAGE ,										//  ダメージ
		XACT_LABEL_SE_LOCKON ,										//  ロックオン
		XACT_LABEL_SE_FIRE ,										//  ファイア
		XACT_LABEL_SE_FLAME ,										//  フレイム
		XACT_LABEL_SE_TORNADE ,										//  竜巻
		XACT_LABEL_SE_LIGHTNING ,									//  雷
		XACT_LABEL_SE_DASH ,										//  ダッシュ
		XACT_LABEL_SE_GUARD ,										//  防御
		XACT_LABEL_SE_AURA ,										//  オーラ
		XACT_LABEL_SE_VOICE0_READY00 ,								//  ボイス0( 準備完了00 )
		XACT_LABEL_SE_VOICE0_START00 ,								//  ボイス0( スタート00 )
		XACT_LABEL_SE_VOICE0_ATTACK00 ,								//  ボイス0( 攻撃時00 )
		XACT_LABEL_SE_VOICE0_ATTACK01 ,								//  ボイス0( 攻撃時01 )
		XACT_LABEL_SE_VOICE0_ATTACK02 ,								//  ボイス0( 攻撃時02 )
		XACT_LABEL_SE_VOICE0_ATTACK03 ,								//  ボイス0( 攻撃時03 )
		XACT_LABEL_SE_VOICE0_FIRE00 ,								//  ボイス0( 火魔法00 )
		XACT_LABEL_SE_VOICE0_LIGHTNING00 ,							//  ボイス0( 雷魔法00 )
		XACT_LABEL_SE_VOICE0_DAMAGE00 ,								//  ボイス0( ダメージ00 )
		XACT_LABEL_SE_VOICE0_DAMAGE01 ,								//  ボイス0( ダメージ01 )
		XACT_LABEL_SE_VOICE0_JUMP00 ,								//  ボイス0( ジャンプ00 )
		XACT_LABEL_MAX
	} XACT_LABEL;

	Xact( );														//  コンストラクタ
	~Xact( );														//  デストラクタ

	HRESULT					Init( HWND hWnd );						//  初期化
	void					Uninit( void );							//  終了
	void					Update( void );							//  更新

	void					Play( XACT_LABEL xactLabel );			//  再生
	void					Stop( XACT_LABEL xactLabel );			//  停止
	void					StopAll( void );						//  全ての停止
	void					ChangePitch( XACT_LABEL xactLabel ,		//  ピッチの変更
										 int nPitch );				

private:
	IXACT3Engine*			m_pXACT3Engine;							//  XACTエンジン
	IXACT3WaveBank*			m_pInMemoryWaveBank;					//  InMemory用Waveバンクのインスタンス先ポインタ
	IXACT3WaveBank*			m_pStreamingWaveBank;					//  ストリーミング用Waveバンクのインスタンス先ポインタ
	IXACT3SoundBank*		m_pSoundBank;							//  サウンドバンクのインスタンス先ポインタ
	HANDLE					m_hStreamingWaveBankFile;				//  ストリーミング用Waveバンクファイルのファイルハンドル
	VOID*					m_pBufInMemoryWaveBank;					//  InMemoryWaveバンクの読み込み先バッファ領域のポインタ
	BYTE*					m_pBufSoundBank;						//  サウンドバンクファイルの読み込み先バッファ領域のポインタ
	IXACT3Cue*				m_pCue[ XACT_LABEL_MAX ];				//  キューのポインタ

	static char				m_aFileName[ XACT_LABEL_MAX ][ 256 ];	//  ファイル名
};

#endif

