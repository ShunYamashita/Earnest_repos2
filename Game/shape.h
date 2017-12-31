//--------------------------------------------------------------------------------------
//  形状  ( shape.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHAPE_H_
#define _SHAPE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  列挙型の定義
//--------------------------------------------------------------------------------------
typedef enum
{
	SHAPE_RECT = 0 ,			//  矩形
	SHAPE_CIRCLE ,				//  円
	SHAPE_SPHERE ,				//  球体
	SHAPE_MAX ,					//  最大
} SHAPE_TYPE;					//  形状の種類

//--------------------------------------------------------------------------------------
//  形状基底クラスの定義
//--------------------------------------------------------------------------------------
class Shape
{
public:
	Shape( ){ };													//  コンストラクタ
	virtual ~Shape( ){ };											//  デストラクタ

	virtual SHAPE_TYPE GetType( void ) const = 0;					//  種類の取得

private:

};

//--------------------------------------------------------------------------------------
//  円形状クラスの定義
//--------------------------------------------------------------------------------------
class CircleShape : public Shape
{
public:
	CircleShape( D3DXVECTOR3 position , 									//  コンストラクタ
				 float fRadius ) :
				 m_position( position ) , 
				 m_fRadius( fRadius ){ };
	virtual ~CircleShape( ){ };										//  デストラクタ

	SHAPE_TYPE	GetType( void ) const { return SHAPE_CIRCLE; }		//  種類の取得

	D3DXVECTOR3	GetPos( void ) const { return m_position; }				//  座標の取得
	float		GetRadius( void ) const { return m_fRadius; }		//  半径の取得

private:
    D3DXVECTOR3	m_position;												//  座標
    float		m_fRadius;											//  半径
};

//--------------------------------------------------------------------------------------
//  矩形形状クラスの定義
//--------------------------------------------------------------------------------------
class RectShape : public Shape
{
public:
	RectShape( D3DXVECTOR3 position ,									//  コンストラクタ
			   D3DXVECTOR3 size ) :
			   m_position( position ) ,
			   m_size( size ){ };
	virtual ~RectShape( ){ };										//  デストラクタ

	SHAPE_TYPE	GetType( void ) const { return SHAPE_RECT; }		//  種類の取得

	D3DXVECTOR3	GetPos( void ) const { return m_position; }				//  座標の取得
	D3DXVECTOR3	GetSize( void ) const { return m_size; }			//  大きさの取得

private:
	D3DXVECTOR3	m_position;												//  座標
	D3DXVECTOR3	m_size;												//  大きさ
};

#endif

