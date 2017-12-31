//--------------------------------------------------------------------------------------
//  便利関数  ( utility.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _UTILITY_H_
#define _UTILITY_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  プレイヤークラスの定義
//--------------------------------------------------------------------------------------
class Utility
{
public:
	typedef struct
	{
		D3DXVECTOR3 position;						//  座標
		float		fLength;					//  半径
	} HIT_SPHERE;

	typedef struct
	{
		D3DXVECTOR2 position;						//  座標
		float		fLength;					//  半径
	} HIT_CIRCLE;

	typedef struct
	{
		D3DXVECTOR3 centerPos;					//  中心点の座標
		D3DXVECTOR3 vecDirect[ 3 ];				//  XYZの各座標軸の傾きを表す方向ベクトル
		D3DXVECTOR3 halfLength;					//  OBBの各座標軸に沿った半分の長さ( 中心点から面までの長さ )
	} OBB;

	typedef struct
	{
		D3DXVECTOR3 position;					//  XYZ座標
		D3DXVECTOR3 normal;						//  法線
		float tu , tv;
	} VERTEX;

	Utility( ){ };								//  コンストラクタ
	~Utility( ){ };								//  デストラクタ

	static float		RadianOf2Vector( D3DXVECTOR3 &vec1 , D3DXVECTOR3 &vec2 );
	static float		AngleOf2Vector( D3DXVECTOR3 &vec1 , D3DXVECTOR3 &vec2 );
	static bool			HitCirclePoint( HIT_CIRCLE &circle , D3DXVECTOR2 &position );
	static bool			HitSphere( HIT_SPHERE &sph1 , HIT_SPHERE &sph2 , float* pDist = NULL );
	static bool			HitSpherePoint( HIT_SPHERE &sph1 , D3DXVECTOR3 position );
	static bool			IntoView( D3DXVECTOR3 vec1 , D3DXVECTOR3 vec2 , float fViewAngle );
	static void			SetupVertexFog( DWORD color , DWORD mode , BOOL useRange , float fStart , float fEnd , float fDensity );
	static D3DXMATRIX*	GetFixedLookAtMatrix( D3DXMATRIX* pMtx ,
											  D3DXVECTOR3* pPos ,
											  D3DXVECTOR3* pPosAt ,
											  D3DXVECTOR3* pVecUp );

	static D3DXVECTOR2	BezierCurve2D( D3DXVECTOR2 p1 , D3DXVECTOR2 p2 , D3DXVECTOR2 controllPoint , float fTime );

private:

};

#endif

