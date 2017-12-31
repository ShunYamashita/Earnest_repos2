//--------------------------------------------------------------------------------------
//  シェーダー管理   ( shaderManager.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "shaderManager.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
Shader*	ShaderManager::m_shader[ ( int )ShaderManager::TYPE::SHADER_MAX ] = { nullptr };

//--------------------------------------------------------------------------------------
//  シェーダー管理クラスのコンストラクタ
//--------------------------------------------------------------------------------------
ShaderManager::ShaderManager( )
{
	//  シェーダーの種類分の初期化
	for( int countShaderType = 0; countShaderType < ( int )TYPE::SHADER_MAX; ++countShaderType )
	{
		m_shader[ countShaderType ] = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  シェーダー管理クラスのデストラクタ
//--------------------------------------------------------------------------------------
ShaderManager::~ShaderManager( )
{

}

//--------------------------------------------------------------------------------------
//  シェーダー管理の初期化処理
//--------------------------------------------------------------------------------------
void ShaderManager::Init( void )
{
	m_shader[ ( int )TYPE::SHADER_3D ] = new Shader3D( );
	m_shader[ ( int )TYPE::SHADER_3D ]->Init( );
	m_shader[ ( int )TYPE::SHADER_3D_NO_TEXTURE ] = new Shader3DNoTexture( );
	m_shader[ ( int )TYPE::SHADER_3D_NO_TEXTURE ]->Init( );
	m_shader[ ( int )TYPE::SHADER_3D_NO_LIGHT ] = new Shader3DNoLight( );
	m_shader[ ( int )TYPE::SHADER_3D_NO_LIGHT ]->Init( );
	m_shader[ ( int )TYPE::SHADER_3D_RIM_LIGHT_TEXTURE ] = new Shader3DRimLightTexture( );
	m_shader[ ( int )TYPE::SHADER_3D_RIM_LIGHT_TEXTURE ]->Init( );
	m_shader[ ( int )TYPE::SHADER_3D_DEPTH_SHADOW ] = new Shader3DDepthShadow( );
	m_shader[ ( int )TYPE::SHADER_3D_DEPTH_SHADOW ]->Init( );
	m_shader[ ( int )TYPE::SHADER_3D_SHADOW_MAP ] = new Shader3DShadowMap( );
	m_shader[ ( int )TYPE::SHADER_3D_SHADOW_MAP ]->Init( );
	m_shader[ ( int )TYPE::SHADER_3D_THUN ] = new Shader3DThun( );
	m_shader[ ( int )TYPE::SHADER_3D_THUN ]->Init( );
	m_shader[ ( int )TYPE::SHADER_3D_OUTLINE ] = new Shader3DOutline( );
	m_shader[ ( int )TYPE::SHADER_3D_OUTLINE ]->Init( );
	m_shader[ ( int )TYPE::SHADER_3D_PLAYER ] = new Shader3DPlayer( );
	m_shader[ ( int )TYPE::SHADER_3D_PLAYER ]->Init( );
}

//--------------------------------------------------------------------------------------
//  シェーダー管理の終了処理
//--------------------------------------------------------------------------------------
void ShaderManager::Uninit( void )
{
	//  シェーダーの種類分のループ
	for( int countShaderType = 0; countShaderType < ( int )TYPE::SHADER_MAX; ++countShaderType )
	{
		//  シェーダーのポインタが存在している場合
		if( m_shader[ countShaderType ] != nullptr )
		{
			//  解放
			m_shader[ countShaderType ]->Uninit( );
			delete m_shader[ countShaderType ];
			m_shader[ countShaderType ] = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------------
//  シェーダー情報の取得
//--------------------------------------------------------------------------------------
Shader* ShaderManager::GetShader( ShaderManager::TYPE type )
{
	return m_shader[ static_cast< int >( type ) ];
}

//--------------------------------------------------------------------------------------
//  シェーダー描画終了
//--------------------------------------------------------------------------------------
void ShaderManager::DrawEnd( void )	
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	// 頂点フォーマットの設定
	device->SetVertexDeclaration( nullptr );

	device->SetVertexShader( nullptr );

	device->SetPixelShader( nullptr );
}