//--------------------------------------------------------------------------------------
//  雷魔法クラス  ( magicLightning.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/09/17
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "magicLightning.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "light.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "particle.h"
#include "hitField.h"
#include "effekseerManager.h"
#include <random>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const int	START_HIT_TIME = 20;
static const int	END_HIT_TIME = 50;
static const float	LIGHTNING_BLOW_POWER = 10.0f;

//--------------------------------------------------------------------------------------
//  雷魔法の初期化処理をする関数
//--------------------------------------------------------------------------------------
HRESULT MagicLightning::Init( void )
{
	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_MAGIC_LIGHTNING );

	m_firstPos = m_position;
	m_magicPos = m_position + m_vecDirect * 2.0f;
	m_bActive = false;
	m_fAngle = D3DX_PI;
	m_fDisance = 2.0f;
	m_fScale = 1.0f;
	m_nHitTime = 0;
	m_handle = -1;

	//  エフェクトの生成
	m_handle = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE,
										  m_magicPos , D3DXVECTOR3( m_rot.x + -D3DX_PI * 0.5f , m_rot.y , m_rot.z ) ,
										  m_size * m_fScale * 10.0f  ,
										  1.0f );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  雷魔法の終了処理をする関数
//--------------------------------------------------------------------------------------
void MagicLightning::Uninit( void )
{
}

//--------------------------------------------------------------------------------------
//  雷魔法の更新処理をする関数
//--------------------------------------------------------------------------------------
void MagicLightning::Update( void )
{
	//  壁との当たり判定
	if( m_magicPos.x > Game::GetFieldMax( ).x )
	{
		m_magicPos.x = Game::GetFieldMax( ).x;
	}
	if( m_magicPos.x < Game::GetFieldMin( ).x )
	{
		m_magicPos.x = Game::GetFieldMin( ).x;
	}
	if( m_magicPos.z > Game::GetFieldMax( ).z )
	{
		m_magicPos.z = Game::GetFieldMax( ).z;
	}
	if( m_magicPos.z < Game::GetFieldMin( ).z )
	{
		m_magicPos.z = Game::GetFieldMin( ).z;
	}

	Scene* pScene = NULL;

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

			//  種類がゴールの場合
			if( objType == Scene::OBJTYPE_HIT_FIELD )
			{
				//  当たり判定用フィールドクラスにダウンキャスト
				HitField* pHitField = ( HitField* )pScene;

				//  魔法Y座標を地面の座標に
				m_magicPos.y = pHitField->GetHeight( m_magicPos );
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	if( m_bActive == true )
	{
		//  当たり判定時間の範囲に入っている場合
		if( m_nHitTime >= START_HIT_TIME && m_nHitTime <= END_HIT_TIME )
		{
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

					//  CPU対戦モードの場合
					if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
					{
						//  種類が敵の弾の場合
						if( m_owner == OWNER_ENEMY )
						{
							//  種類がプレイヤーの場合
							if( objType == Scene::OBJTYPE_PLAYER )
							{
								//  プレイヤークラスにダウンキャスト
								Player* pPlayer = ( Player* )pScene;

								//  プレイヤーの座標の代入
								D3DXVECTOR2 playerPos;
								playerPos.x = pPlayer->GetPos( ).x;
								playerPos.y = pPlayer->GetPos( ).z;

								Utility::HIT_CIRCLE hitCircle;
								hitCircle.position.x = m_magicPos.x;
								hitCircle.position.y = m_magicPos.z;
								hitCircle.fLength = m_size.x * m_fScale * 10.0f;

								D3DXVECTOR3 blowVecDirect( 0.0f , 0.0f , 0.0f );
								blowVecDirect.x = playerPos.x - hitCircle.position.x;
								blowVecDirect.z = playerPos.y - hitCircle.position.y;
								D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

								//  弾とプレイヤーが当たった場合
								if( Utility::HitCirclePoint( hitCircle , playerPos ) )
								{
									//  プレイヤーにダメージ処理
									pPlayer->Damage( blowVecDirect , LIGHTNING_BLOW_POWER , ( int )( 65 * m_fScale ) );
								}
							}
						}
						//  種類がプレイヤーの弾の場合
						else if( m_owner == OWNER_PLAYER )
						{
							//  種類が敵の場合
							if( objType == Scene::OBJTYPE_ENEMY )
							{
								//  敵クラスにダウンキャスト
								Enemy* pEnemy = ( Enemy* )pScene;

								//  プレイヤーの座標の代入
								D3DXVECTOR2 enemyPos;
								enemyPos.x  = pEnemy->GetPos( ).x;
								enemyPos.y  = pEnemy->GetPos( ).z;

								Utility::HIT_CIRCLE hitCircle;
								hitCircle.position.x = m_magicPos.x;
								hitCircle.position.y = m_magicPos.z;
								hitCircle.fLength = m_size.x * m_fScale * 10.0f;

								D3DXVECTOR3 blowVecDirect( 0.0f , 0.0f , 0.0f );
								blowVecDirect.x = enemyPos.x - hitCircle.position.x;
								blowVecDirect.z = enemyPos.y - hitCircle.position.y;
								D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

								//  弾と敵が当たった場合
								if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
								{
									//  敵にダメージ処理
									pEnemy->Damage( blowVecDirect , LIGHTNING_BLOW_POWER , ( int )( 65 * m_fScale ) );
								}
							}
						}
					}
					//  プレイヤー対戦モードの場合
					if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
					{
						//  種類がプレイヤーの弾の場合
						if( m_owner == OWNER_PLAYER )
						{
							//  種類がプレイヤーの場合
							if( objType == Scene::OBJTYPE_PLAYER )
							{
								//  プレイヤークラスにダウンキャスト
								Player* pPlayer = ( Player* )pScene;

								//  所有者プレイヤー番号以外の場合
								if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
								{
									//  プレイヤーの座標の代入
									D3DXVECTOR2 enemyPos;
									enemyPos.x  = pPlayer->GetPos( ).x;
									enemyPos.y  = pPlayer->GetPos( ).z;

									Utility::HIT_CIRCLE hitCircle;
									hitCircle.position.x = m_magicPos.x;
									hitCircle.position.y = m_magicPos.z;
									hitCircle.fLength = m_size.x * m_fScale * 10.0f;

									D3DXVECTOR3 blowVecDirect( 0.0f , 0.0f , 0.0f );
									blowVecDirect.x = enemyPos.x - hitCircle.position.x;
									blowVecDirect.z = enemyPos.y - hitCircle.position.y;
									D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

									//  弾と敵が当たった場合
									if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
									{
										//  敵にダメージ処理
										pPlayer->Damage( blowVecDirect , LIGHTNING_BLOW_POWER , ( int )( 65 * m_fScale ) );
									}
								}
							}
						}
					}

					//  次のポインタを代入
					pScene = pScene->GetNextScene( pScene );
				}
			}
		}

		//  エフェクシアが終了した場合
		if( EffekseerManager::GetExist( m_handle ) == false )
		{
			//  自身の削除
			Release( );
		}

		//  当たり判定の時間のカウント
		m_nHitTime++;
	}
	else 
	{
		m_fDisance += 0.1f;

		if( m_fDisance > 9.0f )
		{
			m_fDisance = 9.0f;
		}

		m_fScale += 0.06f;

		if( m_fScale > 8.0f )
		{
			m_fScale = 8.0f;
		}

		m_fAngle -= 0.1f;

		D3DXVECTOR3 endPos = m_firstPos;
		endPos.x += cosf( m_fAngle ) * m_fDisance;
		endPos.z += sinf( m_fAngle ) * m_fDisance;

		m_position.x += ( endPos.x - m_position.x ) * 0.1f;
		m_position.z += ( endPos.z - m_position.z ) * 0.1f;

		D3DXVec3Normalize( &m_vecDirect , &( m_position - m_beforePos ) );

		for( int i = 0; i < 2; i++ )
		{
			Particle::Create( Particle::TYPE_NORMAL , m_position , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
							   D3DXCOLOR( 0.6f , 0.3f , 0.7f , 1.0f ) , m_fScale , 0.1f , 0.01f , 0.0f , 0.0f , 20 ,
							   m_vecDirect.x , m_vecDirect.y , m_vecDirect.z );
		}

		//  魔法陣の設定
		EffekseerManager::SetPosition( m_handle , m_magicPos );
		EffekseerManager::SetScale( m_handle , m_size * m_fScale * 10.0f );
	}
}

//--------------------------------------------------------------------------------------
//  雷魔法の描画処理をする関数
//--------------------------------------------------------------------------------------
void MagicLightning::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
MagicLightning* MagicLightning::Create( OWNER owner , D3DXVECTOR3 position , D3DXVECTOR3 rot ,
										  D3DXVECTOR3 scale , D3DXVECTOR3 vecDirect , int nPlayerNo )
{
	MagicLightning* pMagicLightning = NULL;

	//  インスタンス生成
	pMagicLightning = new MagicLightning( );

	//  所有者の代入
	pMagicLightning->m_owner = owner;

	//  座標の代入
	pMagicLightning->m_position = position;

	//  回転の代入
	pMagicLightning->m_rot = rot;

	//  大きさの代入
	pMagicLightning->m_size = scale;

	//  方向ベクトルの代入
	pMagicLightning->m_vecDirect = vecDirect;

	//  プレイヤー番号の代入
	pMagicLightning->m_nPlayerNo = nPlayerNo;

	//  初期化
	pMagicLightning->Init( );

	return pMagicLightning;
}

//--------------------------------------------------------------------------------------
//  所有者の確認をする関数
//--------------------------------------------------------------------------------------
MagicLightning::OWNER MagicLightning::GetOwner( void )
{
	return m_owner;
}

//--------------------------------------------------------------------------------------
//  雷魔法が何かに当たった時の処理をする関数
//--------------------------------------------------------------------------------------
void MagicLightning::Hit( void )				
{
	//  自身の削除
	Scene::Release( );
}

//--------------------------------------------------------------------------------------
//  アクティブ状態にする関数
//--------------------------------------------------------------------------------------
void MagicLightning::SetActive( void )				
{
	if( m_bActive == false )
	{
		//  魔法陣の再生停止
		EffekseerManager::Stop( m_handle );

		m_bActive = true;

		//  エフェクトの生成
		m_handle = EffekseerManager::Create( EffekseerManager::TYPE_LIGHTNING ,
											  m_magicPos , m_rot ,
											  D3DXVECTOR3( m_size.x * m_fScale , m_size.y * m_fScale , m_size.z * m_fScale ) ,
											  1.0f );
	}
}

//--------------------------------------------------------------------------------------
//  魔法陣の移動処理する関数
//--------------------------------------------------------------------------------------
void MagicLightning::MoveMagicPos( D3DXVECTOR3 vecDirect , float fSpeed )
{
	m_magicPos += vecDirect * fSpeed;
}

//--------------------------------------------------------------------------------------
//  削除関数
//--------------------------------------------------------------------------------------
void MagicLightning::Delete( void )				
{
	Scene::Release( );

	if( m_handle != -1 )
	{
		EffekseerManager::Stop( m_handle );

		m_handle = -1;
	}
}