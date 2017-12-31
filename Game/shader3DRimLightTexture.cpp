//--------------------------------------------------------------------------------------
//  3Dシェーダー( リムライト )   ( shader3DRimLightTexture.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "shader3DRimLightTexture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  3Dシェーダー( リムライト )の初期化処理
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::Init( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	D3DVERTEXELEMENT9 element[ ] = 
	{  
		//  引数( 1 : パイプライン番号 , 2 : オフセット , 3 : データサイズ( 型 ) , 4 : , 5 : 使用用途 , 6 : , ( 5～6はセマンティクス ) )
		//  ( 自分で考えたデータを送る場合はTEXCOORDで送るように )
		{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 } ,
		{ 0 , sizeof( float ) * 3 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL, 0 } ,
		{ 0 , sizeof( float ) * 6 , D3DDECLTYPE_D3DCOLOR , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_COLOR , 0 } ,
		{ 0 , sizeof( float ) * 6 + sizeof( D3DCOLOR ) , D3DDECLTYPE_FLOAT2 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 0 } ,		
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

	hr = D3DXCompileShaderFromFile( "data/HLSL/3D/rimLightTextureVS.hlsl" ,				//  ファイル名
									NULL ,												//  
									NULL ,												//  
									"main" ,											//  エントリー関数名( 一番初めに行われる関数 )
									"vs_3_0" ,											//  シェーダモデル名
									0 ,													//  
									&codeBinary ,										//  コンパイル後のバイナリコード格納場所
									&error ,											//  エラーメッセージ( コンパイルが済めばNULL , エラーならメッセージ )
									&m_vertexShaderConstantTable );						//  

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

	hr = D3DXCompileShaderFromFile( "data/HLSL/3D/rimLightTexturePS.hlsl" ,				//  ファイル名
									NULL ,												//  
									NULL ,												//  
									"main" ,											//  エントリー関数名( 一番初めに行われる関数 )
									"ps_3_0" ,											//  シェーダモデル名
									0 ,													//  
									&codeBinary ,										//  コンパイル後のバイナリコード格納場所
									&error ,											//  エラーメッセージ( コンパイルが済めばNULL , エラーならメッセージ )
									&m_pixelShaderConstantTable );						//  

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
//  3Dシェーダー( リムライト )の終了処理
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::Uninit( void )
{
	//  シェーダーの終了
	Shader::Uninit( );
}

//--------------------------------------------------------------------------------------
//  頂点フォーマットの定義
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetVertexDeclaration( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	// 頂点フォーマットの設定
	device->SetVertexDeclaration( m_vertexDeclaration );
}

//--------------------------------------------------------------------------------------
//  頂点シェーダーの設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetVertexShader( void )
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
void Shader3DRimLightTexture::SetPixelShader( void )
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
void Shader3DRimLightTexture::SetWorldMatrix( const D3DXMATRIX &worldMatrix )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device , "worldMatrix" , &worldMatrix );
	}
}

//--------------------------------------------------------------------------------------
//  ビュー行列の設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetViewMatrix( const D3DXMATRIX &viewMatrix )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device , "viewMatrix" , &viewMatrix );
	}
}

//--------------------------------------------------------------------------------------
//  プロジェクション行列の設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetProjectionMatrix( const D3DXMATRIX &projectionMatrix )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device , "projectionMatrix" , &projectionMatrix );
	}
}

//--------------------------------------------------------------------------------------
//  ワールド逆行列の転置行列の設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetWorldInverseTransposeMatrix( const D3DXMATRIX &matrix )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device , "worldInverseTransposeMatrix" , &matrix );
	}
}

//--------------------------------------------------------------------------------------
//  カメラのワールド座標の設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetWorldPositionEye( const D3DXVECTOR3 &position )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetValue( device , "worldPosEye" , &position , sizeof( position ) );
	}
}

//--------------------------------------------------------------------------------------
//  ライトのワールド座標系の方向ベクトルの設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetLightDirectWorld( const D3DXVECTOR3 &lightDirectWorld )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetValue( device , "lightDirectWorld" , &lightDirectWorld , sizeof( lightDirectWorld ) );
	}
}

//--------------------------------------------------------------------------------------
//  スペキュラーの度合いの設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetSpecularPower( const float &specularPower )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetFloat( device , "specularPower" , specularPower );
	}
}

//--------------------------------------------------------------------------------------
//  マテリアル拡散光の設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetMaterialDiffuseColor( const D3DXCOLOR &materialDiffuseColor )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetValue( device , "materialDiffuseColor" , &materialDiffuseColor , sizeof( materialDiffuseColor ) );
	}
}

//--------------------------------------------------------------------------------------
//  シェーダーに必要な情報の設定
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetShaderInfo( const D3DXMATRIX &worldMatrix ,
											 const D3DXMATRIX &viewMatrix ,
											 const D3DXMATRIX &projectionMatrix ,
											 const D3DXMATRIX &worldInverseTransposeMatrix ,
											 const D3DXVECTOR3 &cameraPositionEye ,
											 const D3DXVECTOR3 &lightDirectWorld ,
											 const float &specularPower ,
											 const D3DXCOLOR &materialDiffuseColor  )
{
	SetWorldMatrix( worldMatrix );
	SetViewMatrix( viewMatrix );
	SetProjectionMatrix( projectionMatrix );
	SetWorldInverseTransposeMatrix( worldInverseTransposeMatrix );
	SetWorldPositionEye( cameraPositionEye );
	SetLightDirectWorld( lightDirectWorld );
	SetSpecularPower( specularPower );
	SetMaterialDiffuseColor( materialDiffuseColor );
}

//--------------------------------------------------------------------------------------
//  シェーダー描画開始
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::DrawBegin( void )
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