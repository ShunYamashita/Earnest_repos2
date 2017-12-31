//--------------------------------------------------------------------------------------
//  フェード処理   ( scene2D.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "fade.h"
#include <stdio.h>
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define ANIMATION_SHIFT_FRAME	( 20 )							//  アニメーション移行フレーム

#define FADE_TEXTURENAME		"data/TEXTURE/fade.png"			//  テクスチャ名

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9	Fade::m_pVtxBuff = NULL;				//  頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9	Fade::m_pIndexBuff = NULL;				//  インデックスバッファインターフェースへのポインタ

D3DXCOLOR		Fade::m_color = D3DXCOLOR( 0.0f ,				//  フェード色格納用
											0.0f ,
											0.0f ,
											0.0f );			
Fade::STATE	Fade::m_state = FADE_NONE;						//  フェード状態格納用
Mode::MODE		Fade::m_modeNext ;								//  次のモード格納用
float			Fade::m_fFadeSpeed = 0.0f;						//  フェード速度

//--------------------------------------------------------------------------------------
//  フェードクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Fade::Fade( )
{

}

//--------------------------------------------------------------------------------------
//  フェードクラスのデストラクタ
//--------------------------------------------------------------------------------------
Fade::~Fade( )
{

}

//--------------------------------------------------------------------------------------
//  フェードの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Fade::Init( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	//  デバイス情報の取得
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  テクスチャクラスの取得
	Texture* pTexture = SceneManager::GetTexture( );

	//  テクスチャの登録
	pTexture->SetTextureImage( FADE_TEXTURENAME );

	// 頂点バッファの生成
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  フェードの終了処理
//--------------------------------------------------------------------------------------
void Fade::Uninit( void )
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
//  フェードの更新処理
//--------------------------------------------------------------------------------------
void Fade::Update( void )
{
	//  フェードしない場合
	if( m_state == FADE_NONE )
	{
		return;
	}

	//  フェードイン状態の場合
	if( m_state == FADE_IN )
	{
		//  α値を減算して後ろの画面を浮き上がらせる
		m_color.a -= m_fFadeSpeed;

		//  α値が０以下になった場合
		if( m_color.a < 0.0f )
		{
			//  α値の設定
			m_color.a = 0.0f;

			//  フェードイン状態の場合
			if( m_state == FADE_IN )
			{
				//  フェードしない状態に
				m_state = FADE_NONE;
			}
		}
	}
	//  フェードアウト状態の場合
	if( m_state == FADE_OUT )
	{
		//  α値を加算して前のの画面を消していく
		m_color.a += m_fFadeSpeed;

		//  α値が1以上になった場合
		if( m_color.a > 1.0f )
		{
			//  α値の設定
			m_color.a = 1.0f;

			//  フェードアウトの場合
			if( m_state == FADE_OUT )
			{
				//  フェードイン状態に
				m_state = FADE_IN;

				//  次のモードへ
				SceneManager::SetMode( m_modeNext );
			}
		}
	}

	//  頂点の設定
	SetVertex( );
}

//--------------------------------------------------------------------------------------
//  フェードの描画処理
//--------------------------------------------------------------------------------------
void Fade::Draw( void )
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

	// テクスチャの設定
	pDevice->SetTexture( 0 , pTexture->GetTextureImage( FADE_TEXTURENAME ) ); 

	// ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  プリミティブの種類
							0 ,									//  オフセット( 何番目の頂点から描画するか選べる )
							NUM_POLYGON );						//  プリミティブ数
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
HRESULT Fade::MakeVertex( void )
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

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
						  0 );										//  ロックの種類

		//  頂点座標の設定( 2D座標 ・ 右回り )
		pVtx[ 0 ].position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 1 ].position = D3DXVECTOR3( SCREEN_WIDTH , 0.0f , 0.0f );
		pVtx[ 2 ].position = D3DXVECTOR3( 0.0f , SCREEN_HEIGHT , 0.0f );
		pVtx[ 3 ].position = D3DXVECTOR3( SCREEN_WIDTH , SCREEN_HEIGHT , 0.0f );

		//  rhwの設定( 必ず1.0f )
		pVtx[ 0 ].rhw =
		pVtx[ 1 ].rhw =
		pVtx[ 2 ].rhw =
		pVtx[ 3 ].rhw = 1.0f;

		//  頂点色の設定( 0 ～ 255 の整数値 )
		pVtx[ 0 ].color = m_color;
		pVtx[ 1 ].color = m_color;
		pVtx[ 2 ].color = m_color;
		pVtx[ 3 ].color = m_color;

		//  UV座標の指定
		pVtx[ 0 ].texcoord = D3DXVECTOR2( 0.0f , 0.0f );
		pVtx[ 1 ].texcoord = D3DXVECTOR2( 1.0f , 0.0f );
		pVtx[ 2 ].texcoord = D3DXVECTOR2( 0.0f , 1.0f );
		pVtx[ 3 ].texcoord = D3DXVECTOR2( 1.0f , 1.0f );

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  頂点を作成する関数
//--------------------------------------------------------------------------------------
void Fade::SetVertex( void )
{
	VERTEX_2D* pVtx = NULL;				//  頂点バッファのポインタ

	if( m_pVtxBuff != NULL )
	{
		//  頂点バッファをロックして、仮想アドレスを取得する
		m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
						  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
						  0 );										//  ロックの種類

		//  色の設定( 2D座標 ・ 右回り )
		pVtx[ 0 ].color = m_color;
		pVtx[ 1 ].color = m_color;
		pVtx[ 2 ].color = m_color;
		pVtx[ 3 ].color = m_color;

		//  頂点バッファのアンロック
		m_pVtxBuff->Unlock( );
	}
}

//--------------------------------------------------------------------------------------
//  フェードの設定をする関数
//--------------------------------------------------------------------------------------
void Fade::SetFade( STATE state , Mode::MODE modeNext , D3DXCOLOR color , float fFadeSpeed )
{
	//  フェード状態ではない場合
	if( m_state == FADE_NONE )
	{
		//  フェード状態の設定
		m_state = state;

		//  次のモードの設定
		m_modeNext = modeNext;

		//  色情報の代入
		m_color = color;

		//  フェード速度の代入
		m_fFadeSpeed = fFadeSpeed;
	}
}

//--------------------------------------------------------------------------------------
//  フェードの状態を取得する関数
//--------------------------------------------------------------------------------------
Fade::STATE Fade::GetFade( void )
{
	return m_state;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Fade* Fade::Create( void )
{
	Fade *pFade;

	//  インスタンス生成
	pFade = new Fade;

	//  初期化
	pFade->Init( );

	return pFade;
}