//--------------------------------------------------------------------------------------
//  カメラ   ( camera.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "keyboard.h"
#include "game.h"
#include "shaderManager.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float CAMERA_TO_DISTANCE = 20.0f;

#define CAMERA_MOVE_SPEED	( 1.0f )
#define CAMERA_ROT_SPEED	( 0.02f )

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  カメラクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Camera::Camera( )
{
	//  メンバ変数の初期化
	m_posEye = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_posAt = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_vecUp = D3DXVECTOR3( 0.0f, 1.0f , 0.0f );
	m_vecDirect = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_basePos = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_seekPos = D3DXVECTOR2( 0.0f, 0.0f );
	m_fFovY = 0.0f;
	m_fDistance = CAMERA_TO_DISTANCE;
	m_fReviseEye = 1.0f;
	m_fReviseAt = 1.0f;
	m_fRotY = 0.0f;
	m_fRotX = 0.0f;
	m_fSwaySide = 0.0f;
	m_fSwayVertical = 0.0f;
	m_fSwayAngle = 0.0f;
	m_nMode = 0;
	m_bFree = false;
	m_bDivide = false;
	m_nCameraNo = 0;
}

//--------------------------------------------------------------------------------------
//  カメラクラスのデストラクタ
//--------------------------------------------------------------------------------------
Camera::~Camera( )
{

}

//--------------------------------------------------------------------------------------
//  カメラの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Camera::Init( D3DXVECTOR3 posEye , D3DXVECTOR3 posAt , float fFovY , float fNear , float fFar , int nCameraNo , bool bDivide )
{
	LPDIRECT3DDEVICE9	pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  視点座標の代入
	m_posEye = posEye;

	//  注視点座標の代入
	m_posAt = posAt;

	//  画角の代入
	m_fFovY = fFovY;

	//  カメラ番号の代入
	m_nCameraNo = nCameraNo;

	//  分割フラグの代入
	m_bDivide = bDivide;

	//  視野距離最低値の代入
	m_fNear = fNear;

	//  視野距離最大値の代入
	m_fFar = fFar;

	m_bFree = false;

	if( bDivide == false )
	{
		//  プロジェクション行列の作成
		D3DXMatrixPerspectiveFovLH( &m_projectionMatrix ,								//  プロジェクション行列のアドレス
									m_fFovY ,											//  画角
									( float )SCREEN_WIDTH / SCREEN_HEIGHT ,				//  アスペクト比
									fNear ,												//  near( 0.0f < near )
									fFar );												//  far
	}
	else
	{
		//  プロジェクション行列の作成
		D3DXMatrixPerspectiveFovLH( &m_projectionMatrix ,								//  プロジェクション行列のアドレス
									m_fFovY ,											//  画角
									( float )( SCREEN_WIDTH / 2 ) / ( SCREEN_HEIGHT ) ,	//  アスペクト比
									fNear ,												//  near( 0.0f < near )
									fFar );												//  far
	}

	//  ビュー座標変換行列の設定
	D3DXMatrixLookAtLH( &m_viewMatrix , &m_posEye , &m_posAt , &m_vecUp );

	//  基準値の代入
	m_basePosEye = posEye;
	m_basePosAt = posAt;

	if( m_nCameraNo == 0 )
	{
		m_vecDirect = D3DXVECTOR3( 0.0f, 0.0f , 1.0f );
	}
	else if( m_nCameraNo == 1 )
	{
		m_vecDirect = D3DXVECTOR3( 0.0f, 0.0f , -1.0f );
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  カメラの終了処理
//--------------------------------------------------------------------------------------
void Camera::Uninit( void )
{
	LPDIRECT3DDEVICE9	pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DVIEWPORT9 vp;

	vp.X = 0;
	vp.Y = 0;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	//  ビューポート変換の設定
	pDevice->SetViewport( &vp );
}

//--------------------------------------------------------------------------------------
//  カメラの更新処理
//--------------------------------------------------------------------------------------
void Camera::Update( void )
{
	//  ローカル変数の宣言
	D3DXVECTOR3			work;						//  格納用
	D3DXVECTOR3			work2;						//  格納用2
	D3DXMATRIX			mtxView;					//  ビュー座標変換行列
	D3DXMATRIX			mtxProj;					//  プロジェクション座標行列

	LPDIRECT3DDEVICE9	pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  画角の代入
	m_fFovY += ( ( D3DX_PI / 3.0f ) - m_fFovY ) * 0.01f;

	//  フリーカメラモードではない場合
	if( m_bFree == false )
	{
		//  視点座標に補正をかける
		m_posEye += ( m_basePosEye - m_posEye ) * m_fReviseEye;

		//  注視点座標に補正をかける
		m_posAt += ( m_basePosAt - m_posAt ) * m_fReviseAt;

		//  注視点と視点座標の一時代入
		work = m_posAt;
		work.y = 0.0f;
		work2 = m_posEye;
		work2.y = 0.0f;

		//  カメラの方向ベクトルの算出( 視野角カリング用 )
		D3DXVec3Normalize( &m_vecDirect , &( work - work2 ) );

		//  ずらし量分注視点をずらす
		m_posAt.y = m_posAt.y + m_seekPos.y;

		if( m_fSwaySide != 0.0f || m_fSwayVertical != 0.0f )
		{
			m_seekPos.x = sinf( m_fSwayAngle ) * m_fSwaySide;
			m_seekPos.y = sinf( m_fSwayAngle ) * m_fSwayVertical;

			m_fSwayAngle += 0.8f;

			if( m_fSwayAngle > D3DX_PI )
			{
				m_fSwayAngle = -D3DX_PI;

				m_fSwaySide *= 0.3f;
				m_fSwayVertical *= 0.3f;

				if( m_fSwaySide < 0.01f )
				{
					m_fSwaySide = 0.0f;
				}
				if( m_fSwayVertical < 0.01f )
				{
					m_fSwayVertical = 0.0f;
				}

				m_nSwayTime = 0;
			}

			m_nSwayTime++;
		}
		else
		{
			m_fSwayAngle = 0.0f;
			m_nSwayTime = 0;
		}
	}
	else
	{
		//  注視点と視点座標の一時代入
		work = m_posAt;
		work.y = 0.0f;
		work2 = m_posEye;
		work2.y = 0.0f;

		//  カメラの方向ベクトルの算出( 視野角カリング用 )
		D3DXVec3Normalize( &m_vecDirect , &( work - work2 ) );

		// キーボード情報の取得
		Keyboard* pKeyboard = SceneManager::GetKeyboard( );
		float fAxisX = 0.0f;
		float fAxisY = 0.0f;

		//  Wキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_UP ) )
		{
			//  X座標を方向ベクトル×カメラ移動スピード分を加える
			m_posEye.x += m_vecDirect.x * CAMERA_MOVE_SPEED;
			m_posAt.x += m_vecDirect.x * CAMERA_MOVE_SPEED;

			//  Z座標を方向ベクトル×カメラ移動スピード分を加える
			m_posEye.z += m_vecDirect.z * CAMERA_MOVE_SPEED;
			m_posAt.z += m_vecDirect.z * CAMERA_MOVE_SPEED;
		}
		//  Sキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_DOWN ) )
		{
			//  X座標を方向ベクトル×カメラ移動スピード分を減らす
			m_posEye.x -= m_vecDirect.x * CAMERA_MOVE_SPEED;
			m_posAt.x -= m_vecDirect.x * CAMERA_MOVE_SPEED;

			//  Z座標を方向ベクトル×カメラ移動スピード分を減らす
			m_posEye.z -= m_vecDirect.z * CAMERA_MOVE_SPEED;
			m_posAt.z -= m_vecDirect.z * CAMERA_MOVE_SPEED;
		}
		//  Aキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_LEFT ) )
		{
			//  X座標を方向ベクトル×カメラ移動スピード分を減らす
			m_posEye.x -= m_vecDirect.z * CAMERA_MOVE_SPEED;
			m_posAt.x -= m_vecDirect.z * CAMERA_MOVE_SPEED;

			//  Z座標を方向ベクトル×カメラ移動スピード分を減らす
			m_posEye.z += m_vecDirect.x * CAMERA_MOVE_SPEED;
			m_posAt.z += m_vecDirect.x * CAMERA_MOVE_SPEED;
		}
		//  Dキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_RIGHT ) )
		{
			//  X座標を方向ベクトル×カメラ移動スピード分を減らす
			m_posEye.x += m_vecDirect.z * CAMERA_MOVE_SPEED;
			m_posAt.x += m_vecDirect.z * CAMERA_MOVE_SPEED;

			//  Z座標を方向ベクトル×カメラ移動スピード分を減らす
			m_posEye.z -= m_vecDirect.x * CAMERA_MOVE_SPEED;
			m_posAt.z -= m_vecDirect.x * CAMERA_MOVE_SPEED;
		}
		//  Tキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_T ) )
		{
			//  カメラのY軸移動
			m_posEye.y += CAMERA_MOVE_SPEED * 0.2f;
		}
		//  Bキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_B ) )
		{
			//  カメラのY軸移動
			m_posEye.y -= CAMERA_MOVE_SPEED * 0.2f;
		}
		//  Yキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_Y ) )
		{
			//  注視点のY軸移動
			m_posAt.y += CAMERA_MOVE_SPEED * 0.2f;
		}
		//  Nキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_N ) )
		{
			//  注視点のY軸移動
			m_posAt.y -= CAMERA_MOVE_SPEED * 0.2f;
		}

		float fAngle = 0.0f;

		//  Zキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_O ) )
		{
			//  角度の算出
			fAngle = atan2f( ( m_posEye.z - m_posAt.z ) , ( m_posEye.x - m_posAt.x ) );

			//  角度の更新
			fAngle += CAMERA_ROT_SPEED;

			//  座標の更新
			m_posEye.x = cosf( fAngle ) * CAMERA_TO_DISTANCE + m_posAt.x;
			m_posEye.z = sinf( fAngle ) * CAMERA_TO_DISTANCE + m_posAt.z;
		}
		//  Cキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_P ) )
		{
			//  角度の算出
			fAngle = atan2f( ( m_posEye.z - m_posAt.z ) , ( m_posEye.x - m_posAt.x ) );

			//  角度の更新
			fAngle -= CAMERA_ROT_SPEED;

			//  座標の更新
			m_posEye.x = cosf( fAngle ) * CAMERA_TO_DISTANCE + m_posAt.x;
			m_posEye.z = sinf( fAngle ) * CAMERA_TO_DISTANCE + m_posAt.z;
		}

		//  Qキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_Q ) )
		{
			//  角度の算出
			fAngle = atan2f( ( m_posAt.z - m_posEye.z ) , ( m_posAt.x - m_posEye.x ) );

			//  角度の更新
			fAngle += CAMERA_ROT_SPEED;

			//  座標の更新
			m_posAt.x = cosf( fAngle ) * CAMERA_TO_DISTANCE + m_posEye.x;
			m_posAt.z = sinf( fAngle ) * CAMERA_TO_DISTANCE + m_posEye.z;
		}
		//  Eキーが押された場合
		if( pKeyboard->GetKeyboardPress( DIK_E ) )
		{
			//  角度の算出
			fAngle = atan2f( ( m_posAt.z - m_posEye.z ) , ( m_posAt.x - m_posEye.x ) );

			//  角度の更新
			fAngle -= CAMERA_ROT_SPEED;

			//  座標の更新
			m_posAt.x = cosf( fAngle ) * CAMERA_TO_DISTANCE + m_posEye.x;
			m_posAt.z = sinf( fAngle ) * CAMERA_TO_DISTANCE + m_posEye.z;
		}
	}
}

//--------------------------------------------------------------------------------------
//  座標を設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetCameraPosEye( D3DXVECTOR3 posEye , float fRevice )
{
	m_basePosEye = posEye;

	m_fReviseEye = fRevice;
}

//--------------------------------------------------------------------------------------
//  注視点を設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetCameraPosAt( D3DXVECTOR3 posAt , float fRevice )
{
	m_basePosAt = posAt;

	m_fReviseAt = fRevice;
}

//--------------------------------------------------------------------------------------
//  座標を設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetCameraPosEyeDirect( D3DXVECTOR3 posEye )
{
	m_posEye = posEye;

	m_basePosEye = posEye;
}

//--------------------------------------------------------------------------------------
//  注視点を設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetCameraPosAtDirect( D3DXVECTOR3 posAt )
{
	m_posAt = posAt;

	m_basePosAt = posAt;
}

//--------------------------------------------------------------------------------------
//  横揺れを設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetSwaySide( float fSwaySide )
{
	m_fSwaySide = fSwaySide;
}

//--------------------------------------------------------------------------------------
//  縦揺れを設定する関数
//--------------------------------------------------------------------------------------
void Camera::SetSwayVertical( float fSwayVertical )
{
	m_fSwayVertical = fSwayVertical;
}

//--------------------------------------------------------------------------------------
//  座標を移動する関数
//--------------------------------------------------------------------------------------
void Camera::MoveCameraPosEye( D3DXVECTOR3 moveEye , float fRevice )
{
	m_basePosEye = m_basePosEye + moveEye;

	m_fReviseEye = fRevice;
}

//--------------------------------------------------------------------------------------
//  注視点を移動する関数
//--------------------------------------------------------------------------------------
void Camera::MoveCameraPosAt( D3DXVECTOR3 moveAt , float fRevice )
{
	m_basePosAt = m_basePosAt + moveAt;

	m_fReviseAt = fRevice;
}

//--------------------------------------------------------------------------------------
//  ビュー行列を取得する関数
//--------------------------------------------------------------------------------------
const D3DXMATRIX& Camera::GetViewMatrix( void )
{
	return m_viewMatrix;
}

//--------------------------------------------------------------------------------------
//  プロジェクション行列を取得する関数
//--------------------------------------------------------------------------------------
const D3DXMATRIX& Camera::GetProjectionMatrix( void )
{
	return m_projectionMatrix;
}

//--------------------------------------------------------------------------------------
//  視点座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraPosEye( void )
{
	return m_posEye;
}

//--------------------------------------------------------------------------------------
//  注視点座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraPosAt( void )
{
	return m_posAt;
}

//--------------------------------------------------------------------------------------
//  視点座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraBasePosEye( void )
{
	return m_basePosEye;
}

//--------------------------------------------------------------------------------------
//  注視点座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraBasePosAt( void )
{
	return m_basePosAt;
}

//--------------------------------------------------------------------------------------
//  方向ベクトルを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Camera::GetCameraVecDirect( void )
{
	return m_vecDirect;
}

//--------------------------------------------------------------------------------------
//  カメラと対峙する回転行列を取得する関数
//--------------------------------------------------------------------------------------
D3DXMATRIX Camera::GetInvRotateMat( D3DXVECTOR3 position )
{
	D3DXMATRIX mtxInv;								//  回転行列格納用

	//  単位行列に変換
	D3DXMatrixIdentity( &mtxInv );

	//  ビュー行列作成関数から回転行列の作成
	D3DXMatrixLookAtLH( &mtxInv , &D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , &( m_posEye - position ) , &m_vecUp );

	//  回転行列の逆行列を転置行列で求める
	D3DXMatrixTranspose( &mtxInv , &mtxInv );

	return mtxInv;
}

//--------------------------------------------------------------------------------------
//  カメラモードの切り替え関数
//--------------------------------------------------------------------------------------
void Camera::ChangeMode( void )
{
	m_bFree = !m_bFree;
}

//--------------------------------------------------------------------------------------
//  カメラ行列の設定をする関数
//--------------------------------------------------------------------------------------
void Camera::SetCamera( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  モードゲーム以外または、CPU対戦の場合
	if( ( Game::GetModeVS( ) == Game::MODE_VS_CPU && Mode::MODE::GAME ) ||
		SceneManager::GetMode( ) != Mode::MODE::GAME )
	{
		D3DVIEWPORT9 vp;

		vp.X = 0;
		vp.Y = 0;
		vp.Width = SCREEN_WIDTH;
		vp.Height = SCREEN_HEIGHT;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;

		//  ビューポート変換の設定
		pDevice->SetViewport( &vp );
	}
	else
	{
		D3DVIEWPORT9 vp;

		vp.X = ( SCREEN_WIDTH / 2 ) * m_nCameraNo;
		vp.Y = 0;
		vp.Width = SCREEN_WIDTH / 2;
		vp.Height = SCREEN_HEIGHT;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;

		//  ビューポート変換の設定
		pDevice->SetViewport( &vp );
	}

	//  ビュー座標変換行列の設定
	D3DXMatrixLookAtLH( &m_viewMatrix , &m_posEye , &m_posAt , &m_vecUp );

	if( m_bDivide == false )
	{
		//  プロジェクション行列の作成
		D3DXMatrixPerspectiveFovLH( &m_projectionMatrix ,								//  プロジェクション行列のアドレス
									m_fFovY ,											//  画角
									( float )SCREEN_WIDTH / SCREEN_HEIGHT ,				//  アスペクト比
									m_fNear ,											//  near( 0.0f < near )
									m_fFar );											//  far
	}
	else
	{
		//  プロジェクション行列の作成
		D3DXMatrixPerspectiveFovLH( &m_projectionMatrix ,								//  プロジェクション行列のアドレス
									m_fFovY ,											//  画角
									( float )( SCREEN_WIDTH / 2 ) / ( SCREEN_HEIGHT ) ,	//  アスペクト比
									m_fNear ,											//  near( 0.0f < near )
									m_fFar );											//  far
	}
}

//--------------------------------------------------------------------------------------
//  ビューポート行列の設定をする関数
//--------------------------------------------------------------------------------------
void Camera::SetViewport( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  モードゲーム以外または、CPU対戦の場合
	if( ( Game::GetModeVS( ) == Game::MODE_VS_CPU && Mode::MODE::GAME ) ||
		SceneManager::GetMode( ) != Mode::MODE::GAME )
	{
		D3DVIEWPORT9 vp;

		vp.X = 0;
		vp.Y = 0;
		vp.Width = SCREEN_WIDTH;
		vp.Height = SCREEN_HEIGHT;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;

		//  ビューポート変換の設定
		pDevice->SetViewport( &vp );
	}
	else
	{
		D3DVIEWPORT9 vp;

		vp.X = ( SCREEN_WIDTH / 2 ) * m_nCameraNo;
		vp.Y = 0;
		vp.Width = SCREEN_WIDTH / 2;
		vp.Height = SCREEN_HEIGHT;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;

		//  ビューポート変換の設定
		pDevice->SetViewport( &vp );
	}
}