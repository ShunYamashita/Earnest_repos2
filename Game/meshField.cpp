//--------------------------------------------------------------------------------------
//  メッシュフィールド   ( meshField.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "light.h"
#include "game.h"
#include "keyboard.h"
#include "shaderManager.h"
#include "camera.h"
#include "depthShadow.h"
#include <stdio.h>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MESH_FIELD_TEXTURENAME0		"data/TEXTURE/field.jpg"				//  テクスチャ名
#define MESH_FIELD_TEXTURENAME1		"data/TEXTURE/water.jpg"				//  テクスチャ名
#define MESH_FIELD_TEXTURENAME2		"data/TEXTURE/cube_texture02.png"		//  テクスチャ名
#define MESH_FIELD_TEXTURENAME3		"data/TEXTURE/field2.png"				//  テクスチャ名
#define MESH_FIELD_TEXTURENAME4		"data/TEXTURE/cube_texture01.png"		//  テクスチャ名
#define MESH_FIELD_TEXTURENAME5		"data/TEXTURE/cube_texture06.png"		//  テクスチャ名

//--------------------------------------------------------------------------------------
//  メッシュフィールドクラスのコンストラクタ
//--------------------------------------------------------------------------------------
MeshField::MeshField( void ) : Scene( 2 )
{
	m_pVtxBuff = NULL;
	m_pIndexBuff = NULL;
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_nDivideSide = 0;
	m_nDivideVertical = 0;
	m_nFocusSide = 0;
	m_nDivideVertical = 0;
	m_scroll = D3DXVECTOR2( 0.0f , 0.0f );
	m_changeScroll = D3DXVECTOR2( 0.0f , 0.0f );

	m_type = TYPE_FIELD;

	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
		{
			m_fieldPos[ nCntVertical ][ nCntSide ] = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		}
	}
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドクラスのデストラクタ
//--------------------------------------------------------------------------------------
MeshField::~MeshField( )
{

}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの初期化処理
//--------------------------------------------------------------------------------------
HRESULT MeshField::Init( void )
{
	//  テクスチャクラスのポインタ取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  テクスチャの設定
	pTexture->SetTextureImage( MESH_FIELD_TEXTURENAME0 );
	pTexture->SetTextureImage( MESH_FIELD_TEXTURENAME1 );
	pTexture->SetTextureImage( MESH_FIELD_TEXTURENAME2 );
	pTexture->SetTextureImage( MESH_FIELD_TEXTURENAME3 );
	pTexture->SetTextureImage( MESH_FIELD_TEXTURENAME4 );
	pTexture->SetTextureImage( MESH_FIELD_TEXTURENAME5 );

	//  頂点の作成
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの終了処理
//--------------------------------------------------------------------------------------
void MeshField::Uninit( void )
{
	// 頂点バッファの破棄
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release( );
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if( m_pIndexBuff != NULL )
	{
		m_pIndexBuff->Release( );
		m_pIndexBuff = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの更新処理
//--------------------------------------------------------------------------------------
void MeshField::Update( void )
{
	//  UVスクロール
	m_scroll += m_changeScroll;

	//  頂点情報の設定
	SetVertex( );

	ChangeHeight( );
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの更新処理
//--------------------------------------------------------------------------------------
void MeshField::ChangeHeight( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  Aキーが押された場合
	if( pKeyboard->GetKeyboardTrigger( DIK_J ) )
	{
		m_nFocusSide--;

		if( m_nFocusSide < 0 )
		{
			m_nFocusSide = m_nDivideSide;
		}
	}

	//  Dキーが押された場合
	if( pKeyboard->GetKeyboardTrigger( DIK_L ) )
	{
		m_nFocusSide++;

		if( m_nFocusSide > m_nDivideSide )
		{
			m_nFocusSide = 0;
		}
	}

	//  Aキーが押された場合
	if( pKeyboard->GetKeyboardTrigger( DIK_I ) )
	{
		m_nFocusVertical--;

		if( m_nFocusVertical < 0 )
		{
			m_nFocusVertical = m_nDivideVertical;
		}
	}

	//  Dキーが押された場合
	if( pKeyboard->GetKeyboardTrigger( DIK_K ) )
	{
		m_nFocusVertical++;

		if( m_nFocusVertical > m_nDivideVertical )
		{
			m_nFocusVertical = 0;
		}
	}

	//  ↑キーが押された場合
	if( pKeyboard->GetKeyboardPress( DIK_UPARROW ) )
	{
		m_fieldPos[ m_nFocusVertical ][ m_nFocusSide ].y += 1.0f;
	}

	//  ↓キーが押された場合
	if( pKeyboard->GetKeyboardPress( DIK_DOWNARROW ) )
	{
		m_fieldPos[ m_nFocusVertical ][ m_nFocusSide ].y -= 1.0f;
	}
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの更新処理
//--------------------------------------------------------------------------------------
void MeshField::SaveField( void )
{
	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  SPACEキーが押された場合
	if( pKeyboard->GetKeyboardTrigger( DIK_SPACE ) )
	{
		SaveFile( );
	}
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの描画処理
//--------------------------------------------------------------------------------------
void MeshField::Draw( void )
{
	//  テクスチャ情報の取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  移動行列
	D3DXMATRIX mtxRot;								//  回転行列

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  回転行列の作成
	D3DXMatrixRotationYawPitchRoll( &mtxRot ,
									m_rot.y ,
									m_rot.x ,
									m_rot.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  GPUとVRAMの接続
	pDevice->SetStreamSource( 0 ,													//  パイプライン番号
							  m_pVtxBuff ,											//  頂点バッファのアドレス
						  	  0 ,													//  オフセット( byte )
							  sizeof( VERTEX_3D ) );								//  一個分の頂点データのサイズ( ストライド )

	//  インデックスバッファの設定
	pDevice->SetIndices( m_pIndexBuff );

	//  シェーダー情報の取得
	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );

	//  ライトの方向ベクトルを取得する
	D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( );
	D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );

	//  ローカル単位ベクトルに変更
	D3DXVECTOR3 lightDirectLocal;

	D3DXMATRIX worldInverseMatrix;
	D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
	D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
	D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

	Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
	D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
	D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
	D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

	//  シェーダー情報の取得
	Shader3DDepthShadow* shader3DDepthShadow = ( Shader3DDepthShadow* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_DEPTH_SHADOW );

	//  光源のビュープロジェクション行列の取得
	D3DXMATRIX lightViewProjectionMatrix = SceneManager::GetLight( )->GetViewMatrix( ) * SceneManager::GetLight( )->GetProjectionMatrix( );

	//  UVオフセット値の計算
	D3DXVECTOR4	tmpOffset;
	tmpOffset.x = 0.5f / DepthShadow::TEXTURE_WIDTH;
	tmpOffset.y = 0.5f / DepthShadow::TEXTURE_HEIGHT;
	tmpOffset.z = 0.0f;
	tmpOffset.w = 0.0f;

	//  バイアス値の取得
	float bias = DepthShadow::GetBias( );

	//  シェーダーに必要な情報の設定
	//shader3D->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor );
	shader3DDepthShadow->SetShaderInfo( mtxWorld , viewMatrix ,projectionMatrix ,
										lightDirectLocal , lightViewProjectionMatrix , tmpOffset , bias );

	UINT textureSampler = shader3DDepthShadow->GetSamplerTextureIndex( );
	UINT shadowSampler = shader3DDepthShadow->GetSamplerShadowIndex( );

	switch( m_type )
	{
		case TYPE_FIELD:
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureSampler , pTexture->GetTextureImage( MESH_FIELD_TEXTURENAME0 ) );

			break;
		}
		case TYPE_SEA:
		{
			//  テクスチャの設定
			pDevice->SetTexture( textureSampler , pTexture->GetTextureImage( MESH_FIELD_TEXTURENAME1 ) );

			break;
		}
	}

	//  シャドウマップテクスチャの設定
	pDevice->SetTexture( shadowSampler , DepthShadow::GetRendereTargetTexture( ) );

	//  シェーダー3Dの描画開始
	//shader3D->DrawBegin( );
	shader3DDepthShadow->DrawBegin( );

	//  プリミティブの描画
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP ,									//  プリミティブの種類
								   0 ,														//  最初の頂点インデックス番号のオフセット
								   0 ,														//  最小の頂点インデックス番号のオフセット
								   ( NUM_VERTEX2 * ( ( m_nDivideSide + 1 )					//  頂点数
								   * m_nDivideVertical						
								   + m_nDivideVertical - 1 ) ) ,														
								   0 ,														//  スタートインデックス
								   NUM_POLYGON * m_nDivideSide *							//  プリミティブ数
								   m_nDivideVertical +						
								   4 * ( m_nDivideVertical - 1 ) );	

	//  シェーダー3Dの描画終了
	ShaderManager::DrawEnd( );
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドのデプス値書き込み処理
//--------------------------------------------------------------------------------------
void MeshField::DrawDepth( void )
{
	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  移動行列
	D3DXMATRIX mtxRot;								//  回転行列

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  回転行列の作成
	D3DXMatrixRotationYawPitchRoll( &mtxRot ,
									m_rot.y ,
									m_rot.x ,
									m_rot.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  GPUとVRAMの接続
	pDevice->SetStreamSource( 0 ,													//  パイプライン番号
							  m_pVtxBuff ,											//  頂点バッファのアドレス
						  	  0 ,													//  オフセット( byte )
							  sizeof( VERTEX_3D ) );								//  一個分の頂点データのサイズ( ストライド )

	//  シェーダー情報の取得
	Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

	//  カメラクラスの取得
	Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

	D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
	D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );
	float fFar = SceneManager::GetLight( )->GetFar( );

	shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix , fFar );

	//  インデックスバッファの設定
	pDevice->SetIndices( m_pIndexBuff );

	//  テクスチャの設定
	pDevice->SetTexture( 0 , nullptr );

	//  シェーダー描画開始
	shader3DShadowMap->DrawBegin( );

	//  プリミティブの描画
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP ,							//  プリミティブの種類
								   0 ,												//  最初の頂点インデックス番号のオフセット
								   0 ,												//  最小の頂点インデックス番号のオフセット
								   ( NUM_VERTEX2 * ( ( m_nDivideSide + 1 )			//  頂点数
								   * m_nDivideVertical						
								   + m_nDivideVertical - 1 ) ) ,												
								   0 ,												//  スタートインデックス
								   NUM_POLYGON * m_nDivideSide *					//  プリミティブ数
								   m_nDivideVertical +						
								   4 * ( m_nDivideVertical - 1 ) );

	//  描画終了
	ShaderManager::DrawEnd( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
MeshField* MeshField::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 size , D3DXCOLOR color ,
							  int nDivedeSide , int nDivideVertical , D3DXVECTOR2 changeScroll )
{
	MeshField *pMeshField;

	//  インスタンス生成
	pMeshField = new MeshField;

	//  種類の代入
	pMeshField->m_type = type;

	//  座標の代入
	pMeshField->m_position = position;

	//  回転角の代入
	pMeshField->m_rot = rot;

	//  大きさの代入
	pMeshField->m_size = size;

	//  色の代入
	pMeshField->m_color = color;

	//  横分割数の代入
	pMeshField->m_nDivideSide = nDivedeSide;

	//  縦分割数の代入
	pMeshField->m_nDivideVertical = nDivideVertical;

	//  UV変更量の代入
	pMeshField->m_changeScroll = changeScroll;

	for( int nCntVertical = 0; nCntVertical < nDivideVertical + 1; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < nDivedeSide + 1; nCntSide++ )
		{
			pMeshField->m_fieldPos[ nCntVertical ][ nCntSide ].y = position.y;
		}
	}

	//  初期化
	pMeshField->Init( );

	return pMeshField;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void MeshField::MakeVertex( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * ( m_nDivideSide + 1 ) *
																   ( m_nDivideVertical + 1 ) ,		//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,									//  使用方法
											 0 ,													//  
											 D3DPOOL_MANAGED ,										//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuff ,											//  バッファ
											 NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return;
	}

	//  インデックスバッファの作成
	if( FAILED( pDevice->CreateIndexBuffer( sizeof( WORD ) * ( NUM_VERTEX2 * ( ( m_nDivideSide + 1 ) 
															 * m_nDivideVertical								//  作成したい頂点バッファのサイズ
															 + m_nDivideVertical - 1 ) ) ,											
											D3DUSAGE_WRITEONLY ,												//  使用方法
											D3DFMT_INDEX16 ,													//  
											D3DPOOL_MANAGED ,													//  メモリ管理方法( MANAGED → お任せ )
											&m_pIndexBuff ,														//  バッファ
											NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return;
	}

	VERTEX_3D* pVtx;												//  頂点バッファの仮想アドレス用ポインタ

	//  頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
					  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
					  0 );											//  ロックの種類

	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
		{
			//  頂点座標の設定( 2D座標 ・ 右回り )
			pVtx[ 0 ].position = D3DXVECTOR3( m_position.x - ( m_size.x / 2 ) + nCntSide * m_size.x / m_nDivideSide ,
										 m_fieldPos[ nCntVertical ][ nCntSide ].y ,
										 m_position.z + ( m_size.z / 2 ) - nCntVertical * m_size.z / m_nDivideVertical );

			m_fieldPos[ nCntVertical ][ nCntSide ] = pVtx[ 0 ].position;

			//  頂点色の設定( 0 ～ 255 の整数値 )
			pVtx[ 0 ].color = m_color;

			//  UV座標の指定
			pVtx[ 0 ].texcoord = D3DXVECTOR2( ( float )nCntSide  , ( float )nCntVertical );

			//  法線ベクトルの設定
			pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );

			//  アドレスを進める
			pVtx++;
		}
	}

	//  頂点バッファのアンロック
	m_pVtxBuff->Unlock( );

	//D3DXVECTOR3 directionX( 0.0f , 0.0f , 0.0f );
	//D3DXVECTOR3 directionZ( 0.0f , 0.0f , 0.0f );
	//D3DXVECTOR3 normalX( 0.0f , 0.0f , 0.0f );
	//D3DXVECTOR3 normalZ( 0.0f , 0.0f , 0.0f );
	//D3DXVECTOR3 normal( 0.0f , 0.0f , 0.0f );

	////  頂点バッファをロックして、仮想アドレスを取得する
	//m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
	//				  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
	//				  0 );											//  ロックの種類

	//for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
	//{
	//	for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
	//	{
	//		directionX = pVtx[ 1 ].position - pVtx[ -1 ].position;
	//		normalX.x = -directionX.y;
	//		normalX.y = directionX.x;
	//		normalX.z = 0.0f;

	//		directionZ = pVtx[ -( m_nDivideVertical + 1 ) ].position - pVtx[ ( m_nDivideVertical + 1 ) ].position;
	//		normalZ.x = 0.0f;
	//		normalZ.y = directionZ.z;
	//		normalZ.z = -directionZ.y;
	//		
	//		normal = normalX + normalZ;

	//		D3DXVec3Normalize( &normal , &normal );

	//		//  法線ベクトルの設定
	//		pVtx[ 0 ].normal = normal;

	//		//  アドレスを進める
	//		pVtx++;
	//	}
	//}

	////  頂点バッファのアンロック
	//m_pVtxBuff->Unlock( );

	WORD* pIndexBuffer;											//  インデックスバッファの仮想アドレス用ポインタ

	m_pIndexBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
						( void** )&pIndexBuffer ,				//  アドレスが書かれたメモ帳のアドレス
						0 );									//  ロックの種類

	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
		{
			pIndexBuffer[ 0 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 1 ) + nCntSide;
			pIndexBuffer[ 1 ] = ( m_nDivideSide + 1 ) * nCntVertical + nCntSide;

			pIndexBuffer += 2;

			if( nCntSide == m_nDivideSide &&
				nCntVertical != ( m_nDivideVertical - 1 ) )
			{
				pIndexBuffer[ 0 ] = ( m_nDivideSide + 1 ) * nCntVertical + nCntSide;
				pIndexBuffer[ 1 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 2 );

				pIndexBuffer += 2;
			}	
		}
	}

	//  インデックスバッファのアンロック
	m_pIndexBuff->Unlock( );
}

//--------------------------------------------------------------------------------------
//  頂点情報を設定する関数
//--------------------------------------------------------------------------------------
void MeshField::SetVertex( void )
{
	VERTEX_3D* pVtx;												//  頂点バッファの仮想アドレス用ポインタ

	if( m_pVtxBuff != NULL )
	{
		D3DXVECTOR3 directionX( 0.0f , 0.0f , 0.0f );
		D3DXVECTOR3 directionZ( 0.0f , 0.0f , 0.0f );
		D3DXVECTOR3 normalX( 0.0f , 0.0f , 0.0f );
		D3DXVECTOR3 normalZ( 0.0f , 0.0f , 0.0f );
		D3DXVECTOR3 normal( 0.0f , 0.0f , 0.0f );

		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
						  0 );											//  ロックの種類

		for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
		{
			for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
			{
				//  頂点座標の設定( 2D座標 ・ 右回り )
				pVtx[ 0 ].position = m_fieldPos[ nCntVertical ][ nCntSide ];

				//  UV座標の指定
				pVtx[ 0 ].texcoord = D3DXVECTOR2( ( float )nCntSide * 5.0f + m_scroll.x  , ( float )nCntVertical * 5.0f + m_scroll.y );

				//  頂点色の設定( 0 ～ 255 の整数値 )
				pVtx[ 0 ].color = m_color;

				//  アドレスを進める
				pVtx++;
			}
		}

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}
}

//--------------------------------------------------------------------------------------
//  対象の座標がフィールドのどこのポリゴンにいるか検索してそのY座標を返す関数
//--------------------------------------------------------------------------------------
float MeshField::GetHeight( D3DXVECTOR3 position )
{
	D3DXVECTOR3 vec1( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vec2( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 cross( 0.0f , 0.0f , 0.0f );

	D3DXVECTOR3 vec3( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vec4( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 cross2( 0.0f , 0.0f , 0.0f );

	D3DXVECTOR3 vec5( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 vec6( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 cross3( 0.0f , 0.0f , 0.0f );

	D3DXVECTOR3 normal( 0.0f , 0.0f , 0.0f );

	//  ポリゴンの数分の検索
	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide; nCntSide++ )
		{
			vec1 = m_fieldPos[ nCntVertical ][ nCntSide ] - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide ];
			vec2 = position - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide ];

			//  2つのベクトルのなす直交ベクトルを求める
			D3DXVec3Cross( &cross , &vec1 , &vec2 );

			//  単位ベクトル化して法線ベクトルに変換
			D3DXVec3Normalize( &cross , &cross );

			//  法線が上向き方向の場合
			if( cross.y >= 0.0f )
			{
				vec3 = m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide + 1 ] - m_fieldPos[ nCntVertical ][ nCntSide ];
				vec4 = position - m_fieldPos[ nCntVertical ][ nCntSide ];

				//  2つのベクトルのなす直交ベクトルを求める
				D3DXVec3Cross( &cross2 , &vec3 , &vec4 );

				//  単位ベクトル化して法線ベクトルに変換
				D3DXVec3Normalize( &cross2 , &cross2 );

				//  法線が上向き方向の場合
				if( cross2.y >= 0.0f )
				{
					vec5 = m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide ] - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide + 1 ];
					vec6 = position - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide + 1 ];

					//  2つのベクトルのなす直交ベクトルを求める
					D3DXVec3Cross( &cross3 , &vec5 , &vec6 );

					//  単位ベクトル化して法線ベクトルに変換
					D3DXVec3Normalize( &cross3 , &cross3 );

					//  法線が上向き方向の場合
					if( cross3.y >= 0.0f )
					{
						D3DXVECTOR3 workVec = m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide + 1 ] - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide ];

						//  外積計算からそのポリゴンの法線ベクトルを求める
						D3DXVec3Cross( &normal , &vec1 , &workVec );
						D3DXVec3Normalize( &normal , &normal );

						float fHeight = m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide ].y -
										( ( position.x - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide ].x ) * normal.x +
										( position.z - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide ].z ) * normal.z ) / normal.y;

						return fHeight;
					}
				}
			}
		}
	}

	//  ポリゴンの数分の検索
	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide; nCntSide++ )
		{
			vec1 = m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide + 1 ] - m_fieldPos[ nCntVertical ][ nCntSide + 1 ];
			vec2 = position - m_fieldPos[ nCntVertical ][ nCntSide + 1 ];

			//  2つのベクトルのなす直交ベクトルを求める
			D3DXVec3Cross( &cross , &vec1 , &vec2 );

			//  単位ベクトル化して法線ベクトルに変換
			D3DXVec3Normalize( &cross , &cross );

			//  法線が上向き方向の場合
			if( cross.y >= 0.0f )
			{
				vec3 = m_fieldPos[ nCntVertical ][ nCntSide ] - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide + 1 ];
				vec4 = position - m_fieldPos[ ( nCntVertical + 1 ) ][ nCntSide + 1 ];

				//  2つのベクトルのなす直交ベクトルを求める
				D3DXVec3Cross( &cross2 , &vec3 , &vec4 );

				//  単位ベクトル化して法線ベクトルに変換
				D3DXVec3Normalize( &cross2 , &cross2 );

				//  法線が上向き方向の場合
				if( cross2.y >= 0.0f )
				{
					vec5 = m_fieldPos[ nCntVertical ][ nCntSide + 1 ] - m_fieldPos[ nCntVertical ][ nCntSide ];
					vec6 = position - m_fieldPos[ nCntVertical ][ nCntSide ];

					//  2つのベクトルのなす直交ベクトルを求める
					D3DXVec3Cross( &cross3 , &vec5 , &vec6 );

					//  単位ベクトル化して法線ベクトルに変換
					D3DXVec3Normalize( &cross3 , &cross3 );

					//  法線が上向き方向の場合
					if( cross3.y >= 0.0f )
					{
						D3DXVECTOR3 workVec = m_fieldPos[ nCntVertical ][ nCntSide ] - m_fieldPos[ nCntVertical ][ nCntSide + 1 ];

						//  外積計算からそのポリゴンの法線ベクトルを求める
						D3DXVec3Cross( &normal , &vec1 , &workVec );
						D3DXVec3Normalize( &normal , &normal );

						float fHeight = m_fieldPos[ nCntVertical ][ nCntSide + 1 ].y -
										( ( position.x - m_fieldPos[ nCntVertical ][ nCntSide + 1 ].x ) * normal.x +
										( position.z - m_fieldPos[ nCntVertical ][ nCntSide + 1 ].z ) * normal.z ) / normal.y;

						return fHeight;
					}
				}
			}
		}
	}

	return 0.0f;
}

//--------------------------------------------------------------------------------------
//  現状の高さをファイルに保存する関数
//--------------------------------------------------------------------------------------
void MeshField::SaveFile( void )
{
	UINT nID;									//  ID番号

	// キーボード情報の取得
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  キーボード入力情報の初期化
	pKeyboard->Reset( );

	nID = MessageBox( NULL , "フィールドを保存してもよろしいですか?" , "< 注意メッセージ >" , MB_YESNO );

	if( nID == IDYES )
	{
		//  ファイルを開く
		FILE* pFile = fopen( "data/FIELD/field.txt" , "w" );

		//  ファイルが開けた場合
		if( pFile != NULL )
		{
			fprintf( pFile , "//--------------------------------------------------\n" );
			fprintf( pFile , "//   フィールド情報テキスト \n" );
			fprintf( pFile , "//\n" );
			fprintf( pFile , "//   Author : Shun Yamashita\n" );
			fprintf( pFile , "//--------------------------------------------------\n\n" );
			fprintf( pFile , "//--------------------------------------------------\n" );
			fprintf( pFile , "//   フィールド情報 \n" );
			fprintf( pFile , "//--------------------------------------------------\n" );
			fprintf( pFile , "FIELD_TYPE = %d\n" , ( int )m_type );
			fprintf( pFile , "FIELD_SIZE_X = %f\n" , m_size.x );
			fprintf( pFile , "FIELD_SIZE_Z = %f\n" , m_size.z );
			fprintf( pFile , "FIELD_DIVIDE_SIDE = %d\n" , m_nDivideSide );
			fprintf( pFile , "FIELD_DIVIDE_VERTICAL = %d\n\n" , m_nDivideVertical );

			fprintf( pFile , "//--------------------------------------------------\n" );
			fprintf( pFile , "//   フィールド高さ情報 \n" );
			fprintf( pFile , "//--------------------------------------------------\n" );

			//  ポリゴンの数分の検索
			for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
			{
				for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
				{
					//  フィールドの高さの書き込み
					fprintf( pFile , "FIELD_HEIGHT[ %d ][ %d ] = %f\n" , nCntVertical , nCntSide , m_fieldPos[ nCntVertical ][ nCntSide ].y );
				}
			}

			//  ファイルを閉じる
			fclose( pFile );
		}
		else
		{
			MessageBox( NULL , "meshField.cpp : ファイルを正常に開くことが出来ませんでした。" , "< エラーメッセージ >" , MB_OK );
		}
	}
}

//--------------------------------------------------------------------------------------
//  横フォーカス番号を渡す関数
//--------------------------------------------------------------------------------------
int MeshField::GetFocusSide( void )
{
	return m_nFocusSide;
}

//--------------------------------------------------------------------------------------
//  縦フォーカス番号を渡す関数
//--------------------------------------------------------------------------------------
int MeshField::GetFocusVertical( void )
{
	return m_nFocusVertical;
}

//--------------------------------------------------------------------------------------
//  縦フォーカス番号を渡す関数
//--------------------------------------------------------------------------------------
float MeshField::GetFocusHeight( int nVertical , int nSide )
{
	return m_fieldPos[ nVertical ][ nSide ].y;
}