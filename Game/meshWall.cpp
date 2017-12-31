//--------------------------------------------------------------------------------------
//  メッシュウォール   ( meshWall.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "meshWall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "light.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MESH_WALL_TEXTURENAME0		"data/TEXTURE/grove000.png"				//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  メッシュウォールクラスのコンストラクタ
//--------------------------------------------------------------------------------------
MeshWall::MeshWall( int nPriority ) : Scene( nPriority )
{
	m_pVtxBuff = NULL;
	m_pIndexBuff = NULL;
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_nDivideSide = 0;
	m_nDivideVertical = 0;

	m_type = TYPE_WALL;
}

//--------------------------------------------------------------------------------------
//  メッシュウォールクラスのデストラクタ
//--------------------------------------------------------------------------------------
MeshWall::~MeshWall( )
{

}

//--------------------------------------------------------------------------------------
//  メッシュウォールの初期化処理
//--------------------------------------------------------------------------------------
HRESULT MeshWall::Init( void )
{
	//  テクスチャクラスのポインタ取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  テクスチャの設定
	pTexture->SetTextureImage( MESH_WALL_TEXTURENAME0 );

	//  頂点の作成
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  メッシュウォールの終了処理
//--------------------------------------------------------------------------------------
void MeshWall::Uninit( void )
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
//  メッシュウォールの更新処理
//--------------------------------------------------------------------------------------
void MeshWall::Update( void )
{
	//  頂点情報の設定
	SetVertex( );
}

//--------------------------------------------------------------------------------------
//  メッシュウォールの描画処理
//--------------------------------------------------------------------------------------
void MeshWall::Draw( void )
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

	//  回転行列の作成
	D3DXMatrixRotationYawPitchRoll( &mtxRot ,
									m_rot.y ,
									m_rot.x ,
									m_rot.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans ,
						   m_position.x ,
						   m_position.y ,
						   m_position.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  ワールド座標変換
	pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

	//  ライトを消す
	Light* pLight = SceneManager::GetLight( );
	pLight->LightOff( );

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
	pDevice->SetIndices( m_pIndexBuff );

	//  頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_3D );

	switch( m_type )
	{
		case TYPE_WALL:
		{
			//  テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( MESH_WALL_TEXTURENAME0 ) ); 

			break;
		}
	}

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

	//  ライトを点ける
	pLight->LightOn( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
MeshWall* MeshWall::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 size , D3DXCOLOR color ,
							  int nDivedeSide , int nDivideVertical , int nPriority )
{
	MeshWall *pMeshWall;

	//  インスタンス生成
	pMeshWall = new MeshWall( nPriority );

	//  種類の代入
	pMeshWall->m_type = type;

	//  座標の代入
	pMeshWall->m_position = position;

	//  回転角の代入
	pMeshWall->m_rot = rot;

	//  大きさの代入
	pMeshWall->m_size = size;

	//  色の代入
	pMeshWall->m_color = color;

	//  横分割数の代入
	pMeshWall->m_nDivideSide = nDivedeSide;

	//  縦分割数の代入
	pMeshWall->m_nDivideVertical = nDivideVertical;

	//  初期化
	pMeshWall->Init( );

	return pMeshWall;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void MeshWall::MakeVertex( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * ( m_nDivideSide + 1 ) *
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

	D3DXVECTOR3 workNormal;											//  法線計算格納用

	//  頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
					  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
					  0 );											//  ロックの種類

	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
		{
			//  頂点座標の設定( 2D座標 ・ 右回り )
			pVtx[ 0 ].position = D3DXVECTOR3( -( m_size.x / 2 ) + nCntSide * m_size.x / m_nDivideSide ,
										 m_size.y - nCntVertical * m_size.y / m_nDivideVertical ,
										 0.0f );

			D3DXVec3Normalize( &workNormal , &( pVtx[ 0 ].position - D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ) );

			//  法線の設定
			pVtx[ 0 ].normal = workNormal;

			//  頂点色の設定( 0 ～ 255 の整数値 )
			pVtx[ 0 ].color = m_color;

			//  UV座標の指定
			pVtx[ 0 ].texcoord = D3DXVECTOR2( ( float )nCntSide , ( float )nCntVertical );

			//  アドレスを進める
			pVtx++;
		}
	}

	//  頂点バッファのアンロック
	m_pVtxBuff->Unlock( );

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
void MeshWall::SetVertex( void )
{
	VERTEX_3D* pVtx;												//  頂点バッファの仮想アドレス用ポインタ

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,								//  アドレスが書かれたメモ帳のアドレス
						  0 );											//  ロックの種類

		for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
		{
			for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
			{
				//  頂点座標の設定( 2D座標 ・ 右回り )
				pVtx[ 0 ].position = D3DXVECTOR3( -m_size.x * 0.5f + nCntSide * m_size.x / ( float )m_nDivideSide ,
											 m_size.y - nCntVertical * m_size.y / ( float )m_nDivideVertical ,
											 0.0f );

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