//--------------------------------------------------------------------------------------
//  デプスシャドウ   ( stencilShadow.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "cascadeShadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "shaderManager.h"
#include "light.h"
#include "camera.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9	CascadeShadow::m_cascadeShadowTexture[ MAX_CASCADE ] = { nullptr };		//  テクスチャ
LPDIRECT3DSURFACE9	CascadeShadow::m_cascadeShadowSurface[ MAX_CASCADE ] = { nullptr };		//  レンダーターゲット用サーフェイス
LPDIRECT3DSURFACE9	CascadeShadow::m_shadowMap = nullptr;									//  テクスチャ用の深度バッファー
float				CascadeShadow::m_bias = 0.000013f;										//  バイアス値
D3DXMATRIX			CascadeShadow::m_shadowMatrix[ MAX_CASCADE ];							//  シャドウ行列
float				CascadeShadow::m_splitPosition[ MAX_CASCADE ];							//  分割ごとの座標

const UINT			CascadeShadow::TEXTURE_WIDTH = 2048;									//  テクスチャの幅
const UINT			CascadeShadow::TEXTURE_HEIGHT = 2048;									//  テクスチャの高さ

//--------------------------------------------------------------------------------------
//  カスケードシャドウクラスのコンストラクタ
//--------------------------------------------------------------------------------------
CascadeShadow::CascadeShadow( )
{

}

//--------------------------------------------------------------------------------------
//  カスケードシャドウのデストラクタ
//--------------------------------------------------------------------------------------
CascadeShadow::~CascadeShadow( )
{

}

//--------------------------------------------------------------------------------------
//  カスケードシャドウの初期化処理
//--------------------------------------------------------------------------------------
HRESULT CascadeShadow::Init( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	for( int countCascade = 0; countCascade < MAX_CASCADE; countCascade++ )
	{
		//  レンダーターゲット用の空のテクスチャ生成
		D3DXCreateTexture( pDevice ,										//  デバイス
						   TEXTURE_WIDTH ,									//  スクリーンの幅
						   TEXTURE_HEIGHT ,									//  スクリーンの高さ
						   1 ,												//  ミップマップの数
						   D3DUSAGE_RENDERTARGET ,							//  使用用途
						   D3DFMT_A32B32G32R32F ,							//  色の要素
						   D3DPOOL_DEFAULT ,								//  メモリの配置方法
						   &m_cascadeShadowTexture[ countCascade ] );		//	テクスチャ格納場所

		//  サーフェイスの取得
		m_cascadeShadowTexture[ countCascade ]->GetSurfaceLevel( 0 , &m_cascadeShadowSurface[ countCascade ] );
	}

	// テクスチャへのレンダリングに使う深度バッファーの作成
	if( FAILED( pDevice->CreateDepthStencilSurface( TEXTURE_WIDTH ,
													TEXTURE_HEIGHT ,
													D3DFMT_D24S8,
													D3DMULTISAMPLE_NONE,
													0,
													TRUE,
													&m_shadowMap,
													NULL ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  カスケードシャドウの終了処理
//--------------------------------------------------------------------------------------
void CascadeShadow::Uninit( void )
{
	for( int countCascade = 0; countCascade < MAX_CASCADE; countCascade++ )
	{
		//  深度バッファ用のサーフェイスの破棄
		if( m_cascadeShadowSurface[ countCascade ] != nullptr )
		{
			m_cascadeShadowSurface[ countCascade ]->Release( );
			m_cascadeShadowSurface[ countCascade ] = nullptr;
		}

		//  深度バッファ用のテクスチャの破棄
		if( m_cascadeShadowTexture[ countCascade ] != nullptr )
		{
			m_cascadeShadowTexture[ countCascade ]->Release( );
			m_cascadeShadowTexture[ countCascade ] = nullptr;
		}
	}

	//  シャドウマップ用のサーフェイスの破棄
	if( m_shadowMap != nullptr )
	{
		m_shadowMap->Release( );
		m_shadowMap = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  カスケードシャドウの更新処理
//--------------------------------------------------------------------------------------
void CascadeShadow::Update( void )
{
	//  カメラ座標の取得
	D3DXVECTOR3 cameraPosition = SceneManager::GetCamera( )->GetCameraPosEye( );

	//  カメラ方向ベクトルの取得
	D3DXVECTOR3 cameraVectorDirection = SceneManager::GetCamera( )->GetCameraVecDirect( );

	//  フィールドサイズのAABB作成
	AABB aabb;
	aabb.min = Game::GetFieldMin( );
	aabb.min = Game::GetFieldMax( );

	//  near/far格納場所
	float nearValue = 1.0f;
	float farValue = 1.0f;

	AdjustClipPlane( aabb , cameraPosition , cameraVectorDirection , nearValue , farValue );

	//  分割数分の座標格納
	float splitPositions[ MAX_CASCADE + 1 ];
	SetSplitShadowPosition( MAX_CASCADE , 0.5f , nearValue , farValue , splitPositions );

	//  ライト情報の取得
	Light* light = SceneManager::GetLight( );

	//  カスケード処理
	for( int countSplit = 0; countSplit < MAX_CASCADE; countSplit++ )
	{
		//  ライトのビュープロジェクション行列
		m_shadowMatrix[ countSplit ] = light->GetViewMatrix( ) * light->GetProjectionMatrix( );

		//  AABBを計算する
		AABB workAABB = CalculateAABB( splitPositions[ countSplit ] , splitPositions[ countSplit + 1 ] , m_shadowMatrix[ countSplit ] );

		//  クロップ行列を求める
		D3DXMATRIX cropMatrix = CreateCropMatrix( workAABB );

		//  シャドウマップ行列と分割ごとの座標の設定
		m_shadowMatrix[ countSplit ] = m_shadowMatrix[ countSplit ] * cropMatrix;
		m_splitPosition[ countSplit ] = m_splitPosition[ countSplit + 1 ];
	}
}

//--------------------------------------------------------------------------------------
//  カスケードシャドウのバッファのクリア処理
//--------------------------------------------------------------------------------------
void CascadeShadow::DrawClearBuffer( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  バックバッファ＆Ｚバッファのクリア
	pDevice->Clear( 0 , NULL , ( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ) , D3DCOLOR_RGBA( 255 , 255 , 255 , 255 ) , 1.0f , 0 );
}

//--------------------------------------------------------------------------------------
//  レンダーターゲットの設定処理
//--------------------------------------------------------------------------------------
void CascadeShadow::SetRendererTarget( int index )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( m_cascadeShadowSurface != nullptr )
	{
		pDevice->SetRenderTarget( 0 , m_cascadeShadowSurface[ index ] );
	}
}

//--------------------------------------------------------------------------------------
//  深度バッファのサーフェースの設定処理
//--------------------------------------------------------------------------------------
void CascadeShadow::SetDepthSerface( void )
{
	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( m_shadowMap != nullptr )
	{
		//  テクスチャ用の深度バッファを設定
		pDevice->SetDepthStencilSurface( m_shadowMap );
	}
}

//--------------------------------------------------------------------------------------
//  バイアス値の設定処理
//--------------------------------------------------------------------------------------
void CascadeShadow::SetBias( float bias )
{
	m_bias = bias;
}

//--------------------------------------------------------------------------------------
//  バイアス値の取得処理
//--------------------------------------------------------------------------------------
float CascadeShadow::GetBias( void )
{
	return m_bias;
}

//--------------------------------------------------------------------------------------
//  レンダーターゲットのテクスチャ取得
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CascadeShadow::GetRendereTargetTexture( int index )
{
	return m_cascadeShadowTexture[ index ];
}

//--------------------------------------------------------------------------------------
//  シャドウ行列の取得
//--------------------------------------------------------------------------------------
const D3DXMATRIX& CascadeShadow::GetShadowMatrix( int index )
{
	return m_shadowMatrix[ index ];
}

//--------------------------------------------------------------------------------------
//  分割ごとの座標の取得
//--------------------------------------------------------------------------------------
const float& CascadeShadow::GetSplitPosition( int index )
{
	return m_splitPosition[ index ];
}

//--------------------------------------------------------------------------------------
//  平行分割シャドウマップの分割位置の計算
//--------------------------------------------------------------------------------------
void CascadeShadow::SetSplitShadowPosition( int numberSplit ,	
											float lamda , 
											float nearValue ,
											float farValue ,
											float* positions )
{
	//  分割数が1で、普通のシャドウマップの場合
	if( numberSplit == 1 )
	{
		positions[ 0 ] = nearValue;
		positions[ 1 ] = farValue;
	}

	float rateSplit = 1.0f / ( float )numberSplit;

	//  ゼロ徐算の防止
	if( nearValue == 0.0f )
	{
		return;
	}

	float farDivisionNear = farValue / nearValue;
	float farSubtractNear = farValue - nearValue;

	//  実用分割スキームの適用
	for( int countSplit = 1; countSplit < MAX_CASCADE + 1; ++countSplit )
	{
		//  対数分割スキーム計算
		float splitLog = nearValue * powf( farDivisionNear , rateSplit * countSplit );

		//  一様分割スキームの計算
		float splitUni = nearValue + farSubtractNear * countSplit * rateSplit;

		//  二つの値を線形補間
		positions[ countSplit ] = lamda * splitLog + splitUni * ( 1.0f - lamda );
	}

	//  最初と最後にnear/farの代入
	positions[ 0 ] = nearValue;
	positions[ numberSplit ] = farValue;
}

//--------------------------------------------------------------------------------------
//  クリップ平面の距離を調整
//--------------------------------------------------------------------------------------
void CascadeShadow::AdjustClipPlane( const AABB& aabb ,
									 const D3DXVECTOR3& position ,
									 const D3DXVECTOR3& vecDirect ,
									 float& nearValue , 
									 float& farValue )
{
	//  near/farの一時代入
	float minZ = nearValue;
	float maxZ = farValue;

	//  角座標
	D3DXVECTOR3 cornerPosition[ 8 ];

	//  フィールドの最小値と最大値から角の座標を代入
	cornerPosition[ 0 ] = D3DXVECTOR3( aabb.min.x , aabb.max.y , aabb.max.z );
	cornerPosition[ 1 ] = D3DXVECTOR3( aabb.max.x , aabb.max.y , aabb.max.z );
	cornerPosition[ 2 ] = D3DXVECTOR3( aabb.max.x , aabb.min.y , aabb.max.z );
	cornerPosition[ 3 ] = D3DXVECTOR3( aabb.min.x , aabb.min.y , aabb.max.z );
	cornerPosition[ 4 ] = D3DXVECTOR3( aabb.min.x , aabb.max.y , aabb.min.z );
	cornerPosition[ 5 ] = D3DXVECTOR3( aabb.max.x , aabb.max.y , aabb.min.z );
	cornerPosition[ 6 ] = D3DXVECTOR3( aabb.max.x , aabb.min.y , aabb.min.z );
	cornerPosition[ 7 ] = D3DXVECTOR3( aabb.min.x , aabb.min.y , aabb.min.z );

	//  角から最適なnear/far値の算出
	for( int countCorner = 0; countCorner < 8; countCorner++ )
	{
		//  カメラから角へのベクトルの算出
		D3DXVECTOR3 cameraPositionToCorner = cornerPosition[ countCorner ] - position;

		//  Z値の算出
		float workZ = D3DXVec3Dot( &cameraPositionToCorner , &vecDirect );

		//  最低と最大値を求める
		minZ = min( workZ , minZ );
		maxZ = max( workZ , maxZ );
	}

	//  最適なnear/far値の算出
	nearValue = max( minZ , nearValue );
	farValue = max( maxZ , nearValue + 1.0f );
}

//--------------------------------------------------------------------------------------
//  角からAABBを求める
//--------------------------------------------------------------------------------------
const CascadeShadow::AABB& CascadeShadow::CalculateAABB( const float nearValue ,
														 const float farValue ,
														 const D3DXMATRIX& viewProjection )
{
	//  カメラ情報の取得
	Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

	//  AABBの初期化
	AABB aabb;
	aabb.min = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	aabb.max = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	if( camera == nullptr )
	{
		return aabb;
	}

	//  3方向のベクトル軸の算出
	D3DXVECTOR3 vectorZ = camera->GetCameraVecDirect( );
	D3DXVec3Normalize( &vectorZ , &vectorZ );
	D3DXVECTOR3 vectorX;
	D3DXVec3Cross( &vectorX , &camera->GetCameraVectorUp( ) , &vectorZ );
	D3DXVec3Normalize( &vectorX , &vectorX );
	D3DXVECTOR3 vectorY;
	D3DXVec3Cross( &vectorY , &vectorZ , &vectorX );
	D3DXVec3Normalize( &vectorY , &vectorY );

	//  アスペクト比を求める
	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;

	//  カメラから画角情報の取得
	float fov = camera->GetCameraFov( );

	//  near/farの半分の幅と高さ、中央座標を求める
	float nearPlaneHalfHeight = tanf( fov * 0.5f ) * nearValue;
	float nearPlaneHalfWidth = nearPlaneHalfHeight * aspect;
	float farPlaneHalfHeight = tanf( fov * 0.5f ) * farValue;
	float farPlaneHalfWidth = farPlaneHalfHeight * aspect;
	D3DXVECTOR3 nearPlaneCenter = camera->GetCameraPosEye( ) + vectorZ * nearValue;
	D3DXVECTOR3 farPlaneCenter = camera->GetCameraPosEye( ) + vectorZ * farValue;

	//  8個の角
	D3DXVECTOR3 corners[ 8 ];

    corners[ 0 ] = D3DXVECTOR3( nearPlaneCenter - vectorX * nearPlaneHalfWidth - vectorY * nearPlaneHalfHeight );
    corners[ 1 ] = D3DXVECTOR3( nearPlaneCenter - vectorX * nearPlaneHalfWidth + vectorY * nearPlaneHalfHeight );
    corners[ 2 ] = D3DXVECTOR3( nearPlaneCenter + vectorX * nearPlaneHalfWidth + vectorY * nearPlaneHalfHeight );
    corners[ 3 ] = D3DXVECTOR3( nearPlaneCenter + vectorX * nearPlaneHalfWidth - vectorY * nearPlaneHalfHeight );
    corners[ 4 ] = D3DXVECTOR3( farPlaneCenter - vectorX * farPlaneHalfWidth - vectorY * farPlaneHalfHeight );
    corners[ 5 ] = D3DXVECTOR3( farPlaneCenter - vectorX * farPlaneHalfWidth + vectorY * farPlaneHalfHeight );
    corners[ 6 ] = D3DXVECTOR3( farPlaneCenter + vectorX * farPlaneHalfWidth + vectorY * farPlaneHalfHeight );
    corners[ 7 ] = D3DXVECTOR3( farPlaneCenter + vectorX * farPlaneHalfWidth - vectorY * farPlaneHalfHeight );

	//  画面上の座標
	D3DXVECTOR3 point;
	D3DXVec3TransformCoord( &point , &corners[ 0 ] , &viewProjection );

	//  最小値と最大値に一時代入
	D3DXVECTOR3 mini = point;
	D3DXVECTOR3 maxi = point;

	//  8角の中から最小値と最大値のAABBを求める
    for( int countCorner = 1; countCorner < 8; ++countCorner )
    {
		D3DXVec3TransformCoord( &point , &corners[ countCorner ] , &viewProjection );
        mini  = min( point, mini );
        maxi  = max( point, maxi );
    }

	//  AABBの代入
	aabb.min = mini;
	aabb.max = maxi;

	return aabb;
}

//--------------------------------------------------------------------------------------
//  クロップ行列の作成
//--------------------------------------------------------------------------------------
const D3DXMATRIX& CascadeShadow::CreateCropMatrix( const AABB& aabb )
{
	//  初期化
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;

	//  AABBから最小値と最大値の取得
	D3DXVECTOR3 mini = aabb.min;
	D3DXVECTOR3 maxi = aabb.max;

	scaleX = 2.0f / ( maxi.x - mini.x );
	scaleY = 2.0f / ( maxi.y - mini.y );

    offsetX = -0.5f * ( maxi.x + mini.x ) * scaleX;
    offsetY = -0.5f * ( maxi.y + mini.y ) * scaleY;

    //  1.0以下にならない様に調整
    scaleX = max( 1.0f, scaleX );
    scaleY = max( 1.0f, scaleY );

	//  クロップ行列の作成
	D3DXMATRIX cropMatrix;
	cropMatrix._11 = scaleX;
	cropMatrix._12 = 0.0f;
	cropMatrix._13 = 0.0f;
	cropMatrix._14 = 0.0f;
	cropMatrix._21 = 0.0f;
	cropMatrix._22 = scaleY;
	cropMatrix._23 = 0.0f;
	cropMatrix._24 = 0.0f;
	cropMatrix._31 = 0.0f;
	cropMatrix._32 = 0.0f;
	cropMatrix._33 = scaleZ;
	cropMatrix._34 = 0.0f;
	cropMatrix._41 = offsetX;
	cropMatrix._42 = offsetY;
	cropMatrix._43 = offsetZ;
	cropMatrix._44 = 1.0f;

    return cropMatrix;
}