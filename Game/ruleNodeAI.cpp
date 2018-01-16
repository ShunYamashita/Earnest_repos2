//--------------------------------------------------------------------------------------
//  ���[���m�[�hAI�N���X   ( ruleNodeAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "ruleNodeAI.h"
#include "layerAI.h"
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ���s����
//--------------------------------------------------------------------------------------
void RuleNodeAI::Run( void )
{

} 

//--------------------------------------------------------------------------------------
//  �f�o�b�N�`��
//--------------------------------------------------------------------------------------
void RuleNodeAI::DrawDebug( void )
{
	std::string work = m_name + "RuleNode";

	if( ImGui::TreeNode( work.c_str( ) ) ) 
	{
		//  ���O�̎擾
		ImGui::InputText( "Name" , m_addLayerName , sizeof( m_addLayerName ) );

		if( m_childLayer == nullptr )
		{
			//  Layer�̒ǉ�
			if( ImGui::Button( "AddLayer" ) ) 
			{
				std::string work = m_addLayerName;
				AddChildLayer( CharacterAI::LAYER_TYPE::PRIORITY , work , m_characterAI , this );
				strcpy( m_addLayerName , "" );
			}
		}

		//  ���g�̍폜
		if( ImGui::Button( "Delete" ) ) 
		{
			Release( );

			//  �e���C���[�ɍ폜��ʒm
			if( m_parentLayer != nullptr )
			{
				m_parentLayer->DeleteChildNode( m_name );
			}
		}

		if( m_childLayer != nullptr )
		{
			m_childLayer->DrawDebug( );
		}

		ImGui::TreePop( );
	}
}