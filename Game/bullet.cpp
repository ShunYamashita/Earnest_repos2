//--------------------------------------------------------------------------------------
//  DirectX   ( bullet.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2016/06/07
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "bullet.h"
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
#define BULLET_TEXTURENAME000	"data/TEXTURE/BULLET/FFBullet.png"		//  テクスチャ名

#define BULLET_HOMING_REVICE	( 0.3f )								//  追尾比率
#define BULLET_HOMING_DISTANCE	( 25.0f )								//  追尾解除までの距離

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
Bullet::Bullet( int nPriority ) : Scene3D( nPriority )
{
	
}

//--------------------------------------------------------------------------------------
//  説明文の初期化処理をする関数
//--------------------------------------------------------------------------------------
HRESULT Bullet::Init( void )
{
	//  3Dポリゴンの初期化
	Scene3D::Init( );

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_BULLET );

	m_bHoming = true;
	m_hitSphere.fLength = 2.0f;
	m_hitSphere.position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_pShadow = NULL;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  説明文の終了処理をする関数
//--------------------------------------------------------------------------------------
void Bullet::Uninit( void )
{
	//  3Dポリゴンの解放
	Scene3D::Uninit( );
}

//--------------------------------------------------------------------------------------
//  説明文の更新処理をする関数
//--------------------------------------------------------------------------------------
void Bullet::Update( void )
{
	//  速度分の移動
	m_position += m_vecDirect * m_fSpeed;

	m_hitSphere.position = m_position;

	//  壁との当たり判定
	if( m_position.x > Game::GetFieldMax( ).x )
	{
		Release( );
	}
	if( m_position.x < Game::GetFieldMin( ).x )
	{
		Release( );
	}
	if( m_position.z > Game::GetFieldMax( ).z )
	{
		Release( );
	}
	if( m_position.z < Game::GetFieldMin( ).z )
	{
		Release( );
	}

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

			//  種類が敵の弾の場合
			if( m_objType == Scene::OBJTYPE_BULLET && m_owner == OWNER_ENEMY )
			{
				//  種類がプレイヤーの場合
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  プレイヤークラスにダウンキャスト
					Player* pPlayer = ( Player* )pScene;

					//  弾とプレイヤーが当たった場合
					if( Utility::HitSphere( m_hitSphere , pPlayer->GetHitSphere( ) ) )
					{
						if( m_type == TYPE_NORMAL )
						{
							D3DXVECTOR3 blowVecDirect = m_vecDirect;

							D3DXMATRIX mtxRot;
							D3DXMatrixIdentity( &mtxRot );
							D3DXMatrixRotationY( &mtxRot , D3DX_PI * 0.1f );

							D3DXVec3TransformNormal( &blowVecDirect , &blowVecDirect , &mtxRot );
							D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

							//  プレイヤーにダメージ処理
							pPlayer->Damage( blowVecDirect , 1.0f , 400 );

							//  自身の削除
							Release( );
						}
						else if( m_type == TYPE_WIDE )
						{
							D3DXVECTOR3 blowVecDirect = m_vecDirect;

							D3DXMATRIX mtxRot;
							D3DXMatrixIdentity( &mtxRot );
							D3DXMatrixRotationY( &mtxRot , D3DX_PI * 0.1f );

							D3DXVec3TransformNormal( &blowVecDirect , &blowVecDirect , &mtxRot );
							D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

							//  プレイヤーにダメージ処理
							pPlayer->Damage( blowVecDirect , 1.0f , 250 );
						}
					}
				}
			}
			//  種類がプレイヤーの弾の場合
			else if( m_objType == Scene::OBJTYPE_BULLET && m_owner == OWNER_PLAYER )
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
						pEnemy->Damage( m_vecDirect , 1.0f , 250 );

						//  自身の削除
						Scene::Release( );
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
				if( m_hitSphere.position.y - m_hitSphere.fLength * 0.5f < fFieldPosY )
				{
					//  自身の削除
					Release( );
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  3Dポリゴンの更新
	Scene3D::Update( );

	if( m_type == TYPE_WIDE )
	{
		//  追尾時間がある場合
		if( m_bHoming )
		{
			//  所有者が敵の場合
			if( m_owner == Bullet::OWNER_ENEMY )
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
							m_vecDirect.x += ( endVec.x - m_vecDirect.x ) * BULLET_HOMING_REVICE;
							m_vecDirect.y += ( endVec.y - m_vecDirect.y ) * BULLET_HOMING_REVICE;
							m_vecDirect.z += ( endVec.z - m_vecDirect.z ) * BULLET_HOMING_REVICE;

							//  2点間の距離を測る
							float fDistance = sqrtf( ( m_position.x - posPlayer.x ) * ( m_position.x - posPlayer.x ) +
													 ( m_position.y - posPlayer.y ) * ( m_position.y - posPlayer.y ) +
													 ( m_position.z - posPlayer.z ) * ( m_position.z - posPlayer.z ) );

							//  一定の距離近づいた場合
							if( fDistance < BULLET_HOMING_DISTANCE )
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
			else if( m_owner == Bullet::OWNER_PLAYER )
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
							m_vecDirect.x += ( endVec.x - m_vecDirect.x ) * BULLET_HOMING_REVICE;
							m_vecDirect.y += ( endVec.y - m_vecDirect.y ) * BULLET_HOMING_REVICE;
							m_vecDirect.z += ( endVec.z - m_vecDirect.z ) * BULLET_HOMING_REVICE;

							//  2点間の距離を測る
							float fDistance = sqrtf( ( m_position.x - posEnemy.x ) * ( m_position.x - posEnemy.x ) +
													 ( m_position.y - posEnemy.y ) * ( m_position.y - posEnemy.y ) +
													 ( m_position.z - posEnemy.z ) * ( m_position.z - posEnemy.z ) );

							//  一定の距離近づいた場合
							if( fDistance < BULLET_HOMING_DISTANCE )
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

		Particle::Create( Particle::TYPE_NORMAL , m_position , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
						   D3DXCOLOR( 0.8f , 0.2f , 0.1f , 1.0f ) , 5.0f , 0.1f , 0.01f , 0.0f , 0.0f , 20 ,
						   m_vecDirect.x , m_vecDirect.y , m_vecDirect.z );
	}

	if( m_objType == Scene::OBJTYPE_BULLET )
	{
		//  ライフカウント
		if( m_nLife > 0 )
		{
			m_nLife--;
		}
		else
		{
			//  自身の削除
			Release( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  説明文の描画処理をする関数
//--------------------------------------------------------------------------------------
void Bullet::Draw( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ライトクラスの取得
	Light* pLight = SceneManager::GetLight( );

	if( m_type == TYPE_NORMAL )
	{
		pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );						//  αブレンド設定( 加算処理 )	

		//  αテストの有効
		pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );

		//  条件式
		pDevice->SetRenderState( D3DRS_ALPHAREF , 1 );

		//  演算式
		pDevice->SetRenderState( D3DRS_ALPHAFUNC , D3DCMP_GREATER );

		//  ライトを消す
		pLight->LightOff( );

		for( int i = 0; i < 8; i++ )
		{
			//  3Dポリゴンの描画
			Scene3D::Draw( );
		}

		//  ライトを点ける
		pLight->LightOn( );

		//  αテストの無効
		pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );

		pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );				//  αブレンド設定( 半透過処理 )
	}
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Bullet* Bullet::Create( TYPE type , OWNER owner , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR3 normal ,
						  D3DXVECTOR3 rot , D3DXCOLOR color , D3DXVECTOR3 vecDirect , float fSpeed , int nLife )
{
	Bullet* pBullet = NULL;

	switch( type )
	{
		case TYPE_NORMAL:
		{
			//  インスタンス生成
			pBullet = new Bullet( 4 );

			//  ビルボード処理をする状態に
			pBullet->m_bInverse = true;

			break;
		}
		case TYPE_WIDE:
		{
			//  インスタンス生成
			pBullet = new Bullet( 7 );

			break;
		}
	}

	//  種類の代入
	pBullet->m_type = type;

	//  所有者の代入
	pBullet->m_owner = owner;

	//  座標の代入
	pBullet->m_position = position;

	//  大きさの代入
	pBullet->m_size = size;

	//  回転の代入
	pBullet->m_rot = rot;

	//  色の代入
	pBullet->m_color = color;

	//  進行方向ベクトルの代入
	pBullet->m_vecDirect = vecDirect;

	//  速度の代入
	pBullet->m_fSpeed = fSpeed;

	//  体力の代入
	pBullet->m_nLife = nLife;

	switch( type )
	{
		case TYPE_NORMAL:
		{
			//  テクスチャの設定
			pBullet->SetTextureName( BULLET_TEXTURENAME000 );

			//  ビルボード処理をする状態に
			pBullet->m_bInverse = true;

			break;
		}
		case TYPE_WIDE:
		{
			//  テクスチャの設定
			pBullet->SetTextureName( BULLET_TEXTURENAME000 );

			break;
		}
	}

	//  回転角増加分の代入
	pBullet->m_fAddRot = -1.0f;

	//  初期化
	pBullet->Init( );

	return pBullet;
}

//--------------------------------------------------------------------------------------
//  説明文の終了処理をする関数
//--------------------------------------------------------------------------------------
Utility::HIT_SPHERE Bullet::GetHitSphere( void )
{
	return m_hitSphere;
}

//--------------------------------------------------------------------------------------
//  種類の取得をする関数
//--------------------------------------------------------------------------------------
Bullet::TYPE Bullet::GetType( void )
{
	return m_type;
}

//--------------------------------------------------------------------------------------
//  所有者の確認をする関数
//--------------------------------------------------------------------------------------
Bullet::OWNER Bullet::GetOwner( void )
{
	return m_owner;
}

//--------------------------------------------------------------------------------------
//  弾が何かに当たった時の処理をする関数
//--------------------------------------------------------------------------------------
void Bullet::Hit( D3DXVECTOR3 vecDirect )				
{
	switch( m_type )
	{
		case TYPE_NORMAL:
		{
			//  所有者の変更
			if( m_owner == OWNER_PLAYER )
			{
				m_owner = OWNER_ENEMY;
			}
			else if( m_owner == OWNER_ENEMY )
			{
				m_owner = OWNER_PLAYER;
			}

			//  進行方向を変える
			m_vecDirect = vecDirect;

			//  速度を上昇
			m_fSpeed *= 1.5f;

			::Effekseer::Handle handle;

			//  エフェクトの生成
			handle = EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_position.x , m_position.y , m_position.z ) ,
												D3DXVECTOR3( 0.0f , -D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) );

			break;
		}
		case TYPE_WIDE:
		{
			//  自身の削除
			Scene::Release( );

			::Effekseer::Handle handle;

			//  エフェクトの生成
			handle = EffekseerManager::Create( EffekseerManager::TYPE_HEAL000 , D3DXVECTOR3( m_position.x , m_position.y , m_position.z ) ,
												D3DXVECTOR3( 0.0f , -D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) );

			break;
		}
	}
}