//--------------------------------------------------------------------------------------
//  DirectX   ( magicFire.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/09/16
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "magicFire.h"
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
#define MAGIC_HOMING_REVICE		( 0.13f )								//  追尾比率
#define MAGIC_HOMING_DISTANCE	( 20.0f )								//  追尾解除までの距離

static const float	FIRE_BLOW_POWER = 1.0f;

//--------------------------------------------------------------------------------------
//  炎魔法の初期化処理をする関数
//--------------------------------------------------------------------------------------
HRESULT MagicFire::Init( void )
{
	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_MAGIC_FIRE );

	m_firstPos = m_position;
	m_bHoming = true;
	m_bActive = false;
	m_fAngle = D3DX_PI;
	m_fDisance = 2.0f;
	m_fScale = 1.0f;
	m_hitSphere.fLength = 2.0f;
	m_hitSphere.position = m_position;
	m_handle = -1;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  炎魔法の終了処理をする関数
//--------------------------------------------------------------------------------------
void MagicFire::Uninit( void )
{
}

//--------------------------------------------------------------------------------------
//  炎魔法の更新処理をする関数
//--------------------------------------------------------------------------------------
void MagicFire::Update( void )
{
	m_hitSphere.position = m_position;
	m_hitSphere.fLength = 2.0f * m_fScale;

	//  前回の座標の代入
	m_beforePos = m_position;

	if( m_bActive == true )
	{
		//  壁との当たり判定
		if( m_position.x > Game::GetFieldMax( ).x )
		{
			Scene::Release( );

			if( m_handle != -1 )
			{
				//  停止
				EffekseerManager::Stop( m_handle );

				m_handle = -1;
			}
		}
		if( m_position.x < Game::GetFieldMin( ).x )
		{
			Scene::Release( );

			if( m_handle != -1 )
			{
				//  停止
				EffekseerManager::Stop( m_handle );

				m_handle = -1;
			}
		}
		if( m_position.z > Game::GetFieldMax( ).z )
		{
			Scene::Release( );

			if( m_handle != -1 )
			{
				//  停止
				EffekseerManager::Stop( m_handle );

				m_handle = -1;
			}
		}
		if( m_position.z < Game::GetFieldMin( ).z )
		{
			Scene::Release( );

			if( m_handle != -1 )
			{
				//  停止
				EffekseerManager::Stop( m_handle );

				m_handle = -1;
			}
		}

		//  速度分の移動
		m_position += m_vecDirect * ( m_fSpeed + m_fScale * 0.2f );

		Scene *pScene = NULL;										//  シーンクラスのポインタ

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

							//  弾とプレイヤーが当たった場合
							if( Utility::HitSphere( m_hitSphere , pPlayer->GetHitSphere( ) ) )
							{
								D3DXVECTOR3 blowVecDirect = m_vecDirect;

								D3DXMATRIX mtxRot;
								D3DXMatrixIdentity( &mtxRot );
								D3DXMatrixRotationY( &mtxRot , D3DX_PI * 0.1f );

								D3DXVec3TransformNormal( &blowVecDirect , &blowVecDirect , &mtxRot );
								D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

								//  プレイヤーにダメージ処理
								pPlayer->Damage( blowVecDirect , FIRE_BLOW_POWER , ( int )( 75 * m_fScale ) );

								//  自身の削除
								Release( );		

								if( m_handle != -1 )
								{
									//  停止
									EffekseerManager::Stop( m_handle );

									m_handle = -1;
								}
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

							//  弾と敵が当たった場合
							if( Utility::HitSphere( m_hitSphere , pEnemy->GetHitSphere( ) ) )
							{
								//  敵にダメージ処理
								pEnemy->Damage( ( int )( 75 * m_fScale ) );

								//  自身の削除
								Scene::Release( );

								if( m_handle != -1 )
								{
									//  停止
									EffekseerManager::Stop( m_handle );

									m_handle = -1;
								}
							}
						}
					}
				}
				//  プレイヤー対戦モードの場合
				else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
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
								//  弾と敵が当たった場合
								if( Utility::HitSphere( m_hitSphere , pPlayer->GetHitSphere( ) ) )
								{
									//  敵プレイヤーにダメージ処理
									pPlayer->Damage( m_vecDirect , FIRE_BLOW_POWER , ( int )( 75 * m_fScale ) );

									//  自身の削除
									Scene::Release( );

									if( m_handle != -1 )
									{
										//  停止
										EffekseerManager::Stop( m_handle );

										m_handle = -1;
									}
								}
							}
						}
					}
				}

				if( objType == Scene::OBJTYPE_HIT_FIELD )
				{
					//  当たり判定用フィールドクラスにダウンキャスト
					HitField* pHitField = ( HitField* )pScene;

					//  フィールド座標Yの取得
					float fFieldPosY = pHitField->GetHeight( m_position );

					//  弾のY座標が地面のY座標を下回った場合
					if( m_hitSphere.position.y - m_hitSphere.fLength * 0.1f < fFieldPosY )
					{
						//  自身の削除
						Release( );

						if( m_handle != -1 )
						{
							//  停止
							EffekseerManager::Stop( m_handle );

							m_handle = -1;
						}
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}

		//  追尾時間がある場合
		if( m_bHoming )
		{
			//  CPU対戦モードの場合
			if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
			{
				//  所有者が敵の場合
				if( m_owner == MagicFire::OWNER_ENEMY )
				{
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
								//  プレイヤークラスにダウンキャスト
								Player* pPlayer = ( Player* )pScene;

								//  座標の取得
								D3DXVECTOR3 posPlayer = pPlayer->GetHitSphere( ).position;

								//  目標方向ベクトル
								D3DXVECTOR3 endVec = posPlayer - m_position;

								//  単位ベクトル化
								D3DXVec3Normalize( &endVec , &endVec );

								//  目標方向ベクトルに徐々に近づける
								m_vecDirect.x += ( endVec.x - m_vecDirect.x ) *( MAGIC_HOMING_REVICE + m_fScale * 0.05f );
								m_vecDirect.y += ( endVec.y - m_vecDirect.y ) *( MAGIC_HOMING_REVICE + m_fScale * 0.05f );
								m_vecDirect.z += ( endVec.z - m_vecDirect.z ) *( MAGIC_HOMING_REVICE + m_fScale * 0.05f );

								//  2点間の距離を測る
								float fDistance = sqrtf( ( m_position.x - posPlayer.x ) * ( m_position.x - posPlayer.x ) +
														 ( m_position.y - posPlayer.y ) * ( m_position.y - posPlayer.y ) +
														 ( m_position.z - posPlayer.z ) * ( m_position.z - posPlayer.z ) );

								//  一定の距離近づいた場合
								if( fDistance < MAGIC_HOMING_DISTANCE )
								{
									//  追尾しない状態に
									m_bHoming = false;
								}
							}

							//  次のポインタを代入
							pScene = pScene->GetNextScene( pScene );
						}
					}
				}
				//  所有者がプレイヤーの場合
				else if( m_owner == MagicFire::OWNER_PLAYER )
				{
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

							//  種類が敵の場合
							if( objType == Scene::OBJTYPE_ENEMY )
							{
								//  プレイヤークラスにダウンキャスト
								Enemy* pEnemy = ( Enemy* )pScene;

								//  座標の取得
								D3DXVECTOR3 posEnemy = pEnemy->GetHitSphere( ).position;

								//  目標方向ベクトル
								D3DXVECTOR3 endVec = posEnemy - m_position;

								//  単位ベクトル化
								D3DXVec3Normalize( &endVec , &endVec );

								//  目標方向ベクトルに徐々に近づける
								m_vecDirect.x += ( endVec.x - m_vecDirect.x ) * MAGIC_HOMING_REVICE;
								m_vecDirect.y += ( endVec.y - m_vecDirect.y ) * MAGIC_HOMING_REVICE;
								m_vecDirect.z += ( endVec.z - m_vecDirect.z ) * MAGIC_HOMING_REVICE;

								//  2点間の距離を測る
								float fDistance = sqrtf( ( m_position.x - posEnemy.x ) * ( m_position.x - posEnemy.x ) +
														 ( m_position.y - posEnemy.y ) * ( m_position.y - posEnemy.y ) +
														 ( m_position.z - posEnemy.z ) * ( m_position.z - posEnemy.z ) );

								//  一定の距離近づいた場合
								if( fDistance < MAGIC_HOMING_DISTANCE )
								{
									//  追尾しない状態に
									m_bHoming = false;
								}
							}

							//  次のポインタを代入
							pScene = pScene->GetNextScene( pScene );
						}
					}
				}
			}
			//  プレイヤー対戦モードの場合
			else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
			{
				//  所有者がプレイヤーの場合
				if( m_owner == MagicFire::OWNER_PLAYER )
				{
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

							//  種類が敵の場合
							if( objType == Scene::OBJTYPE_PLAYER )
							{
								//  プレイヤークラスにダウンキャスト
								Player* pPlayer = ( Player* )pScene;

								//  所有者プレイヤー番号以外の場合
								if( m_nPlayerNo != pPlayer->GetPlayerNo( ) )
								{
									//  座標の取得
									D3DXVECTOR3 posEnemy = pPlayer->GetHitSphere( ).position;

									//  目標方向ベクトル
									D3DXVECTOR3 endVec = posEnemy - m_position;

									//  単位ベクトル化
									D3DXVec3Normalize( &endVec , &endVec );

									//  目標方向ベクトルに徐々に近づける
									m_vecDirect.x += ( endVec.x - m_vecDirect.x ) * MAGIC_HOMING_REVICE;
									m_vecDirect.y += ( endVec.y - m_vecDirect.y ) * MAGIC_HOMING_REVICE;
									m_vecDirect.z += ( endVec.z - m_vecDirect.z ) * MAGIC_HOMING_REVICE;

									//  2点間の距離を測る
									float fDistance = sqrtf( ( m_position.x - posEnemy.x ) * ( m_position.x - posEnemy.x ) +
															 ( m_position.y - posEnemy.y ) * ( m_position.y - posEnemy.y ) +
															 ( m_position.z - posEnemy.z ) * ( m_position.z - posEnemy.z ) );

									//  一定の距離近づいた場合
									if( fDistance < MAGIC_HOMING_DISTANCE )
									{
										//  追尾しない状態に
										m_bHoming = false;
									}
								}
							}

							//  次のポインタを代入
							pScene = pScene->GetNextScene( pScene );
						}
					}
				}
			}
		}

		//  座標の代入
		EffekseerManager::SetPosition( m_handle , m_position );

		float fAngle = atan2f( m_vecDirect.x , m_vecDirect.z );

		m_rot.y = fAngle + D3DX_PI * 0.5f;

		//  回転の代入
		EffekseerManager::SetRot( m_handle , m_rot );

		//  大きさの代入
		EffekseerManager::SetScale( m_handle , D3DXVECTOR3( 2.0f * m_fScale , 2.0f * m_fScale , 2.0f * m_fScale ) );

		//  ライフカウント
		if( m_nLife > 0 )
		{
			m_nLife--;
		}
		else
		{
			//  自身の削除
			Scene::Release( );

			if( m_handle != -1 )
			{
				//  停止
				EffekseerManager::Stop( m_handle );

				m_handle = -1;
			}
		}
	}
	else 
	{
		m_fDisance += 0.1f;

		if( m_fDisance > 9.0f )
		{
			m_fDisance = 9.0f;
		}

		m_fScale += 0.03f;

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
	}

	for( int i = 0; i < 2; i++ )
	{
		Particle::Create( Particle::TYPE_NORMAL , m_position , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
							D3DXCOLOR( 0.8f , 0.2f , 0.1f , 1.0f ) , m_fScale , 0.1f , 0.01f , 0.0f , 0.0f , 20 ,
							m_vecDirect.x , m_vecDirect.y , m_vecDirect.z );
	}
}

//--------------------------------------------------------------------------------------
//  炎魔法の描画処理をする関数
//--------------------------------------------------------------------------------------
void MagicFire::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
MagicFire* MagicFire::Create( OWNER owner , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale ,
								D3DXVECTOR3 vecDirect ,  int nPlayerNo , float fSpeed , int nLife )
{
	MagicFire* pMagicFire = NULL;

	//  インスタンス生成
	pMagicFire = new MagicFire( );

	//  所有者の代入
	pMagicFire->m_owner = owner;

	//  座標の代入
	pMagicFire->m_position = position;

	//  回転の代入
	pMagicFire->m_rot = rot;

	//  大きさの代入
	pMagicFire->m_size = scale;

	//  進行方向ベクトルの代入
	pMagicFire->m_vecDirect = vecDirect;

	//  速度の代入
	pMagicFire->m_fSpeed = fSpeed;

	//  体力の代入
	pMagicFire->m_nLife = nLife;

	//  プレイヤー番号の代入
	pMagicFire->m_nPlayerNo = nPlayerNo;

	//  初期化
	pMagicFire->Init( );

	return pMagicFire;
}

//--------------------------------------------------------------------------------------
//  炎魔法の終了処理をする関数
//--------------------------------------------------------------------------------------
Utility::HIT_SPHERE MagicFire::GetHitSphere( void )
{
	return m_hitSphere;
}

//--------------------------------------------------------------------------------------
//  所有者の確認をする関数
//--------------------------------------------------------------------------------------
MagicFire::OWNER MagicFire::GetOwner( void )
{
	return m_owner;
}

//--------------------------------------------------------------------------------------
//  炎魔法が何かに当たった時の処理をする関数
//--------------------------------------------------------------------------------------
void MagicFire::Hit( void )				
{
	//  自身の削除
	Scene::Release( );

	//  停止
	EffekseerManager::Stop( m_handle );
}

//--------------------------------------------------------------------------------------
//  アクティブ状態にする関数
//--------------------------------------------------------------------------------------
void MagicFire::SetActive( void )				
{
	if( m_bActive == false )
	{
		if( m_handle == -1 )
		{
			m_bActive = true;

			//  エフェクトの生成
			m_handle = EffekseerManager::Create( EffekseerManager::TYPE_FIRE000 ,
												  m_position , m_rot , m_size , 1.0f );
		}
	}
}

//--------------------------------------------------------------------------------------
//  削除関数
//--------------------------------------------------------------------------------------
void MagicFire::Delete( void )				
{
	Scene::Release( );

	if( m_handle != -1 )
	{
		//  停止
		EffekseerManager::Stop( m_handle );
	}
}