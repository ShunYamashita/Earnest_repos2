//--------------------------------------------------------------------------------------
//  2Dレンダリング処理   ( goalScene2D.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include "selectVSMode.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define SOLO_UI_TEXTURE_NAME			"data/TEXTURE/STAGESELECT/solo_UI.png"			//  テクスチャ名
#define PARTY_UI_TEXTURE_NAME			"data/TEXTURE/STAGESELECT/party_UI.png"			//  テクスチャ名
#define TUTORIAL_UI_TEXTURE_NAME		"data/TEXTURE/STAGESELECT/tutorial_UI.png"		//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SelectVSMode::SelectVSMode( )
{
	//  情報の代入
	m_type = TYPE_MAX;
	m_position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	m_posUV = D3DXVECTOR2( 0.0f , 0.0f );
	m_divideUV = D3DXVECTOR2( 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SelectVSMode::SelectVSMode( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV , D3DXCOLOR color )
{
	//  情報の代入
	m_type = type;
	m_position = position;
	m_size = size;
	m_color = color;
	m_posUV = posUV;
	m_divideUV = divideUV;
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのデストラクタ
//--------------------------------------------------------------------------------------
SelectVSMode::~SelectVSMode( )
{

}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SelectVSMode::Init( void )
{
	Scene2D::Init( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの終了処理
//--------------------------------------------------------------------------------------
void SelectVSMode::Uninit( void )
{
	Scene2D::Uninit( );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの更新処理
//--------------------------------------------------------------------------------------
void SelectVSMode::Update( void )
{
	Scene2D::Update( );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの描画処理
//--------------------------------------------------------------------------------------
void SelectVSMode::Draw( void )
{
	Scene2D::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
SelectVSMode* SelectVSMode::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV )
{
	SelectVSMode *pSelectVSMode;

	//  インスタンス生成
	pSelectVSMode = new SelectVSMode;

	//  座標の代入
	pSelectVSMode->m_position = position;

	//  大きさの代入
	pSelectVSMode->m_size = size;

	//  UV座標開始地点の代入
	pSelectVSMode->m_posUV = posUV;

	//  テクスチャUV分割数の代入
	pSelectVSMode->m_divideUV = divideUV;

	//  種類によっての場合分け
	switch( type )
	{
		case SelectVSMode::TYPE_SOLO:
		{
			//  テクスチャ名の設定
			pSelectVSMode->SetTextureName( SOLO_UI_TEXTURE_NAME );

			break;
		}

		case SelectVSMode::TYPE_PARTY:
		{
			//  テクスチャ名の設定
			pSelectVSMode->SetTextureName( PARTY_UI_TEXTURE_NAME );

			break;
		}

		case SelectVSMode::TYPE_TUTORIAL:
		{
			//  テクスチャ名の設定
			pSelectVSMode->SetTextureName( TUTORIAL_UI_TEXTURE_NAME );

			break;
		}
	}

	//  初期化
	pSelectVSMode->Init( );

	return pSelectVSMode;
}