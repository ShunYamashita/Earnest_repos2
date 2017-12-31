//--------------------------------------------------------------------------------------
//  ライフ   ( life.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include "life.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define LIFE_TEXTURE_NAME000			"data/TEXTURE/UI/life.png"			//  テクスチャ名
#define LIFE_TEXTURE_NAME001			"data/TEXTURE/UI/life2.png"			//  テクスチャ名
#define LIFE_TEXTURE_NAME002			"data/TEXTURE/UI/life1_UI.png"		//  テクスチャ名
#define LIFE_TEXTURE_NAME003			"data/TEXTURE/UI/life2_UI.png"			//  テクスチャ名

#define LIFE000_POS_X					( 450.0f )							//  種類000座標X
#define LIFE000_POS_Y					( 672.0f )							//  種類000座標Y
#define LIFE000_SIZE_X					( 16.0f )							//  種類000サイズX
#define LIFE000_SIZE_Y					( 24.0f )							//  種類000サイズY

#define LIFE001_POS_X					( 1030.0f )							//  種類001座標X
#define LIFE001_POS_Y					( 612.0f )							//  種類001座標Y
#define LIFE001_SIZE_X					( 10.0f )							//  種類001サイズX
#define LIFE001_SIZE_Y					( 15.0f )							//  種類001サイズY

#define LIFE002_POS_X					( 400.0f )							//  種類000座標X
#define LIFE002_POS_Y					( 692.0f )							//  種類000座標Y
#define LIFE002_SIZE_X					( 16.0f )							//  種類000サイズX
#define LIFE002_SIZE_Y					( 24.0f )							//  種類000サイズY

#define LIFE003_POS_X					( 810.0f )							//  種類001座標X
#define LIFE003_POS_Y					( 692.0f )							//  種類001座標Y
#define LIFE003_SIZE_X					( 16.0f )							//  種類001サイズX
#define LIFE003_SIZE_Y					( 24.0f )							//  種類001サイズY

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Life::Life( )
{
	//  情報の代入
	m_position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_color = D3DXCOLOR( 0.0f , 0.9f , 1.0f , 1.0f );
	m_posUV = D3DXVECTOR2( 0.0f , 0.0f );
	m_divideUV = D3DXVECTOR2( 0.0f , 0.0f );
	m_nLife = 0;
	m_nBaseLife = 0;
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのコンストラクタ
//--------------------------------------------------------------------------------------
Life::Life( D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV , D3DXCOLOR color )
{
	//  情報の代入
	m_position = position;
	m_size = size;
	m_color = color;
	m_posUV = posUV;
	m_divideUV = divideUV;
	m_fEndLife = 0.0f;
	m_nLife = 0;
	m_nBaseLife = 0;
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンクラスのデストラクタ
//--------------------------------------------------------------------------------------
Life::~Life( )
{

}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの初期化処理
//--------------------------------------------------------------------------------------
HRESULT Life::Init( void )
{
	//  数字の一時格納
	int nLife = m_nLife;
	int nNumber = 0;

	Scene2DL::Init( );

	//  桁数分のループ
	for( int nCntDigit = 4 - 1; nCntDigit >= 0; nCntDigit-- )
	{
		//  下1桁目を抽出
		nNumber = nLife % 10;
		
		//  下1桁目の削除
		nLife /= 10;

		if( m_type == Life::TYPE_LIFE000 )
		{
			//  数字クラスの生成
			m_pNumber.push_back( Number::Create( D3DXVECTOR3( LIFE000_POS_X + LIFE000_SIZE_X * nCntDigit , LIFE000_POS_Y , 0.0f ) ,
															   D3DXVECTOR3( LIFE000_SIZE_X , LIFE000_SIZE_Y , 0.0f ) ,
															   D3DXVECTOR2( 0.0f , 0.0f ) , D3DXVECTOR2( 5.0f , 2.0f ) , 
															   nNumber ) );
		}
		else if( m_type == Life::TYPE_LIFE001 )
		{
			//  数字クラスの生成
			m_pNumber.push_back( Number::Create( D3DXVECTOR3( LIFE001_POS_X + LIFE001_SIZE_X * nCntDigit , LIFE001_POS_Y , 0.0f ) ,
															   D3DXVECTOR3( LIFE001_SIZE_X , LIFE001_SIZE_Y , 0.0f ) ,
															   D3DXVECTOR2( 0.0f , 0.0f ) , D3DXVECTOR2( 5.0f , 2.0f ) , 
															   nNumber ) );
		}

		else if( m_type == Life::TYPE_LIFE002 )
		{
			//  数字クラスの生成
			m_pNumber.push_back( Number::Create( D3DXVECTOR3( LIFE002_POS_X + LIFE002_SIZE_X * nCntDigit , LIFE002_POS_Y , 0.0f ) ,
															   D3DXVECTOR3( LIFE002_SIZE_X , LIFE002_SIZE_Y , 0.0f ) ,
															   D3DXVECTOR2( 0.0f , 0.0f ) , D3DXVECTOR2( 5.0f , 2.0f ) , 
															   nNumber ) );
		}
		else if( m_type == Life::TYPE_LIFE003 )
		{
			//  数字クラスの生成
			m_pNumber.push_back( Number::Create( D3DXVECTOR3( LIFE003_POS_X + LIFE003_SIZE_X * nCntDigit , LIFE003_POS_Y , 0.0f ) ,
															   D3DXVECTOR3( LIFE003_SIZE_X , LIFE003_SIZE_Y , 0.0f ) ,
															   D3DXVECTOR2( 0.0f , 0.0f ) , D3DXVECTOR2( 5.0f , 2.0f ) , 
															   nNumber ) );
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの終了処理
//--------------------------------------------------------------------------------------
void Life::Uninit( void )
{
	std::list< Number* >::iterator it;

	//  要素の全削除
    for( it = m_pNumber.begin( ); it != m_pNumber.end( ); ) 
	{
		if( ( *it ) == NULL )
		{
			it = m_pNumber.erase( it );

			continue;
		}

		//  次の要素へ
		it++;
	}

	Scene2DL::Uninit( );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの更新処理
//--------------------------------------------------------------------------------------
void Life::Update( void )
{
	//  体力徐々に変更
	m_fEndLife += ( ( float )m_nLife - m_fEndLife ) * 0.1f;

	//  目標の値に近づいた場合
	if( m_fEndLife <= ( float )m_nLife + 1.0f &&
		m_fEndLife >= ( float )m_nLife - 1.0f )
	{
		//  現状の体力を目標の体力に
		m_fEndLife = ( float )m_nLife;
	}

	if( m_fEndLife <= 0.0f )
	{
		m_fEndLife = 0.0f;
	}

	//  X方向の大きさ比率の設定
	m_scale.x = m_fEndLife / ( float )m_nBaseLife;

	m_endUV.x = 1.0f * m_fEndLife / ( float )m_nBaseLife;

	if( m_scale.x <= 0.0f )
	{
		m_scale.x = 0.0f;
	}

	std::list< Number* >::iterator it;

	//  数字の一時格納
	int nLife = ( int )m_fEndLife;
	int nNumber = 0;

	//  体力が0を下回った場合
	if( m_nLife <= 0 )
	{
		//  体力を0に
		m_nLife = 0;
		m_fEndLife = 0.0f;
		nLife = 0;
	}

	//  要素の全更新
    for( it = m_pNumber.begin( ); it != m_pNumber.end( ); ++it ) 
	{
		//  下1桁目を抽出
		nNumber = nLife % 10;
		
		//  下1桁目の削除
		nLife /= 10;
		
		//  値の格納
		( *it )->SetNumber( nNumber );
	}

	if( m_fSwaySide != 0.0f || m_fSwayVertical != 0.0f )
	{
		m_seekPos.x = sinf( m_fSwayAngle ) * m_fSwaySide;
		m_seekPos.y = sinf( m_fSwayAngle ) * m_fSwayVertical;

		m_fSwayAngle += 0.8f;

		if( m_fSwayAngle > D3DX_PI )
		{
			m_fSwayAngle = -D3DX_PI;

			m_fSwaySide *= 0.3f;
			m_fSwayVertical *= 0.3f;

			if( m_fSwaySide < 0.01f )
			{
				m_fSwaySide = 0.0f;
			}
			if( m_fSwayVertical < 0.01f )
			{
				m_fSwayVertical = 0.0f;
			}

			m_nSwayTime = 0;
		}

		m_nSwayTime++;
	}
	else
	{
		m_fSwayAngle = 0.0f;
		m_nSwayTime = 0;
	}

	//  座標をずらす
	m_position = m_basePos + m_seekPos;

	Scene2DL::Update( );
}

//--------------------------------------------------------------------------------------
//  2Dポリゴンの描画処理
//--------------------------------------------------------------------------------------
void Life::Draw( void )
{
	Scene2DL::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
Life* Life::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 size , D3DXVECTOR2 posUV , D3DXVECTOR2 divideUV , int nBaseLife )
{
	Life* pLife;

	//  インスタンス生成
	pLife = new Life;

	//  種類の代入
	pLife->m_type = type;

	//  基準座標の代入
	pLife->m_basePos = position;

	//  座標の代入
	pLife->m_position = position;

	//  大きさの代入
	pLife->m_size = size;

	//  UV座標開始地点の代入
	pLife->m_posUV = posUV;

	//  テクスチャUV分割数の代入
	pLife->m_divideUV = divideUV;

	//  基準体力値の代入
	pLife->m_nBaseLife = nBaseLife;

	//  体力値の代入
	pLife->m_nLife = nBaseLife;

	if( type == Life::TYPE_LIFE000 )
	{
		//  テクスチャ名の設定
		pLife->SetTextureName( LIFE_TEXTURE_NAME000 );
	}
	else if( type == Life::TYPE_LIFE001 )
	{
		//  テクスチャ名の設定
		pLife->SetTextureName( LIFE_TEXTURE_NAME001 );
	}
	else if( type == Life::TYPE_LIFE002 )
	{
		//  テクスチャ名の設定
		pLife->SetTextureName( LIFE_TEXTURE_NAME002 );
	}
	else if( type == Life::TYPE_LIFE003 )
	{
		//  テクスチャ名の設定
		pLife->SetTextureName( LIFE_TEXTURE_NAME003 );
	}

	//  初期化
	pLife->Init( );

	return pLife;
}

//--------------------------------------------------------------------------------------
//  体力の設定をする関数
//--------------------------------------------------------------------------------------
void Life::SetLife( int nLife )
{
	m_nLife = nLife;
}

//--------------------------------------------------------------------------------------
//  目標体力の設定をする関数
//--------------------------------------------------------------------------------------
void Life::SetEndLife( int nLife )
{
	m_fEndLife = ( float )nLife;
}

//--------------------------------------------------------------------------------------
//  横揺れを設定する関数
//--------------------------------------------------------------------------------------
void Life::SetSwaySide( float fSwaySide )
{
	m_fSwaySide = fSwaySide;
}

//--------------------------------------------------------------------------------------
//  縦揺れを設定する関数
//--------------------------------------------------------------------------------------
void Life::SetSwayVertical( float fSwayVertical )
{
	m_fSwayVertical = fSwayVertical;
}