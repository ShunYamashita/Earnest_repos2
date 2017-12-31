//--------------------------------------------------------------------------------------
//  モデルレンダリング処理   ( sceneModel.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "sceneModelAnim.h"
#include "sceneModelParts.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "animation.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define	MODEL_FILENAME000		"data/MODEL/robot.x"
#define	MODEL_FILENAME001		"data/MODEL/goal.x"

#define TEXTURE_FILEPATH		"data\\TEXTURE"					//  テクスチャへのファイルパス

#define CHANGE_NEUTRAL_TIME		( 8 )							//  ニュートラル状態への移行フレーム

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  xモデルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneModelAnim::SceneModelAnim( int nPriority ) : Scene( nPriority )
{
	m_posAt = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_scale = D3DXVECTOR3( 1.0f, 1.0f , 1.0f );
	m_rot = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  xモデルクラスのコンストラクタ
//--------------------------------------------------------------------------------------
SceneModelAnim::SceneModelAnim( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	m_position = position;
	m_rot = rot;
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  xモデルのデストラクタ
//--------------------------------------------------------------------------------------
SceneModelAnim::~SceneModelAnim( )
{

}

//--------------------------------------------------------------------------------------
//  xモデルの初期化処理
//--------------------------------------------------------------------------------------
HRESULT SceneModelAnim::Init( void )
{
	D3DXMATERIAL*	pMat = NULL;									//  マテリアル
	FILE*			pFile = NULL;
	char			aModelName[ MAX_MODEL_PARTS ][ 128 ];
	char			aTextureFileName[ 128 ] = { };
	char			aWork[ 128 ];
	int				nIndex = 0;
	int				nNumModel;

	char aTextName[ TYPE_MAX ][ 256 ] = {
											"data/MODEL/THIEF/motion.txt" ,
											"data/MODEL/ROBOT/motion.txt" ,
										};

	D3DXMATRIX		mtxScale;						//  拡大行列
	D3DXMATRIX		mtxRot;							//  回転行列
	D3DXMATRIX		mtxTrans;						//  平行移動行列

	D3DXVECTOR3		position;

	//  メインからデバイス情報を取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ファイルの読み込み
	pFile = fopen( aTextName[ m_type ] , "r+" );

	if( pFile == NULL )
	{
		//  エラーだった場合
		MessageBox( NULL , "ファイルが読み込めませんでした。" , "エラーメッセージ" , MB_OK | MB_ICONWARNING );

		return E_FAIL;
	}
	else
	{
		// 文字検索
		while( 1 )
		{
			//  ファイルから文字を格納
			fscanf( pFile , "%s" , &aWork );

			//  文字が一致した場合
			if( strcmp( aWork , "=" ) == 0 )
			{
				break;
			}
		}

		//  ファイルから値を格納
		fscanf( pFile , "%d" , &nNumModel );

#pragma omp parallel for
		//  モデルの数分のループ
		for( int nCntModelChar = 0; nCntModelChar < nNumModel; nCntModelChar++ )
		{
			// 文字検索
			while( 1 )
			{
				//  ファイルから文字を格納
				fscanf( pFile , "%s" , &aWork );

				//  文字が一致した場合
				if( strcmp( aWork , "=" ) == 0 )
				{
					break;
				}
			}

			//  ファイルから文字を格納
			fscanf( pFile , "%s" , &aModelName[ nCntModelChar ] );
		}

		// 文字検索
		while( 1 )
		{
			//  ファイルから文字を格納
			fscanf( pFile , "%s" , &aWork );

			//  文字が一致した場合
			if( strcmp( aWork , "=" ) == 0 )
			{
				break;
			}
		}

		//  ファイルから速度を格納
		fscanf( pFile , "%f" , &m_fVelocity );

		// 文字検索
		while( 1 )
		{
			//  ファイルから文字を格納
			fscanf( pFile , "%s" , &aWork );

			//  文字が一致した場合
			if( strcmp( aWork , "=" ) == 0 )
			{
				break;
			}
		}

		//  ファイルからジャンプ力を格納
		fscanf( pFile , "%f" , &m_fJumpPower );

		// 文字検索
		while( 1 )
		{
			//  ファイルから文字を格納
			fscanf( pFile , "%s" , &aWork );

			//  文字が一致した場合
			if( strcmp( aWork , "=" ) == 0 )
			{
				break;
			}
		}

		//  ファイルからパーツ数を格納
		fscanf( pFile , "%d" , &m_nNumParts );

		//  モデルの数分のループ
		for( int nCntModelChar = 0; nCntModelChar < m_nNumParts; nCntModelChar++ )
		{
			/*  モデルのインデックス、親、座標、回転、拡大の初期化 */

			// 文字検索
			while( 1 )
			{
				//  ファイルから文字を格納
				fscanf( pFile , "%s" , &aWork );

				//  文字が一致した場合
				if( strcmp( aWork , "=" ) == 0 )
				{
					break;
				}
			}

			int			nWorkIndex = 0;
			int			nWorkParent = -1;
			D3DXVECTOR3 workPos( 0.0f , 0.0f , 0.0f );
			D3DXVECTOR3 workRot( 0.0f , 0.0f , 0.0f );
			D3DXVECTOR3 workScale( 1.0f , 1.0f , 1.0f );

			//  ファイルからインデックス番号を格納
			fscanf( pFile , "%d" , &nWorkIndex );

			// 文字検索
			while( 1 )
			{
				//  ファイルから文字を格納
				fscanf( pFile , "%s" , &aWork );

				//  文字が一致した場合
				if( strcmp( aWork , "=" ) == 0 )
				{
					break;
				}
			}

			//  ファイルから親番号を格納
			fscanf( pFile , "%d" , &nWorkParent );

			// 文字検索
			while( 1 )
			{
				//  ファイルから文字を格納
				fscanf( pFile , "%s" , &aWork );

				//  文字が一致した場合
				if( strcmp( aWork , "=" ) == 0 )
				{
					break;
				}
			}

			//  ファイルから座標を格納
			fscanf( pFile , "%f" , &workPos.x );
			fscanf( pFile , "%f" , &workPos.y );
			fscanf( pFile , "%f" , &workPos.z );

			// 文字検索
			while( 1 )
			{
				//  ファイルから文字を格納
				fscanf( pFile , "%s" , &aWork );

				//  文字が一致した場合
				if( strcmp( aWork , "=" ) == 0 )
				{
					break;
				}
			}

			//  ファイルから回転を格納
			fscanf( pFile , "%f" , &workRot.x );
			fscanf( pFile , "%f" , &workRot.y );
			fscanf( pFile , "%f" , &workRot.z );
	
			//  スケールサイズの代入
			workScale.x = m_scale.x;
			workScale.y = m_scale.y;
			workScale.z = m_scale.z;

			m_pModelParts[ nCntModelChar ] = SceneModelParts::Create( nWorkIndex ,
																	  nWorkParent ,
																	  aModelName[ nCntModelChar ] ,
																	  workPos ,
																	  workRot ,
																	  workScale );
		}

		for( int nCntAnimation = 0; nCntAnimation < MAX_ANIMATION; nCntAnimation++ )
		{
			int nWorkLoop = 0;
			int nWorkNumKey = 0;

			// 文字検索
			while( 1 )
			{
				//  ファイルから文字を格納
				fscanf( pFile , "%s" , &aWork );

				//  文字が一致した場合
				if( strcmp( aWork , "=" ) == 0 )
				{
					break;
				}
			}

			//  ファイルからループ数を格納
			fscanf( pFile , "%d" , &nWorkLoop );

			// 文字検索
			while( 1 )
			{
				//  ファイルから文字を格納
				fscanf( pFile , "%s" , &aWork );

				//  文字が一致した場合
				if( strcmp( aWork , "=" ) == 0 )
				{
					break;
				}
			}

			//  ファイルからループ数を格納
			fscanf( pFile , "%d" , &nWorkNumKey );

			m_pAnimation[ nCntAnimation ] = Animation::Create( nWorkNumKey , nWorkLoop );

#pragma omp parallel for
			//  
			for( int nCntKey = 0; nCntKey < nWorkNumKey; nCntKey++ )
			{
				Animation::KEY_FRAME workKeyFrame;

				// 文字検索
				while( 1 )
				{
					//  ファイルから文字を格納
					fscanf( pFile , "%s" , &aWork );

					//  文字が一致した場合
					if( strcmp( aWork , "=" ) == 0 )
					{
						break;
					}
				}

				//  ファイルからループ数を格納
				fscanf( pFile , "%d" , &workKeyFrame.nFrame );

				for( int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++ )
				{
					// 文字検索
					while( 1 )
					{
						//  ファイルから文字を格納
						fscanf( pFile , "%s" , &aWork );

						//  文字が一致した場合
						if( strcmp( aWork , "=" ) == 0 )
						{
							break;
						}
					}

					//  ファイルからループ数を格納
					fscanf( pFile , "%f" , &workKeyFrame.position[ nCntParts ].x );
					fscanf( pFile , "%f" , &workKeyFrame.position[ nCntParts ].y );
					fscanf( pFile , "%f" , &workKeyFrame.position[ nCntParts ].z );

					// 文字検索
					while( 1 )
					{
						//  ファイルから文字を格納
						fscanf( pFile , "%s" , &aWork );

						//  文字が一致した場合
						if( strcmp( aWork , "=" ) == 0 )
						{
							break;
						}
					}

					//  ファイルからループ数を格納
					fscanf( pFile , "%f" , &workKeyFrame.rot[ nCntParts ].x );
					fscanf( pFile , "%f" , &workKeyFrame.rot[ nCntParts ].y );
					fscanf( pFile , "%f" , &workKeyFrame.rot[ nCntParts ].z );
				}

				// キーフレーム情報の代入
				m_pAnimation[ nCntAnimation ]->SetKeyFrame( nCntKey , workKeyFrame );
			}

			//  ファイルから文字を格納
			fscanf( pFile , "%s" , &aWork );
			fscanf( pFile , "%s" , &aWork );
			fscanf( pFile , "%s" , &aWork );
			fscanf( pFile , "%s" , &aWork );

			//  文字が一致した場合
			if( strcmp( aWork , "END_SCRIPT" ) == 0 )
			{
				break;
			}
		}

		//  ファイルを閉じる
		fclose( pFile );
	}

	m_motion = StateAnimator::MOTION::MOTION_NEUTRAL;
	m_nNextAnimation = 0;
	m_nKey = 0;
	m_fFrame = 0.0f;

	//  最初の移行フレームを代入
	m_nShiftFrame = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).nFrame;

#pragma omp parallel for
	//  パーツ数分のループ
	for( int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++ )
	{
		//  現在の角度の代入
		D3DXVECTOR3 workRot = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ];

		//  回転角の設定
		m_pModelParts[ nCntParts ]->SetRot( workRot );
	}

	//  アニメーションする状態か
	m_bAnimation = true;

	//  アニメーションが終了しているかのフラグ
	m_bAnimationFinish = false;

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  xモデルの終了処理
//--------------------------------------------------------------------------------------
void SceneModelAnim::Uninit( void )
{
#pragma omp parallel for
	//  パーツ数分のループ
	for( int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++ )
	{
		if( m_pModelParts[ nCntParts ] != NULL )
		{
			m_pModelParts[ nCntParts ]->Uninit( );
			delete m_pModelParts[ nCntParts ];
			m_pModelParts[ nCntParts ] = NULL;
		}
	}

#pragma omp parallel for
	//  アニメーション数分のループ
	for( int nCntAnimation = 0; nCntAnimation < MAX_ANIMATION; nCntAnimation++ )
	{
		if( m_pAnimation[ nCntAnimation ] != NULL )
		{
			delete m_pAnimation[ nCntAnimation ];
			m_pAnimation[ nCntAnimation ] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------
//  xモデルの更新処理
//--------------------------------------------------------------------------------------
void SceneModelAnim::Update( void )
{
	D3DXMATRIX		mtxWorld;								//  ワールド行列
	D3DXVECTOR3		workRot( 0.0f, 0.0f , 0.0f );			//  回転角格納用
	D3DXVECTOR3		workSeekPos( 0.0f, 0.0f , 0.0f );		//  ずらす座標量格納用
	int				nCntAxis;								//  XYZ軸分のカウント用					

	// アニメーション移行フレームから現在のフレームを割って割合を求める
	float dt = ( ( float )m_fFrame / ( float )m_nShiftFrame );
	float dg;
	float dp;

	//  アニメーションをする状態の場合
	if( m_bAnimation == true )
	{
#pragma omp parallel for
		//  パーツ数分のループ
		for( int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++ )
		{
			//  現在の角度の代入
			workRot = m_pModelParts[ nCntParts ]->GetRot( );

			//  XYZ軸分のループ
			for( nCntAxis = 0; nCntAxis < 3; nCntAxis++ )
			{
				//  ループするものでかつ最終キーにいった場合
				if( m_pAnimation[ m_motion ]->GetLoop( ) == 1 && m_nKey == m_pAnimation[ m_motion ]->GetNumKey( ) - 1 )
				{
					//  座標の線形補間
					dp = ( float )( m_pAnimation[ m_motion ]->GetKeyFrame( 0 ).position[ nCntParts ][ nCntAxis ] - m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).position[ nCntParts ][ nCntAxis ] );
					workSeekPos[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).position[ nCntParts ][ nCntAxis ] + ( dp * dt );

					if( fabsf( m_pAnimation[ m_motion ]->GetKeyFrame( 0 ).rot[ nCntParts ][ nCntAxis ] - m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] ) > D3DX_PI )
					{
						//  角度の線形補間
						dg = ( float )( m_pAnimation[ m_motion ]->GetKeyFrame( 0 ).rot[ nCntParts ][ nCntAxis ] - m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] );
						workRot[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] - ( dg * dt );
					}
					else
					{
						//  角度の線形補間
						dg = ( float )( m_pAnimation[ m_motion ]->GetKeyFrame( 0 ).rot[ nCntParts ][ nCntAxis ] - m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] );
						workRot[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] + ( dg * dt );
					}
				}
				else if( m_pAnimation[ m_motion ]->GetLoop( ) == 0 && m_nKey == m_pAnimation[ m_motion ]->GetNumKey( ) - 1 )
				{
					//  座標の線形補間
					dp = ( float )( m_pAnimation[ 0 ]->GetKeyFrame( 0 ).position[ nCntParts ][ nCntAxis ] - m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).position[ nCntParts ][ nCntAxis ] );
					workSeekPos[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).position[ nCntParts ][ nCntAxis ] + ( dp * dt );

					if( fabsf( m_pAnimation[ 0 ]->GetKeyFrame( 0 ).rot[ nCntParts ][ nCntAxis ] - m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] ) > D3DX_PI )
					{
						//  角度の線形補間
						dg = ( float )( m_pAnimation[ 0 ]->GetKeyFrame( 0 ).rot[ nCntParts ][ nCntAxis ] - m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] );
						workRot[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] - ( dg * dt );
					}
					else
					{
						//  角度の線形補間
						dg = ( float )( m_pAnimation[ 0 ]->GetKeyFrame( 0 ).rot[ nCntParts ][ nCntAxis ] - m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] );
						workRot[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] + ( dg * dt );
					}
				}
				else 
				{
					//  座標の線形補間
					dp = ( float )( m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey + 1 ).position[ nCntParts ][ nCntAxis ] -
									m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).position[ nCntParts ][ nCntAxis ] );
					workSeekPos[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).position[ nCntParts ][ nCntAxis ] + ( dp * dt );

					if( fabsf( m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey + 1 ).rot[ nCntParts ][ nCntAxis ] - 
							   m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] ) > D3DX_PI )
					{
						//  角度の線形補間
						dg = ( float )( m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey + 1 ).rot[ nCntParts ][ nCntAxis ] -
										m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] );
						workRot[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] - ( dg * dt );
					}
					else
					{
						//  角度の線形補間
						dg = ( float )( m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey + 1 ).rot[ nCntParts ][ nCntAxis ] -
										m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] );
						workRot[ nCntAxis ] = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).rot[ nCntParts ][ nCntAxis ] + ( dg * dt );
					}
				}
			}

			//  ずらす座標の代入
			m_pModelParts[ nCntParts ]->SetSeekPos( workSeekPos );

			//  回転角の設定
			m_pModelParts[ nCntParts ]->SetRot( workRot );
		}

		if( m_bSlow == false )
		{
			//  フレームカウント
			m_fFrame += 1.0f;
		}
		else
		{
			//  フレームカウント
			m_fFrame += 1.0f;
		}

		//  最終フレームになった場合
		if( m_fFrame >= m_nShiftFrame )
		{
			//  フレーム数の初期化
			m_fFrame = 0.0f;

			//  次のキーへ
			m_nKey++;

			//  次の移行フレームの代入
			m_nShiftFrame = m_pAnimation[ m_motion ]->GetKeyFrame( m_nKey ).nFrame;

			//  最後のキーにいった場合
			if( m_nKey == m_pAnimation[ m_motion ]->GetNumKey( ) )
			{
				//  アニメーションがループしない場合
				if( m_pAnimation[ m_motion ]->GetLoop( ) == 0 )
				{
					m_bAnimationFinish = true;

					m_bAnimation = false;
				}
				else
				{
					//  キーの初期化
					m_nKey = 0;

					//  次の移行フレームの代入
					m_nShiftFrame = m_pAnimation[ m_motion ]->GetKeyFrame( 0 ).nFrame;
				}
			}
		}
	}

#pragma omp parallel for
	//  パーツ数分のループ
	for( int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++ )
	{
		if( m_pModelParts[ nCntParts ] != NULL )
		{
			int nParent = m_pModelParts[ nCntParts ]->GetParent( );

			if( nParent == -1 )
			{
				D3DXMatrixIdentity( &mtxWorld );

				//  モデルのワールド行列を掛け合わせる
				m_pModelParts[ nCntParts ]->Update( mtxWorld );
			}
			else
			{
				//  モデルのワールド行列を掛け合わせる
				m_pModelParts[ nCntParts ]->Update( m_pModelParts[ nParent ]->GetMtxWorld( ) );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  xモデルの描画処理
//--------------------------------------------------------------------------------------
void SceneModelAnim::Draw( void )
{
#pragma omp parallel for
	//  パーツ数分のループ
	for( int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++ )
	{
		if( m_pModelParts[ nCntParts ] != NULL )
		{
			m_pModelParts[ nCntParts ]->Draw( m_position , m_posAt , m_color );
		}
	}
}

//--------------------------------------------------------------------------------------
//  xモデルのデプス値の書き込み処理
//--------------------------------------------------------------------------------------
void SceneModelAnim::DrawDepth( void )
{
#pragma omp parallel for
	//  パーツ数分のループ
	for( int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++ )
	{
		if( m_pModelParts[ nCntParts ] != NULL )
		{
			m_pModelParts[ nCntParts ]->DrawDepth( m_position , m_posAt , m_color );
		}
	}
}

//--------------------------------------------------------------------------------------
//  xモデルの大きさ設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelAnim::SetScale( D3DXVECTOR3 scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  xポリゴンの大きさを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 SceneModelAnim::GetScale( void )
{
	return m_scale;
}

//--------------------------------------------------------------------------------------
//  xモデルの注視点設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelAnim::SetPositionAt( D3DXVECTOR3 posAt )
{
	m_posAt = posAt;
}

//--------------------------------------------------------------------------------------
//  xモデルのアニメーションが終了かしたかを確認する関数
//--------------------------------------------------------------------------------------
bool SceneModelAnim::GetAnimationFinish( void )
{
	return m_bAnimationFinish;
}

//--------------------------------------------------------------------------------------
//  xモデルのアニメーションの設定をする関数
//--------------------------------------------------------------------------------------
void SceneModelAnim::SetAnimation( StateAnimator::MOTION motion )
{
	if( !( m_motion == motion && !m_pAnimation[ motion ]->GetLoop( ) ) )
	{
		m_motion = motion;

		m_nKey = 0;
		m_fFrame = 0.0f;

		m_bAnimationFinish = false;

		m_bAnimation = true;
	}
	else
	{
		m_nKey = 0;
		m_fFrame = 0.0f;

		m_bAnimationFinish = false;

		m_bAnimation = true;
	}

	//  次の移行フレームの代入
	m_nShiftFrame = m_pAnimation[ m_motion ]->GetKeyFrame( 0 ).nFrame;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
SceneModelAnim* SceneModelAnim::Create( TYPE type ,  D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	SceneModelAnim *pSceneModel;

	//  インスタンス生成
	pSceneModel = new SceneModelAnim;

	//  種類の代入
	pSceneModel->m_type = type;

	//  座標の代入
	pSceneModel->m_position = position;

	//  回転角の代入
	pSceneModel->m_rot = rot;

	//  大きさ倍率の代入
	pSceneModel->m_scale = scale;

	//  初期化
	pSceneModel->Init( );

	return pSceneModel;
}

//--------------------------------------------------------------------------------------
//  モデルパーツの座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 SceneModelAnim::GetModelPartsPos( int nIndex )
{
	if( m_pModelParts[ nIndex ] != NULL )
	{
		D3DXMATRIX mtxWorld = m_pModelParts[ nIndex ]->GetMtxWorld( );

		D3DXVECTOR3 resultPos( mtxWorld._41 , mtxWorld._42 , mtxWorld._43 );

		return resultPos;
	}
	
	return D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  受ける側の球体当たり判定を取得する関数
//--------------------------------------------------------------------------------------
Utility::HIT_SPHERE	SceneModelAnim::GetHitSphere( void )
{
	return m_hitSphere;
}