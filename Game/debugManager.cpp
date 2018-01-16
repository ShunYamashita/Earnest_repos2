//--------------------------------------------------------------------------------------
//  デバッグ管理   ( debugManager.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "debugManager.h"
#include "imgui_impl_dx9.h"
#include "manager.h"
#include "renderer.h"
#include "depthShadow.h"
#include "scene.h"
#include "player.h"
#include "light.h"
#include "AIManager.h"
#include "characterAI.h"
#include "rootNodeAI.h"
#include "texture.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  デバッグ管理クラスのコンストラクタ
//--------------------------------------------------------------------------------------
DebugManager::DebugManager( )
{

}

//--------------------------------------------------------------------------------------
//  デバッグ管理クラスのデストラクタ
//--------------------------------------------------------------------------------------
DebugManager::~DebugManager( )
{

}

//--------------------------------------------------------------------------------------
//  デバッグ管理の初期化処理
//--------------------------------------------------------------------------------------
void DebugManager::Init( void )
{
    // Setup ImGui binding
    ImGui_ImplDX9_Init( GetWindow( ) , SceneManager::GetRenderer( )->GetDevice( ) );
}

//--------------------------------------------------------------------------------------
//  デバッグ管理の終了処理
//--------------------------------------------------------------------------------------
void DebugManager::Uninit( void )
{
	//  ImGuiの終了
	ImGui_ImplDX9_Shutdown( );
}

//--------------------------------------------------------------------------------------
//  デバッグ管理の更新処理
//--------------------------------------------------------------------------------------
void DebugManager::Update( void )
{
	ImGui_ImplDX9_NewFrame( );

    static bool show_test_window = true;
	static bool show_AI_window = false;
    static bool show_another_window = false;
    ImVec4 clear_col = ImColor(114, 144, 154);

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
		ImGui::Begin( "Debug Window", &show_test_window );
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Checkbox( "AI Manager On/Off" , &show_AI_window );

		if( ImGui::TreeNode( "Object Manager" ) ) 
		{
			D3DXVECTOR3 posPlayer( 0.0f , 0.0f , 0.0f );			//  プレイヤーの座標
			D3DXVECTOR3 positionAtPlayer( 0.0f , 0.0f , 0.0f );		//  プレイヤーの注視点
			D3DXVECTOR3 vecDirectPlayer( 0.0f , 0.0f , 0.0f );		//  プレイヤーの方向ベクトル
			Scene* pScene = NULL;

			//  優先度の最大数分のループ
			for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
			{
				//  シーンの先頭アドレスを取得
				pScene = Scene::GetScene( nCntPriority );

				//  シーンが空ではない間ループ
				while( pScene != NULL )
				{
					Scene::OBJTYPE objType;						//  物体の種類

					//  物体の種類の取得
					objType = pScene->GetObjType( );

					//  種類がプレイヤーの場合
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  プレイヤー情報の取得
						posPlayer = pScene->GetPos( );

						Player* player = ( Player* )pScene;

						//  プレイヤー情報の取得
						positionAtPlayer = player->GetPositionAt( );

						vecDirectPlayer = player->GetVecDirect( );
					}

					//  次のポインタを代入
					pScene = pScene->GetNextScene( pScene );
				}
			}

			ImGui::Text( "PLAYER : Position( %.2f , %.2f , %.2f )" , posPlayer.x , posPlayer.y , posPlayer.z );
			ImGui::Text( "PLAYER : PositionAt( %.2f , %.2f , %.2f )" , positionAtPlayer.x , positionAtPlayer.y , positionAtPlayer.z );
			ImGui::Text( "PLAYER : VecDirect( %.2f , %.2f , %.2f )" , vecDirectPlayer.x , vecDirectPlayer.y , vecDirectPlayer.z );

			ImGui::TreePop( );
		}

		if( ImGui::TreeNode( "DepthShadow Manager" ) ) 
		{
			Light* light = SceneManager::GetLight( );

			D3DXVECTOR3 lightPosition;
			D3DXVECTOR3 lightPositionAt;
			lightPosition = light->GetPosition( );
			ImGui::InputFloat3( "Position" , lightPosition , 0.1f );
			lightPositionAt = light->GetPositionAt( );
			ImGui::InputFloat3( "PositionAt" , lightPositionAt , 0.1f );
			light->SetViewMatrix( lightPosition , lightPositionAt );

			float fov = light->GetFov( );
			float fNear = light->GetNear( );
			float fFar = light->GetFar( );
			ImGui::InputFloat( "Fov" , &fov , 0.01f );
			ImGui::InputFloat( "Near" , &fNear , 1.0f );
			ImGui::InputFloat( "Far" , &fFar , 1.0f );
			light->SetProjectionMatrix( fov , fNear , fFar );

			float bias = DepthShadow::GetBias( );
			ImGui::InputFloat( "Bias" , &bias , 0.0000001f );
			DepthShadow::SetBias( bias );

			ImGui::Image( ( void* )DepthShadow::GetRendereTargetTexture( ) , 
						  ImVec2( 128 , 128 ) , ImVec2( 0 , 0 ) , ImVec2( 1 , 1 ) , ImVec4( 1 , 1 , 1 , 1 ) , ImVec4( 1 , 1 , 1 , 1 ) ); 

			ImGui::TreePop( );
		}

		ImGui::End( );
    }

	if( show_AI_window )
	{
		//  AI管理クラスのデバッグ描画
		AIManager::DrawDebug( show_AI_window );
	}
}

//--------------------------------------------------------------------------------------
//  デバッグ管理の描画処理
//--------------------------------------------------------------------------------------
void DebugManager::Draw( void )
{
	//  GUIの描画
	ImGui::Render( );
}