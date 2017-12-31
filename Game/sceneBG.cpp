//--------------------------------------------------------------------------------------
//  BGレンダリング処理   ( sceneBG.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include "sceneBG.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define ANIMATION_SHIFT_FRAME	( 20 )									//  アニメーション移行フレーム

#define BG_TEXTURENAME000		"data/TEXTURE/TITLE/titleBG.png"		//  テクスチャ名
#define BG_TEXTURENAME001		"data/TEXTURE/UI/UI.png"				//  テクスチャ名
#define BG_TEXTURENAME002		"data/TEXTURE/STAGESELECT/menu.png"		//  テクスチャ名
#define BG_TEXTURENAME003		"data/TEXTURE/RESULT/result.png"		//  テクスチャ名
#define BG_TEXTURENAME004		"data/TEXTURE/RESULT/result2.png"		//  テクスチャ名
#define BG_TEXTURENAME005		"data/TEXTURE/RESULT/result5.png"		//  テクスチャ名
#define BG_TEXTURENAME006		"data/TEXTURE/RESULT/result6.png"		//  テクスチャ名
#define BG_TEXTURENAME007		"data/TEXTURE/earnest_logo2.png"		//  テクスチャ名
#define BG_TEXTURENAME008		"data/TEXTURE/TUTORIAL/tutorial.png"	//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  BGポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneBG::SceneBG( ) : Scene( 7 )
{
	//  情報の代入
	m_pVtxBuff = NULL;
	m_pIndexBuff = NULL;
	m_position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	m_posUV = D3DXVECTOR2( 0.0f , 0.0f );
	m_divideUV = D3DXVECTOR2( 0.0f , 0.0f );

	m_fRot = 0.0f;

	m_nAnimationShift = 0;
}

//--------------------------------------------------------------------------------------
//  BGポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneBG::SceneBG( D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV , D3DXCOLOR color )
{
	//  情報の代入
	m_position = position;
	m_size = size;
	m_color = color;
	m_posUV = posUV;
	m_divideUV = divideUV;

	m_fRot = 0.0f;

	m_nAnimationShift = 0;
}

//--------------------------------------------------------------------------------------
//  BGポリゴンクラスのデストラクタ
//--------------------------------------------------------------------------------------
SceneBG::~SceneBG( )
{

}

//--------------------------------------------------------------------------------------
//  BGポリゴンの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SceneBG::Init( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  テクスチャの登録
	pTexture->SetTextureImage( BG_TEXTURENAME000 );
	pTexture->SetTextureImage( BG_TEXTURENAME001 );
	pTexture->SetTextureImage( BG_TEXTURENAME002 );
	pTexture->SetTextureImage( BG_TEXTURENAME003 );
	pTexture->SetTextureImage( BG_TEXTURENAME004 );
	pTexture->SetTextureImage( BG_TEXTURENAME005 );
	pTexture->SetTextureImage( BG_TEXTURENAME006 );
	pTexture->SetTextureImage( BG_TEXTURENAME007 );
	pTexture->SetTextureImage( BG_TEXTURENAME008 );

	// 頂点バッファの生成
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  BGポリゴンの終了処理
//--------------------------------------------------------------------------------------
void SceneBG::Uninit( void )
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
//  BGポリゴンの更新処理
//--------------------------------------------------------------------------------------
void SceneBG::Update( void )
{
	//  頂点の設定
	SetVertex( );

	//  UV分割数が1以外の場合
	if( m_divideUV.x != 1.0f || m_divideUV.y != 1.0f )
	{
		//  移行フレームが既定値を超えた場合
		if( m_nAnimationShift > ANIMATION_SHIFT_FRAME )
		{
			//  UV座標を進める
			m_posUV.x += ( 1.0f / m_divideUV.x );

			//  アニメーション移行フレームの初期化
			m_nAnimationShift = 0;
		}

		//  アニメーション移行フレームカウント
		m_nAnimationShift++;
	}
}

//--------------------------------------------------------------------------------------
//  BGポリゴンの描画処理
//--------------------------------------------------------------------------------------
void SceneBG::Draw( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource( 0 ,								//  パイプライン番号
							  m_pVtxBuff ,						//  頂点バッファのアドレス
							  0 ,								//  オフセット( byte )
							  sizeof( VERTEX_2D ) );			//  一個分の頂点データのサイズ( ストライド )

	// 頂点フォーマットの設定
	pDevice->SetFVF( FVF_VERTEX_2D );

	//  種類によっての場合分け
	switch( m_type )
	{
		case TYPE_TITLE:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME000 ) ); 

			break;
		}
		case TYPE_GAME:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME001 ) ); 

			break;
		}
		case TYPE_STAGE_SELECT:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME002 ) ); 

			break;
		}
		case TYPE_RESULT:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME003 ) ); 

			break;
		}
		case TYPE_RESULT2:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME004 ) ); 

			break;
		}
		case TYPE_RESULT3:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME005 ) ); 

			break;
		}
		case TYPE_RESULT4:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME006 ) ); 

			break;
		}
		case TYPE_TEAM_LOGO:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME007 ) ); 

			break;
		}
		case TYPE_TUTORIAL:
		{
			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( BG_TEXTURENAME008 ) ); 

			break;
		}
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
							0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
							NUM_POLYGON );						//  プリミティブ数
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
HRESULT SceneBG::MakeVertex( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX ,		//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,					//  使用方法
											 0 ,									//  
											 D3DPOOL_MANAGED ,						//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuff ,							//  バッファ
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

	float fLength = sqrtf( 0.0f * 0.0f + 0.0f * 0.0f ) * 0.5f;
	float fAngle = atan2f( 0.0f , 0.0f );

	//  左上座標の算出
	LeftTop = D3DXVECTOR3( m_position.x + cosf( D3DX_PI + fAngle ) * fLength ,
						   m_position.y + sinf( D3DX_PI + fAngle ) * fLength , 0.0f );

	//  右上座標の算出
	RightTop = D3DXVECTOR3( m_position.x + cosf( - fAngle ) * fLength ,
							m_position.y + sinf( - fAngle ) * fLength , 0.0f );

	//  左下座標の算出
	LeftButtom = D3DXVECTOR3( m_position.x + cosf( D3DX_PI - fAngle ) * fLength ,
							  m_position.y + sinf( D3DX_PI - fAngle ) * fLength , 0.0f );

	//  右下座標の算出
	RightButtom = D3DXVECTOR3( m_position.x + cosf( fAngle ) * fLength ,
							   m_position.y + sinf( fAngle ) * fLength , 0.0f );

	//  頂点バッファをロックして、仮想アドレスを取得する
	m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
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
	pVtx[ 0 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	pVtx[ 1 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	pVtx[ 2 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	pVtx[ 3 ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	//  UV座標の指定
	pVtx[ 0 ].texcoord = D3DXVECTOR2( 0.0f , 0.0f );
	pVtx[ 1 ].texcoord = D3DXVECTOR2( 1.0f , 0.0f );
	pVtx[ 2 ].texcoord = D3DXVECTOR2( 0.0f , 1.0f );
	pVtx[ 3 ].texcoord = D3DXVECTOR2( 1.0f , 1.0f );

	//  頂点バッファのアンロック
	m_pVtxBuff->Unlock( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void SceneBG::SetVertex( void )
{
	VERTEX_2D* pVtx = NULL;				//  頂点バッファのポインタ
	D3DXVECTOR3 LeftTop;				//  左上座標
	D3DXVECTOR3 RightTop;				//  右上座標
	D3DXVECTOR3 LeftButtom;				//  左下座標
	D3DXVECTOR3 RightButtom;			//  右下座標

	float fLength = sqrtf( m_size.x * m_size.x + m_size.y * m_size.y ) * 0.5f;
	float fAngle = atan2f( m_size.y , m_size.x );

	//  左上座標の算出
	LeftTop = D3DXVECTOR3( m_position.x + cosf( D3DX_PI + fAngle + m_fRot ) * fLength ,
						   m_position.y + sinf( D3DX_PI + fAngle + m_fRot ) * fLength , 0.0f );

	//  右上座標の算出
	RightTop = D3DXVECTOR3( m_position.x + cosf( - fAngle + m_fRot ) * fLength ,
							m_position.y + sinf( - fAngle + m_fRot ) * fLength , 0.0f );

	//  左下座標の算出
	LeftButtom = D3DXVECTOR3( m_position.x + cosf( D3DX_PI - fAngle + m_fRot ) * fLength ,
							  m_position.y + sinf( D3DX_PI - fAngle + m_fRot ) * fLength , 0.0f );

	//  右下座標の算出
	RightButtom = D3DXVECTOR3( m_position.x + cosf( fAngle + m_fRot ) * fLength ,
							   m_position.y + sinf( fAngle + m_fRot ) * fLength , 0.0f );

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
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

		//  色の設定( 2D座標 ・ 右回り )
		pVtx[ 0 ].color = m_color;
		pVtx[ 1 ].color = m_color;
		pVtx[ 2 ].color = m_color;
		pVtx[ 3 ].color = m_color;

		//  UV座標の指定
		pVtx[ 0 ].texcoord = D3DXVECTOR2( m_posUV.x , m_posUV.y );
		pVtx[ 1 ].texcoord = D3DXVECTOR2( m_posUV.x + 1.0f / m_divideUV.x , m_posUV.y );
		pVtx[ 2 ].texcoord = D3DXVECTOR2( m_posUV.x , m_posUV.y + 1.0f / m_divideUV.y );
		pVtx[ 3 ].texcoord = D3DXVECTOR2( m_posUV.x + 1.0f / m_divideUV.x , m_posUV.y + 1.0f / m_divideUV.y );

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}
}

//--------------------------------------------------------------------------------------
//  BGポリゴンのUV座標設定
//--------------------------------------------------------------------------------------
void SceneBG::SetPositionUV( D3DXVECTOR2 posUV )
{
	m_posUV = posUV;
}

//--------------------------------------------------------------------------------------
//  BGポリゴンのUV座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR2 SceneBG::GetPosUV( void )
{
	return m_posUV;
}

//--------------------------------------------------------------------------------------
//  BGポリゴンのUV分割数設定
//--------------------------------------------------------------------------------------
void SceneBG::SetDivideUV( D3DXVECTOR2 divideUV )
{
	m_divideUV = divideUV;
}

//--------------------------------------------------------------------------------------
//  BGポリゴンのUV分割数を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR2 SceneBG::GetDivideUV( void )
{
	return m_divideUV;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
SceneBG* SceneBG::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV )
{
	SceneBG *pSceneBG;

	//  インスタンス生成
	pSceneBG = new SceneBG;

	//  種類の代入
	pSceneBG->m_type = type;

	//  座標の代入
	pSceneBG->m_position = position;

	//  大きさの代入
	pSceneBG->m_size = size;

	//  UV座標開始地点の代入
	pSceneBG->m_posUV = posUV;

	//  テクスチャUV分割数の代入
	pSceneBG->m_divideUV = divideUV;

	//  初期化
	pSceneBG->Init( );

	return pSceneBG;
}