//--------------------------------------------------------------------------------------
//  シーンクラス   ( scene.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "scene.h"
#include "effekseerManager.h"

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
Scene*	Scene::m_pTop[ MAX_PRIORITY ] = { NULL };
int		Scene::m_nNumScene;

//--------------------------------------------------------------------------------------
//  シーンのコンストラクタ
//--------------------------------------------------------------------------------------
Scene::Scene( int nPriority )
{
	//  最初の1回目の場合
	if( NULL == m_pTop[ nPriority ] )
	{
		m_pTop[ nPriority ] = this;
	}
	else
	{
		//  先頭ポインタの代入
		Scene* pScene = m_pTop[ nPriority ];

		//  終端ポインタの検索
		while( NULL != pScene->m_pNext )
		{
			//  次のポインタを代入
			pScene = pScene->m_pNext;
		}

		//  自分自身のポインタを次のポインタへ
		pScene->m_pNext = this;
	}

	//  優先順位の代入
	m_nPriority = nPriority;

	//  メンバ変数の初期化
	m_position = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
	m_color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	m_objType = OBJTYPE_NONE;
	m_bDelete = false;
	m_drawDepth = false;
}

//--------------------------------------------------------------------------------------
//  シーンのデストラクタ
//--------------------------------------------------------------------------------------
Scene::~Scene( )
{

}

//--------------------------------------------------------------------------------------
//  全てのシーンの更新処理
//--------------------------------------------------------------------------------------
void Scene::UpdateAll( void )
{
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		if( m_pTop[ nCntPriority ] != NULL )
		{
			//  先頭ポインタの代入
			Scene* pScene = m_pTop[ nCntPriority ];
			Scene* pSceneNext = NULL;
			Scene* pScenePrev = m_pTop[ nCntPriority ];

			//  シーンが空ではない間ループ
			while( pScene != NULL )
			{
				//  シーンの更新
				pScene->Update( );

				//  次のポインタを代入
				pScene = pScene->m_pNext;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  全てのシーンの更新処理
//--------------------------------------------------------------------------------------
void Scene::UpdateAllDelete( void )
{
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		if( m_pTop[ nCntPriority ] != NULL )
		{
			//  先頭ポインタの代入
			Scene* pScene = m_pTop[ nCntPriority ];
			Scene* pSceneNext = NULL;
			Scene* pScenePrev = m_pTop[ nCntPriority ];

			//  シーンが空ではない場合ループ
			while( pScene != NULL )
			{
				pSceneNext = pScene->m_pNext;

				//  削除フラグが立っている場合
				if( pScene->m_bDelete == true )
				{
					//  先頭アドレスと等しい場合
					if( pScene == m_pTop[ nCntPriority ] )
					{
						m_pTop[ nCntPriority ] = pScene->m_pNext;
					}
					else
					{
						pScenePrev->m_pNext = pScene->m_pNext;
					}

					//  シーンの終了処理
					pScene->Uninit( );
					delete pScene;
				}
				else
				{
					//  現在のシーンを前回のシーンに代入
					pScenePrev = pScene;
				}

				//  次のシーンを現在のシーンに代入
				pScene = pSceneNext;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  全てのシーンの描画処理
//--------------------------------------------------------------------------------------
void Scene::DrawAll( void )
{
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		if( m_pTop[ nCntPriority ] != NULL )
		{
			//  先頭ポインタの代入
			Scene* pScene = m_pTop[ nCntPriority ];

			//  シーンが空ではない間ループ
			while( pScene != NULL )
			{
				//  シーンの描画
				pScene->Draw( );

				//  次のポインタを代入
				pScene = pScene->m_pNext;
			}
		}

		if( nCntPriority == MAX_NUM_PRIORITY - 2 )
		{
			EffekseerManager::Draw( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  全てのデプス値書き込み処理
//--------------------------------------------------------------------------------------
void Scene::DrawDepthAll( void )
{
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY - 1; nCntPriority++ )
	{
		if( m_pTop[ nCntPriority ] != NULL )
		{
			//  先頭ポインタの代入
			Scene* pScene = m_pTop[ nCntPriority ];

			//  シーンが空ではない間ループ
			while( pScene != NULL )
			{
				if( pScene->m_drawDepth == true )
				{
					//  シーンの描画
					pScene->DrawDepth( );
				}

				//  次のポインタを代入
				pScene = pScene->m_pNext;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  全てのシーンの解放処理
//--------------------------------------------------------------------------------------
void Scene::ReleaseAll( void )
{
	//  優先度の最大数分のループ
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		if( m_pTop[ nCntPriority ] != NULL )
		{
			//  先頭ポインタの代入
			Scene* pScene = m_pTop[ nCntPriority ];
			Scene* pNextScene = pScene->m_pNext;

			while( pScene != NULL )
			{
				pNextScene = pScene->m_pNext;
				pScene->Uninit( );
				delete pScene;
				pScene = pNextScene;
			}

			//  先頭アドレスの初期化
			m_pTop[ nCntPriority ] = NULL;
		}
	}
}

//--------------------------------------------------------------------------------------
//  シーンの解放処理
//--------------------------------------------------------------------------------------
void Scene::Release( void )
{
	m_bDelete = true;
}

//--------------------------------------------------------------------------------------
//  シーン情報の次のアドレスの取得
//--------------------------------------------------------------------------------------
Scene* Scene::GetNextScene( Scene* pScene )
{
	return pScene->m_pNext;
}

//--------------------------------------------------------------------------------------
//  シーン情報先頭アドレスの取得
//--------------------------------------------------------------------------------------
Scene* Scene::GetScene( int nCntPriority )
{
	return m_pTop[ nCntPriority ];
}

//--------------------------------------------------------------------------------------
//  シーンの種類の設定
//--------------------------------------------------------------------------------------
void Scene::SetObjType( OBJTYPE objType )
{
	m_objType = objType;
}

//--------------------------------------------------------------------------------------
//  シーンの種類の取得
//--------------------------------------------------------------------------------------
Scene::OBJTYPE Scene::GetObjType( void )
{
	return m_objType;
}

//--------------------------------------------------------------------------------------
//  シーンの座標設定
//--------------------------------------------------------------------------------------
void Scene::SetPosition( D3DXVECTOR3 position )
{
	m_position = position;
}

//--------------------------------------------------------------------------------------
//  シーンの座標を移動させる関数
//--------------------------------------------------------------------------------------
void Scene::MovePos( D3DXVECTOR3 velocity )
{
	m_position += velocity;
}

//--------------------------------------------------------------------------------------
//  シーンの座標を取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Scene::GetPos( void )
{
	return m_position;
}

//--------------------------------------------------------------------------------------
//  ポリゴンの大きさ設定
//--------------------------------------------------------------------------------------
void Scene::SetSize( D3DXVECTOR3 size )
{
	m_size = size;
}

//--------------------------------------------------------------------------------------
//  ポリゴンの大きさを取得する関数
//--------------------------------------------------------------------------------------
D3DXVECTOR3 Scene::GetSize( void )
{
	return m_size;
}

//--------------------------------------------------------------------------------------
//  色を設定する関数
//--------------------------------------------------------------------------------------
void Scene::SetColor( D3DXCOLOR color )
{
	m_color = color;
}