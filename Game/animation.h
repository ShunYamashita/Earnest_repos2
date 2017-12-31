//--------------------------------------------------------------------------------------
//  アニメーション  ( animation.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModelAnim.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MAX_KEY_FRAME		( 24 )								//  モデルパーツ最大数

//--------------------------------------------------------------------------------------
//  アニメーションクラスの定義
//--------------------------------------------------------------------------------------
class Animation
{
public:
	typedef struct
	{
		int				nFrame;
		D3DXVECTOR3		position[ MAX_MODEL_PARTS ];
		D3DXVECTOR3		rot[ MAX_MODEL_PARTS ];
	} KEY_FRAME;

	Animation( );												//  コンストラクタ
	~Animation( );												//  デストラクタ

	void				SetKeyFrame( int nKey ,					//  キーフレーム情報の代入
									 KEY_FRAME keyFrame );
	KEY_FRAME			GetKeyFrame( int nKey );
	int					GetNumKey( void );
	int					GetLoop( void );

	static Animation*	Create( int				nNumKey ,		//  生成
								int				nLoop );

private:
	int					m_nNumKey;
	int					m_nLoop;
	KEY_FRAME			m_keyFrame[ MAX_KEY_FRAME ];

};

#endif

