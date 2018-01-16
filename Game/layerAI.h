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
#include <list>
#include <string>
#include "characterAI.h"

//--------------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------------
class NodeAI;

//--------------------------------------------------------------------------------------
//  ���C���[AI�N���X�̒�`
//--------------------------------------------------------------------------------------
class LayerAI
{
public:
	LayerAI( ) : m_parentNode( nullptr ) { }									//  �R���X�g���N�^
	LayerAI( CharacterAI::LAYER_TYPE type ,										//  �����t���R���X�g���N�^
			 std::string name ,
			 CharacterAI* characterAI ,
			 NodeAI* parentNode ) :
			 m_type( type ) ,
			 m_name( name ) ,
			 m_characterAI( characterAI ) ,
			 m_parentNode( parentNode ) { }			

	virtual void			Release( void );									//  �f�[�^�̔j��
	virtual void			AllJudgeActive( void );								//  �S�Ẵm�[�h�̃A�N�e�B�u���f
	virtual void			DrawDebug( void );									//  �f�o�b�O�`��
	virtual	std::string		GetName( void ){ return m_name; }					//  ���O�̎擾
	virtual void			AddChildNode( CharacterAI::NODE_TYPE type ,			//  �q�m�[�h�̒ǉ�
										  std::string name ,
										  CharacterAI* characterAI ,
										  LayerAI* parentLayer );
	virtual void			DeleteChildNode( std::string name );				//  �q�m�[�h�̍폜

	virtual void			Run( void ) = 0;									//  ���s

protected:
	const std::string		RenameChildNodeSameName( const std::string& name );	//  �������O���������ꍇ���Ȃ��悤�ɖ��O�𒼂�

	CharacterAI::LAYER_TYPE	m_type;												//  ���
	std::string				m_name;												//  ���C���[��
	NodeAI*					m_parentNode;										//  �e�m�[�h
	std::list< NodeAI* >	m_childNode;										//  �q�m�[�h
	CharacterAI*			m_characterAI;										//  �L�����N�^�[AI
	char					m_addNodeName[ 32 ];								//  �ǉ��m�[�h��
};

#endif
