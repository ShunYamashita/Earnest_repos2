//--------------------------------------------------------------------------------------
//  プレイヤーUI   ( playerUI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_UI_H_
#define _PLAYER_UI_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------------
//  プレイヤーUIクラスの定義
//--------------------------------------------------------------------------------------
class PlayerUI : public Scene2D
{
public:
	typedef enum
	{
		TYPE_PLAYER1 = 0 ,
		TYPE_PLAYER2 ,
	} TYPE;

	PlayerUI( );														//  コンストラクタ
	~PlayerUI( );														//  デストラクタ

	HRESULT				Init( void );									//  初期化
	void				Uninit( void );									//  終了
	void				Update( void );									//  更新
	void				Draw( void );									//  描画

	void				SetSwaySide( float fSwaySide );					//  横揺れの設定
	void				SetSwayVertical( float fSwayVertical );			//  縦揺れの設定

	static PlayerUI*	Create( TYPE type ,								//  生成
								D3DXVECTOR3 position ,						
								D3DXVECTOR3 size );

private:
	TYPE				m_type;											//  種類
	D3DXVECTOR3			m_basePos;										//  基準座標
	D3DXVECTOR3			m_seekPos;										//  座標ずらし量
	float				m_fSwaySide;									//  横振動
	float				m_fSwayVertical;								//  縦振動
	float				m_fSwayAngle;									//  揺れ角度
	int					m_nSwayTime;									//  揺れ時間カウント
};

#endif
