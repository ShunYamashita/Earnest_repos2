//--------------------------------------------------------------------------------------
//  プレイヤー   ( player.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "hitField.h"
#include "utility.h"
#include "game.h"
#include <iostream>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  プレイヤークラスのコンストラクタ
//--------------------------------------------------------------------------------------
HitField::HitField( )
{

}

//--------------------------------------------------------------------------------------
//  プレイヤークラスのデストラクタ
//--------------------------------------------------------------------------------------
HitField::~HitField( )
{

}

//--------------------------------------------------------------------------------------
//  プレイヤーの初期化処理
//--------------------------------------------------------------------------------------
HRESULT HitField::Init( void )
{
	//  シーンモデル
	SceneModel::Init( );

	//  物体の種類の設定
	Scene::SetObjType( Scene::OBJTYPE_HIT_FIELD );

	//////////////////////////////////////////////////////////
	//  頂点座標の取得と、最小、最大XZ座標の取得
	//////////////////////////////////////////////////////////

	//  メッシュの頂点データ取得
	Utility::VERTEX *pVtx = NULL;

	//  最小値、最大値の初期化
	D3DXVECTOR3 min = D3DXVECTOR3( 10000.0f, 10000.0f, 10000.0f );
	D3DXVECTOR3 max = D3DXVECTOR3( -10000.0f , -10000.0f , -10000.0f );

	//  座標格納用
	D3DXVECTOR3 workPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	int nNumVertex = m_pMeshModel->GetNumVertices( );

	//  メッシュから頂点情報を取得
	m_pMeshModel->LockVertexBuffer( 0 , ( void** )&pVtx );

	//  頂点数分のループ
	for( int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++ )
	{
		//  座標を一時格納
		//m_vtxPos.push_back( pVtx->position );
		m_vtxPos[ nCntVtx ] = pVtx->position;

		//  座標を一時格納
		workPos = pVtx->position;

		//  X座標が最小値を下回っている場合
		if( workPos.x < min.x )
		{
			//  X座標の最小値更新
			min.x = workPos.x;
		}

		//  Y座標が最小値を下回っている場合
		if( workPos.y < min.y )
		{
			//  Y座標の最小値更新
			min.y = workPos.y;
		}

		//  Z座標が最小値を下回っている場合
		if( workPos.z < min.z )
		{
			//  Z座標の最小値更新
			min.z = workPos.z;
		}

		//  X座標が最大値を上回っている場合
		if( workPos.x > max.x )
		{
			//  X座標の最大値更新
			max.x = workPos.x;
		}

		//  Y座標が最大値を上回っている場合
		if( workPos.y > max.y )
		{
			//  Y座標の最大値更新
			max.y = workPos.y;
		}

		//  Z座標が最大値を上回っている場合
		if( workPos.z > max.z )
		{
			//  Z座標の最大値更新
			max.z = workPos.z;
		}

		pVtx++;
	}

	//  頂点バッファのアンロック
	m_pMeshModel->UnlockVertexBuffer( );

	//  最小座標の設定
	Game::SetFieldMin( min );

	//  最大座標の設定
	Game::SetFieldMax( max );

	//////////////////////////////////////////////////////////
	//  頂点インデックスの取得
	//////////////////////////////////////////////////////////

	int nNumFace = m_pMeshModel->GetNumFaces( );

	WORD* pIndexBuffer;

	//  インデックスバッファのロック
	m_pMeshModel->LockIndexBuffer( 0 , ( void** )&pIndexBuffer );

	//  頂点数分のループ
	for( int nCntFace = 0; nCntFace < nNumFace; nCntFace++ )
	{
		//  インデックス番号を一時格納
		m_vtxIndex[ nCntFace * 3 + 0 ] = pIndexBuffer[ nCntFace * 3 + 0 ];
		m_vtxIndex[ nCntFace * 3 + 1 ] = pIndexBuffer[ nCntFace * 3 + 1 ];
		m_vtxIndex[ nCntFace * 3 + 2 ] = pIndexBuffer[ nCntFace * 3 + 2 ];
	}

	//  インデックスバッファのアンロック
	m_pMeshModel->UnlockIndexBuffer( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  プレイヤーの終了処理
//--------------------------------------------------------------------------------------
void HitField::Uninit( void )
{
	//  シーンモデル
	SceneModel::Uninit( );
}

//--------------------------------------------------------------------------------------
//  プレイヤーの更新処理
//--------------------------------------------------------------------------------------
void HitField::Update( void )
{
	//  シーンモデル
	SceneModel::Update( );
}

//--------------------------------------------------------------------------------------
//  プレイヤーの描画処理
//--------------------------------------------------------------------------------------
void HitField::Draw( void )
{
	//  シーンモデル
	//SceneModel::Draw( );
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
HitField* HitField::Create( HitField::TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	HitField* pHitField;

	//  インスタンス生成
	pHitField = new HitField;

	//  座標の代入
	pHitField->m_position = position;

	//  回転角の代入
	pHitField->m_rot = rot;

	//  大きさ倍率の代入
	pHitField->m_scale = scale;

	if( type == HitField::TYPE_HIT_STAGE000 )
	{
		//  種類の代入
		pHitField->m_type = SceneModel::TYPE_HIT_FIELD000;
	}
	else if( type == HitField::TYPE_HIT_STAGE001 )
	{
		//  種類の代入
		pHitField->m_type = SceneModel::TYPE_HIT_FIELD001;
	}

	//  初期化
	pHitField->Init( );

	return pHitField;
}

//--------------------------------------------------------------------------------------
//  対象の座標がフィールドのどこのポリゴンにいるか検索してそのY座標を返す関数
//--------------------------------------------------------------------------------------
float HitField::GetHeight( D3DXVECTOR3 position )
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

	int nNumFace = m_pMeshModel->GetNumFaces( );

	const float fRange = 400.0f;

#pragma omp parallel for
	//  頂点数分のループ
	for( int nCntFace = 0; nCntFace < nNumFace; nCntFace++ )
	{
		//if( m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 0 ] ].x <= m_position.x + fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 0 ] ].x >= m_position.x - fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 0 ] ].z <= m_position.z + fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 0 ] ].z >= m_position.z - fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 1 ] ].x <= m_position.x + fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 1 ] ].x >= m_position.x - fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 1 ] ].z <= m_position.z + fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 1 ] ].z >= m_position.z - fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ].x <= m_position.x + fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ].x >= m_position.x - fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ].z <= m_position.z + fRange &&
		//	m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ].z >= m_position.z - fRange )
		{
			vec1 = m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 0 ] ] - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ];
			vec2 = position - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ];

			//  2つのベクトルのなす直交ベクトルを求める
			D3DXVec3Cross( &cross , &vec1 , &vec2 );

			//  単位ベクトル化して法線ベクトルに変換
			D3DXVec3Normalize( &cross , &cross );

			//  法線が上向き方向の場合
			if( cross.y >= 0.0f )
			{
				vec3 = m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 1 ] ] - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 0 ] ];
				vec4 = position - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 0 ] ];

				//  2つのベクトルのなす直交ベクトルを求める
				D3DXVec3Cross( &cross2 , &vec3 , &vec4 );

				//  単位ベクトル化して法線ベクトルに変換
				D3DXVec3Normalize( &cross2 , &cross2 );

				//  法線が上向き方向の場合
				if( cross2.y >= 0.0f )
				{
					vec5 = m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ] - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 1 ] ];
					vec6 = position - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 1 ] ];

					//  2つのベクトルのなす直交ベクトルを求める
					D3DXVec3Cross( &cross3 , &vec5 , &vec6 );

					//  単位ベクトル化して法線ベクトルに変換
					D3DXVec3Normalize( &cross3 , &cross3 );

					//  法線が上向き方向の場合
					if( cross3.y >= 0.0f )
					{
						D3DXVECTOR3 workVec = m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 1 ] ] - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ];

						//  外積計算からそのポリゴンの法線ベクトルを求める
						D3DXVec3Cross( &normal , &vec1 , &workVec );
						D3DXVec3Normalize( &normal , &normal );

						float fHeight = m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ].y -
										( ( position.x - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ].x ) * normal.x +
										( position.z - m_vtxPos[ m_vtxIndex[ nCntFace * 3 + 2 ] ].z ) * normal.z ) / normal.y;

						return fHeight + 1.0f;
					}
				}
			}		
		}
	}

	return -100000.0f;
}