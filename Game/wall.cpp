//--------------------------------------------------------------------------------------
//  壁  ( field.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "wall.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  壁クラスのコンストラクタ
//--------------------------------------------------------------------------------------
Wall::Wall( )
{

}

//--------------------------------------------------------------------------------------
//  壁クラスのデストラクタ
//--------------------------------------------------------------------------------------
Wall::~Wall( )
{

}

//--------------------------------------------------------------------------------------
//  壁の初期化処理
//--------------------------------------------------------------------------------------
HRESULT Wall::Init( void )
{
	//  シーンモデル
	SceneModel::Init( );

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_WALL );

	m_drawDepth = true;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  壁の終了処理
//--------------------------------------------------------------------------------------
void Wall::Uninit( void )
{
	//  シーンモデル
	SceneModel::Uninit( );
}

//--------------------------------------------------------------------------------------
//  壁の更新処理
//--------------------------------------------------------------------------------------
void Wall::Update( void )
{
	//  シーンモデル
	SceneModel::Update( );
}

//--------------------------------------------------------------------------------------
//  壁の描画処理
//--------------------------------------------------------------------------------------
void Wall::Draw( void )
{
	//  シーンモデル
	SceneModel::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Wall* Wall::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	Wall* pWall;

	//  インスタンス生成
	pWall = new Wall;

	//  座標の代入
	pWall->m_position = position;

	//  回転角の代入
	pWall->m_rot = rot;

	//  大きさ倍率の代入
	pWall->m_scale = scale;

	//  種類の代入
	pWall->m_type = SceneModel::TYPE_WALL000;

	//  初期化
	pWall->Init( );

	return pWall;
}
