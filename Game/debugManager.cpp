//--------------------------------------------------------------------------------------
//  �f�o�b�O�Ǘ�   ( debugManager.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
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
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �C���X�^���X����
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �f�o�b�O�Ǘ��N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
DebugManager::DebugManager( )
{

}

//--------------------------------------------------------------------------------------
//  �f�o�b�O�Ǘ��N���X�̃f�X�g���N�^
//--------------------------------------------------------------------------------------
DebugManager::~DebugManager( )
{

}

//--------------------------------------------------------------------------------------
//  �f�o�b�O�Ǘ��̏���������
//--------------------------------------------------------------------------------------
void DebugManager::Init( void )
{
    // Setup ImGui binding
    ImGui_ImplDX9_Init( GetWindow( ) , SceneManager::GetRenderer( )->GetDevice( ) );
}

//--------------------------------------------------------------------------------------
//  �f�o�b�O�Ǘ��̏I������
//--------------------------------------------------------------------------------------
void DebugManager::Uninit( void )
{
	//  ImGui�̏I��
	ImGui_ImplDX9_Shutdown( );
}

//--------------------------------------------------------------------------------------
//  �f�o�b�O�Ǘ��̍X�V����
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
			D3DXVECTOR3 posPlayer( 0.0f , 0.0f , 0.0f );			//  �v���C���[�̍��W
			D3DXVECTOR3 positionAtPlayer( 0.0f , 0.0f , 0.0f );		//  �v���C���[�̒����_
			D3DXVECTOR3 vecDirectPlayer( 0.0f , 0.0f , 0.0f );		//  �v���C���[�̕����x�N�g��
			Scene* pScene = NULL;

			//  �D��x�̍ő吔���̃��[�v
			for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
			{
				//  �V�[���̐擪�A�h���X���擾
				pScene = Scene::GetScene( nCntPriority );

				//  �V�[������ł͂Ȃ��ԃ��[�v
				while( pScene != NULL )
				{
					Scene::OBJTYPE objType;						//  ���̂̎��

					//  ���̂̎�ނ̎擾
					objType = pScene->GetObjType( );

					//  ��ނ��v���C���[�̏ꍇ
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  �v���C���[���̎擾
						posPlayer = pScene->GetPos( );

						Player* player = ( Player* )pScene;

						//  �v���C���[���̎擾
						positionAtPlayer = player->GetPositionAt( );

						vecDirectPlayer = player->GetVecDirect( );
					}

					//  ���̃|�C���^����
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
		//  AI�Ǘ��N���X�̃f�o�b�O�`��
		AIManager::DrawDebug( show_AI_window );
	}
}

//--------------------------------------------------------------------------------------
//  �f�o�b�O�Ǘ��̕`�揈��
//--------------------------------------------------------------------------------------
void DebugManager::Draw( void )
{
	//  GUI�̕`��
	ImGui::Render( );
}