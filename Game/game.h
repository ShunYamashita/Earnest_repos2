//--------------------------------------------------------------------------------------
//  ゲームプログラム   ( game.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "mode.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MESH_FIELD_SIZE			( 100.0f )
#define MESH_FIELD_DIVIDE		( 20 )
#define ONE_CUBE_SIZE			( 50.0f )
#define MAX_BLOCK				( 32 )
#define MAX_FIELD				( 10 )
#define FIELD_SIZE_X			( 10000.0f )
#define FIELD_SIZE_Z			( 10000.0f )

#define KEYBOARD_ENABLE

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class MeshField;

//--------------------------------------------------------------------------------------
//  ゲームクラスの定義
//--------------------------------------------------------------------------------------
class Game : public Mode
{
public:
	typedef enum
	{
		MODE_VS_CPU = 0 ,
		MODE_VS_PLAYER ,
	} MODE_VS;

	typedef enum
	{
		CRYSTAL_FIRE = 0 ,
		CRYSTAL_TORNADE ,
		CRYSTAL_LIGHTNING ,
		CRYSTAL_MAX
	} CRYSTAL;

	Game( );														//  コンストラクタ
	~Game( );														//  デストラクタ

	void					Init( void );							//  初期化						
	void					Uninit( void );							//  終了
	void					Update( void );							//  更新
	void					Draw( void );							//  描画

	static	int				GetNumDivide( void );					//  フィールド分割数の取得
	static	int				GetNumField( void );					//  フィールド数の取得
	static	int				GetFocusField( void );					//  フォーカスがあっているフィールド番号
	
	static	void			SetNextMode( Mode::MODE nextMode );		//  次のモードを設定
	static	void			SetModeVS( MODE_VS modeVS );			//  対戦モードを設定
	static	MODE_VS			GetModeVS( void );						//  対戦モードを取得
	static	void			SetCrystal( int nIndex ,
										CRYSTAL crystal );			//  クリスタルを設定

	static	void			SetFieldMin( D3DXVECTOR3 min );			//  フィールド最小座標を設定
	static	void			SetFieldMax( D3DXVECTOR3 max );			//  フィールド最大座標を設定
	static	D3DXVECTOR3		GetFieldMin( void );					//  フィールド最小座標を取得
	static	D3DXVECTOR3		GetFieldMax( void );					//  フィールド最大座標を取得

private:
	void					SetStage( void );						//  ステージの設定

	static	MODE_VS			m_modeVS;								//  対戦モード
	static	CRYSTAL			m_crystal[ 2 ];							//  魔法クリスタル

	static	D3DXVECTOR3		m_fieldPos;								//  現在フィールド座標
	static	int				m_nNumDivide;							//  フィールド分割数
	static	int				m_nNumField;							//  フィールド数
	static	int				m_nFocusField;							//  フォーカスがあっているフィールド番号
	static	float			m_fSize;								//  大きさ
	static  Mode::MODE		m_nextMode;								//  次の場面

	static	D3DXVECTOR3		m_fieldMin;								//  フィールド最小座標
	static	D3DXVECTOR3		m_fieldMax;								//  フィールド最大座標

	static	bool			m_bClear;								//  クリアしたかどうか
	
};

#endif
