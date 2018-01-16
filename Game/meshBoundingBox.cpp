////--------------------------------------------------------------------------------------
////  メッシュバウンディングボックス   ( meshField.cpp )
////
////  Author : SHUN YAMASHITA
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
////  ヘッダーファイル
////--------------------------------------------------------------------------------------
//#include "meshBoundingBox.h"
//#include "manager.h"
//#include "renderer.h"
//#include "texture.h"
//#include "shaderManager.h"
//#include "camera.h"
//#include <stdio.h>
//
////--------------------------------------------------------------------------------------
////  マクロ定義
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
////  メッシュバウンディングクラスのコンストラクタ
////--------------------------------------------------------------------------------------
//MeshBoundingBox::MeshBoundingBox( void ) : Scene( 2 )
//{
//	m_pVtxBuff = NULL;
//	m_pIndexBuff = NULL;
//}
//
////--------------------------------------------------------------------------------------
////  メッシュバウンディングクラスのデストラクタ
////--------------------------------------------------------------------------------------
//MeshBoundingBox::~MeshBoundingBox( )
//{
//
//}
//
////--------------------------------------------------------------------------------------
////  メッシュバウンディングの初期化処理
////--------------------------------------------------------------------------------------
//HRESULT MeshBoundingBox::Init( void )
//{
//	//  頂点の作成
//	MakeVertex( );
//
//	return S_OK;
//}
//
////--------------------------------------------------------------------------------------
////  メッシュバウンディングの終了処理
////--------------------------------------------------------------------------------------
//void MeshBoundingBox::Uninit( void )
//{
//	// 頂点バッファの破棄
//	if( m_pVtxBuff != NULL )
//	{
//		m_pVtxBuff->Release( );
//		m_pVtxBuff = NULL;
//	}
//
//	// インデックスバッファの破棄
//	if( m_pIndexBuff != NULL )
//	{
//		m_pIndexBuff->Release( );
//		m_pIndexBuff = NULL;
//	}
//}
//
////--------------------------------------------------------------------------------------
////  メッシュバウンディングの更新処理
////--------------------------------------------------------------------------------------
//void MeshBoundingBox::Update( void )
//{
//	//  頂点情報の設定
//	SetVertex( );
//}
//
////--------------------------------------------------------------------------------------
////  メッシュバウンディングの描画処理
////--------------------------------------------------------------------------------------
//void MeshBoundingBox::Draw( void )
//{
//	//  テクスチャ情報の取得
//	Texture* pTexture = SceneManager::GetTexture( );
//
//	//  メインからデバイス情報を取得
//	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );
//
//	//  ワールド行列
//	D3DXMATRIX mtxWorld;							
//
//	//  行列を単位行列に変換
//	D3DXMatrixIdentity( &mtxWorld );
//
//	//  GPUとVRAMの接続
//	pDevice->SetStreamSource( 0 ,													//  パイプライン番号
//							  m_pVtxBuff ,											//  頂点バッファのアドレス
//						  	  0 ,													//  オフセット( byte )
//							  sizeof( VERTEX_3D ) );								//  一個分の頂点データのサイズ( ストライド )
//
//	//  インデックスバッファの設定
//	pDevice->SetIndices( m_pIndexBuff );
//
//	//  シェーダー情報の取得
//	Shader3DNoLight* shader3DNoLight = ( Shader3DNoLight* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_NO_LIGHT );
//
//	//  ビューとプロジェクション行列の取得
//	D3DXMATRIX viewMatrix = SceneManager::GetCamera( SceneManager::GetLoop( ) )->GetViewMatrix( );
//	D3DXMATRIX projectionMatrix = SceneManager::GetCamera( SceneManager::GetLoop( ) )->GetViewMatrix( );
//
//	//  シェーダーに必要な情報の設定
//	shader3DNoLight->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix );
//
//	//  シェーダー3Dの描画開始
//	shader3DNoLight->DrawBegin( );
//
//	//  プリミティブの描画
//	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST ,				//  プリミティブの種類
//								   0 ,								//  最初の頂点インデックス番号のオフセット
//								   0 ,								//  最小の頂点インデックス番号のオフセット
//								   8 ,								//  頂点数												
//								   0 ,								//  スタートインデックス
//								   NUM_POLYGON * 8 );				//  プリミティブ数	
//
//	//  シェーダー3Dの描画終了
//	ShaderManager::DrawEnd( );
//}
//
////--------------------------------------------------------------------------------------
////  インスタンス生成をする関数
////--------------------------------------------------------------------------------------
//MeshBoundingBox* MeshBoundingBox::Create( const D3DXVECTOR3& min , const D3DXVECTOR3& max )
//{
//	MeshBoundingBox *pMeshBoundingBox;
//
//	//  インスタンス生成
//	pMeshBoundingBox = new MeshBoundingBox;
//
//	//  初期化
//	pMeshBoundingBox->Init( );
//
//	return pMeshBoundingBox;
//}
//
////--------------------------------------------------------------------------------------
////  頂点を作成する関数
////--------------------------------------------------------------------------------------
//void MeshBoundingBox::MakeVertex( void )
//{
//	//  デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );
//
//	//  頂点バッファの作成
//	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * 8 ,		//  作成したい頂点バッファのサイズ
//											 D3DUSAGE_WRITEONLY ,			//  使用方法
//											 0 ,							//  
//											 D3DPOOL_MANAGED ,				//  メモリ管理方法( MANAGED → お任せ )
//											 &m_pVtxBuff ,					//  バッファ
//											 NULL ) ) )
//	{
//		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );
//
//		return;
//	}
//
//	//  インデックスバッファの作成
//	if( FAILED( pDevice->CreateIndexBuffer( sizeof( WORD ) * 48 ,			//  作成したい頂点バッファのサイズ											
//											D3DUSAGE_WRITEONLY ,			//  使用方法
//											D3DFMT_INDEX16 ,				//  
//											D3DPOOL_MANAGED ,				//  メモリ管理方法( MANAGED → お任せ )
//											&m_pIndexBuff ,					//  バッファ
//											NULL ) ) )
//	{
//		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );
//
//		return;
//	}
//
//	VERTEX_3D* pVtx;												//  頂点バッファの仮想アドレス用ポインタ
//
//	//  頂点バッファをロックして、仮想アドレスを取得する
//	m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
//					  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
//					  0 );											//  ロックの種類
//
//	for( int countCorner = 0; countCorner < 8; countCorner++ )
//	{
//		//  頂点座標の設定( 2D座標 ・ 右回り )
//		pVtx[ 0 ].position = m_corners[ countCorner ];
//
//		//  頂点色の設定( 0 ～ 255 の整数値 )
//		pVtx[ 0 ].color = m_color;
//
//		//  UV座標の指定
//		pVtx[ 0 ].texcoord = D3DXVECTOR2( 0.0f  , 0.0f );
//
//		//  法線ベクトルの設定
//		pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
//
//		//  アドレスを進める
//		pVtx++;
//	}
//
//	//  頂点バッファのアンロック
//	m_pVtxBuff->Unlock( );
//
//	WORD* pIndexBuffer;											//  インデックスバッファの仮想アドレス用ポインタ
//
//	m_pIndexBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
//						( void** )&pIndexBuffer ,				//  アドレスが書かれたメモ帳のアドレス
//						0 );									//  ロックの種類
//
//	for( int i = 0; i < 48; i++ )
//	{
//		pIndexBuffer[ i ] = i;
//	}
//
//	//  インデックスバッファのアンロック
//	m_pIndexBuff->Unlock( );
//}