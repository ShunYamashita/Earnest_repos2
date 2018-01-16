//--------------------------------------------------------------------------------------
//  エネミーステート( Brave攻撃000( 前 ) )   ( enemyBraveAttack000Before.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyBraveAttack000Before.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "player.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	ENEMY_HOMIMG_TIME				= 90.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Init( void )
{
	//  親の初期化
	EnemyState::Init( );

	m_homingTime = 0;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Update( void )
{
	//  アクション
	Action( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Action( void )
{
	//  相手に当たらない様に
	GetEnemy( )->SetJudgeHit( false );

	//  前回の座標を代入
	m_beforePosition = GetEnemy( )->GetPos( );

	//  結果座標
	D3DXVECTOR2 resultPos;
	resultPos.x = GetEnemy( )->GetPos( ).x;
	resultPos.y = GetEnemy( )->GetPos( ).z;

	D3DXVECTOR3 targetPos;

	//  シーンクラスのポインタ
	Scene* pScene = NULL;			

	float fDistance = 0.0f;

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

			if( objType == Scene::OBJTYPE_PLAYER )
			{
				//  ダウンキャスト
				Player* pPlayer = ( Player* )pScene;

				//  目標の3D座標の取得
				targetPos = pPlayer->GetPos( );

				//  敵の座標の代入
				D3DXVECTOR2 posPlayer;
				posPlayer.x = pPlayer->GetPos( ).x;
				posPlayer.y = pPlayer->GetPos( ).z;

				//  距離の算出
				fDistance = sqrtf( ( posPlayer.x - resultPos.x ) * ( posPlayer.x - resultPos.x ) +
								   ( posPlayer.y - resultPos.y ) * ( posPlayer.y - resultPos.y ) );

				D3DXVECTOR2 vecDirect;
				D3DXVec2Normalize( &vecDirect , &( posPlayer - resultPos ) );

				D3DXVECTOR2 controllPoint;
				controllPoint = resultPos + vecDirect * fDistance * 0.2f;

				if( GetEnemy( )->GetHomingDirect( ) == false )
				{					
					controllPoint.x += vecDirect.y * fDistance * 0.12f;
					controllPoint.y -= vecDirect.x * fDistance * 0.12f;
				}
				else
				{
					controllPoint.x -= vecDirect.y * fDistance * 0.12f;
					controllPoint.y += vecDirect.x * fDistance * 0.12f;
				}

				//  ベジェ曲線から結果座標の算出
				resultPos = Utility::BezierCurve2D( resultPos , posPlayer , controllPoint , m_homingTime );
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  エネミー座標の設定
	D3DXVECTOR3 enemyPosition;
	enemyPosition.x = resultPos.x;
	enemyPosition.y += ( targetPos.y - GetEnemy( )->GetPos( ).y ) * 0.1f;
	enemyPosition.z = resultPos.y;
	GetEnemy( )->SetPosition( enemyPosition );

	//  エネミー方向ベクトルの設定
	D3DXVECTOR3 enemyVecDirect = GetEnemy( )->GetPos( ) - m_beforePosition;
	D3DXVec3Normalize( &enemyVecDirect , &enemyVecDirect );
	GetEnemy( )->SetVecDirect( enemyVecDirect );

	//  注視点を向いている方向に
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );

	//  総フレーム数から算出して足す
	m_homingTime += 1.0f / ( ENEMY_HOMIMG_TIME + fDistance * 0.8f );

	if( m_homingTime >= 0.8f || fDistance < 10.0f )
	{
		//  目標に向かったベクトルの代入
		enemyVecDirect = targetPos - GetEnemy( )->GetPos( );
		enemyVecDirect.y = 0.0f;
		D3DXVec3Normalize( &enemyVecDirect , &enemyVecDirect );
		GetEnemy( )->SetVecDirect( enemyVecDirect );

		m_homingTime = 0.8f;

		//  ブレイブ攻撃000( 後 )状態に変更
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

		float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

		//  攻撃側の当たり判定の代入
		D3DXVECTOR3 effekseerPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
		effekseerPosition.y += 2.0f;

		//  エフェクトの生成
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
															   D3DXVECTOR3( GetEnemy( )->GetPos( ).x , effekseerPosition.y , GetEnemy( )->GetPos( ).z ) ,
															   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
		GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );
	}
}
