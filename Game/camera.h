//--------------------------------------------------------------------------------------
//  カメラ  ( camera.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  カメラクラスの定義
//--------------------------------------------------------------------------------------
class Camera
{
public:
	Camera( );															//  コンストラクタ
	~Camera( );														//  デストラクタ

	HRESULT				Init( D3DXVECTOR3 posEye ,						//  初期化
							  D3DXVECTOR3 posAt , 
							  float fFovY , 
							  float fNear ,
							  float fFar , 
							  int nCameraNo = 0 ,
							  bool bDivide = false );
	void				Uninit( void );									//  終了
	void				Update( void );									//  更新

	void				SetCameraPosEye( D3DXVECTOR3 posEye ,			//  視点座標の設定
										 float fRevice );		
	void				SetCameraPosAt( D3DXVECTOR3 posAt ,				//  注視点座標の設定
										float fRevice );
	void				SetCameraPosEyeDirect( D3DXVECTOR3 posEye );	//  視点座標の設定		
	void				SetCameraPosAtDirect( D3DXVECTOR3 posAt );		//  注視点座標の設定
	void				MoveCameraPosEye( D3DXVECTOR3 moveEye ,			//  視点座標を動かす
										  float fRevice );
	void				MoveCameraPosAt( D3DXVECTOR3 moveAt ,			//  注視点座標を動かす
										 float fRevice );
	void				ChangeMode( void );								//  モードの切り替え

	void				SetSwaySide( float fSwaySide );					//  横揺れの設定
	void				SetSwayVertical( float fSwayVertical );			//  縦揺れの設定

	const D3DXMATRIX&	GetViewMatrix( void );							//  ビュー行列の取得
	const D3DXMATRIX&	GetProjectionMatrix( void );					//  プロジェクション行列の取得
	D3DXVECTOR3			GetCameraPosEye( void );						//  視点座標の取得
	D3DXVECTOR3			GetCameraPosAt( void );							//  注視点座標の取得
	D3DXVECTOR3			GetCameraVecDirect( void );						//  方向ベクトルの取得
	D3DXVECTOR3			GetCameraBasePosEye( void );					//  目標視点座標の取得
	D3DXVECTOR3			GetCameraBasePosAt( void );						//  目標注視点座標の取得

	D3DXMATRIX			GetInvRotateMat( D3DXVECTOR3 position );		//  ビルボード用の行列取得

	void				SetCamera( void );								//  カメラ行列の設定
	void				SetViewport( void );							//  ビューポート行列の設定

private:
	D3DXMATRIX			m_viewMatrix;									//  ビュー行列
	D3DXMATRIX			m_projectionMatrix;								//  プロジェクション行列
	D3DXVECTOR3			m_posEye;										//  視点の座標
	D3DXVECTOR3			m_posAt;										//  注視点の座標
	D3DXVECTOR3			m_basePosEye;									//  視点の基準座標
	D3DXVECTOR3			m_basePosAt;									//  注視点の基準座標
	D3DXVECTOR3			m_vecUp;										//  上向き方向
	D3DXVECTOR3			m_vecDirect;									//  方向ベクトル
	D3DXVECTOR3			m_basePos;										//  基準座標
	D3DXVECTOR2			m_seekPos;										//  座標ずらし量
	float				m_fSwaySide;									//  横振動
	float				m_fSwayVertical;								//  縦振動
	float				m_fFovY;										//  視野角
	float				m_fDistance;									//  距離
	float				m_fReviseEye;									//  カメラ補正値( 視点用 )
	float				m_fReviseAt;									//  カメラ補正値( 注視点用 )
	float				m_fRotY;										//  Y軸回転角度
	float				m_fRotX;										//  X軸回転角度
	float				m_fSwayAngle;									//  揺れ角度
	float				m_fNear;										//  視野距離最低値
	float				m_fFar;											//  視野距離最大値
	int					m_nSwayTime;									//  揺れ時間カウント
	int					m_nMode;										//  カメラモード( 0 : フリーカメラ , 1 : 補正カメラ )
	int					m_nCameraNo;									//  カメラ番号
	bool				m_bFree;										//  フリーカメラモード
	bool				m_bDivide;										//  分割しているかのフラグ
};

#endif

