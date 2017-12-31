//--------------------------------------------------------------------------------------
//  2Dベクタークラス   ( vector2D.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _VECTOR_2D_H_
#define _VECTOR_2D_H_

//--------------------------------------------------------------------------------------
//  2Dベクタークラスの定義
//--------------------------------------------------------------------------------------
class Vector2D
{
public: 
	Vector2D( );												//  コンストラクタ
	~Vector2D( );												//  デストラクタ
	Vector2D operator+( const Vector2D &vector2D );			//  オペレータ演算子+の定義
	Vector2D operator-( const Vector2D &vector2D );			//  オペレータ演算子-の定義
	Vector2D operator*( const Vector2D &vector2D );			//  オペレータ演算子*の定義
	Vector2D &operator=( const Vector2D &vector2D );			//  オペレータ演算子=の定義
	Vector2D &operator+=( const Vector2D &vector2D );			//  オペレータ演算子+=の定義
	Vector2D &operator-=( const Vector2D &vector2D );			//  オペレータ演算子-=の定義
	Vector2D &operator*=( const Vector2D &vector2D );			//  オペレータ演算子*=の定義

	float	x;
	float	y;
};

#endif

