//--------------------------------------------------------------------------------------
//  DirectX   ( particle.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2016/06/07
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "particle.h"
#include "camera.h"
#include "player.h"
#include "light.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "shaderManager.h"
#include <random>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define PARTICLE_TEXTURENAME000		"data/TEXTURE/EFFECT/effect000.jpg"			//  テクスチャ名

#define PARTICLE_BASE_ADD_SCALE		( 1.2f )									//  基底値スケール増やす量
#define PARTICLE_ADD_SCALE			( 0.01f )									//  スケール増やす量

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
Particle::Particle( int nPriority ) : Scene3D( nPriority )
{

}

//--------------------------------------------------------------------------------------
//  粒子の初期化処理をする関数
//--------------------------------------------------------------------------------------
HRESULT Particle::Init( void )
{
	//  3Dポリゴンの初期化
	Scene3D::Init( );

	m_fAddScale = 0.0f;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  粒子の終了処理をする関数
//--------------------------------------------------------------------------------------
void Particle::Uninit( void )
{
	//  3Dポリゴンの解放
	Scene3D::Uninit( );
}

//--------------------------------------------------------------------------------------
//  粒子の更新処理をする関数
//--------------------------------------------------------------------------------------
void Particle::Update( void )
{
	//  種類が通常の場合
	if( m_type == Particle::TYPE_NORMAL )
	{
		//  速度を加速させる
		m_fVelocity +=m_fAccel;

		//  座標を速度分移動させる
		m_position += m_vecDirect * m_fVelocity;

		//  長さを減らす
		m_fScale += ( 0.0f - m_fScale ) * 0.001f;
	}

	//  α値の値をライフから決める
	m_color.a -= 1.0f / m_fLife;

	//  体力を減らす
	m_fLife -= 1.0f;

	//  体力が0以下になった場合
	if( m_fLife <= 0.0f || m_color.a <= 0.0f )
	{
		//  自身の破棄
		Scene::Release( );
	}

	//  3Dポリゴンの更新
	Scene3D::Update( );
}

//--------------------------------------------------------------------------------------
//  粒子の描画処理をする関数
//--------------------------------------------------------------------------------------
void Particle::Draw( void )
{  
	//  カメラの情報取得
	Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

	//  シェーダー情報の取得
	Shader3DNoLight* shader3DNoLight = ( Shader3DNoLight* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_NO_LIGHT );

	if( pCamera != NULL )
	{
		D3DXVECTOR3 cameraToPos = m_position - pCamera->GetCameraPosEye( );
		cameraToPos.y = 0.0f;
		D3DXVec3Normalize( &cameraToPos , &cameraToPos );

		//  視界に入っていた場合
		if( Utility::IntoView( pCamera->GetCameraVecDirect( ) , cameraToPos , D3DX_PI * 0.5f ) )
		{
			//  メインからデバイス情報を取得
			LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

			//  αブレンド設定( 加算処理 )	
			pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_ONE );						

			//  αテストの有効
			pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );

			//  条件式
			pDevice->SetRenderState( D3DRS_ALPHAREF , 1 );

			//  演算式
			pDevice->SetRenderState( D3DRS_ALPHAFUNC , D3DCMP_GREATER );

			//  Zバッファの書き込み設定
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE , FALSE );

			//  3Dポリゴンの描画
			Scene3D::Draw( );

			//  Zバッファの書き込み設定
			pDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

			//  αテストの無効
			pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );

			//  αブレンド設定( 半透過処理 )
			pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );		
		}
	}
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Particle*	Particle::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXCOLOR color ,
							   float fScale , float fVelocity , float fAccel , float fSeekRot , float fRangeRot , float fLife ,
							   float fVecDirectX , float fVecDirectY , float fVecDirectZ )
{
	//  乱数の宣言
	std::random_device rd;

	//  乱数の設定
	std::uniform_int_distribution< int > RandomPos( -100 , 100 );
	std::uniform_int_distribution< int > RandomMove( -100 , 100 );
	std::uniform_int_distribution< int > RandomRot( 0 , 359 );

	Particle* pParticle;

	//  インスタンス生成
	pParticle = new Particle( 6 );

	//  種類の代入
	pParticle->m_type = type;

	//  大きさの代入
	pParticle->m_size = size;

	//  色の代入
	pParticle->m_color = color;

	//  大きさ倍率の代入
	pParticle->m_fScale = fScale;

	//  速度の代入
	pParticle->m_fVelocity = fVelocity;

	//  加速度の代入
	pParticle->m_fAccel = fAccel;

	//  体力の代入
	pParticle->m_fLife = ( float )( rand( ) % ( int )fLife ) + fLife;

	switch( type )
	{
		case Particle::TYPE_NORMAL:
		{
			//  座標の代入
			pParticle->m_position.x = position.x + RandomPos( rd ) * 0.01f;
			pParticle->m_position.y = position.y + RandomPos( rd ) * 0.01f;
			pParticle->m_position.z = position.z + RandomPos( rd ) * 0.01f;

			//  回転の代入
			pParticle->m_rot.x = 0.0f;
			pParticle->m_rot.y = 0.0f;
			pParticle->m_rot.z = 0.0f;

			//  テクスチャの設定
			pParticle->SetTextureName( PARTICLE_TEXTURENAME000 );
			pParticle->m_bInverse = true;

			//  移動方向の決定
			D3DXVECTOR3 vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

			while( 1 )
			{
				vecDirect = D3DXVECTOR3( ( float )RandomMove( rd ) , ( float )RandomMove( rd ) , ( float )RandomMove( rd ) );

				if( vecDirect.y > 0.0f )
				{
					//  単位ベクトル化
					D3DXVec3Normalize( &pParticle->m_vecDirect , &vecDirect );

					break;
				}
			}

			break;
		}
		case Particle::TYPE_WIDE:
		{
			//  座標の代入
			pParticle->m_position.x = position.x + RandomPos( rd ) * 0.01f;
			pParticle->m_position.y = position.y + RandomPos( rd ) * 0.01f;
			pParticle->m_position.z = position.z + RandomPos( rd ) * 0.01f;

			//  回転の代入
			pParticle->m_rot.x = 0.0f;
			pParticle->m_rot.y = 0.0f;
			pParticle->m_rot.z = 0.0f;

			//  テクスチャの設定
			pParticle->SetTextureName( PARTICLE_TEXTURENAME000 );
			pParticle->m_bInverse = true;

			//  基準方向ベクトルの代入
			D3DXVECTOR3 baseVecDirect( fVecDirectX , fVecDirectY , fVecDirectZ );

			//  移動方向の決定
			D3DXVECTOR3 vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

			while( 1 )
			{
				vecDirect = D3DXVECTOR3( ( float )RandomMove( rd ) , 0.0f , ( float )RandomMove( rd ) );

				//if( vecDirect.y > 0.0f )
				{
					//  単位ベクトル化
					D3DXVec3Normalize( &pParticle->m_vecDirect , &vecDirect );

					break;
				}
			}

			break;
		}
	}

	//  初期化
	pParticle->Init( );

	return pParticle;
}