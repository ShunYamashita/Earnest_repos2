//--------------------------------------------------------------------------------------
//  敵   ( enemy.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemy.h"
#include "keyboard.h"
#include "xboxController.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "scene2D.h"
#include "meshField.h"
#include "bullet.h"
#include "particle.h"
#include "hitField.h"
#include "result.h"
#include "player.h"
#include "effekseerManager.h"
#include <random>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define ENEMY_TO_CAMERA_DISTANCE		( 10.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Z		( 25.0f )
#define ENEMY_TO_CUBE_DISTANCE			( 7.0f )
#define ENEMY_TO_CUBE_PUT_DISTANCE		( 2.5f )
#define ENEMY_SIZE						( 2.5f )
#define ENEMY_GOAL_SIZE					( 0.8f )
#define ENEMY_HIT_SIZE					( 2.0f )
#define ENEMY_WAIT_TIME					( 20 )
#define ENEMY_GRAVITY					( 0.1f )
#define ENEMY_GRAVITY2					( 0.98f )
#define ENEMY_ENABLE_NUM_JUMP			( 2 )
#define ENEMY_HIT_SPHERE_LENGTH			( 5.0f )

#define ENEMY_LIFE_POS_X				( 975.0f )
#define ENEMY_LIFE_POS_Y				( 605.0f )
#define ENEMY_LIFE_SIZE_X				( 255.0f )
#define ENEMY_LIFE_SIZE_Y				( 30.0f )

#define ENEMY_BRAVE_POS_X				( 990.0f )
#define ENEMY_BRAVE_POS_Y				( 565.0f )
#define ENEMY_BRAVE_SIZE_X				( 35.0f )
#define ENEMY_BRAVE_SIZE_Y				( 35.0f )

#define ENEMY_HIT_SPHERE_POS_Y			( 3.0f )

#define ENEMY_FOLLOW_DISTANCE			( 300.0f )

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  敵クラスのコンストラクタ
//--------------------------------------------------------------------------------------
Enemy::Enemy( )
{
	m_vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	m_fSpeed = 0.0f;
	m_nWaitTime = 0;
	m_state = STATE::STATE_NEUTRAL;
	m_fCurrentJumpPower = 0.0f;
	m_nCntJump = 0;
	m_nThinkTime = 0;
	m_hitSphere.fLength = ENEMY_HIT_SPHERE_LENGTH;
	m_pLife = NULL;
	m_pBrave = NULL;
	m_pShadow = NULL;
	m_pStateAnimator = NULL;
	m_nLife = BASE_LIFE;
	m_nBrave = BASE_BRAVE;
}

//--------------------------------------------------------------------------------------
//  敵クラスのデストラクタ
//--------------------------------------------------------------------------------------
Enemy::~Enemy( )
{

}

//--------------------------------------------------------------------------------------
//  敵の初期化処理
//--------------------------------------------------------------------------------------
HRESULT Enemy::Init( void )
{
	m_drawDepth = true;

	//  シーンモデル
	SceneModelAnim::Init( );

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_ENEMY );

	////  影の生成
	//m_pShadow = Shadow::Create( m_position , D3DXVECTOR3( 3.0f , 0.0f , 3.0f ) , D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) ,
	//							 D3DXVECTOR3( 0.0f , -D3DXToRadian( 90 ) , 0.0f ) , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) ); 

	//  体力の生成
	m_pLife = Life::Create( Life::TYPE_LIFE001 ,
							 D3DXVECTOR3( ENEMY_LIFE_POS_X , ENEMY_LIFE_POS_Y , 0.0f ) ,
							 D3DXVECTOR3( ENEMY_LIFE_SIZE_X , ENEMY_LIFE_SIZE_Y , 0.0f ) ,
							 D3DXVECTOR2( 0.0f , 0.0f ) ,
							 D3DXVECTOR2( 1.0f , 1.0f ) ,
							 m_nLife );

	//  体力の設定
	m_pLife->SetEndLife( m_nLife );

	////  ブレイブの生成
	//m_pBrave = Brave::Create( D3DXVECTOR3( ENEMY_BRAVE_POS_X , ENEMY_BRAVE_POS_Y , 0.0f ) ,
	//						   D3DXVECTOR3( ENEMY_BRAVE_SIZE_X , ENEMY_BRAVE_SIZE_Y , 0.0f ) ,
	//						   1000 , 4 );

	//  ステートマシン( アニメーター )の生成
	m_pStateAnimator = StateAnimator::Create( StateAnimator::MOTION_NEUTRAL );

	//  方向ベクトルの代入
	D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  敵の終了処理
//--------------------------------------------------------------------------------------
void Enemy::Uninit( void )
{
	//  シーンモデル
	SceneModelAnim::Uninit( );

	if( m_pShadow != NULL )
	{
		m_pShadow->Release( );
		m_pShadow = NULL;
	}

	if( m_pLife != NULL )
	{
		m_pLife->Release( );
		m_pLife = NULL;
	}

	if( m_pBrave != NULL )
	{
		m_pBrave->Release( );
		m_pBrave = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  敵の更新処理
//--------------------------------------------------------------------------------------
void Enemy::Update( void )
{
	// キーボード情報の取得
	Keyboard*			pKeyboard = SceneManager::GetKeyboard( );

	//  Xboxコントローラー情報の取得
	XboxController*	pXInput = SceneManager::GetXInput( );

	//  カメラの取得
	Camera* pCamera = SceneManager::GetCamera( 0 );

	//  シーンクラスのポインタ
	Scene *pScene = NULL;										

	//  壁との当たり判定
	if( m_position.x > Game::GetFieldMax( ).x )
	{
		m_position.x = Game::GetFieldMax( ).x;
	}
	if( m_position.x < Game::GetFieldMin( ).x )
	{
		m_position.x = Game::GetFieldMin( ).x;
	}
	if( m_position.z > Game::GetFieldMax( ).z )
	{
		m_position.z = Game::GetFieldMax( ).z;
	}
	if( m_position.z < Game::GetFieldMin( ).z )
	{
		m_position.z = Game::GetFieldMin( ).z;
	}

	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		//  シーンの先頭アドレスを取得
		pScene = Scene::GetScene( nCntPriority );

		//  シーンが空ではない間ループ
		while( pScene != NULL )
		{
			Scene::OBJTYPE objType;						//  物体の種類

			//  物体の種類の取得
			objType = pScene->GetObjType( );

			//  種類が当たり判定用フィールドの場合
			if( objType == Scene::OBJTYPE_HIT_FIELD )
			{
				//  当たり判定用フィールドクラスにダウンキャスト
				HitField* pHitField = ( HitField* )pScene;

				if( m_position.y <= pHitField->GetHeight( m_position ) )
				{
					m_position.y -= ENEMY_GRAVITY;
				}
				else
				{
					m_position.y -= ENEMY_GRAVITY2;
				}			

				if( m_position.y <= pHitField->GetHeight( m_position ) )
				{
					//  ジャンプ状態の場合
					if( m_state == STATE_JUMP )
					{
						//  通常状態に
						m_state = STATE_NEUTRAL;
					}

					m_nCntJump = 0;

					//  Y座標フィールドに合わせる
					m_position.y = pHitField->GetHeight( m_position );
				}

				if( m_pShadow != NULL )
				{
					float fScale = 1.0f + ( m_position.y - pHitField->GetHeight( m_position )  ) * 0.01f;

					//  大きさ比率の設定
					m_pShadow->SetScale( fScale );

					//  座標の代入
					m_pShadow->SetPosition( D3DXVECTOR3( m_position.x , pHitField->GetHeight( m_position ) + 0.01f , m_position.z ) );
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}
	if( m_nThinkTime > 0 )
	{
		m_nThinkTime--;
	}

	m_hitSphere.position = m_position;
	m_hitSphere.position.y += ENEMY_HIT_SPHERE_POS_Y;

#pragma omp parallel for
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		//  シーンの先頭アドレスを取得
		pScene = Scene::GetScene( nCntPriority );

		//  シーンが空ではない間ループ
		while( pScene != NULL )
		{
			Scene::OBJTYPE objType;						//  物体の種類

			//  物体の種類の取得
			objType = pScene->GetObjType( );

			//  種類がプレイヤーの場合
			if( objType == Scene::OBJTYPE_PLAYER )
			{
				//  ダウンキャスト
				Player* pPlayer = ( Player* )pScene;

				float fDist = 0.0f;

				//  プレイヤーが当たり判定をする場合
				if( pPlayer->GetJudgeHit( )  )
				{
					if( Utility::HitSphere( m_hitSphere , pPlayer->GetHitSphere( ) , &fDist ) )
					{
						//  速度をなくす
						m_fSpeed = 0.0f;

						//  反射するベクトルを求める
						D3DXVECTOR3 reflectVec = m_hitSphere.position - pPlayer->GetHitSphere( ).position;
						reflectVec.y = 0.0f;

						D3DXVec3Normalize( &reflectVec , &reflectVec );

						//  めり込んだ分座標を戻す
						m_position += reflectVec * fDist;
					}
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  シーンモデル
	SceneModelAnim::Update( );

	if( m_state == STATE_ATTACK_FIRE )
	{
		Particle::Create( Particle::TYPE_NORMAL , m_position , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
						   D3DXCOLOR( 0.8f , 0.1f , 0.1f , 1.0f ) , 4.0f , 0.1f , 0.01f , 0.0f , 0.0f , 20 );
	}
	else if( m_state == STATE_ATTACK_NORMAL )
	{
		Particle::Create( Particle::TYPE_NORMAL , m_position , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
						   D3DXCOLOR( 0.8f , 0.8f , 0.1f , 1.0f ) , 4.0f , 0.1f , 0.01f , 0.0f , 0.0f , 20 );
	}

	//  体力ポインタがある場合
	if( m_pLife != NULL )
	{
		//  体力の設定
		m_pLife->SetLife( m_nLife );
	}

	//  無敵時間の場合
	if( m_nInvisibleTime > 0 )
	{
		//  無敵時間のカウント
		m_nInvisibleTime--;

		m_color.a = 0.5f;
	}
	else
	{
		m_color.a = 1.0f;
	}
}

//--------------------------------------------------------------------------------------
//  敵の描画処理
//--------------------------------------------------------------------------------------
void Enemy::Draw( void )
{
	//  シーンモデル
	SceneModelAnim::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Enemy* Enemy::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed )
{
	Enemy* pEnemy;

	//  インスタンス生成
	pEnemy = new Enemy;

	//  モデルの種類の代入
	pEnemy->m_type = SceneModelAnim::TYPE_PLAYER;

	//  座標の代入
	pEnemy->m_position = position;

	//  最初の座標の代入
	pEnemy->m_firstPos = position;

	//  注視点座標の代入
	pEnemy->m_posAt = position;
	pEnemy->m_posAt.z -= 2.0f;

	//  回転角の代入
	pEnemy->m_rot = rot;

	//  大きさ倍率の代入
	pEnemy->m_scale = scale;

	//  速度の代入
	pEnemy->m_fSpeed = fSpeed;

	//  初期化
	pEnemy->Init( );

	return pEnemy;
}

//--------------------------------------------------------------------------------------
//  敵のダメージ処理
//--------------------------------------------------------------------------------------
void Enemy::Damage( int nDamage )
{
	if( m_nInvisibleTime != 0 )
	{
		return;
	}

	//  ダメージ処理
	m_nLife -= nDamage;

	//  無敵時間の設定
	m_nInvisibleTime = 60;

	//  乱数の宣言
	std::random_device rd;

	//  乱数の設定
	std::uniform_int_distribution< int > RandomSeekPos( 5 , 20 );

	float fSeekPosX = ( float )RandomSeekPos( rd ) * 0.1f;
	float fSeekPosZ = ( float )RandomSeekPos( rd ) * 0.1f;

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x + fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z + fSeekPosZ ) ,
							   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x - fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z - fSeekPosZ ) ,
							   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  体力が0以下になった場合
	if( m_nLife <= 0 )
	{
		m_nLife = 0;

		//  自分自身の削除
		Release( );

		//  ゲームクリア画面へ
		Result::SetResult( Result::GAME_CLEAR );

		//  結果画面への移行フラグをたてる
		Game::SetNextMode( Mode::MODE::RESULT );	
	}
}

//--------------------------------------------------------------------------------------
//  敵のブレイブダメージ処理
//--------------------------------------------------------------------------------------
int Enemy::BraveDamage( int nDamage )
{
	if( m_nInvisibleTime != 0 )
	{
		return 0;
	}

	int nRealDamage = nDamage;	//  実際のダメージ格納
	int nOverDamage = 0;		//  オーバーした分のダメージ格納

	//  ブレイブダメージ処理
	m_nBrave -= nDamage;

	//  無敵時間の設定
	m_nInvisibleTime = 60;

	//  乱数の宣言
	std::random_device rd;

	//  乱数の設定
	std::uniform_int_distribution< int > RandomSeekPos( 5 , 20 );

	float fSeekPosX = ( float )RandomSeekPos( rd ) * 0.1f;
	float fSeekPosZ = ( float )RandomSeekPos( rd ) * 0.1f;

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x + fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z + fSeekPosZ ) ,
							   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  エフェクトの生成
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x - fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z - fSeekPosZ ) ,
							   D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  体力が0以下になった場合
	if( m_nBrave <= 0 )
	{
		//  オーバーした分の代入
		nOverDamage = abs( 0 - m_nBrave );

		m_nBrave = 0;
	}

	//  オーバーした分を実際のダメージから引く
	nRealDamage -= nOverDamage;

	return nRealDamage;
}