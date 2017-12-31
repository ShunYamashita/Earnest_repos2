//--------------------------------------------------------------------------------------
//  ���C���[AI�N���X   ( layerAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _LAYER_AI_H_
#define _LAYER_AI_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "nodeAI.h"
#include <list>

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ���C���[AI�N���X�̒�`
//--------------------------------------------------------------------------------------
class LayerAI
{
public: 
	LayerAI( ) : m_parentNode( nullptr ) { }			//  �R���X�g���N�^

	void					Release( void );			//  �f�[�^�̔j��
	void					AllJudgeActive( void );		//  �S�Ẵm�[�h�̃A�N�e�B�u���f

private:
	NodeAI*					m_parentNode;				//  �e�m�[�h
	std::list< NodeAI* >	m_actionNode;				//  �A�N�V�����m�[�h	
};

#endif