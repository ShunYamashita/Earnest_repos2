//--------------------------------------------------------------------------------------
//  エネミーステート( 突進攻撃 )   ( enemyRushAttack.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "enemyRushAttack.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "effekseerManager.h"
#include "player.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
static const float	ENEMY_TO_CAMERA_DISTANCE_Y		= 9.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Y2		= 10.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z		= 20.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z2		= 23.0f;

static const float	CAMERA_ROTATE_ANGLE				= 0.025f;

static const float	ENEMY_RUSH_ATTACK_SPEED			= 3.0f;
static const int	ENEMY_RUSH_ATTACK_DAMAGE		= 250;
static const float	ENEMY_RUSH_ATTACK_BLOW_POWER	= 7.0f;

static const float	ENEMY_HIT_SPHERE_POS_Y			= 3.0f;

static const float	ENEMY_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  初期化処理をする関数
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Init( void )
{
	//  親の初期化
	EnemyState::Init( );

	//  速度の初期化
	m_speed = ENEMY_RUSH_ATTACK_SPEED;
}

//--------------------------------------------------------------------------------------
//  終了処理をする関数
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Uninit( void )
{
	m_speed = 0.0f;
}

//--------------------------------------------------------------------------------------
//  更新処理をする関数
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Update( void )
{
	//  アクション
	Action( );

	//  重力をかける
	AddGravity( );

	//  ニュートラル状態に変更するかの判断
	JudgeChangeNeutral( );
}

//--------------------------------------------------------------------------------------
//  描画処理をする関数
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  アクションをする関数
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Action( void )
{
	//  座標の更新( 進行方向 × 速度 分の移動 )
	GetEnemy( )->MovePos( GetEnemy( )->GetVecDirect( ) * m_speed );

	//  注視点を向いている方向に
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );

	//  エフェクシア座標の設定
	D3DXVECTOR3 effekseerPos( GetEnemy( )->GetPos( ).x , GetEnemy( )->GetPos( ).y + ENEMY_HIT_SPHERE_POS_Y , GetEnemy( )->GetPos( ).z );
	effekseerPos += GetEnemy( )->GetVecDirect( ) * 8.0f;
	EffekseerManager::SetPosition( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_LANCE ) , effekseerPos );

	//  キーがある程度までいった場合
	if( GetEnemy( )->GetAnimationKey( ) >= 11 )
	{
		//  速度の減衰
		m_speed += ( 0.0f - m_speed ) * 0.06f;

		//  エフェクトの停止
		EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_LANCE ) );
	}

	//  攻撃側の当たり判定の代入
	Utility::HIT_SPHERE attackHitSphere;
	attackHitSphere.position = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position.y += 2.0f;
	attackHitSphere.fLength = ENEMY_ATTACK_HIT_SPHERE_LENGTH;

	//  シーンクラスのポインタ
	Scene* pScene = NULL;

	//  キーがこの範囲内にいる場合
	if( GetEnemy( )->GetAnimationKey( ) >= 1 && GetEnemy( )->GetAnimationKey( ) <= 10 )
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

				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  ダウンキャスト
					Player* pPlayer = ( Player* )pScene;

					if( Utility::HitSphere( attackHitSphere , pPlayer->GetHitSphere( ) ) )
					{
						//  吹っ飛び方向のベクトル算出
						D3DXVECTOR3 blowVecDirect = GetEnemy( )->GetVecDirect( );
						D3DXMATRIX mtxRot;
						D3DXMatrixIdentity( &mtxRot );
						D3DXMatrixRotationY( &mtxRot , D3DX_PI * 0.1f );
						D3DXVec3TransformNormal( &blowVecDirect , &blowVecDirect , &mtxRot );
						D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

						pPlayer->Damage( blowVecDirect , ENEMY_RUSH_ATTACK_BLOW_POWER , ENEMY_RUSH_ATTACK_DAMAGE );
					}
				}

				//  次のポインタを代入
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}
}