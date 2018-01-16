//--------------------------------------------------------------------------------------
//  ���C���[AI���N���X   ( layerAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "layerAI.h"
#include "nodeAI.h"
#include "actionNodeAI.h"
#include "enemy.h"
#include "stateAnimator.h"
#include "imgui_impl_dx9.h"
#include <algorithm>

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �S�Ẵm�[�h�̃f�[�^�̔j��
//--------------------------------------------------------------------------------------
void LayerAI::Release( void )
{
	std::for_each( m_childNode.begin( ) , m_childNode.end( ) ,
				   [ ]( NodeAI* nodeAI )
				   {
						nodeAI->Release( );
						nodeAI = nullptr;
				   } );

	m_childNode.clear( );
}

//--------------------------------------------------------------------------------------
//  �S�Ẵm�[�h�̎��s�\��Ԃ̔��f
//--------------------------------------------------------------------------------------
void LayerAI::AllJudgeActive( void )
{
	std::for_each( m_childNode.begin( ) , m_childNode.end( ) ,
				   [ ]( NodeAI* nodeAI )
				   {
						nodeAI->JudgeActive( );
				   } );
}

//--------------------------------------------------------------------------------------
//  �f�o�b�O�`��
//--------------------------------------------------------------------------------------
void LayerAI::DrawDebug( void )
{
	std::string work = m_name + "Layer";

	if( ImGui::TreeNode( work.c_str( ) ) ) 
	{
		//  ���O�̎擾
		ImGui::InputText( "Name" , m_addNodeName , sizeof( m_addNodeName ) );

		//  Node�̒ǉ�
		if( ImGui::Button( "AddNode" ) ) 
		{
			std::string work = m_addNodeName;

			//  �������O���������ꍇ���Ȃ��悤�ɐݒ�
			work = RenameChildNodeSameName( work );

			AddChildNode( CharacterAI::NODE_TYPE::ACTION , work , m_characterAI , this );
			strcpy( m_addNodeName , "" );
		}

		//  ���g�̍폜
		if( ImGui::Button( "Delete" ) ) 
		{
			Release( );

			//  �e���C���[�ɍ폜��ʒm
			if( m_parentNode != nullptr )
			{
				m_parentNode->DeleteChildLayer( );
			}
		}

		//  �����m�[�h�S�Ẵf�o�b�N�`��
		for( auto iterator : m_childNode )
		{
			iterator->DrawDebug( );
		}

		ImGui::TreePop( );
	}
}

//--------------------------------------------------------------------------------------
//  �m�[�h�̒ǉ�
//--------------------------------------------------------------------------------------
void LayerAI::AddChildNode( CharacterAI::NODE_TYPE type , std::string name , CharacterAI* characterAI , LayerAI* parentLayer )
{
	switch( type )
	{
		case CharacterAI::NODE_TYPE::ACTION:
		{
			m_childNode.push_back( new ActionNodeAI( name , characterAI , parentLayer , Enemy::STATE::MOVE ) );

			break;
		}
		case CharacterAI::NODE_TYPE::RULE:
		{
			break;
		}
	}
}

//--------------------------------------------------------------------------------------
//  �q���C���[�̍폜
//--------------------------------------------------------------------------------------
void LayerAI::DeleteChildNode( std::string name )
{
	int countChild = 0;

	//  �����m�[�h�S�Ă̒��Ŗ��O�����������̂��폜
	for( auto iterator : m_childNode )
	{
		if( iterator->GetName( ) == name )
		{
			m_childNode.remove( iterator );

			return;
		}
	}
}

//--------------------------------------------------------------------------------------
//  �������O���������ꍇ���Ȃ��悤�ɖ��O�𒼂�
//--------------------------------------------------------------------------------------
const std::string LayerAI::RenameChildNodeSameName( const std::string& name )
{
	//  �������O�J�E���g�p
	int countSameName = 0; 

	//  ���O�̃R�s�[
	std::string workName = name;

	while( 1 )
	{
		//  �������O�����������̃t���O
		bool samaName = false;

		//  �������O�̐����J�E���g
		for( auto iterator : m_childNode )
		{
			if( iterator->GetName( ) == workName )
			{
				countSameName++;

				samaName = true;
			}
		}

		if( samaName == false )
		{
			return workName;
		}

		//  �����̒ǉ�( _�l+1 )
		workName = name + "_" + std::to_string( countSameName );
	}

	return name;
}