//--------------------------------------------------------------------------------------
//  ステージセレクトプログラム   ( stageSelect.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _STAGE_SELECT_H_
#define _STAGE_SELECT_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "mode.h"
#include "playerSelect.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class SceneBG;
class StageSelect2D;
class SelectVSMode;
class CrystalMagic;
class ReadySelect;

//--------------------------------------------------------------------------------------
//  ステージセレクトクラスの定義
//--------------------------------------------------------------------------------------
class StageSelect : public Mode
{
public: 
	typedef enum
	{
		FOCUS_STAGE1 = 0 ,
		FOCUS_STAGE2 ,
		FOCUS_MAX
	} FOCUS;

	typedef enum
	{
		VS_SOLO = 0 ,
		VS_PARTY ,
		VS_TUTORIAL ,
		VS_MAX
	} VS;

	StageSelect( );												//  コンストラクタ
	~StageSelect( );												//  デストラクタ

	void				Init( void );								//  初期化						
	void				Uninit( void );								//  終了
	void				Update( void );								//  更新
	void				Draw( void );								//  描画

private:
	VS					m_vs;										//  対戦モード
	FOCUS				m_foucus;									//  注目している
	Game::CRYSTAL		m_crystal;									//  クリスタル
	Game::CRYSTAL		m_crystal2;									//  クリスタル2
	StageSelect2D*		m_pScene2D[ FOCUS_MAX ];					//  ステージ選択
	SelectVSMode*		m_pVSMode[ VS_MAX ];						//  VSモード
	CrystalMagic*		m_pCrystalMagic[ Game::CRYSTAL_MAX ];		//  魔法クリスタル
	CrystalMagic*		m_pCrystalMagic2[ Game::CRYSTAL_MAX ];		//  魔法クリスタル
	ReadySelect*		m_pReadySelect;								//  選択準備完了
	PlayerSelect*		m_pPlayer[ 2 ];								//  プレイヤー
	bool				m_bCheckPlayer[ 2 ];						//  両方のプレイヤー
	float				m_fAddScale;								//  大きさ拡大率
	bool				m_bAdd;										//  計算フラグ
	float				m_fAddScale2;								//  大きさ拡大率
	bool				m_bAdd2;									//  計算フラグ
	int					m_nWaitTime;								//  待ち時間
	int					m_nWaitTime2;								//  待ち時間
	int					m_nFase;									//  段階
};

#endif
