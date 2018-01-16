//--------------------------------------------------------------------------------------
//  �m�[�hAI���N���X   ( nodeAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "nodeAI.h"
#include "layerAI.h"
#include "priorityLayerAI.h"
#include "sequenceLayerAI.h"
#include "randomLayerAI.h"
#include <algorithm>

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �m�[�h�̔j��
//--------------------------------------------------------------------------------------
void NodeAI::Release( void )
{
	//  �q���C���[�̍폜
	DeleteChildLayer( );
}

//--------------------------------------------------------------------------------------
//  ����������s�\���ǂ����̔��f
//--------------------------------------------------------------------------------------
void NodeAI::JudgeActive( void )
{
	if( m_childLayer != nullptr )
	{
		m_childLayer->AllJudgeActive( );
	}
}

//--------------------------------------------------------------------------------------
//  ���C���[�̒ǉ�
//--------------------------------------------------------------------------------------
void NodeAI::AddChildLayer( CharacterAI::LAYER_TYPE type , std::string name , CharacterAI* characterAI , NodeAI* parentNode )
{
	if( m_childLayer != nullptr )
	{
		return;
	}

	switch( type )
	{
		case CharacterAI::LAYER_TYPE::PRIORITY:
		{
			m_childLayer = new PriorityLayerAI( name , characterAI , parentNode );

			break;
		}
		case CharacterAI::LAYER_TYPE::SEQUENCE:
		{
			m_childLayer = new SequenceLayerAI( name , characterAI , parentNode );

			break;
		}
		case CharacterAI::LAYER_TYPE::RANDOM:
		{
			m_childLayer = new RandomLayerAI( name , characterAI , parentNode );

			break;
		}
	}
}

//--------------------------------------------------------------------------------------
//  ���C���[�̍폜
//--------------------------------------------------------------------------------------
void NodeAI::DeleteChildLayer( void )
{
	if( m_childLayer == nullptr )
	{
		return;
	}

	m_childLayer->Release( );
	delete m_childLayer;
	m_childLayer = nullptr;
}