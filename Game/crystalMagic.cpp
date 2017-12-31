//--------------------------------------------------------------------------------------
//  魔法クリスタル   ( crystalMagic.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include "crystalMagic.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define CRYSTAL_FIRE_TEXTURE_NAME			"data/TEXTURE/STAGESELECT/crystalFire.png"			//  テクスチャ名
#define CRYSTAL_TORNADE_TEXTURE_NAME		"data/TEXTURE/STAGESELECT/crystalTornade.png"		//  テクスチャ名
#define CRYSTAL_LIGHTNING_TEXTURE_NAME		"data/TEXTURE/STAGESELECT/crystalLightning.png"		//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
CrystalMagic::CrystalMagic( )
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
CrystalMagic::CrystalMagic( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV , D3DXCOLOR color )
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
CrystalMagic::~CrystalMagic( )
{

}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの初期化処理
//--------------------------------------------------------------------------------------
HRESULT CrystalMagic::Init( void )
{
	Scene2D::Init( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの終了処理
//--------------------------------------------------------------------------------------
void CrystalMagic::Uninit( void )
{
	Scene2D::Uninit( );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの更新処理
//--------------------------------------------------------------------------------------
void CrystalMagic::Update( void )
{
	Scene2D::Update( );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの描画処理
//--------------------------------------------------------------------------------------
void CrystalMagic::Draw( void )
{
	Scene2D::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
CrystalMagic* CrystalMagic::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV )
{
	CrystalMagic *pCrystalMagic;

	//  インスタンス生成
	pCrystalMagic = new CrystalMagic;

	//  座標の代入
	pCrystalMagic->m_position = position;

	//  大きさの代入
	pCrystalMagic->m_size = size;

	//  UV座標開始地点の代入
	pCrystalMagic->m_posUV = posUV;

	//  テクスチャUV分割数の代入
	pCrystalMagic->m_divideUV = divideUV;

	//  種類によっての場合分け
	switch( type )
	{
		case CrystalMagic::TYPE_FIRE:
		{
			//  テクスチャ名の設定
			pCrystalMagic->SetTextureName( CRYSTAL_FIRE_TEXTURE_NAME );

			break;
		}

		case CrystalMagic::TYPE_TORNADE:
		{
			//  テクスチャ名の設定
			pCrystalMagic->SetTextureName( CRYSTAL_TORNADE_TEXTURE_NAME );

			break;
		}

		case CrystalMagic::TYPE_LIGHTNING:
		{
			//  テクスチャ名の設定
			pCrystalMagic->SetTextureName( CRYSTAL_LIGHTNING_TEXTURE_NAME );

			break;
		}
	}

	//  初期化
	pCrystalMagic->Init( );

	return pCrystalMagic;
}