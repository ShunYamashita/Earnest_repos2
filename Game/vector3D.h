//--------------------------------------------------------------------------------------
//  3Dベクタークラス   ( vector3D.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _VECTOR_3D_H_
#define _VECTOR_3D_H_

//--------------------------------------------------------------------------------------
//  3Dベクタークラスの定義
//--------------------------------------------------------------------------------------
class Vector3D
{
public: 
	Vector3D( );																//  コンストラクタ
	Vector3D( float workX , float workY , float workZ ) : x( workX ) ,			//  コンストラクタ
														   y( workY ) ,
														   z( workZ ){ };	
	~Vector3D( );																//  デストラクタ
	Vector3D operator+( const Vector3D &vector3D );							//  オペレータ演算子+の定義
	Vector3D operator-( const Vector3D &vector3D );							//  オペレータ演算子-の定義
	Vector3D operator*( const Vector3D &vector3D );							//  オペレータ演算子*の定義
	Vector3D &operator=( const Vector3D &vector3D );							//  オペレータ演算子=の定義
	Vector3D &operator+=( const Vector3D &vector3D );							//  オペレータ演算子+=の定義
	Vector3D &operator-=( const Vector3D &vector3D );							//  オペレータ演算子-=の定義
	Vector3D &operator*=( const Vector3D &vector3D );							//  オペレータ演算子*=の定義

	float	x;
	float	y;
	float	z;
};

#endif

