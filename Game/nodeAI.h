//--------------------------------------------------------------------------------------
//  �m�[�hAI�N���X   ( nodeAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _NODE_AI_H_
#define _NODE_AI_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include <string>
#include <list>
#include "characterAI.h"

//--------------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------------
class LayerAI;

//--------------------------------------------------------------------------------------
//  �m�[�hAI�N���X�̒�`
//--------------------------------------------------------------------------------------
class NodeAI
{
public:
	enum class STATE			//  ���
	{
		INACTIVE = 0 ,			//  ��A�N�e�B�u
		SUCCESS ,				//  ����
		FAILURE ,				//  ���s
		RUNNING ,				//  ���s��
		COMPLETED ,				//  ����
	};

	NodeAI( ) : m_active( true ){ }											//  �R���X�g���N�^
	NodeAI( CharacterAI::NODE_TYPE type ,									//  �����t���R���X�g���N�^
			std::string name ,												
			CharacterAI* characterAI ,
			LayerAI* parentLayer ) :
			m_type( type ) ,
			m_name( name ) ,
			m_characterAI( characterAI ) ,
			m_parentLayer( parentLayer ) ,			
			m_active( true ) ,
			m_state( STATE::INACTIVE ){ }																		

	virtual void			Release( void );								//  ���
	virtual void			Run( void ) = 0;								//  ���s
	virtual void			JudgeActive( void );							//  ���s�\���̔��f������
	virtual void			EnableActive( void ){ m_active = true; }		//  ���s�\��Ԃ�
	virtual void			DisableActive( void ){ m_active = false; }		//  ���s�s�\��Ԃ�
	virtual void			DrawDebug( void ) = 0;							//  �f�o�b�O�`��
	virtual void			AddChildLayer( CharacterAI::LAYER_TYPE type ,	//  ���C���[�̒ǉ�
										   std::string name ,				
										   CharacterAI* characterAI ,
										   NodeAI* parentNode );
	virtual void			DeleteChildLayer( void );						//  �q���C���[�̍폜
	virtual bool			GetActive( void ){ return m_active; }			//  ���s��Ԃ̎擾
	virtual STATE			GetState( void ){ return m_state; }				//  ��Ԃ̎擾
	virtual std::string		GetName( void ){ return m_name; }				//  ���O�̎擾

protected:
	std::string				m_name;											//  �m�[�h��
	LayerAI*				m_parentLayer;									//  �e���C���[
	LayerAI*				m_childLayer;									//  �q���C���[
	CharacterAI*			m_characterAI;									//  �L�����N�^�[AI
	STATE					m_state;										//  ���
	CharacterAI::NODE_TYPE	m_type;											//  ���
	bool					m_active;										//  �A�N�e�B�u���̃t���O
	char					m_addLayerName[ 32 ];							//  �ǉ����C���[��
};

#endif
