//--------------------------------------------------------------------------------------
//  3Dシェーダー ( シャドウマップ )  ( shader3DShadowMap.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "shader3DShadowMap.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  3Dシェーダー ( シャドウマップ )の初期化処理
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::Init( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	D3DVERTEXELEMENT9 element[ ] = 
	{  
		//  引数( 1 : パイプライン番号 , 2 : オフセット , 3 : データサイズ( 型 ) , 4 : , 5 : 使用用途 , 6 : , ( 5～6はセマンティクス ) )
		//  ( 自分で考えたデータを送る場合はTEXCOORDで送るように )
		{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 } ,	
		D3DDECL_END( )  //  終了
	};

	//  頂点宣言格納インターフェース生成
	device->CreateVertexDeclaration( element , &m_vertexDeclaration );

	/*------------------------------------------------------------
		バーテックスシェーダーの生成
	------------------------------------------------------------*/

	HRESULT hr; 

	LPD3DXBUFFER error = NULL;				//  エラー用
	LPD3DXBUFFER codeBinary = NULL;			//  バイナリー( ※メモリ確保して格納用 )

	hr = D3DXCompileShaderFromFile( "data/HLSL/3D/shadowMapVS.hlsl" ,	//  ファイル名
									NULL ,								//  
									NULL ,								//  
									"main" ,							//  エントリー関数名( 一番初めに行われる関数 )
									"vs_3_0" ,							//  シェーダモデル名
									0 ,									//  
									&codeBinary ,						//  コンパイル後のバイナリコード格納場所
									&error ,							//  エラーメッセージ( コンパイルが済めばNULL , エラーならメッセージ )
									&m_vertexShaderConstantTable );		//  

	//  エラーチェック
	if( FAILED( hr ) )
	{
		MessageBox( NULL , ( char* )error->GetBufferPointer( ) , "D3DXCompileShaderFromFileでのエラー" , MB_OK );
		//OutputDebugStringA( "Error!!" );

		//  エラーメッセージの解放
		error->Release( );

		return;
	}

	//  バーテックスシェーダーの生成
	hr = device->CreateVertexShader( ( LPDWORD )codeBinary->GetBufferPointer( ) ,		//  コードバイナリーデーター
												&m_vertexShader );						//  バーテックスシェーダーの管理者に代入

	//  エラーチェック
	if( FAILED( hr ) )
	{
		MessageBox( NULL , "バーテックスシェーダーが上手く生成出来ませんでした。" , "CreateVertexShaderでのエラー" , MB_OK );

		return;
	}

	//  エラーコードのメモリ解放
	if( error != NULL )
	{
		error->Release( );
	}

	//  コードバイナリーのメモリ解放
	if( codeBinary != NULL )
	{
		codeBinary->Release( );
	}

	/*------------------------------------------------------------
		ピクセルシェーダーの生成
	------------------------------------------------------------*/

	error = NULL;				//  エラー用
	codeBinary = NULL;			//  バイナリー( ※メモリ確保して格納用 )

	hr = D3DXCompileShaderFromFile( "data/HLSL/3D/shadowMapPS.hlsl" ,	//  ファイル名
									NULL ,								//  
									NULL ,								//  
									"main" ,							//  エントリー関数名( 一番初めに行われる関数 )
									"ps_3_0" ,							//  シェーダモデル名
									0 ,									//  
									&codeBinary ,						//  コンパイル後のバイナリコード格納場所
									&error ,							//  エラーメッセージ( コンパイルが済めばNULL , エラーならメッセージ )
									&m_pixelShaderConstantTable );		//  

	//  エラーチェック
	if( FAILED( hr ) )
	{
		MessageBox( NULL , ( char* )error->GetBufferPointer( ) , "D3DXCompileShaderFromFileでのエラー" , MB_OK );
		//OutputDebugStringA( "Error!!" );

		//  エラーメッセージの解放
		error->Release( );

		return;
	}

	//  ピクセルシェーダーの生成
	hr = device->CreatePixelShader( ( LPDWORD )codeBinary->GetBufferPointer( ) ,		//  コードバイナリーデーター
											   &m_pixelShader );								//  ピクセルシェーダーの管理者に代入

	//  エラーチェック
	if( FAILED( hr ) )
	{
		MessageBox( NULL , "ピクセルシェーダーが上手く生成出来ませんでした。" , "CreatePixelShaderでのエラー" , MB_OK );

		return;
	}

	//  エラーコードのメモリ解放
	if( error != NULL )
	{
		error->Release( );
	}

	//  コードバイナリーのメモリ解放
	if( codeBinary != NULL )
	{
		codeBinary->Release( );
	}
}

//--------------------------------------------------------------------------------------
//  3Dシェーダー ( シャドウマップ )の終了処理
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::Uninit( void )
{
	//  シェーダーの終了
	Shader::Uninit( );
}

//--------------------------------------------------------------------------------------
//  頂点フォーマットの定義
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::SetVertexDeclaration( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	// 頂点フォーマットの設定
	device->SetVertexDeclaration( m_vertexDeclaration );
}

//--------------------------------------------------------------------------------------
//  頂点シェーダーの設定
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::SetVertexShader( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShader != nullptr )
	{
		device->SetVertexShader( m_vertexShader );
	}
}

//--------------------------------------------------------------------------------------
//  ピクセルシェーダーの設定
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::SetPixelShader( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShader != nullptr )
	{
		device->SetPixelShader( m_pixelShader );
	}
}

//--------------------------------------------------------------------------------------
//  ワールド行列の設定
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::SetWorldMatrix( const D3DXMATRIX &matrix )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device ,  "worldMatrix" , &matrix );
	}
}

//--------------------------------------------------------------------------------------
//  ライトから見たビュープロジェクション行列の設定
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::SetLightViewProjectionMatrix( const D3DXMATRIX &matrix )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device ,  "lightViewProjectionMatrix" , &matrix );
	}
}

//--------------------------------------------------------------------------------------
//  ファーの値設定
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::SetFar( const float& fFar )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetFloat( device ,  "far" , fFar );
	}
}

//--------------------------------------------------------------------------------------
//  シェーダー描画に必要な情報の設定
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::SetShaderInfo( const D3DXMATRIX &worldMatrix , const D3DXMATRIX &lightViewProjectionMatrix , const float& fFar )
{
	SetWorldMatrix( worldMatrix );
	SetLightViewProjectionMatrix( lightViewProjectionMatrix );
	SetFar( fFar );
}

//--------------------------------------------------------------------------------------
//  シェーダー描画開始
//--------------------------------------------------------------------------------------
void Shader3DShadowMap::DrawBegin( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点シェーダーの設定
	if( m_vertexShader != nullptr )
	{
		device->SetVertexShader( m_vertexShader );
	}

	//  ピクセルシェーダーの設定
	if( m_pixelShader != nullptr )
	{
		device->SetPixelShader( m_pixelShader );
	}	

	// 頂点フォーマットの設定
	device->SetVertexDeclaration( m_vertexDeclaration );
}