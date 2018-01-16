//--------------------------------------------------------------------------------------
//  体力ゲージ   ( life.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _LIFE_H_
#define _LIFE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2DL.h"
#include "number.h"

//--------------------------------------------------------------------------------------
//  体力クラスの定義
//--------------------------------------------------------------------------------------
class Life : public Scene2DL
{
public:
	typedef enum
	{
		TYPE_LIFE000 = 0 ,
		TYPE_LIFE001 ,
		TYPE_LIFE002 ,
		TYPE_LIFE003 ,
		TYPE_MAX ,
	} TYPE;

	Life( );
	Life( D3DXVECTOR3 position , D3DXVECTOR3 size ,
		   D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV ,
		   D3DXCOLOR color );												//  コンストラクタ
	~Life( );																//  デストラクタ

	HRESULT					Init( void );									//  初期化
	void					Uninit( void );									//  終了
	void					Update( void );									//  更新
	void					Draw( void );									//  描画

	static Life*			Create( TYPE type ,
									D3DXVECTOR3 position ,									
									D3DXVECTOR3 size , 
									D3DXVECTOR2 posUV ,
									D3DXVECTOR2 divideUV ,
									D3DXCOLOR color ,
									int nBaseLife );

	void					SetLife( int nLife );							//  体力の設定
	void					SetEndLife( int nLife );						//  体力の設定
	void					SetSwaySide( float fSwaySide );					//  横揺れの設定
	void					SetSwayVertical( float fSwayVertical );			//  縦揺れの設定

private:
	TYPE					m_type;											//  種類
	std::list< Number* >	m_pNumber;										//  数字クラスのポインタ
	float					m_fEndLife;										//  目標の体力
	int						m_nLife;										//  体力
	int						m_nBaseLife;									//  基準体力 

	D3DXVECTOR3				m_basePos;										//  基準座標
	D3DXVECTOR3				m_seekPos;										//  座標ずらし量
	float					m_fSwaySide;									//  横振動
	float					m_fSwayVertical;								//  縦振動
	float					m_fSwayAngle;									//  揺れ角度
	int						m_nSwayTime;									//  揺れ時間カウント
};

#endif
