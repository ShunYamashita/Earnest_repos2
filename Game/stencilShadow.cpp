//--------------------------------------------------------------------------------------
//  ステンシルシャドウ   ( stencilShadow.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "stencilShadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "keyboard.h"
#include "sceneBG.h"
#include "shaderManager.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define	MODEL_FILENAME		"data/MODEL/SHADOW/sphere.x"
#define	MODEL_FILENAME2		"data/MODEL/SHADOW/pillar.x"
#define	MODEL_FILENAME3		"data/MODEL/SHADOW/test.x"

#define TEXTURE_FILEPATH	"data\\TEXTURE"						//  テクスチャへのファイルパス

#define MAX_MODEL_PARTS		( 20 )								//  マテリアル最大数

#define SHADOW_TEXTURENAME	"data/TEXTURE/fade.png"				//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ステンシルシャドウクラスのコンストラクタ
//--------------------------------------------------------------------------------------
StencilShadow::StencilShadow( ) : Scene( 4 )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_scale = D3DXVECTOR3( 1.0f, 1.0f , 1.0f );
	m_rot = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  ステンシルシャドウクラスのコンストラクタ
//--------------------------------------------------------------------------------------
StencilShadow::StencilShadow( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_position = position;
	m_rot = rot;
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  ステンシルシャドウのデストラクタ
//--------------------------------------------------------------------------------------
StencilShadow::~StencilShadow( )
{

}

//--------------------------------------------------------------------------------------
//  ステンシルシャドウの初期化処理
//--------------------------------------------------------------------------------------
HRESULT StencilShadow::Init( void )
{
	D3DXMATERIAL*	pMat = NULL;									//  マテリアル
	char			aTextureFileName[ 128 ] = { };
	int				nIndex = 0;

	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  頂点バッファの作成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX ,		//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,					//  使用方法
											 0 ,									//  
											 D3DPOOL_MANAGED ,						//  メモリ管理方法( MANAGED → お任せ )
											 &m_shadowVtxBuff ,						//  バッファ
											 NULL ) ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return E_FAIL;
	}

	VERTEX_2D* pVtx = NULL;				//  頂点バッファのポインタ
	D3DXVECTOR3 LeftTop;				//  左上座標
	D3DXVECTOR3 RightTop;				//  右上座標
	D3DXVECTOR3 LeftButtom;				//  左下座標
	D3DXVECTOR3 RightButtom;			//  右下座標

	//  左上座標の算出
	LeftTop = D3DXVECTOR3( 0.0f ,
						   0.0f ,
						   0.0f );

	//  右上座標の算出
	RightTop = D3DXVECTOR3( SCREEN_WIDTH ,
							0.0f ,
							0.0f );

	//  左下座標の算出
	LeftButtom = D3DXVECTOR3( 0.0f ,
							  SCREEN_HEIGHT ,
							  0.0f );

	//  右下座標の算出
	RightButtom = D3DXVECTOR3( SCREEN_WIDTH ,
							   SCREEN_HEIGHT ,
							   0.0f );

	if( m_shadowVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_shadowVtxBuff->Lock( 0 , 0 ,								//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
						  0 );										//  ロックの種類

		//  頂点座標の設定( 2D座標 ・ 右回り )
		pVtx[ 0 ].position = LeftTop;
		pVtx[ 1 ].position = RightTop;
		pVtx[ 2 ].position = LeftButtom;
		pVtx[ 3 ].position = RightButtom;

		//  rhwの設定( 必ず1.0f )
		pVtx[ 0 ].rhw =
		pVtx[ 1 ].rhw =
		pVtx[ 2 ].rhw =
		pVtx[ 3 ].rhw = 1.0f;

		//  頂点色の設定( 0 ～ 255 の整数値 )
		pVtx[ 0 ].color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.5f );
		pVtx[ 1 ].color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.5f );
		pVtx[ 2 ].color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.5f );
		pVtx[ 3 ].color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.5f );

		//  UV座標の指定
		pVtx[ 0 ].texcoord = D3DXVECTOR2( 0.0f , 0.0f );
		pVtx[ 1 ].texcoord = D3DXVECTOR2( 1.0f , 0.0f );
		pVtx[ 2 ].texcoord = D3DXVECTOR2( 0.0f , 1.0f );
		pVtx[ 3 ].texcoord = D3DXVECTOR2( 1.0f , 1.0f );

		//  頂点バッファのアンロック
		m_shadowVtxBuff->Unlock( );
	}

	if( m_type == StencilShadow::TYPE::SPHERE )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME ,				//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &m_pMeshModel ) ) )			//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == StencilShadow::TYPE::PILLAR )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME2 ,			//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &m_pMeshModel ) ) )			//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == StencilShadow::TYPE::THIEF )
	{
		//  xファイルの読み込み
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME3 ,			//  ファイル名
		 							   D3DXMESH_MANAGED ,			//  オプション
									   pDevice ,					//  デバイス
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  マテリアル情報
									   NULL ,						//  
									   &m_nNumMatModel ,			//  マテリアル数
									   &m_pMeshModel ) ) )			//  メッシュ情報
		{
			MessageBox( NULL , "xファイルを上手く取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

			return E_FAIL;
		}
	}

	//  マテリアル情報の先頭アドレスを取得
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  テクスチャポインタがある場合
		if( pMat[ nCntMaterial ].pTextureFilename != NULL )
		{
			//  テクスチャファイルパス用の文字列
			char aTextureFilePath[ 128 ] = { TEXTURE_FILEPATH };

			//  文字数分のループ
			for( int nCntLength = strlen( pMat[ nCntMaterial ].pTextureFilename ) - 1; nCntLength >= 0 ; nCntLength-- )
			{
				//  テクスチャ名の取り出し
				aTextureFileName[ nIndex ] = pMat[ nCntMaterial ].pTextureFilename[ nCntLength ];

				nIndex++;

				//  \があるとループを抜け出す
				if( pMat[ nCntMaterial ].pTextureFilename[ nCntLength ] == '\\' )
				{
					break;
				}
			}

			//  \0の格納
			aTextureFileName[ nIndex ] = '\0';

			//  インデックス番号の初期化
			nIndex = 0;

			char aWork;											//  １文字分の格納用
			int	 nLength = strlen( aTextureFileName );			//  文字の長さ

			//  文字数分のループ
			for( int nCntLength = 0; nCntLength < nLength / 2; nCntLength++ )
			{
				//  テクスチャ名の反転
				aWork = aTextureFileName[ nCntLength ];
				aTextureFileName[ nCntLength ] = aTextureFileName[ nLength - nCntLength - 1 ];
				aTextureFileName[ nLength - nCntLength - 1 ] = aWork;
			}

			//  テクスチャ名をテクスチャパス名に
			strcat( aTextureFilePath , aTextureFileName );

			//  テクスチャ名のコピー
			strcpy( m_aTextureName[ nCntMaterial ] , aTextureFilePath );

			//  テクスチャの登録
			pTexture->SetTextureImage( aTextureFilePath );
		}
	}

	//  テクスチャの登録
	pTexture->SetTextureImage( SHADOW_TEXTURENAME );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  ステンシルシャドウの終了処理
//--------------------------------------------------------------------------------------
void StencilShadow::Uninit( void )
{
	//  メッシュ情報の解放
	SAFE_RELEASE( m_pMeshModel );

	//  マテリアル情報の解放
	SAFE_RELEASE( m_pBuffMatModel );
}

//--------------------------------------------------------------------------------------
//  ステンシルシャドウの更新処理
//--------------------------------------------------------------------------------------
void StencilShadow::Update( void )
{
}

//--------------------------------------------------------------------------------------
//  ステンシルシャドウの描画処理
//--------------------------------------------------------------------------------------
void StencilShadow::Draw( void )
{
	//  影の描画
	//DrawShadow( );
}

//--------------------------------------------------------------------------------------
//  ステンシルシャドウの影の描画処理
//--------------------------------------------------------------------------------------
void StencilShadow::DrawShadow( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DMATERIAL9	matDef;							//  マテリアルの初期情報
	D3DXMATERIAL*	pMat = NULL;					//  マテリアル

	D3DXMATRIX mtxWorld;							//  ワールド行列
	D3DXMATRIX mtxTrans;							//  平行移動行列
	D3DXMATRIX mtxScale;							//  スケール行列
	D3DXMATRIX mtxRot;								//  回転行列

	D3DXMATRIX mtxView;								//  ビュー座標変換行列
	D3DXMATRIX mtxProj;								//  プロジェクション座標行列

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  行列を単位行列に変換
	D3DXMatrixIdentity( &mtxWorld );

	//  拡大行列の作成
	D3DXMatrixScaling( &mtxScale , m_scale.x , m_scale.y , m_scale.z );

	//  拡大行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.y , m_rot.x , m_rot.z );

	//  回転行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  平行移動行列の作成
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

	//  平行移動行列の掛け算
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  ワールド座標変換
	pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

	//  マテリアルの初期情報の取得
	pDevice->GetMaterial( &matDef );

	//  マテリアル情報の先頭アドレスを取得
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

	pDevice->SetRenderState( D3DRS_STENCILENABLE , TRUE );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE , FALSE );
	pDevice->SetRenderState( D3DRS_COLORWRITEENABLE , 0 );
	pDevice->SetRenderState( D3DRS_STENCILFUNC , D3DCMP_ALWAYS );
	pDevice->SetRenderState( D3DRS_STENCILREF , 1 );
	pDevice->SetRenderState( D3DRS_STENCILPASS , D3DSTENCILOP_INCR );
	pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  拡散光の設定
		pMat[ nCntMaterial ].MatD3D.Diffuse = m_color;

		//  環境光の設定
		pMat[ nCntMaterial ].MatD3D.Ambient = D3DXCOLOR( 0.6f , 0.6f , 0.6f , 1.0f );

		//  マテリアルの設定
		pDevice->SetMaterial( &pMat[ nCntMaterial ].MatD3D );

		if( pMat[ nCntMaterial ].pTextureFilename == NULL )
		{
			//  テクスチャの設定
			pDevice->SetTexture( 0 , NULL );
		}
		else
		{
			//  テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_aTextureName[ nCntMaterial ] ) );
		}

		//  メッシュの描画
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  材質の設定
		pDevice->SetMaterial( &matDef );
	}

	pDevice->SetRenderState( D3DRS_STENCILPASS , D3DSTENCILOP_DECR );
	pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CW );

	//  マテリアルの数分のループ
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  拡散光の設定
		pMat[ nCntMaterial ].MatD3D.Diffuse = m_color;

		//  環境光の設定
		pMat[ nCntMaterial ].MatD3D.Ambient = D3DXCOLOR( 0.6f , 0.6f , 0.6f , 1.0f );

		//  マテリアルの設定
		pDevice->SetMaterial( &pMat[ nCntMaterial ].MatD3D );

		if( pMat[ nCntMaterial ].pTextureFilename == NULL )
		{
			//  テクスチャの設定
			pDevice->SetTexture( 0 , NULL );
		}
		else
		{
			//  テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_aTextureName[ nCntMaterial ] ) );
		}

		//  メッシュの描画
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  材質の設定
		pDevice->SetMaterial( &matDef );
	}

	pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );
	pDevice->SetRenderState( D3DRS_COLORWRITEENABLE , 0xf );
	pDevice->SetRenderState( D3DRS_STENCILFUNC , D3DCMP_EQUAL );

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0 ,								//  パイプライン番号
							  m_shadowVtxBuff ,					//  頂点バッファのアドレス
							  0 ,								//  オフセット( byte )
							  sizeof( VERTEX_2D ) );			//  一個分の頂点データのサイズ( ストライド )

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	pDevice->SetTexture( 0 , pTexture->GetTextureImage( SHADOW_TEXTURENAME ) ); 

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
							0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
							NUM_POLYGON );						//  プリミティブ数

	pDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE );
	pDevice->SetRenderState( D3DRS_STENCILENABLE , FALSE );
}

//--------------------------------------------------------------------------------------
//  ステンシルシャドウの大きさ設定をする関数
//--------------------------------------------------------------------------------------
void StencilShadow::SetScale( D3DXVECTOR3 scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  xポリゴンの大きさを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 StencilShadow::GetScale( void )
{
	return m_scale;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
StencilShadow* StencilShadow::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	StencilShadow *pStencilShadow;

	//  インスタンス生成
	pStencilShadow = new StencilShadow;

	//  種類の代入
	pStencilShadow->m_type = type;

	//  座標の代入
	pStencilShadow->m_position = position;

	//  回転角の代入
	pStencilShadow->m_rot = rot;

	//  大きさ倍率の代入
	pStencilShadow->m_scale = scale;

	//  初期化
	pStencilShadow->Init( );

	return pStencilShadow;
}