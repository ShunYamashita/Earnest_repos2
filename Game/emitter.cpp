// //--------------------------------------------------------------------------------------
////  DirectX   ( bullet.cpp )
////
////  Author : SHUN YAMASHITA
////
////  Update : 2016/06/07
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
////  ヘッダーファイル
////--------------------------------------------------------------------------------------
//#include "main.h"
//#include "emitter.h"
//#include <random>
//
////--------------------------------------------------------------------------------------
////  マクロ定義
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
////  プロトタイプ宣言
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
////  グローバル変数
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
////  エミットの初期化処理をする関数
////--------------------------------------------------------------------------------------
//HRESULT InitEmitter( void )
//{
//	return S_OK;
//}
//
////--------------------------------------------------------------------------------------
////  エミットの終了処理をする関数
////--------------------------------------------------------------------------------------
//void UninitEmitter( void )
//{
//
//}
//
////--------------------------------------------------------------------------------------
////  エミットの更新処理をする関数
////--------------------------------------------------------------------------------------
//void UpdateEmitter( void )
//{
//
//}
//
////--------------------------------------------------------------------------------------
////  エミットの描画処理をする関数
////--------------------------------------------------------------------------------------
//void DrawEmitter( void )
//{
//
//}
//
////--------------------------------------------------------------------------------------
////  粒子をセットする関数
////--------------------------------------------------------------------------------------
//bool EmitParticle( Emitter* pEmitter , Particle* pParticle , Particle::TYPE type , D3DXVECTOR2 size )
//{
//	//  乱数の宣言
//	std::random_device rd;
//
//	//  乱数の設定
//	std::uniform_int_distribution< int > RandomAccel( -100 , 100 );
//	std::uniform_int_distribution< int > RandomPos( 0 , 100 );
//
//	//  エミッターと粒子のどちらかがNULLの場合
//	if( pEmitter == NULL || pParticle == NULL )
//	{
//		return false;
//	}
//
//	//  粒子の数分のループ
//	for( int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++ )
//	{
//		//  粒子が存在していない場合
//		if( pParticle[ nCntParticle ].m_bExist == false )
//		{
//			//  種類の代入
//			pParticle[ nCntParticle ].type = type;
//
//			//  乱数の設定
//			std::uniform_int_distribution< int > RandomRot( 0 , pEmitter->nRangeRot );
//
//			//  共通項目の設定
//			pParticle[ nCntParticle ].nLife = pEmitter->nLife + rand( ) % pEmitter->nLife;
//			pParticle[ nCntParticle ].color = pEmitter->color;
//			pParticle[ nCntParticle ].bExist = true;
//
//			//  斜辺と角度の算出
//			pParticle[ nCntParticle ].fLength = sqrtf( size.x * size.x + size.y * size.y ) * 0.5f;
//			pParticle[ nCntParticle ].fAngle = atan2f( size.y , size.x );
//
//			//  種類によっての場合分け
//			switch( type )
//			{
//				case Particle::TYPE_NORMAL:
//				{
//					pParticle[ nCntParticle ].fRot = pEmitter->fRot;
//					pParticle[ nCntParticle ].velocity = D3DXVECTOR3( cosf( ( RandomRot( rd ) + pEmitter->nSeekRot ) * D3DX_PI / 180.0f ) * pEmitter->fVelocity ,
//																	  cosf( ( RandomRot( rd ) + pEmitter->nSeekRot ) * D3DX_PI / 180.0f ) * pEmitter->fVelocity ,
//																	  cosf( ( RandomRot( rd ) + pEmitter->nSeekRot ) * D3DX_PI / 180.0f ) * pEmitter->fVelocity );
//
//					//  粒子の設定
//					pParticle[ nCntParticle ].position = pEmitter->position;
//					pParticle[ nCntParticle ].accel.x = pEmitter->accel.x;
//					pParticle[ nCntParticle ].accel.y = pEmitter->accel.y;
//					pParticle[ nCntParticle ].accel.z = pEmitter->accel.z;
//
//					pParticle[ nCntParticle ].nDivideU = 1;
//					pParticle[ nCntParticle ].nDivideV = 1;
//
//					break;
//				}
//				case Particle::TYPE_LINE:
//				{
//					pParticle[ nCntParticle ].fRot = pEmitter->fRot;
//					pParticle[ nCntParticle ].fVelocity = pEmitter->fVelocity;
//					pParticle[ nCntParticle ].vecDirect = pEmitter->vecDirect;
//
//					//  粒子の設定
//					pParticle[ nCntParticle ].position = pEmitter->position;
//					pParticle[ nCntParticle ].accel.x = pEmitter->accel.x;
//					pParticle[ nCntParticle ].accel.y = pEmitter->accel.y;
//					pParticle[ nCntParticle ].accel.z = pEmitter->accel.z;
//
//					pParticle[ nCntParticle ].nDivideU = 1;
//					pParticle[ nCntParticle ].nDivideV = 1;
//
//					break;
//				}
//			}
//
//			return true;
//		}
//	}
//
//	return false;
//}