//--------------------------------------------------------------------------------------
//  便利関数  ( utility.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "utility.h"
#include "manager.h"
#include "renderer.h"
#include <math.h>

//--------------------------------------------------------------------------------------
//  円と点との当たり判定をする関数
//--------------------------------------------------------------------------------------
bool Utility::HitCirclePoint( HIT_CIRCLE &circle , D3DXVECTOR2 &position )
{
	float fDistA = sqrtf( ( circle.position.x - position.x ) * ( circle.position.x - position.x ) + 
						  ( circle.position.y - position.y ) * ( circle.position.y - position.y ) );

	float fDistB = circle.fLength;

	if( fDistA <= fDistB )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  球と球との当たり判定をする関数
//--------------------------------------------------------------------------------------
bool Utility::HitSphere( HIT_SPHERE &sph1 , HIT_SPHERE &sph2 , float* pDist )
{
	float fDistA = sqrtf( ( sph2.position.x - sph1.position.x ) * ( sph2.position.x - sph1.position.x ) + 
				   ( sph2.position.y - sph1.position.y ) * ( sph2.position.y - sph1.position.y ) +
				   ( sph2.position.z - sph1.position.z ) * ( sph2.position.z - sph1.position.z ) );

	float fDistB = sqrtf( ( sph1.fLength + sph2.fLength ) * ( sph1.fLength + sph2.fLength ) );

	if( fDistA <= fDistB )
	{
		if( pDist != NULL )
		{
			*pDist = fabsf( fDistA - fDistB );
		}

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  球と点の当たり判定をする関数
//--------------------------------------------------------------------------------------
bool Utility::HitSpherePoint( HIT_SPHERE &sph1 , D3DXVECTOR3 position )
{
	if( ( sph1.position.x - position.x ) * ( sph1.position.x - position.x ) +
		( sph1.position.y - position.y ) * ( sph1.position.y - position.y ) +
		( sph1.position.z - position.z ) * ( sph1.position.z - position.z ) <
		sph1.fLength * sph1.fLength )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  2つのベクトルのなすラジアン角を求める関数
//--------------------------------------------------------------------------------------
float Utility::RadianOf2Vector( D3DXVECTOR3 &vec1 , D3DXVECTOR3 &vec2 )
{
	float fAngle = 0.0f;

	//  軸ベクトルとカメラベクトルからのcosθを求める
	fAngle = D3DXVec3Dot( &vec1 , &vec2 );

	//  cosθをラジアン角に変更
	fAngle = acosf( fAngle );

	return fAngle;
}

//--------------------------------------------------------------------------------------
//  2つのベクトルのなすラジアン角を求める関数
//--------------------------------------------------------------------------------------
float Utility::AngleOf2Vector( D3DXVECTOR3 &vec1 , D3DXVECTOR3 &vec2 )
{
	float fAngle = 0.0f;

	//  軸ベクトルとカメラベクトルからのcosθを求める
	fAngle = D3DXVec3Dot( &vec1 , &vec2 );

	//  cosθをラジアン角に変更
	fAngle = acosf( fAngle );

	//  角度に変更
	fAngle = D3DXToDegree( fAngle );

	return fAngle;
}

//--------------------------------------------------------------------------------------
//  視界に入ってるかどうか判定する関数
//--------------------------------------------------------------------------------------
bool Utility::IntoView( D3DXVECTOR3 vec1 , D3DXVECTOR3 vec2 , float fViewAngle )
{
	//  ローカル変数の宣言
	float fAngle = 0.0f;

	//  二つのベクトルの内積をとる
	fAngle = D3DXVec3Dot( &vec1 , &vec2 );

	//  その結果から二つのベクトルのなすラジアン角を求める
	fAngle = acosf( fAngle );

	if( fAngle <= fViewAngle && fAngle >= -fViewAngle )
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  フォグを設定する関数
//--------------------------------------------------------------------------------------
void Utility::SetupVertexFog( DWORD color , DWORD mode , BOOL useRange , float fStart , float fEnd , float fDensity )
{
	float fWorkStart = fStart;
	float fWorkEnd = fEnd;

	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  フォグ処理を有効化
	pDevice->SetRenderState( D3DRS_FOGENABLE , TRUE );

	//  フォグ色の設定
	pDevice->SetRenderState( D3DRS_FOGCOLOR , color );

	//  ライナーモードの場合
	if( D3DFOG_LINEAR == mode )
	{ 
		//  パラメータの設定
		pDevice->SetRenderState( D3DRS_FOGVERTEXMODE , mode );
		pDevice->SetRenderState( D3DRS_FOGSTART , *( DWORD* )( &fStart ) );
		pDevice->SetRenderState( D3DRS_FOGEND , *( DWORD* )( &fEnd ) );
	}
	else
	{
		//  パラメータの設定
		pDevice->SetRenderState( D3DRS_FOGVERTEXMODE , mode );
		pDevice->SetRenderState( D3DRS_FOGDENSITY , *( DWORD* )( &fDensity ) );
	}

	//  
	if( useRange == TRUE )
	{
		pDevice->SetRenderState( D3DRS_RANGEFOGENABLE , TRUE );
	}
}

//--------------------------------------------------------------------------------------
//  ベジェ曲線から座標を求める関数
//--------------------------------------------------------------------------------------
D3DXVECTOR2	Utility::BezierCurve2D( D3DXVECTOR2 p1 , D3DXVECTOR2 p2 , D3DXVECTOR2 controllPoint , float fTime )
{
	D3DXVECTOR2 result( 0.0f , 0.0f );

	result.x = ( 1.0f - fTime ) * ( 1.0f - fTime ) * p1.x + 2.0f * ( 1.0f - fTime ) * fTime * controllPoint.x +
			   fTime * fTime * p2.x;

	result.y = ( 1.0f - fTime ) * ( 1.0f - fTime ) * p1.y + 2.0f * ( 1.0f - fTime ) * fTime * controllPoint.y +
			   fTime * fTime * p2.y;

	return result;
}

//--------------------------------------------------------------------------------------
//  外積によって求められる垂直ベクトルから行列を作る関数
//--------------------------------------------------------------------------------------
D3DXMATRIX* Utility::GetLookAtMatrix( D3DXMATRIX* pMtx , D3DXVECTOR3* pPos , D3DXVECTOR3* pPosAt , D3DXVECTOR3* pVecUp )
{
	//  ローカル変数の宣言
	D3DXVECTOR3 X , Y , Z;

	//  Z軸方向のベクトルを求める
	Z = *pPosAt - *pPos;

	//  Zを単位ベクトル化
	D3DXVec3Normalize( &Z , &Z );

	//  上方向ベクトルとZ軸ベクトルの外積よりX軸ベクトルを求める
	D3DXVec3Cross( &X , D3DXVec3Normalize( &Y , pVecUp ) , &Z );

	//  Xを単位ベクトル化
	D3DXVec3Normalize( &X , &X );

	//  Yを単位ベクトル化
	D3DXVec3Normalize( &Y , D3DXVec3Cross( &Y , &Z , &X ) );

	//  1行目の代入
	pMtx->_11 = X.x;
	pMtx->_12 = X.y;
	pMtx->_13 = X.z;
	pMtx->_14 = 0;

	//  2行目の代入
	pMtx->_21 = Y.x;
	pMtx->_22 = Y.y;
	pMtx->_23 = Y.z;
	pMtx->_24 = 0;

	//  3行目の代入
	pMtx->_31 = Z.x;
	pMtx->_32 = Z.y;
	pMtx->_33 = Z.z;
	pMtx->_34 = 0;

	//  4行目の代入
	pMtx->_41 = 0.0f;
	pMtx->_42 = 0.0f;
	pMtx->_43 = 0.0f;
	pMtx->_44 = 1.0f;

	return pMtx;
}

//--------------------------------------------------------------------------------------
//  外積によって求められる垂直ベクトルから行列を作る関数
//--------------------------------------------------------------------------------------
D3DXMATRIX* Utility::GetFixedLookAtMatrix( D3DXMATRIX* pMtx , D3DXVECTOR3* pPos , D3DXVECTOR3* pPosAt , D3DXVECTOR3* pVecUp )
{
	//  ローカル変数の宣言
	D3DXVECTOR3 X , Y , Z , D;

	//  Z軸方向のベクトルを求める
	D = *pPosAt - *pPos;

	//  Zを単位ベクトル化
	D3DXVec3Normalize( &D , &D );

	//  上方向ベクトルとZ軸ベクトルの外積よりX軸ベクトルを求める
	D3DXVec3Cross( &X , D3DXVec3Normalize( &Y , pVecUp ) , &D );

	//  Xを単位ベクトル化
	D3DXVec3Normalize( &X , &X );

	//  Yを単位ベクトル化
	D3DXVec3Normalize( &Z , D3DXVec3Cross( &Z , &X , &Y ) );

	//  1行目の代入
	pMtx->_11 = X.x;
	pMtx->_12 = X.y;
	pMtx->_13 = X.z;
	pMtx->_14 = 0;

	//  2行目の代入
	pMtx->_21 = Y.x;
	pMtx->_22 = Y.y;
	pMtx->_23 = Y.z;
	pMtx->_24 = 0;

	//  3行目の代入
	pMtx->_31 = Z.x;
	pMtx->_32 = Z.y;
	pMtx->_33 = Z.z;
	pMtx->_34 = 0;

	//  4行目の代入
	pMtx->_41 = 0.0f;
	pMtx->_42 = 0.0f;
	pMtx->_43 = 0.0f;
	pMtx->_44 = 1.0f;

	return pMtx;
}