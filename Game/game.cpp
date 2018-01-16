//--------------------------------------------------------------------------------------
//  ゲームプログラム   ( game.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "game.h"
#include "sceneModel.h"
#include "meshField.h"
#include "meshWall.h"
#include "meshDome.h"
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "keyboard.h"
#include "fade.h"
#include "renderer.h"
#include "enemy.h"
#include "sceneBG.h"
#include "sceneFBX.h"
#include "field.h"
#include "effekseerManager.h"
#include "light.h"
#include <stdio.h>
#include <random>
#include "Wwise.h"
#include "unityFBX.h"
#include "block.h"
#include "wall.h"
#include "cascadeShadow.h"
#include "playerWinks.h"
#include "playerRauder.h"
#include "enemyRauder.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define PLAYER_SPEED			( 0.8f )
#define WALL_DIVIDE				( 1 )

#define STAGE1_FILENAME			"data/STAGE/stage1.txt"
#define STAGE2_FILENAME			"data/STAGE/stage2.txt"
#define STAGE3_FILENAME			"data/STAGE/stage3.txt"

static const float	ATMOSPHERE_SIZE = 15.0f; 
static const int	ATMOSPHERE_NUM = 1; 

static const float	THIEF_SIZE = 1.0f; 

static const float	STAGE1_DISTACE= 300.0f; 

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
Game::MODE_VS	Game::m_modeVS = Game::MODE_VS_CPU;				//  対戦モード
Game::CRYSTAL	Game::m_crystal[ 2 ] = { Game::CRYSTAL_FIRE };	//  魔法クリスタル
int				Game::m_nNumDivide = MESH_FIELD_DIVIDE;			//  フィールド分割数
int				Game::m_nNumField = 0;							//  フィールド数
int				Game::m_nFocusField = 0;						//  フォーカスがあっているフィールド数
float			Game::m_fSize = ONE_CUBE_SIZE;					//  フォーカスがあっているフィールド数
bool			Game::m_bClear = false;							//  クリアしたかどうか
D3DXVECTOR3		Game::m_fieldPos( 0.0f , 0.0f , 0.0f );			//  フォーカスがあっているフィールド数
D3DXVECTOR3		Game::m_fieldMin( 0.0f , 0.0f , 0.0f );			//  フィールド最小座標
D3DXVECTOR3		Game::m_fieldMax( 0.0f , 0.0f , 0.0f );			//  フィールド最大座標
Mode::MODE		Game::m_nextMode = Mode::MODE::RESULT;			//  次の場面

//--------------------------------------------------------------------------------------
//  ゲームクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Game::Game( )
{

}

//--------------------------------------------------------------------------------------
//  ゲームクラスのデストラクタ
//--------------------------------------------------------------------------------------
Game::~Game( )
{

}

//--------------------------------------------------------------------------------------
//  ゲームクラスの初期化処理
//--------------------------------------------------------------------------------------
void Game::Init( void )
{
	m_bClear = false;

	//  ステージの生成
	SetStage( );

	Block::Create( D3DXVECTOR3( -200.0f , 0.0f , -100.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
				   D3DXVECTOR3( 0.1f , 0.1f , 0.1f ) ); 

	Block::Create( D3DXVECTOR3( -190.0f , 0.0f , 150.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
				   D3DXVECTOR3( 0.1f , 0.1f , 0.1f ) ); 

	Block::Create( D3DXVECTOR3( 180.0f , 0.0f , 200.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
				   D3DXVECTOR3( 0.1f , 0.1f , 0.1f ) ); 

	Block::Create( D3DXVECTOR3( 150.0f , 0.0f , -300.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
				   D3DXVECTOR3( 0.1f , 0.1f , 0.1f ) ); 
}

//--------------------------------------------------------------------------------------
//  ゲームクラスの終了処理
//--------------------------------------------------------------------------------------
void Game::Uninit( void )
{
	//  オブジェクトクラスの全解放
	Scene::ReleaseAll( );
}

//--------------------------------------------------------------------------------------
//  ゲームクラスの更新処理
//--------------------------------------------------------------------------------------
void Game::Update( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	for( int i = 0; i < 2; i++ )
	{
		Camera* camera = SceneManager::GetCamera( i );

		//  カメラクラスポインタが空ではない場合
		if( camera != NULL )
		{
			camera->Update( );
		}
	}

	//  カスケードシャドウの更新
	CascadeShadow::SetCascadeShadowInfo( m_fieldMin , m_fieldMax );

	if( m_bClear )
	{
		//  フェードの設定
		Fade::SetFade( Fade::FADE_OUT , m_nextMode , D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.0f ) , 0.02f );
	}
}

//--------------------------------------------------------------------------------------
//  ゲームクラスの描画処理
//--------------------------------------------------------------------------------------
void Game::Draw( void )
{
	//  シーン全ての描画
	Scene::DrawAll( );
}

//--------------------------------------------------------------------------------------
//  フィールド分割数の取得
//--------------------------------------------------------------------------------------
int Game::GetNumDivide( void )
{
	return m_nNumDivide;
}

//--------------------------------------------------------------------------------------
//  フィールド数の取得
//--------------------------------------------------------------------------------------
int Game::GetNumField( void )
{
	return m_nNumField;
}

//--------------------------------------------------------------------------------------
//  フォーカスがあっているフィールド番号
//--------------------------------------------------------------------------------------
int Game::GetFocusField( void )
{
	return m_nFocusField;
}

//--------------------------------------------------------------------------------------
//  ゴールクラスを取得をする関数
//--------------------------------------------------------------------------------------
void Game::SetStage( void )
{
	char		aFileName[ SceneManager::STAGE_MAX ][ 128 ] = {
																	STAGE1_FILENAME ,
																	STAGE2_FILENAME ,
																	STAGE3_FILENAME ,
															  };

	int			nNumBlock = 0;						//  ブロック数の格納
	int			nType = -1;							//  ブロックの種類
	int			nNumCube = 0;						//  キューブ数の格納
	D3DXVECTOR3 workPos( 0.0f , 0.0f , 0.0f );		//  座標の格納
	int			nBaseCube = -1;						//  基準キューブ数の格納
	int			nCubeDirect = -1;					//  追加進行方向の代入

	m_nNumDivide = MESH_FIELD_DIVIDE;

	Light* pLight = SceneManager::GetLight( );

	if( SceneManager::GetStage( ) == SceneManager::STAGE1 )
	{
		if( pLight != NULL )
		{
			pLight->SetDiffuseColor( D3DXCOLOR( 1.0f , 0.9f , 0.3f , 1.0f ) );
		}

		//  メッシュドームの生成
		MeshDome::Create( MeshDome::TYPE_SKY,
						  D3DXVECTOR3( 0.0f , -15.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						  D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.00001f , 0.0f ) ,
						  32 , 4 );

		//  CPU対戦の場合
		if( m_modeVS == Game::MODE_VS_CPU )
		{
			//  背景
			SceneBG::Create( SceneBG::TYPE_GAME , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
							  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
							  D3DXVECTOR2( 1.0f , 1.0f ) );

			Camera* camera = SceneManager::GetCamera( 0 );

			//  カメラクラスポインタが空の場合
			if( camera != nullptr )
			{
				//  カメラクラスの生成
				camera->Init( D3DXVECTOR3( 0.0f , 10.0f , -( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							  D3DX_PI / 3.0f , 1.0f , 10000.0f );
			}

			Player::MAGIC magic;

			switch( m_crystal[ 0 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic = Player::MAGIC::FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic = Player::MAGIC::TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic = Player::MAGIC::LIGHTNING;

					break;
				}
			}

			//  プレイヤーの生成
			PlayerWinks::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , PlayerWinks::MAGIC::FIRE );

			//PlayerRauder::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
			//					  D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , PlayerRauder::MAGIC::FIRE );

			//  敵の生成
			EnemyRauder::Create( D3DXVECTOR3( 0.0f , 0.0f , STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED , EnemyRauder::MAGIC::FIRE );
		}
		//  プレイヤー対戦の場合
		else if( m_modeVS == Game::MODE_VS_PLAYER )
		{
			Camera* camera = SceneManager::GetCamera( 0 );

			//  カメラクラスの初期化
			if( camera != nullptr )
			{
				camera->Init( D3DXVECTOR3( 0.0f , 10.0f , -( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							  D3DX_PI / 3.0f , 1.0f , 10000.0f , 0 , true );
			}

			Camera* camera2 = SceneManager::GetCamera( 1 );

			//  カメラクラスの初期化
			if( camera2 != nullptr )
			{
				camera2->Init( D3DXVECTOR3( 0.0f , 10.0f , ( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							   D3DX_PI / 3.0f , 1.0f , 10000.0f , 1 , true );
			}

			Player::MAGIC magic;

			switch( m_crystal[ 0 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic = Player::MAGIC::FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic = Player::MAGIC::TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic = Player::MAGIC::LIGHTNING;

					break;
				}
			}

			Player::MAGIC magic2;

			switch( m_crystal[ 1 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic2 = Player::MAGIC::FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic2 = Player::MAGIC::TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic2 = Player::MAGIC::LIGHTNING;

					break;
				}
			}

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic , 0 );

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic2 , 1 );
		}

		//  フィールドの生成
		Field::Create( Field::TYPE_STAGE000 , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

		//  壁の生成
		Wall::Create( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

		//  メッシュフィールドの生成
		MeshField::Create( MeshField::TYPE_SEA , 
						   D3DXVECTOR3( 0.0f , -9.0f , 0.0f ) , 
						   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						   D3DXVECTOR3( 20000.0f , 0.0f , 20000.0f ) , 
						   D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) , 
						   16 , 
						   16 ,
						   D3DXVECTOR2( 0.0001f , 0.0001f ) );
	}
	else if( SceneManager::GetStage( ) == SceneManager::STAGE2 )
	{
		if( pLight != NULL )
		{
			pLight->SetDiffuseColor( D3DXCOLOR( 1.0f , 0.3f , 0.3f , 1.0f ) );
		}

		//  メッシュドームの生成
		MeshDome::Create( MeshDome::TYPE_COSMO ,
						   D3DXVECTOR3( 0.0f , -15.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						   D3DXVECTOR3( 10000.0f , 0.0f , 10000.0f ) , D3DXVECTOR2( 0.00001f , 0.0f ) ,
						   32 , 4 );

		//  CPU対戦の場合
		if( m_modeVS == Game::MODE_VS_CPU )
		{
			//  背景
			SceneBG::Create( SceneBG::TYPE_GAME , D3DXVECTOR3( SCREEN_WIDTH * 0.5f , SCREEN_HEIGHT * 0.5f , 0.0f ) ,
							  D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f ) , D3DXVECTOR2( 0.0f , 0.0f ) , 
							  D3DXVECTOR2( 1.0f , 1.0f ) );

			Camera* camera = SceneManager::GetCamera( 0 );

			//  カメラクラスポインタが空の場合
			if( camera != nullptr )
			{
				//  カメラクラスの生成
				camera->Init( D3DXVECTOR3( 0.0f , 10.0f , -( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							  D3DX_PI / 3.0f , 1.0f , 10000.0f );
			}

			Player::MAGIC magic;

			switch( m_crystal[ 0 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic = Player::MAGIC::FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic = Player::MAGIC::TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic = Player::MAGIC::LIGHTNING;

					break;
				}
			}

			//  プレイヤーの生成
			PlayerWinks::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , PlayerWinks::MAGIC::FIRE );

			//PlayerRauder::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
			//					  D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , PlayerRauder::MAGIC::FIRE );

			//  敵の生成
			EnemyRauder::Create( D3DXVECTOR3( 0.0f , 0.0f , STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								 D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED , EnemyRauder::MAGIC::FIRE );
		}
		//  プレイヤー対戦の場合
		else if( m_modeVS == Game::MODE_VS_PLAYER )
		{
			Camera* camera = SceneManager::GetCamera( 0 );

			//  カメラクラスの初期化
			if( camera != nullptr )
			{
				camera->Init( D3DXVECTOR3( 0.0f , 10.0f , -( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							  D3DX_PI / 3.0f , 1.0f , 10000.0f , 0 , true );
			}

			Camera* camera2 = SceneManager::GetCamera( 1 );

			//  カメラクラスの初期化
			if( camera2 != nullptr )
			{
				camera2->Init( D3DXVECTOR3( 0.0f , 10.0f , ( STAGE1_DISTACE + 20.0f ) ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							   D3DX_PI / 3.0f , 1.0f , 10000.0f , 1 , true );
			}

			Player::MAGIC magic;

			switch( m_crystal[ 0 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic = Player::MAGIC::FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic = Player::MAGIC::TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic = Player::MAGIC::LIGHTNING;

					break;
				}
			}

			Player::MAGIC magic2;

			switch( m_crystal[ 1 ] )
			{
				case Game::CRYSTAL_FIRE:
				{
					magic2 = Player::MAGIC::FIRE;

					break;
				}
				case Game::CRYSTAL_TORNADE:
				{
					magic2 = Player::MAGIC::TORNADE;

					break;
				}
				case Game::CRYSTAL_LIGHTNING:
				{
					magic2 = Player::MAGIC::LIGHTNING;

					break;
				}
			}

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , -STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic, 0 );

			//  プレイヤーの生成
			Player::Create( D3DXVECTOR3( 0.0f , 0.0f , STAGE1_DISTACE ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
							D3DXVECTOR3( THIEF_SIZE , THIEF_SIZE , THIEF_SIZE ) , PLAYER_SPEED  , magic2 , 1 );
		}

		//  フィールドの生成
		Field::Create( Field::TYPE_STAGE000 , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

		//  壁の生成
		Wall::Create( D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

		//  メッシュフィールドの生成
		MeshField::Create( MeshField::TYPE_SEA , 
						   D3DXVECTOR3( 0.0f , -9.0f , 0.0f ) , 
						   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
						   D3DXVECTOR3( 20000.0f , 0.0f , 20000.0f ) , 
						   D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) , 
						   16 , 
						   16 ,
						   D3DXVECTOR2( 0.0001f , 0.0001f ) );
	}

	for( int i = 0; i < ATMOSPHERE_NUM; i++ )
	{
		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		/////////////////////////////////////////////////////
		//  第1ライン
		/////////////////////////////////////////////////////

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE , 
								  D3DXVECTOR3( 0.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -500.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , -500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 500.0f , 0.0f , 500.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		/////////////////////////////////////////////////////
		//  第2ライン
		/////////////////////////////////////////////////////

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE , 
								  D3DXVECTOR3( 0.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 0.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( -250.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , -250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );

		//  エフェクトの生成
		EffekseerManager::Create( EffekseerManager::TYPE_ATMOSPHERE ,
								  D3DXVECTOR3( 250.0f , 0.0f , 250.0f ) ,
								  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
								  D3DXVECTOR3( ATMOSPHERE_SIZE , ATMOSPHERE_SIZE , ATMOSPHERE_SIZE ) ,
								  1.0f );
	}
}

//--------------------------------------------------------------------------------------
//  次のモード情報の代入
//--------------------------------------------------------------------------------------
void Game::SetNextMode( Mode::MODE mode )
{
	//  クリア状態にして次のモードの設定
	m_bClear = true;
	m_nextMode = mode;
}

//--------------------------------------------------------------------------------------
//  対戦モードを設定
//--------------------------------------------------------------------------------------
void Game::SetModeVS( MODE_VS modeVS )
{
	m_modeVS = modeVS;
}

//--------------------------------------------------------------------------------------
//  対戦モードを取得
//--------------------------------------------------------------------------------------
Game::MODE_VS Game::GetModeVS( void )
{
	return m_modeVS;
}

//--------------------------------------------------------------------------------------
//  対戦モードを設定
//--------------------------------------------------------------------------------------
void Game::SetCrystal( int nIndex , CRYSTAL crystal )
{
	m_crystal[ nIndex ] = crystal;
}

//--------------------------------------------------------------------------------------
//  フィールド最小座標を設定
//--------------------------------------------------------------------------------------
void Game::SetFieldMin( D3DXVECTOR3 min )
{
	m_fieldMin = min;
}

//--------------------------------------------------------------------------------------
//  フィールド最大座標を設定
//--------------------------------------------------------------------------------------
void Game::SetFieldMax( D3DXVECTOR3 max )
{
	m_fieldMax = max;
}

//--------------------------------------------------------------------------------------
//  フィールド最小座標を取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Game::GetFieldMin( void )
{
	return m_fieldMin;
}

//--------------------------------------------------------------------------------------
//  フィールド最大座標を取得
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Game::GetFieldMax( void )
{
	return m_fieldMax;
}