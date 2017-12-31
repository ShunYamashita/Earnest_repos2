//--------------------------------------------------------------------------------------
//  メッシュキューブ   ( meshWall.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "meshCube.h"
#include "game.h"
#include "meshField.h"
#include "meshWall.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  メッシュキューブクラスのコンストラクタ
//--------------------------------------------------------------------------------------
MeshCube::MeshCube( void )
{
	m_pMeshField = NULL;

	for( int nCntWall = 0; nCntWall < 4; nCntWall++ )
	{
		m_pMeshWall[ nCntWall ] = NULL;
	}

	m_type = TYPE_SIDE;
}

//--------------------------------------------------------------------------------------
//  メッシュキューブクラスのデストラクタ
//--------------------------------------------------------------------------------------
MeshCube::~MeshCube( )
{

}

//--------------------------------------------------------------------------------------
//  メッシュキューブの初期化処理
//--------------------------------------------------------------------------------------
HRESULT MeshCube::Init( void )
{


	return S_OK;
}

//--------------------------------------------------------------------------------------
//  メッシュキューブの終了処理
//--------------------------------------------------------------------------------------
void MeshCube::Uninit( void )
{
	if( m_pMeshField != NULL )
	{
		m_pMeshField->Release( );
		m_pMeshField = NULL;
	}

	for( int nCntWall = 0; nCntWall < 4; nCntWall++ )
	{
		if( m_pMeshWall[ nCntWall ] != NULL )
		{
			m_pMeshWall[ nCntWall ]->Release( );
			m_pMeshWall[ nCntWall ] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------
//  メッシュキューブの更新処理
//--------------------------------------------------------------------------------------
void MeshCube::Update( void )
{
	if( m_pMeshField != NULL )
	{
		//  大きさの更新
		m_pMeshField->SetSize( m_size );
		m_pMeshField->SetPosition( D3DXVECTOR3( m_position.x , m_position.y + m_size.y , m_position.z ) );
		m_pMeshField->SetColor( m_color );
	}

	if( m_pMeshWall[ 0 ] != NULL )
	{
		m_pMeshWall[ 0 ]->SetSize( m_size );
		m_pMeshWall[ 0 ]->SetPosition( D3DXVECTOR3( m_position.x , m_position.y , m_position.z + m_size.z * 0.5f ) );
		m_pMeshWall[ 0 ]->SetColor( m_color );
	}

	if( m_pMeshWall[ 1 ] != NULL )
	{				 
		m_pMeshWall[ 1 ]->SetSize( m_size );
		m_pMeshWall[ 1 ]->SetPosition( D3DXVECTOR3( m_position.x , m_position.y , m_position.z - m_size.z * 0.5f  ) );
		m_pMeshWall[ 1 ]->SetColor( m_color );
	}

	if( m_pMeshWall[ 2 ] != NULL )
	{	
		m_pMeshWall[ 2 ]->SetSize( m_size );
		m_pMeshWall[ 2 ]->SetPosition( D3DXVECTOR3( m_position.x - m_size.x * 0.5f  , m_position.y , m_position.z ) );
		m_pMeshWall[ 2 ]->SetColor( m_color );
	}
	
	if( m_pMeshWall[ 3 ] != NULL )
	{	
		m_pMeshWall[ 3 ]->SetSize( m_size );
		m_pMeshWall[ 3 ]->SetPosition( D3DXVECTOR3( m_position.x + m_size.x * 0.5f  , m_position.y , m_position.z ) );
		m_pMeshWall[ 3 ]->SetColor( m_color );
	}
}

//--------------------------------------------------------------------------------------
//  メッシュキューブの描画処理
//--------------------------------------------------------------------------------------
void MeshCube::Draw( void )
{


}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
MeshCube* MeshCube::Create( TYPE type , D3DXVECTOR3 position , D3DXCOLOR color )
{
	MeshCube* pMeshCube;

	//  インスタンス生成
	pMeshCube = new MeshCube;

	//  種類の代入
	pMeshCube->m_type = type;

	//  座標の代入
	pMeshCube->m_position = position;

	//  大きさの代入
	pMeshCube->m_size = D3DXVECTOR3( MESH_FIELD_SIZE / ( float )MESH_FIELD_DIVIDE , 
									 MESH_FIELD_SIZE / ( float )MESH_FIELD_DIVIDE ,
									 MESH_FIELD_SIZE / ( float )MESH_FIELD_DIVIDE );

	//  色の代入
	pMeshCube->m_color = color;

	//  初期化
	pMeshCube->Init( );

	return pMeshCube;
}