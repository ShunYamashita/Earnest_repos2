//--------------------------------------------------------------------------------------
//  �A�N�V�����m�[�hAI�N���X   ( actionNodeAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "actionNodeAI.h"
#include "layerAI.h"
#include "enemyRauder.h"
#include "enemyRauderState.h"
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  ���s����
//--------------------------------------------------------------------------------------
void ActionNodeAI::Run( void )
{
	if( m_active )
	{
		//  �G�l�~�[���̎擾
		EnemyRauder* enemy = ( EnemyRauder* )m_characterAI->GetScene( );

		if( m_state == NodeAI::STATE::RUNNING )
		{
			if( enemy->GetMotionFinish( ) )
			{
				m_state = NodeAI::STATE::COMPLETED;
			}
		}
		else 
		{
			if( m_characterAI != nullptr )
			{
				if( enemy != nullptr )
				{
					//  ���s����
					m_state = NodeAI::STATE::RUNNING;

					enemy->GetCurrentEnemyState( )->ChangeMove( EnemyRauderState::MOVE_TYPE::TARGET_CHASE );				
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  �f�o�b�O�̕`��
//--------------------------------------------------------------------------------------
void ActionNodeAI::DrawDebug( void )
{
	std::string work = m_name + "ActionNode";

	if( ImGui::TreeNode( work.c_str( ) ) ) 
	{
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

		ImGui::TreePop( );
	}
}