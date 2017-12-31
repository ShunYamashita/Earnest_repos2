//--------------------------------------------------------------------------------------
//  ライト  ( light.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _LIGHT_H_
#define _LIGHT_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MAX_LIGHT	( 3 )

//--------------------------------------------------------------------------------------
//  ライトクラスの定義
//--------------------------------------------------------------------------------------
class Light
{
public:
	Light( );																//  コンストラクタ
	~Light( );																//  デストラクタ

	HRESULT				Init( void );										//  初期化
	void				Uninit( void );										//  終了
	void				Update( void );										//  更新
	void				Draw( void );										//  描画

	void				SetDiffuseColor( D3DXCOLOR color ,					//  指定ライトの拡散光の設定
										 int nNo = 0 );
	void				LightOn( void );									//  ライト点ける
	void				LightOff( void );									//  ライト消す

	void				SetViewMatrix( D3DXVECTOR3 position ,				//	ビュー行列の設定
									   D3DXVECTOR3 positionAt );	
	void				SetProjectionMatrix( float fov ,					//	プロジェクション行列の設定	
											 float fNear ,
											 float fFar );	
	void				SetDistance( float distance );						//  距離の設定
	void				SetVectorDirection( D3DXVECTOR3 direction );		//  進行方向ベクトルの設定
	void				SetVectorDirectionX( float directionX );			//  進行方向Xベクトルの設定
	void				SetVectorDirectionY( float directionY );			//  進行方向Yベクトルの設定
	void				SetVectorDirectionZ( float directionZ );			//  進行方向Zベクトルの設定

	const D3DLIGHT9&	GetLight( int index );								//  指定番号のライト情報の取得
	const D3DXVECTOR3&	GetDirection( void );								//  ライトの方向ベクトルの取得
	const D3DXVECTOR3&	GetPosition( void );								//	座標の取得	
	const D3DXVECTOR3&	GetPositionAt( void );								//  注視点の取得
	const D3DXMATRIX&	GetViewMatrix( void );								//  ビュー行列の取得
	const D3DXMATRIX&	GetDirectionalViewMatrix( D3DXVECTOR3 position );	//	ビュー行列の設定	
	const D3DXMATRIX&	GetProjectionMatrix( void );						//  プロジェクション行列の取得
	const float&		GetFov( void );										//  画角の取得
	const float&		GetNear( void );									//  ニアの取得
	const float&		GetFar( void );										//  ファーの取得
	const float&		GetDistance( void );								//  距離の取得

private:
	LPD3DXMESH			m_pMeshModel;										//  メッシュ情報
	LPD3DXBUFFER		m_pBuffMatModel;									//  マテリアル情報
	DWORD				m_nNumMatModel;										//  マテリアル数
	D3DLIGHT9			m_light[ MAX_LIGHT ];								//  ライト
	D3DXVECTOR3			m_position;											//  座標
	D3DXVECTOR3			m_positionAt;										//  注視点座標
	D3DXVECTOR3			m_vectorDirection;									//  方向ベクトル
	float				m_distance;											//  距離
	float				m_fov;												//  画角
	float				m_near;												//  ニア
	float				m_far;												//  ファー
	D3DXMATRIX			m_viewMatrix;										//  ビュー行列
	D3DXMATRIX			m_projectionMatrix;									//  プロジェクション行列
	char				m_aTextureName[ 3 ][ 256 ];							//  テクスチャ名保存用
};

#endif

