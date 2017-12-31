//--------------------------------------------------------------------------------------
//  メッシュフィールド   ( meshField.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "meshTracing.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "light.h"
#include "game.h"
#include "keyboard.h"
#include <stdio.h>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MESH_FIELD_TEXTURENAME0		"data/TEXTURE/EFFECT/effect000.jpg"				//  テクスチャ名

//--------------------------------------------------------------------------------------
//  メッシュフィールドクラスのコンストラクタ
//--------------------------------------------------------------------------------------
MeshTracing::MeshTracing( void ) : Scene( 5 )
{
	m_pVtxBuff = NULL;
	m_pIndexBuff = NULL;
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドクラスのデストラクタ
//--------------------------------------------------------------------------------------
MeshTracing::~MeshTracing( )
{

}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの初期化処理
//--------------------------------------------------------------------------------------
HRESULT MeshTracing::Init( void )
{
	//  テクスチャクラスのポインタ取得
	Texture* pTexture = SceneManager::GetTexture( );

	m_nDivideSide = 5;
	m_nDivideVertical = 1;

	//  テクスチャの設定
	pTexture->SetTextureImage( MESH_FIELD_TEXTURENAME0 );

	for( int nCntSide = 0; nCntSide < m_nDivideSide; nCntSide++ )
	{
		//  頂点座標の設定( 2D座標 ・ 右回り )
		m_vertexPosition[ nCntSide * 2 ] = m_newVertexPositionUp; 
		m_vertexPosition[ nCntSide * 2 + 1 ] = m_newVertexPositionDown; 
	}

	//  頂点の作成
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの終了処理
//--------------------------------------------------------------------------------------
void MeshTracing::Uninit( void )
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
void MeshTracing::Update( void )
{
	//for( int nCntSide = 0; nCntSide < m_nDivideSide - 1; nCntSide++ )
	//{
	//	//  座標の入れ替え
	//	m_vertexPosition[ nCntSide * 2 ] = m_vertexPosition[ ( nCntSide + 1 ) * 2 ]; 
	//	m_vertexPosition[ nCntSide * 2 + 1 ] = m_vertexPosition[ ( nCntSide + 1 ) * 2 + 1 ]; 
	//}

	//m_vertexPosition[ ( m_nDivideSide - 1 ) * 2 ] = m_newVertexPositionUp;
	//m_vertexPosition[ ( m_nDivideSide - 1 ) * 2 + 1 ] = m_newVertexPositionDown;

	//  頂点情報の設定
	SetVertex( );

	m_countFrame++;

	if( m_countFrame > 300 )
	{
		m_countFrame = 0;
	}
}

//--------------------------------------------------------------------------------------
//  メッシュフィールドの描画処理
//--------------------------------------------------------------------------------------
void MeshTracing::Draw( void )
{
	//  テクスチャ情報の取得
	Texture* pTexture = SceneManager::GetTexture( );

	D3DMATERIAL9 material;

	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  移動行列
	D3DXMATRIX mtxRot;								//  回転行列

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  ワールド座標変換
	pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

	//  ライトを消す
	Light* pLight = SceneManager::GetLight( );
	pLight->LightOn( );

	//  材質クラスの初期化
	ZeroMemory( &material , sizeof( D3DMATERIAL9 ) );

	material.Diffuse = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	material.Ambient = D3DXCOLOR( 0.6f , 0.6f , 0.6f , 1.0f );

	//  材質の設定
	pDevice->SetMaterial( &material );

	//  GPUとVRAMの接続
	pDevice->SetStreamSource( 0 ,													//  パイプライン番号
							  m_pVtxBuff ,											//  頂点バッファのアドレス
						  	  0 ,													//  オフセット( byte )
							  sizeof( VERTEX_3D ) );								//  一個分の頂点データのサイズ( ストライド )

	//  インデックスバッファの設定
	//pDevice->SetIndices( m_pIndexBuff );

	// テクスチャの設定
	pDevice->SetTexture( 0 , pTexture->GetTextureImage( MESH_FIELD_TEXTURENAME0 ) ); 

	//  頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_3D );

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
							0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
							NUM_POLYGON * m_nDivideSide );		//  プリミティブ数

	////  プリミティブの描画
	//pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP ,									//  プリミティブの種類
	//							   0 ,														//  最初の頂点インデックス番号のオフセット
	//							   0 ,														//  最小の頂点インデックス番号のオフセット
	//							   ( NUM_VERTEX2 * ( ( m_nDivideSide + 1 )					//  頂点数
	//							   * m_nDivideVertical						
	//							   + m_nDivideVertical - 1 ) ) ,														
	//							   0 ,														//  スタートインデックス
	//							   NUM_POLYGON * m_nDivideSide *							//  プリミティブ数
	//							   m_nDivideVertical +						
	//							   4 * ( m_nDivideVertical - 1 ) );	

	//  ライトを点ける
	pLight->LightOff( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
MeshTracing* MeshTracing::Create( D3DXCOLOR color , D3DXVECTOR3 positionUp , D3DXVECTOR3 positionDown )
{
	MeshTracing *pMeshTracing;

	//  インスタンス生成
	pMeshTracing = new MeshTracing;

		//  色の代入
	pMeshTracing->m_color = color;

	//  座標の代入
	pMeshTracing->m_newVertexPositionUp = positionUp;

	//  ダウン座標の代入
	pMeshTracing->m_newVertexPositionDown = positionDown;

	//  初期化
	pMeshTracing->Init( );

	return pMeshTracing;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void MeshTracing::MakeVertex( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * ( m_nDivideSide ) *
																   ( m_nDivideVertical + 1 ) ,		//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,									//  使用方法
											 FVF_VERTEX_3D ,										//  
											 D3DPOOL_MANAGED ,										//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuff ,											//  バッファ
											 NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return;
	}

	////  インデックスバッファの作成
	//if( FAILED( pDevice->CreateIndexBuffer( sizeof( WORD ) * ( NUM_VERTEX2 * ( ( m_nDivideSide + 1 ) 
	//														 * m_nDivideVertical								//  作成したい頂点バッファのサイズ
	//														 + m_nDivideVertical - 1 ) ) ,											
	//										D3DUSAGE_WRITEONLY ,												//  使用方法
	//										D3DFMT_INDEX16 ,													//  
	//										D3DPOOL_MANAGED ,													//  メモリ管理方法( MANAGED → お任せ )
	//										&m_pIndexBuff ,														//  バッファ
	//										NULL ) ) )
	//{
	//	MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

	//	return;
	//}

	VERTEX_3D* pVtx;												//  頂点バッファの仮想アドレス用ポインタ

	//  頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
					  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
					  0 );											//  ロックの種類

	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
		{
			//  頂点色の設定( 0 ～ 255 の整数値 )
			pVtx[ 0 ].color = m_color;

			//  UV座標の指定
			pVtx[ 0 ].tex = D3DXVECTOR2( ( float )nCntSide  , ( float )nCntVertical );

			//  法線ベクトルの設定
			pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );

			//  アドレスを進める
			pVtx++;
		}
	}

	//  頂点バッファのアンロック
	m_pVtxBuff->Unlock( );

	////  頂点バッファをロックして、仮想アドレスを取得する
	//m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
	//				  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
	//				  0 );											//  ロックの種類

	//for( int nCntSide = 0; nCntSide < m_nDivideSide; nCntSide++ )
	//{
	//	//  頂点座標の設定( 2D座標 ・ 右回り )
	//	pVtx[ nCntSide * 2 ].position = m_vertexPosition[ nCntSide * 2 ];
	//	pVtx[ nCntSide * 2 + 1 ].position = m_vertexPosition[ nCntSide * 2 + 1 ];
	//}

	////  頂点バッファのアンロック
	//m_pVtxBuff->Unlock( );

	//WORD* pIndexBuffer;											//  インデックスバッファの仮想アドレス用ポインタ

	//m_pIndexBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
	//					( void** )&pIndexBuffer ,				//  アドレスが書かれたメモ帳のアドレス
	//					0 );									//  ロックの種類

	//for( int nCntVertical = 0; nCntVertical < m_nDivideVertical; nCntVertical++ )
	//{
	//	for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
	//	{
	//		pIndexBuffer[ 0 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 1 ) + nCntSide;
	//		pIndexBuffer[ 1 ] = ( m_nDivideSide + 1 ) * nCntVertical + nCntSide;

	//		pIndexBuffer += 2;

	//		if( nCntSide == m_nDivideSide &&
	//			nCntVertical != ( m_nDivideVertical - 1 ) )
	//		{
	//			pIndexBuffer[ 0 ] = ( m_nDivideSide + 1 ) * nCntVertical + nCntSide;
	//			pIndexBuffer[ 1 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 2 );

	//			pIndexBuffer += 2;
	//		}	
	//	}
	//}

	////  インデックスバッファのアンロック
	//m_pIndexBuff->Unlock( );
}

//--------------------------------------------------------------------------------------
//  頂点情報を設定する関数
//--------------------------------------------------------------------------------------
void MeshTracing::SetVertex( void )
{
	VERTEX_3D* pVtx;												//  頂点バッファの仮想アドレス用ポインタ

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
						  0 );											//  ロックの種類

		for( int nCntSide = 0; nCntSide < m_nDivideSide * 2; nCntSide++ )
		{
			//  頂点座標の設定( 2D座標 ・ 右回り )
			pVtx[ nCntSide ].position = m_vertexPosition[ nCntSide ];
		}

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}
}

//--------------------------------------------------------------------------------------
//  新しい頂点座標のトップ座標の設定
//
//  第1引数 : 座標  --  D3DXVETOR3
//  返り値  : なし
//--------------------------------------------------------------------------------------
void MeshTracing::SetNewVertexPositionUp( D3DXVECTOR3 position )
{
	m_newVertexPositionUp = position;
}

//--------------------------------------------------------------------------------------
//  新しい頂点座標のダウン座標の設定
//
//  第1引数 : 座標  --  D3DXVETOR3
//  返り値  : なし
//--------------------------------------------------------------------------------------
void MeshTracing::SetNewVertexPositionDown( D3DXVECTOR3 position )
{
	m_newVertexPositionDown = position;
}