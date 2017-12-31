//--------------------------------------------------------------------------------------
//  メッシュドーム   ( meshDome.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "meshDome.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "light.h"
#include "shaderManager.h"
#include "camera.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MESH_DOME_TEXTURENAME	"data/TEXTURE/sky000.png"		//  テクスチャ名
#define MESH_DOME_TEXTURENAME2	"data/TEXTURE/skydome.jpg"		//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  メッシュドームクラスのコンストラクタ
//--------------------------------------------------------------------------------------
MeshDome::MeshDome( void ) : Scene( 0 )
{
	m_pVtxBuff = NULL;
	m_pIndexBuff = NULL;
	m_nDivideSide = 0;
	m_nDivideVertical = 0;
	m_changeScroll = D3DXVECTOR2( 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  メッシュドームクラスのデストラクタ
//--------------------------------------------------------------------------------------
MeshDome::~MeshDome( )
{

}

//--------------------------------------------------------------------------------------
//  メッシュドームの初期化処理
//--------------------------------------------------------------------------------------
HRESULT MeshDome::Init( void )
{
	//  テクスチャクラスのポインタ取得
	Texture* pTexture = SceneManager::GetTexture( );

	if( m_type == MeshDome::TYPE_SKY )
	{
		//  テクスチャの設定
		pTexture->SetTextureImage( MESH_DOME_TEXTURENAME );

		//  テクスチャ名の代入
		strcpy( m_aTextureName , MESH_DOME_TEXTURENAME );
	}
	else if( m_type == MeshDome::TYPE_COSMO )
	{
		//  テクスチャの設定
		pTexture->SetTextureImage( MESH_DOME_TEXTURENAME2 );

		//  テクスチャ名の代入
		strcpy( m_aTextureName , MESH_DOME_TEXTURENAME2 );
	}

	//  頂点の作成
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  メッシュドームの終了処理
//--------------------------------------------------------------------------------------
void MeshDome::Uninit( void )
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
//  メッシュドームの更新処理
//--------------------------------------------------------------------------------------
void MeshDome::Update( void )
{
	m_scroll.x += m_changeScroll.x;

	//  テクスチャの設定
	SetTexture( );
}

//--------------------------------------------------------------------------------------
//  メッシュドームの描画処理
//--------------------------------------------------------------------------------------
void MeshDome::Draw( void )
{
	//  テクスチャ情報の取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  シェーダー情報の取得
	Shader3DNoLight* shader3DNoLight = ( Shader3DNoLight* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_NO_LIGHT );

	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  移動行列
	D3DXMATRIX mtxRot;								//  回転行列

	//  テクスチャの設定
	SetTexture( );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  回転行列の作成
	D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.y , m_rot.x , m_rot.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x ,m_position.y , m_position.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  GPUとVRAMの接続
	pDevice->SetStreamSource( 0 ,										//  パイプライン番号
							  m_pVtxBuff ,								//  頂点バッファのアドレス
						  	  0 ,										//  オフセット( byte )
							  sizeof( VERTEX_3D ) );					//  一個分の頂点データのサイズ( ストライド )

	//  インデックスバッファの設定
	pDevice->SetIndices( m_pIndexBuff );

	Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
	D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
	D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );

	//  シェーダーに必要な情報の設定
	shader3DNoLight->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix );

	//  テクスチャの設定
	pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_aTextureName ) );

	//  シェーダー3Dの描画開始
	shader3DNoLight->DrawBegin( );

	//  プリミティブの描画
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
								   0 ,									//  最初の頂点インデックス番号のオフセット
								   0 ,									//  最小の頂点インデックス番号のオフセット
								   m_nNumVertex ,						//  頂点数														
								   0 ,									//  スタートインデックス
								   m_nNumPolygon );						//  プリミティブ数	

	//  シェーダー3Dの描画終了
	ShaderManager::DrawEnd( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
MeshDome* MeshDome::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 size , D3DXVECTOR2 scroll , int nDivideSide , int nDivideVertical )
{
	MeshDome *pMeshDome;

	//  インスタンス生成
	pMeshDome = new MeshDome;

	//  種類の代入
	pMeshDome->m_type = type;

	//  座標の代入
	pMeshDome->m_position = position;

	//  回転角の代入
	pMeshDome->m_rot = rot;

	//  大きさの代入
	pMeshDome->m_size = size;

	//  スクロール量の代入
	pMeshDome->m_changeScroll = scroll;

	//  横分割数の代入
	pMeshDome->m_nDivideSide = nDivideSide;

	//  縦分割数の代入
	pMeshDome->m_nDivideVertical = nDivideVertical;

	//  距離の代入
	pMeshDome->m_fDistance = sqrtf( size.x * size.x + size.z * size.z ) * 0.5f;

	//  頂点数の代入
	pMeshDome->m_nNumVertex = NUM_VERTEX2 * ( nDivideSide + 1 ) * ( nDivideVertical ) + ( nDivideVertical - 1 ) * 2;

	//  ポリゴン数の代入
	pMeshDome->m_nNumPolygon = NUM_POLYGON * nDivideSide * nDivideVertical + 4 * ( nDivideVertical - 1 );

	//  初期化
	pMeshDome->Init( );

	return pMeshDome;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void MeshDome::MakeVertex( void )
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * ( m_nDivideSide + 1 ) *
																   ( m_nDivideVertical + 1 ) ,			//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,										//  使用方法
											 0 ,														//  
											 D3DPOOL_MANAGED ,											//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuff ,												//  バッファ
											 NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ( メッシュドーム )" , MB_OK );

		return;
	}

	//  インデックスバッファの作成
	if( FAILED( pDevice->CreateIndexBuffer( sizeof( WORD ) * m_nNumVertex , 				//  作成したい頂点バッファのサイズ											
											D3DUSAGE_WRITEONLY ,							//  使用方法
											D3DFMT_INDEX16 ,								//  
											D3DPOOL_MANAGED ,								//  メモリ管理方法( MANAGED → お任せ )
											&m_pIndexBuff ,									//  バッファ
											NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ( メッシュドーム )" , MB_OK );

		return;
	}

	VERTEX_3D* pVtx;														//  頂点バッファの仮想アドレス用ポインタ

	//  頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuff->Lock( 0 , 0 ,												//  取る先頭と、サイズ( 0 , 0 で全部 )
					  ( void** )&pVtx ,										//  アドレスが書かれたメモ帳のアドレス
					  0 );													//  ロックの種類

	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
		{
			if( nCntSide == m_nDivideSide )
			{
				//  頂点座標の設定( 2D座標 ・ 右回り )
				pVtx[ 0 ].position = D3DXVECTOR3( cosf( D3DXToRadian( 0.0f ) ) * m_fDistance * sinf( D3DXToRadian( 90.0f / ( float )m_nDivideVertical * ( float )nCntVertical ) ) ,
											 m_fDistance * cosf( D3DXToRadian( 90.0f / ( float )m_nDivideVertical * ( float )nCntVertical ) ) ,
											 sinf( D3DXToRadian( 0.0f ) ) * m_fDistance * sinf( D3DXToRadian( 90.0f / ( float )m_nDivideVertical * ( float )nCntVertical ) ) );
			}
			else
			{
				//  頂点座標の設定( 2D座標 ・ 右回り )
				pVtx[ 0 ].position = D3DXVECTOR3( cosf( D3DXToRadian( ( 360.0f / ( float )m_nDivideSide ) * ( float )nCntSide ) ) * m_fDistance * sinf( D3DXToRadian( 90.0f / ( float )m_nDivideVertical * ( float )nCntVertical ) ) ,
											 m_fDistance * cosf( D3DXToRadian( 90.0f / ( float )m_nDivideVertical * ( float )nCntVertical ) ) ,
											 sinf( D3DXToRadian( 360.0f / ( float )m_nDivideSide * ( float )nCntSide ) ) * m_fDistance * sinf( D3DXToRadian( 90.0f / ( float )m_nDivideVertical * ( float )nCntVertical ) ) );
			}

			//  頂点色の設定( 0 ～ 255 の整数値 )
			pVtx[ 0 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

			//  UV座標の指定
			pVtx[ 0 ].texcoord = D3DXVECTOR2( ( float )nCntSide * ( 0.5f / ( float )m_nDivideSide ) , ( float )nCntVertical * ( 1.0f / ( float )m_nDivideVertical ) );

			//  アドレスを進める
			pVtx++;
		}
	}

	//  頂点バッファのアンロック
	m_pVtxBuff->Unlock( );

	WORD* pIndexBuffer;												//  インデックスバッファの仮想アドレス用ポインタ

	m_pIndexBuff->Lock( 0 , 0 ,										//  取る先頭と、サイズ( 0 , 0 で全部 )
						( void** )&pIndexBuffer ,					//  アドレスが書かれたメモ帳のアドレス
						0 );										//  ロックの種類

	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
		{
			pIndexBuffer[ 0 ] = ( m_nDivideSide + 1 ) * nCntVertical + nCntSide;
			pIndexBuffer[ 1 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 1 ) + nCntSide;

			pIndexBuffer += 2;

			if( nCntSide == m_nDivideSide &&
				nCntVertical != ( m_nDivideVertical - 1 ) )
			{
				pIndexBuffer[ 0 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 1 ) + nCntSide;
				pIndexBuffer[ 1 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 2 );

				pIndexBuffer += 2;
			}
		}
	}

	//  インデックスバッファのアンロック
	m_pIndexBuff->Unlock( );
}

//--------------------------------------------------------------------------------------
//  テクスチャの設定処理をする関数
//--------------------------------------------------------------------------------------
void MeshDome::SetTexture( void )
{
	VERTEX_3D* pVtx;													//  頂点バッファの仮想アドレス用ポインタ

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,											//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,									//  アドレスが書かれたメモ帳のアドレス
						  0 );												//  ロックの種類

		for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
		{
			for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
			{
				if( m_type == MeshDome::TYPE_SKY )
				{
					//  UV座標の指定
					pVtx[ 0 ].texcoord = D3DXVECTOR2( ( float )nCntSide * ( 1.0f / ( float )m_nDivideSide ) + m_scroll.x , ( float )nCntVertical * ( 1.0f / m_nDivideVertical ) + m_scroll.y );
				}
				else if( m_type == MeshDome::TYPE_COSMO )
				{
					//  UV座標の指定
					pVtx[ 0 ].texcoord = D3DXVECTOR2( ( float )nCntSide * ( 1.0f / ( float )m_nDivideSide ) + m_scroll.x , ( float )nCntVertical * ( 1.0f / m_nDivideVertical ) + m_scroll.y );
				}

				//  アドレスを進める
				pVtx++;
			}
		}

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}
}