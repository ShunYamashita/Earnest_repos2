//--------------------------------------------------------------------------------------
//  管理プログラム   ( manager.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "mode.h"
#include "Wwise.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MAX_CAMERA_NUMBER ( 2 )

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class Renderer;
class Texture;
class Camera;
class Keyboard;
class XboxController;
class PS4Controller;
class Light;
class Sound;
class Xact;
class Fade;
class Wwise;

//--------------------------------------------------------------------------------------
//  管理クラスの定義
//--------------------------------------------------------------------------------------
class SceneManager
{
public: 
	typedef enum
	{
		STAGE1 = 0 ,
		STAGE2 ,
		STAGE3 ,
		STAGE_MAX ,
	} STAGE;

	SceneManager( );													//  コンストラクタ
	~SceneManager( );													//  デストラクタ

	virtual HRESULT				Init( HINSTANCE hInstance ,				//  初期化
									  HWND hWnd ,
									  bool bWindow );					
	virtual void				Uninit( void );							//  終了
	virtual void				Update( void );							//  更新
	virtual void				Draw( void );							//  描画

	static	void				SetMode( Mode::MODE mode );				//  モードの設定

	static  Renderer*			GetRenderer( void );					//  レンダラークラスの取得
	static	Texture*			GetTexture( void );						//  テクスチャクラスの取得
	static  Keyboard*			GetKeyboard( void );					//  キーボードクラスの取得
	static  XboxController*		GetXInput( void );						//  Xboxコントローラークラスの取得
	static  PS4Controller*		GetPS4Input( void );					//  PS4コントローラークラスの取得
	static	Camera*				GetCamera( int cameraNo = 0 );			//  指定番号のカメラクラスの取得
	static	Light*				GetLight( void );						//  ライトクラスの取得
	static	Sound*				GetSound( void );						//  サウンドクラスの取得
	static	Wwise*				GetWwise( void );						//  Wwiseクラスの取得
	static	WWISE_GAMEOBJ*		GetWwiseObject( void );					//  Wwise用のオブジェクト取得
	static	Mode*				GetModeClass( void );					//  モード情報の取得
	static	Mode::MODE			GetMode( void );						//  モード情報の取得
	static	SceneManager::STAGE	GetStage( void );						//  ステージ情報の取得
	static	void				SetStage( SceneManager::STAGE stage );	//  ステージの種類の設定
	static	int					GetLoop( void );						//  ループ数の取得

private:
	static	STAGE				m_stage;								//  ステージの種類

	static	Renderer*			m_pRenderer;							//  レンダラークラスのポインタ
	static	Texture*			m_pTexture;								//  テクスチャクラスのポインタ
	static	Keyboard*			m_pKeyboard;							//  キーボードクラスのポインタ
	static	XboxController*		m_pXboxInput;							//  Xboxコントローラークラスのポインタ
	static	PS4Controller*		m_pPS4Input;							//  PS4コントローラークラスのポインタ
	static	Camera*				m_pCamera[ MAX_CAMERA_NUMBER ];			//  カメラクラスのポインタ
	static	Light*				m_pLight;								//  ライトクラスのポインタ
	static	Sound*				m_pSound;								//  サウンドクラスのポインタ
	static	Wwise*				m_pWwise;								//  Wwiseクラスのポインタ
	static	Fade*				m_pFade;								//  フェードクラスのポインタ
	static	WWISE_GAMEOBJ*		m_wwiseObject;							//  Wwise用のオブジェクト

	static	Mode*				m_pMode;								//  モードクラスのポインタ
	static	Mode::MODE			m_mode;									//  モード列挙型の宣言

	static	int					m_nLoop;								//  ループ数

	LPDIRECT3DVERTEXBUFFER9		m_vertexBuffer;							//  頂点バッファ
};

#endif
