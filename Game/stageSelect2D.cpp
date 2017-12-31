//--------------------------------------------------------------------------------------
//  2Dレンダリング処理   ( stageSelect2D.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include "stageSelect2D.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define STAGE1_TEXTURE_NAME				"data/TEXTURE/stage000.PNG"			//  テクスチャ名
#define STAGE2_TEXTURE_NAME				"data/TEXTURE/stage001.PNG"			//  テクスチャ名
#define STAGE3_TEXTURE_NAME				"data/TEXTURE/stage3.png"			//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
StageSelect2D::StageSelect2D( )
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
StageSelect2D::StageSelect2D( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV , D3DXCOLOR color )
{
	//  情報の代入
	m_type = type;
	m_position = position;
	m_size = size;
	m_color = color;
	m_posUV = posUV;
	m_divideUV = divideUV;

	//  種類によっての場合分け
	switch( type )
	{
		case StageSelect2D::TYPE_STAGE1:
		{
			//  テクスチャ名の設定
			SetTextureName( STAGE1_TEXTURE_NAME );

			break;
		}

		case StageSelect2D::TYPE_STAGE2:
		{
			//  テクスチャ名の設定
			SetTextureName( STAGE2_TEXTURE_NAME );

			break;
		}

		case StageSelect2D::TYPE_STAGE3:
		{
			//  テクスチャ名の設定
			SetTextureName( STAGE3_TEXTURE_NAME );

			break;
		}
	}
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのデストラクタ
//--------------------------------------------------------------------------------------
StageSelect2D::~StageSelect2D( )
{

}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの初期化処理
//--------------------------------------------------------------------------------------
HRESULT StageSelect2D::Init( void )
{
	Scene2D::Init( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの終了処理
//--------------------------------------------------------------------------------------
void StageSelect2D::Uninit( void )
{
	Scene2D::Uninit( );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの更新処理
//--------------------------------------------------------------------------------------
void StageSelect2D::Update( void )
{
	Scene2D::Update( );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの描画処理
//--------------------------------------------------------------------------------------
void StageSelect2D::Draw( void )
{
	Scene2D::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
StageSelect2D* StageSelect2D::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV )
{
	StageSelect2D *pStageSelect2D;

	//  インスタンス生成
	pStageSelect2D = new StageSelect2D;

	//  座標の代入
	pStageSelect2D->m_position = position;

	//  大きさの代入
	pStageSelect2D->m_size = size;

	//  UV座標開始地点の代入
	pStageSelect2D->m_posUV = posUV;

	//  テクスチャUV分割数の代入
	pStageSelect2D->m_divideUV = divideUV;

	//  種類によっての場合分け
	switch( type )
	{
		case StageSelect2D::TYPE_STAGE1:
		{
			//  テクスチャ名の設定
			pStageSelect2D->SetTextureName( STAGE1_TEXTURE_NAME );

			break;
		}

		case StageSelect2D::TYPE_STAGE2:
		{
			//  テクスチャ名の設定
			pStageSelect2D->SetTextureName( STAGE2_TEXTURE_NAME );

			break;
		}

		case StageSelect2D::TYPE_STAGE3:
		{
			//  テクスチャ名の設定
			pStageSelect2D->SetTextureName( STAGE3_TEXTURE_NAME );

			break;
		}
	}

	//  初期化
	pStageSelect2D->Init( );

	return pStageSelect2D;
}