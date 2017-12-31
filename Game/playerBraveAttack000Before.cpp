//--------------------------------------------------------------------------------------
//  プレイヤーステート( Brave攻撃000( 前 ) )   ( playerBraveAttack000Before.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "playerBraveAttack000Before.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "enemy.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	PLAYER_HOMIMG_TIME				= 90.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Init( void )
{
	//  親の初期化
	PlayerState::Init( );

	m_homingTime = 0;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Update( void )
{
	//  カメラ制御
	ControlCamera( );

	//  アクション
	Action( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Action( void )
{
	//  相手に当たらない様に
	GetPlayer( )->SetJudgeHit( false );

	//  前回の座標を代入
	m_beforePosition = GetPlayer( )->GetPos( );

	//  結果座標
	D3DXVECTOR2 resultPos;
	resultPos.x = GetPlayer( )->GetPos( ).x;
	resultPos.y = GetPlayer( )->GetPos( ).z;

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

			//  CPU対戦モードの場合
			if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
			{
				//  種類が敵の場合
				if( objType == Scene::OBJTYPE_ENEMY )
				{
					//  ダウンキャスト
					Enemy* pEnemy = ( Enemy* )pScene;

					//  目標の3D座標の取得
					targetPos = pEnemy->GetPos( );

					//  敵の座標の代入
					D3DXVECTOR2 posEnemy;
					posEnemy.x = pEnemy->GetPos( ).x;
					posEnemy.y = pEnemy->GetPos( ).z;

					//  距離の算出
					fDistance = sqrtf( ( posEnemy.x - resultPos.x ) * ( posEnemy.x - resultPos.x ) +
										( posEnemy.y - resultPos.y ) * ( posEnemy.y - resultPos.y ) );

					D3DXVECTOR2 vecDirect;
					D3DXVec2Normalize( &vecDirect , &( posEnemy - resultPos ) );

					D3DXVECTOR2 controllPoint;
					controllPoint = resultPos + vecDirect * fDistance * 0.2f;

					if( GetPlayer( )->GetHomingDirect( ) == false )
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
					resultPos = Utility::BezierCurve2D( resultPos , posEnemy , controllPoint , m_homingTime );
				}
			}
			//  プレイヤー対戦モードの場合
			else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
			{
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  ダウンキャスト
					Player* pPlayer = ( Player* )pScene;

					//  自分以外のプレイヤー番号の場合
					if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
					{
						//  目標の3D座標の取得
						targetPos = pPlayer->GetPos( );

						//  敵の座標の代入
						D3DXVECTOR2 posEnemy;
						posEnemy.x = pPlayer->GetPos( ).x;
						posEnemy.y = pPlayer->GetPos( ).z;

						//  距離の算出
						fDistance = sqrtf( ( posEnemy.x - resultPos.x ) * ( posEnemy.x - resultPos.x ) +
										   ( posEnemy.y - resultPos.y ) * ( posEnemy.y - resultPos.y ) );

						D3DXVECTOR2 vecDirect;
						D3DXVec2Normalize( &vecDirect , &( posEnemy - resultPos ) );

						D3DXVECTOR2 controllPoint;
						controllPoint = resultPos + vecDirect * fDistance * 0.2f;

						if( GetPlayer( )->GetHomingDirect( ) == false )
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
						resultPos = Utility::BezierCurve2D( resultPos , posEnemy , controllPoint , m_homingTime );
					}
				}
			}

			//  次のポインタを代入
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  プレイヤー座標の設定
	D3DXVECTOR3 playerPosition;
	playerPosition.x = resultPos.x;
	playerPosition.y += ( targetPos.y - GetPlayer( )->GetPos( ).y ) * 0.1f;
	playerPosition.z = resultPos.y;
	GetPlayer( )->SetPosition( playerPosition );

	//  プレイヤー方向ベクトルの設定
	D3DXVECTOR3 playerVecDirect = GetPlayer( )->GetPos( ) - m_beforePosition;
	D3DXVec3Normalize( &playerVecDirect , &playerVecDirect );
	GetPlayer( )->SetVecDirect( playerVecDirect );

	//  注視点を向いている方向に
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  総フレーム数から算出して足す
	m_homingTime += 1.0f / ( PLAYER_HOMIMG_TIME + fDistance * 0.8f );

	if( m_homingTime >= 0.8f || fDistance < 10.0f )
	{
		//  目標に向かったベクトルの代入
		playerVecDirect = targetPos - GetPlayer( )->GetPos( );
		playerVecDirect.y = 0.0f;
		D3DXVec3Normalize( &playerVecDirect , &playerVecDirect );
		GetPlayer( )->SetVecDirect( playerVecDirect );

		m_homingTime = 0.8f;

		//  ブレイブ攻撃000( 後 )状態に変更
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  攻撃側の当たり判定の代入
		D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
		effekseerPosition.y += 2.0f;

		//  エフェクトの生成
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
															   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , effekseerPosition.y , GetPlayer( )->GetPos( ).z ) ,
															   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
		GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );
	}
}
