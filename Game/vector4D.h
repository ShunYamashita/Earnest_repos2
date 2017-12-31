//--------------------------------------------------------------------------------------
//  4Dベクタークラス   ( vector4D.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _VECTOR_4D_H_
#define _VECTOR_4D_H_

//--------------------------------------------------------------------------------------
//  4Dベクタークラスの定義
//--------------------------------------------------------------------------------------
class Vector4D
{
public: 
	Vector4D( );																//  コンストラクタ
	Vector4D( float workX ,
			   float workY ,
			   float workZ ,
			   float workW ) : x( workX ) ,			//  コンストラクタ
							   y( workY ) ,
							   z( workZ ) ,
							   w( workW ) { };	
	~Vector4D( );																//  デストラクタ
	Vector4D operator+( const Vector4D &vector4D );							//  オペレータ演算子+の定義
	Vector4D operator-( const Vector4D &vector4D );							//  オペレータ演算子-の定義
	Vector4D operator*( const Vector4D &vector4D );							//  オペレータ演算子*の定義
	Vector4D &operator=( const Vector4D &vector4D );							//  オペレータ演算子=の定義
	Vector4D &operator+=( const Vector4D &vector4D );							//  オペレータ演算子+=の定義
	Vector4D &operator-=( const Vector4D &vector4D );							//  オペレータ演算子-=の定義
	Vector4D &operator*=( const Vector4D &vector4D );							//  オペレータ演算子*=の定義

	float	x;
	float	y;
	float	z;
	float	w;
};

#endif


