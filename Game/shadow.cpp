//--------------------------------------------------------------------------------------
//  DirectX   ( shadow.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2016/06/07
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "shadow.h"
#include "camera.h"
#include "player.h"
#include "light.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include <random>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define SHADOW_TEXTURENAME000		"data/TEXTURE/EFFECT/effect000.jpg"		//  テクスチャ名

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
Shadow::Shadow( int nPriority ) : Scene3D( nPriority )
{

}

//--------------------------------------------------------------------------------------
//  説明文の初期化処理をする関数
//--------------------------------------------------------------------------------------
HRESULT Shadow::Init( void )
{
	//  3Dポリゴンの初期化
	Scene3D::Init( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  説明文の終了処理をする関数
//--------------------------------------------------------------------------------------
void Shadow::Uninit( void )
{
	//  3Dポリゴンの解放
	Scene3D::Uninit( );
}

//--------------------------------------------------------------------------------------
//  説明文の更新処理をする関数
//--------------------------------------------------------------------------------------
void Shadow::Update( void )
{
	//  3Dポリゴンの更新
	Scene3D::Update( );
}

//--------------------------------------------------------------------------------------
//  説明文の描画処理をする関数
//--------------------------------------------------------------------------------------
void Shadow::Draw( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	pDevice->SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_REVSUBTRACT );			//  αブレンド設定( 減算処理 )
	pDevice->SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );				//  αブレンド設定( 減算処理 )
	pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );					//  αブレンド設定( 加算処理 )

	//  Zバッファの書き込み設定
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE , FALSE );

	//  3Dポリゴンの描画
	Scene3D::Draw( );

	//  Zバッファの書き込み設定
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

	pDevice->SetRenderState( D3DRS_BLENDOP , D3DBLENDOP_ADD );					//  αブレンド設定( 半透過処理 )
	pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );			//  αブレンド設定( 半透過処理 )
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Shadow* Shadow::Create( D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR3 normal , D3DXVECTOR3 rot , D3DXCOLOR color )
{
	Shadow* pShadow;

	//  インスタンス生成
	pShadow = new Shadow( 6 );

	//  座標の代入
	pShadow->m_position = position;

	//  大きさの代入
	pShadow->m_size = size;

	//  回転の代入
	pShadow->m_rot = rot;

	//  色の代入
	pShadow->m_color = color;

	//  テクスチャの設定
	pShadow->SetTextureName( SHADOW_TEXTURENAME000 );

	//  初期化
	pShadow->Init( );

	return pShadow;
}