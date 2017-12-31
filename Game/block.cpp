//--------------------------------------------------------------------------------------
//  ブロック  ( block.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "block.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ブロッククラスのコンストラクタ
//--------------------------------------------------------------------------------------
Block::Block( )
{

}

//--------------------------------------------------------------------------------------
//  ブロッククラスのデストラクタ
//--------------------------------------------------------------------------------------
Block::~Block( )
{

}

//--------------------------------------------------------------------------------------
//  ブロックの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Block::Init( void )
{
	//  シーンモデル
	SceneModel::Init( );

	m_drawDepth = true;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  ブロックの終了処理
//--------------------------------------------------------------------------------------
void Block::Uninit( void )
{
	//  シーンモデル
	SceneModel::Uninit( );
}

//--------------------------------------------------------------------------------------
//  ブロックの更新処理
//--------------------------------------------------------------------------------------
void Block::Update( void )
{
	//  シーンモデル
	SceneModel::Update( );
}

//--------------------------------------------------------------------------------------
//  ブロックの描画処理
//--------------------------------------------------------------------------------------
void Block::Draw( void )
{
	//  シーンモデル
	SceneModel::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Block* Block::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	Block* pBlock;

	//  インスタンス生成
	pBlock = new Block;

	//  座標の代入
	pBlock->m_position = position;

	//  回転角の代入
	pBlock->m_rot = rot;

	//  大きさ倍率の代入
	pBlock->m_scale = scale;

	//  種類の設定
	pBlock->m_type = SceneModel::TYPE_TREE;

	//  初期化
	pBlock->Init( );

	return pBlock;
}
