//--------------------------------------------------------------------------------------
//  エフェクシア管理   ( effkseerManager.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "effekseerManager.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "game.h"
#include <algorithm>
#include <iostream>
#include <locale> 
#include <codecvt> 

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
::EffekseerRenderer::Renderer*	EffekseerManager::m_renderer = NULL;	//  描画管理インスタンス
::EffekseerSound::Sound*		EffekseerManager::m_sound = NULL;		//  サウンド管理インスタンス
::Effekseer::Manager*			EffekseerManager::m_manager = NULL;	//  エフェクト管理用インスタンス

std::map< std::string , ::Effekseer::Effect* >				EffekseerManager::m_effectMap;
std::unordered_map< std::string , ::Effekseer::Effect* >	EffekseerManager::m_effectMap2;

//--------------------------------------------------------------------------------------
//  エフェクシア管理クラスのコンストラクタ
//--------------------------------------------------------------------------------------
EffekseerManager::EffekseerManager( )
{

}

//--------------------------------------------------------------------------------------
//  エフェクシア管理クラスのデストラクタ
//--------------------------------------------------------------------------------------
EffekseerManager::~EffekseerManager( )
{

}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の初期化処理
//--------------------------------------------------------------------------------------
void EffekseerManager::Init( void )
{
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//IXAudio2*		  pXAudio = SceneManager::GetSound( )->GetXAudio( );

	// 描画管理インスタンスの生成
	m_renderer = ::EffekseerRendererDX9::Renderer::Create( pDevice , 10000 );

	// サウンド管理インスタンスの生成
	//m_sound = ::EffekseerSound::Sound::Create( pXAudio , 16 , 16 );

	// エフェクト管理用インスタンスの生成
	m_manager = ::Effekseer::Manager::Create( 10000 );

	// 描画方法の指定、独自に拡張しない限り定形文です。
	m_manager->SetSpriteRenderer( m_renderer->CreateSpriteRenderer( ) );
	m_manager->SetRibbonRenderer( m_renderer->CreateRibbonRenderer( ) );
	m_manager->SetRingRenderer( m_renderer->CreateRingRenderer( ) );
	m_manager->SetTrackRenderer( m_renderer->CreateTrackRenderer( ) );
	m_manager->SetModelRenderer( m_renderer->CreateModelRenderer( ) );

	// テクスチャ画像の読込方法の指定(パッケージ等から読み込む場合拡張する必要があります。)
	m_manager->SetTextureLoader( m_renderer->CreateTextureLoader( ) );
	m_manager->SetModelLoader( m_renderer->CreateModelLoader( ) );

	// サウンド再生用インスタンスの指定
	//m_manager->SetSoundPlayer( m_sound->CreateSoundPlayer( ) );

	// サウンドデータの読込方法の指定(圧縮フォーマット、パッケージ等から読み込む場合拡張する必要があります。)
	//m_manager->SetSoundLoader( m_sound->CreateSoundLoader( ) );

	// 座標系の指定( RHで右手系、LHで左手系 )
	//m_manager->SetCoordinateSystem( ::Effekseer::CoordinateSystem::LH );

	// カメラ行列を設定
	m_renderer->SetCameraMatrix( ::Effekseer::Matrix44( ).LookAtLH( ::Effekseer::Vector3D( 0.0f , 0.0f , -10.0f ) ,
																	::Effekseer::Vector3D( 0.0f , 0.0f , 0.0f ) ,
																	::Effekseer::Vector3D( 0.0f, 1.0f, 0.0f ) ) );

	::Effekseer::Effect*	effect = NULL;

	std::string aFileName[ EffekseerManager::TYPE_MAX ];

	aFileName[ TYPE_SLASH000 ]			= "sword000.efk";
	aFileName[ TYPE_SLASH001 ]			= "sword001.efk";
	aFileName[ TYPE_SLASH002 ]			= "sword002.efk";
	aFileName[ TYPE_SLASH003 ]			= "sword003.efk";
	aFileName[ TYPE_DRILL ]				= "drill.efk";
	aFileName[ TYPE_LANCE ]				= "lance.efk";
	aFileName[ TYPE_BLOOD_LANCE ]		= "bloodLance.efk";
	aFileName[ TYPE_HIT000 ]			= "hit.efk";
	aFileName[ TYPE_LASER000 ]			= "laser000.efk";
	aFileName[ TYPE_LASER001 ]			= "laser001.efk";
	aFileName[ TYPE_FIRE000 ]			= "fire000.efk";
	aFileName[ TYPE_FIRE001 ]			= "fire001.efk";
	aFileName[ TYPE_FIRE002 ]			= "fire002.efk";
	aFileName[ TYPE_FLAME ]				= "flame.efk";
	aFileName[ TYPE_BANE ]				= "bane.efk";
	aFileName[ TYPE_WATER ]				= "water.efk";
	aFileName[ TYPE_LIGHTNING ]			= "lightning.efk";
	aFileName[ TYPE_METOR ]				= "metor.efk";
	aFileName[ TYPE_PHANTASM_METOR ]	= "phantasmMetor.efk";
	aFileName[ TYPE_SANDSTORM ]			= "sandStorm.efk";
	aFileName[ TYPE_SNOWSTORM ]			= "snowStorm.efk";
	aFileName[ TYPE_SONICSTORM ]		= "sonicStorm.efk";
	aFileName[ TYPE_TORNADE000 ]		= "tornade.efk";
	aFileName[ TYPE_TORNADE001 ]		= "tornade2.efk";
	aFileName[ TYPE_MAGIC_CIRCLE ]		= "magicCircle.efk";
	aFileName[ TYPE_HEAL000 ]			= "heal000.efk";
	aFileName[ TYPE_HEAL001 ]			= "heal001.efk";
	aFileName[ TYPE_AURA ]				= "aura.efk";
	aFileName[ TYPE_BOSS_DEATH ]		= "boss_death.efk";
	aFileName[ TYPE_POWER_UP ]			= "powerUp.efk";
	aFileName[ TYPE_ATMOSPHERE ]		= "atmosphere.efk";
	aFileName[ TYPE_BRAVE ]				= "brave.efk";
	aFileName[ TYPE_SHIELD ]			= "shield.efk";
	aFileName[ TYPE_SMASH ]				= "smash.efk";
	aFileName[ TYPE_THUNDER ]			= "thunder.efk";
	aFileName[ TYPE_SYLPH ]				= "sylph.efk";
	aFileName[ TYPE_JUMP_FOOT ]			= "jump_foot.efk";

#pragma omp parallel for
	//  エフェクトの数分のループ
	for( int nCntEffekseer = 0; nCntEffekseer < EffekseerManager::TYPE_MAX; nCntEffekseer++ )
	{
		std::wstring_convert< std::codecvt_utf8< wchar_t > , wchar_t > cv;
		std::wstring ws = cv.from_bytes( aFileName[ nCntEffekseer ] );
		
		ws = L"data\\effekseer\\" + ws;

		// エフェクトの読込
		effect = Effekseer::Effect::Create( m_manager , ( const EFK_CHAR* )ws.c_str( ) );

		m_effectMap2.insert( std::make_pair( aFileName[ nCntEffekseer ] , effect ) );
	}
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の終了処理
//--------------------------------------------------------------------------------------
void EffekseerManager::Uninit( void )
{
	std::string aFileName[ EffekseerManager::TYPE_MAX ];

	aFileName[ TYPE_SLASH000 ]			= "sword000.efk";
	aFileName[ TYPE_SLASH001 ]			= "sword001.efk";
	aFileName[ TYPE_SLASH002 ]			= "sword002.efk";
	aFileName[ TYPE_SLASH003 ]			= "sword003.efk";
	aFileName[ TYPE_DRILL ]				= "drill.efk";
	aFileName[ TYPE_LANCE ]				= "lance.efk";
	aFileName[ TYPE_BLOOD_LANCE ]		= "bloodLance.efk";
	aFileName[ TYPE_HIT000 ]			= "hit.efk";
	aFileName[ TYPE_LASER000 ]			= "laser000.efk";
	aFileName[ TYPE_LASER001 ]			= "laser001.efk";
	aFileName[ TYPE_FIRE000 ]			= "fire000.efk";
	aFileName[ TYPE_FIRE001 ]			= "fire001.efk";
	aFileName[ TYPE_FIRE002 ]			= "fire002.efk";
	aFileName[ TYPE_FLAME ]				= "flame.efk";
	aFileName[ TYPE_BANE ]				= "bane.efk";
	aFileName[ TYPE_WATER ]				= "water.efk";
	aFileName[ TYPE_LIGHTNING ]			= "lightning.efk";
	aFileName[ TYPE_METOR ]				= "metor.efk";
	aFileName[ TYPE_PHANTASM_METOR ]	= "phantasmMetor.efk";
	aFileName[ TYPE_SANDSTORM ]			= "sandStorm.efk";
	aFileName[ TYPE_SNOWSTORM ]			= "snowStorm.efk";
	aFileName[ TYPE_SONICSTORM ]		= "sonicStorm.efk";
	aFileName[ TYPE_TORNADE000 ]		= "tornade.efk";
	aFileName[ TYPE_TORNADE001 ]		= "tornade2.efk";
	aFileName[ TYPE_MAGIC_CIRCLE ]		= "magicCircle.efk";
	aFileName[ TYPE_HEAL000 ]			= "heal000.efk";
	aFileName[ TYPE_HEAL001 ]			= "heal001.efk";
	aFileName[ TYPE_AURA ]				= "aura.efk";
	aFileName[ TYPE_BOSS_DEATH ]		= "boss_death.efk";
	aFileName[ TYPE_POWER_UP ]			= "powerUp.efk";
	aFileName[ TYPE_ATMOSPHERE ]		= "atmosphere.efk";
	aFileName[ TYPE_BRAVE ]				= "brave.efk";
	aFileName[ TYPE_SHIELD ]			= "shield.efk";
	aFileName[ TYPE_SMASH ]				= "smash.efk";
	aFileName[ TYPE_THUNDER ]			= "thunder.efk";
	aFileName[ TYPE_SYLPH ]				= "sylph.efk";
	aFileName[ TYPE_JUMP_FOOT ]			= "jump_foot.efk";

	::Effekseer::Effect*	effect = NULL;

#pragma omp parallel for
	//  エフェクトの数分のループ
	for( int nCntEffekseer = 0; nCntEffekseer < EffekseerManager::TYPE_MAX; nCntEffekseer++ )
	{
		effect = m_effectMap2.at( aFileName[ nCntEffekseer ] );

		if( effect != NULL )
		{
			// エフェクトを解放します。再生中の場合は、再生が終了した後、自動的に解放されます。
			ES_SAFE_RELEASE( effect );
		}
	}

	if( m_manager != NULL )
	{
		// エフェクト管理用インスタンスを破棄
		m_manager->Destroy( );
	}

	if( m_sound != NULL )
	{
		// サウンド用インスタンスを破棄
		m_sound->Destroy( );
	}

	if( m_renderer != NULL )
	{
		// 描画用インスタンスを破棄
		m_renderer->Destroy( );
	}
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の更新処理
//--------------------------------------------------------------------------------------
void EffekseerManager::Update( void )
{
	// 全てのエフェクトの更新
	m_manager->Update( );
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の描画処理
//--------------------------------------------------------------------------------------
void EffekseerManager::Draw( void )
{
	if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
	{
		// 投影行列を設定
		m_renderer->SetProjectionMatrix( ::Effekseer::Matrix44( ).PerspectiveFovLH( D3DX_PI / 3.0f ,
										 ( float )SCREEN_WIDTH / SCREEN_HEIGHT , 1.0f , 30000.0f ) );
	}
	else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
	{
		// 投影行列を設定
		m_renderer->SetProjectionMatrix( ::Effekseer::Matrix44( ).PerspectiveFovLH( D3DX_PI / 3.0f ,
										 ( float )( SCREEN_WIDTH / 2 ) / ( SCREEN_HEIGHT ) , 1.0f , 30000.0f ) );
	}

	if( m_renderer != NULL )
	{
		//  レンダリング開始
		m_renderer->BeginRendering( );

		if( m_manager != NULL )
		{
			//  描画
			m_manager->Draw( );
		}

		//  レンダリング終了
		m_renderer->EndRendering( );
	}

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	// サンプラーステートの設定
	pDevice->SetSamplerState( 0 , D3DSAMP_ADDRESSU , D3DTADDRESS_WRAP );		// テクスチャＵ値の繰り返し設定
	pDevice->SetSamplerState( 0 , D3DSAMP_ADDRESSV , D3DTADDRESS_WRAP );		// テクスチャＶ値の繰り返し設定
	pDevice->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_ANISOTROPIC );	// テクスチャ拡大時の補間設定
	pDevice->SetSamplerState( 0 , D3DSAMP_MAXANISOTROPY , 8 );					// テクスチャの補間精度の設定
	pDevice->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );			// テクスチャ縮小時の補間設定
	pDevice->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );			// テクスチャ縮小時の補間設定
}

//--------------------------------------------------------------------------------------
//  エフェクシア管理の描画処理
//--------------------------------------------------------------------------------------
::Effekseer::Handle EffekseerManager::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed )
{
	::Effekseer::Handle		handle = -1;

	std::string aFileName[ EffekseerManager::TYPE_MAX ];

	aFileName[ TYPE_SLASH000 ]			= "sword000.efk";
	aFileName[ TYPE_SLASH001 ]			= "sword001.efk";
	aFileName[ TYPE_SLASH002 ]			= "sword002.efk";
	aFileName[ TYPE_SLASH003 ]			= "sword003.efk";
	aFileName[ TYPE_DRILL ]				= "drill.efk";
	aFileName[ TYPE_LANCE ]				= "lance.efk";
	aFileName[ TYPE_BLOOD_LANCE ]		= "bloodLance.efk";
	aFileName[ TYPE_HIT000 ]			= "hit.efk";
	aFileName[ TYPE_LASER000 ]			= "laser000.efk";
	aFileName[ TYPE_LASER001 ]			= "laser001.efk";
	aFileName[ TYPE_FIRE000 ]			= "fire000.efk";
	aFileName[ TYPE_FIRE001 ]			= "fire001.efk";
	aFileName[ TYPE_FIRE002 ]			= "fire002.efk";
	aFileName[ TYPE_FLAME ]				= "flame.efk";
	aFileName[ TYPE_BANE ]				= "bane.efk";
	aFileName[ TYPE_WATER ]				= "water.efk";
	aFileName[ TYPE_LIGHTNING ]			= "lightning.efk";
	aFileName[ TYPE_METOR ]				= "metor.efk";
	aFileName[ TYPE_PHANTASM_METOR ]	= "phantasmMetor.efk";
	aFileName[ TYPE_SANDSTORM ]			= "sandStorm.efk";
	aFileName[ TYPE_SNOWSTORM ]			= "snowStorm.efk";
	aFileName[ TYPE_SONICSTORM ]		= "sonicStorm.efk";
	aFileName[ TYPE_TORNADE000 ]		= "tornade.efk";
	aFileName[ TYPE_TORNADE001 ]		= "tornade2.efk";
	aFileName[ TYPE_MAGIC_CIRCLE ]		= "magicCircle.efk";
	aFileName[ TYPE_HEAL000 ]			= "heal000.efk";
	aFileName[ TYPE_HEAL001 ]			= "heal001.efk";
	aFileName[ TYPE_AURA ]				= "aura.efk";
	aFileName[ TYPE_BOSS_DEATH ]		= "boss_death.efk";
	aFileName[ TYPE_POWER_UP ]			= "powerUp.efk";
	aFileName[ TYPE_ATMOSPHERE ]		= "atmosphere.efk";
	aFileName[ TYPE_BRAVE ]				= "brave.efk";
	aFileName[ TYPE_SHIELD ]			= "shield.efk";
	aFileName[ TYPE_SMASH ]				= "smash.efk";
	aFileName[ TYPE_THUNDER ]			= "thunder.efk";
	aFileName[ TYPE_SYLPH ]				= "sylph.efk";
	aFileName[ TYPE_JUMP_FOOT ]			= "jump_foot.efk";

	// エフェクトの再生
	handle = m_manager->Play( m_effectMap2.at( aFileName[ ( int )type ] ) , position.x , position.y , position.z );
	m_manager->SetRotation( handle , rot.x , rot.y , rot.z );
	m_manager->SetScale( handle , scale.x , scale.y , scale.z );
	m_manager->SetSpeed( handle , fSpeed ); 

	return handle;
}

//--------------------------------------------------------------------------------------
//  特定のエフェクシアの再生ストップ
//--------------------------------------------------------------------------------------
void EffekseerManager::Stop( ::Effekseer::Handle handle )
{
	if( handle == -1 )
	{
		return;
	}

	if( m_manager != NULL )
	{
		// エフェクト管理用インスタンスを破棄
		m_manager->StopEffect( handle );
	}
}

//--------------------------------------------------------------------------------------
//  全ての特定のエフェクシアの再生ストップ
//--------------------------------------------------------------------------------------
void EffekseerManager::StopAll( void )
{
	if( m_manager != NULL )
	{
		m_manager->StopAllEffects( );
	}
}

//--------------------------------------------------------------------------------------
//  同一ディレクトリのファイル名取得
//--------------------------------------------------------------------------------------
std::vector<std::string> EffekseerManager::get_file_path_in_dir( const std::string& dir_name )
{
	HANDLE						hFind;
	WIN32_FIND_DATA				win32fd;
	std::vector< std::string >	file_names;

	//拡張子の設定
	std::string search_name = dir_name + "\\*";

	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		MessageBox( NULL , "ファイル名を取得出来ませんでした。( effekseer.cpp )" , "Error!!" , MB_OK );
	}

	do 
	{
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			MessageBox( NULL , "ファイル名を取得出来ませんでした。( effekseer.cpp )" , "Error!!" , MB_OK );
		}
		else 
		{
			file_names.push_back(win32fd.cFileName);
		}
	} while ( FindNextFile( hFind , &win32fd ) );

	FindClose( hFind );

	return file_names;
}

//--------------------------------------------------------------------------------------
//  エフェクシアの座標を設定する処理
//--------------------------------------------------------------------------------------
void EffekseerManager::SetPosition( ::Effekseer::Handle handle , D3DXVECTOR3 position )
{
	if( handle == -1 )
	{
		return;
	}

	m_manager->SetLocation( handle , position.x , position.y , position.z );
}

//--------------------------------------------------------------------------------------
//  エフェクシアの回転を設定する処理
//--------------------------------------------------------------------------------------
void EffekseerManager::SetRot( ::Effekseer::Handle handle , D3DXVECTOR3 rot )
{
	if( handle == -1 )
	{
		return;
	}

	m_manager->SetRotation( handle , rot.x , rot.y , rot.z );
}

//--------------------------------------------------------------------------------------
//  エフェクシアの大きさを設定する処理
//--------------------------------------------------------------------------------------
void EffekseerManager::SetScale( ::Effekseer::Handle handle , D3DXVECTOR3 scale )
{
	if( handle == -1 )
	{
		return;
	}

	m_manager->SetScale( handle , scale.x , scale.y , scale.z );
}

//--------------------------------------------------------------------------------------
//  エフェクシアの速度を設定する処理
//--------------------------------------------------------------------------------------
void EffekseerManager::SetSpeed( ::Effekseer::Handle handle , float fSpeed )
{
	if( handle == -1 )
	{
		return;
	}

	m_manager->SetSpeed( handle , fSpeed );
}

//--------------------------------------------------------------------------------------
//  エフェクシアの行列を設定する処理
//--------------------------------------------------------------------------------------
void EffekseerManager::SetMatrix( int nCameraNo )
{
	//  カメラ情報の取得
	Camera* pCamera = SceneManager::GetCamera( nCameraNo );

	//  カメラクラスのポインタがある場合
	if( pCamera != NULL )
	{
		//  カメラの視点、注視点、上向き方向
		::Effekseer::Vector3D posEye( pCamera->GetCameraPosEye( ).x , pCamera->GetCameraPosEye( ).y , pCamera->GetCameraPosEye( ).z );
		::Effekseer::Vector3D posAt( pCamera->GetCameraPosAt( ).x , pCamera->GetCameraPosAt( ).y , pCamera->GetCameraPosAt( ).z );
		::Effekseer::Vector3D vecUp( 0.0f, 1.0f, 0.0f );
		
		// カメラ行列を設定
		m_renderer->SetCameraMatrix( ::Effekseer::Matrix44( ).LookAtLH( posEye , posAt , vecUp ) );
	}

	// 3Dサウンド用リスナー設定の更新
	//m_sound->SetListener( position , posAt , vecUp );	
}

//--------------------------------------------------------------------------------------
//  エフェクシアが存在しているかを取得する処理
//--------------------------------------------------------------------------------------
bool EffekseerManager::GetExist( Effekseer::Handle handle )
{
	if( handle == -1 )
	{
		return false;
	}

	return m_manager->Exists( handle );
}