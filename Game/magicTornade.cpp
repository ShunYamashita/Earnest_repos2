//--------------------------------------------------------------------------------------
//  竜巻魔法クラス   ( magicTornade.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/09/18
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "magicTornade.h"
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
static const float	TORNADE_BLOW_POWER = 5.0f;

//--------------------------------------------------------------------------------------
//  竜巻魔法の初期化処理をする関数
//--------------------------------------------------------------------------------------
HRESULT MagicTornade::Init( void )
{
	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_MAGIC_LIGHTNING );

	m_firstPos = m_position;
	m_magicPos = m_position + m_vecDirect * 2.0f;
	m_bActive = false;
	m_fAngle = D3DX_PI;
	m_fDisance = 2.0f;
	m_fScale = 1.0f;
	m_fSpeed = 1.0f;
	m_moveVecDirect = m_vecDirect;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  竜巻魔法の終了処理をする関数
//--------------------------------------------------------------------------------------
void MagicTornade::Uninit( void )
{
}

//--------------------------------------------------------------------------------------
//  竜巻魔法の更新処理をする関数
//--------------------------------------------------------------------------------------
void MagicTornade::Update( void )
{
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
		//  速度分の移動
		m_magicPos += m_moveVecDirect * ( m_fSpeed - m_fScale * 0.01f );

		//  竜巻の設定
		EffekseerManager::SetPosition( m_handle[ 0 ] , m_magicPos );
		EffekseerManager::SetPosition( m_handle[ 1 ] , m_magicPos );

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
							hitCircle.fLength = m_size.x * m_fScale * 4.0f;

							D3DXVECTOR3 blowVecDirect( 0.0f , 0.0f , 0.0f );
							blowVecDirect.x = playerPos.x - hitCircle.position.x;
							blowVecDirect.z = playerPos.y - hitCircle.position.y;
							D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

							//  弾とプレイヤーが当たった場合
							if( Utility::HitCirclePoint( hitCircle , playerPos ) )
							{
								//  プレイヤーにダメージ処理
								pPlayer->Damage( blowVecDirect , TORNADE_BLOW_POWER , ( int )( 90 * m_fScale ) );
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
							hitCircle.fLength = m_size.x * m_fScale * 4.0f;

							//  弾と敵が当たった場合
							if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
							{
								//  敵にダメージ処理
								pEnemy->Damage( ( int )( 90 * m_fScale ) );
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
								hitCircle.fLength = m_size.x * m_fScale * 4.0f;

								D3DXVECTOR3 blowVecDirect( 0.0f , 0.0f , 0.0f );
								blowVecDirect.x = enemyPos.x - hitCircle.position.x;
								blowVecDirect.z = enemyPos.y - hitCircle.position.y;
								D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

								//  弾と敵が当たった場合
								if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
								{
									//  敵にダメージ処理
									pPlayer->Damage( blowVecDirect , TORNADE_BLOW_POWER , ( int )( 90 * m_fScale ) );
								}
							}
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}

		//  ライフカウント
		if( m_nLife > 0 )
		{
			m_nLife--;
		}
		else
		{
			//  自身の削除
			Release( );

			//  停止
			EffekseerManager::Stop( m_handle[ 0 ] );
			EffekseerManager::Stop( m_handle[ 1 ] );
		}

		//  壁との当たり判定
		if( m_position.x > Game::GetFieldMax( ).x )
		{
			Release( );

			//  停止
			EffekseerManager::Stop( m_handle[ 0 ] );
			EffekseerManager::Stop( m_handle[ 1 ] );
		}
		if( m_position.x < Game::GetFieldMin( ).x )
		{
			Release( );

			//  停止
			EffekseerManager::Stop( m_handle[ 0 ] );
			EffekseerManager::Stop( m_handle[ 1 ] );
		}
		if( m_position.z > Game::GetFieldMax( ).z )
		{
			Release( );

			//  停止
			EffekseerManager::Stop( m_handle[ 0 ] );
			EffekseerManager::Stop( m_handle[ 1 ] );
		}
		if( m_position.z < Game::GetFieldMin( ).z )
		{
			Release( );

			//  停止
			EffekseerManager::Stop( m_handle[ 0 ] );
			EffekseerManager::Stop( m_handle[ 1 ] );
		}
	}
	else 
	{
		m_fDisance += 0.1f;

		if( m_fDisance > 9.0f )
		{
			m_fDisance = 9.0f;
		}

		m_fScale += 0.05f;

		if( m_fScale > 5.0f )
		{
			m_fScale = 5.0f;
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
							   D3DXCOLOR( 0.2f , 0.6f , 0.4f , 1.0f ) , m_fScale , 0.1f , 0.01f , 0.0f , 0.0f , 20 ,
							   m_vecDirect.x , m_vecDirect.y , m_vecDirect.z );
		}
	}
}

//--------------------------------------------------------------------------------------
//  竜巻魔法の描画処理をする関数
//--------------------------------------------------------------------------------------
void MagicTornade::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
MagicTornade* MagicTornade::Create( OWNER owner , D3DXVECTOR3 position , D3DXVECTOR3 rot ,
									  D3DXVECTOR3 scale , D3DXVECTOR3 vecDirect , int nPlayerNo , int nLife )
{
	MagicTornade* pMagicTornade = NULL;

	//  インスタンス生成
	pMagicTornade = new MagicTornade( );

	//  所有者の代入
	pMagicTornade->m_owner = owner;

	//  座標の代入
	pMagicTornade->m_position = position;

	//  回転の代入
	pMagicTornade->m_rot = rot;

	//  大きさの代入
	pMagicTornade->m_size = scale;

	//  方向ベクトルの代入
	pMagicTornade->m_vecDirect = vecDirect;

	//  プレイヤー番号の代入
	pMagicTornade->m_nPlayerNo = nPlayerNo;

	//  体力の代入
	pMagicTornade->m_nLife = nLife;

	//  初期化
	pMagicTornade->Init( );

	return pMagicTornade;
}

//--------------------------------------------------------------------------------------
//  所有者の確認をする関数
//--------------------------------------------------------------------------------------
MagicTornade::OWNER MagicTornade::GetOwner( void )
{
	return m_owner;
}

//--------------------------------------------------------------------------------------
//  竜巻魔法が何かに当たった時の処理をする関数
//--------------------------------------------------------------------------------------
void MagicTornade::Hit( void )				
{
	//  自身の削除
	Scene::Release( );
}

//--------------------------------------------------------------------------------------
//  アクティブ状態にする関数
//--------------------------------------------------------------------------------------
void MagicTornade::SetActive( void )				
{
	if( m_bActive == false )
	{
		m_bActive = true;

		//  エフェクトの生成
		m_handle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_TORNADE001 ,
												   m_magicPos , m_rot ,
												   D3DXVECTOR3( m_size.x * m_fScale , m_size.y * m_fScale , m_size.z * m_fScale ) ,
												   1.0f );

		//  エフェクトの生成
		m_handle[ 1 ] = EffekseerManager::Create( EffekseerManager::TYPE_TORNADE001 ,
												   m_magicPos , m_rot ,
												   D3DXVECTOR3( m_size.x * m_fScale , m_size.y * m_fScale , m_size.z * m_fScale ) ,
												   1.0f );

		//  大きさによってライフの増加
		m_nLife += ( int )( m_fScale * 150.0f );
	}
}

//--------------------------------------------------------------------------------------
//  移動方向ベクトルの設定  
//--------------------------------------------------------------------------------------
void MagicTornade::SetMoveVecDirect( D3DXVECTOR3 move )			
{
	m_moveVecDirect = move;
}

//--------------------------------------------------------------------------------------
//  削除関数
//--------------------------------------------------------------------------------------
void MagicTornade::Delete( void )				
{
	Release( );

	//  停止
	EffekseerManager::Stop( m_handle[ 0 ] );
	EffekseerManager::Stop( m_handle[ 1 ] );
}