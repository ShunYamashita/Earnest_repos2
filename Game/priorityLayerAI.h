//--------------------------------------------------------------------------------------
//  �D��x���C���[AI�N���X   ( priorityLayerAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PRIORITY_LAYER_AI_H_
#define _PRIORITY_LAYER_AI_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include <list>
#include <string>
#include "characterAI.h"
#include "layerAI.h"

//--------------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------------
class NodeAI;

//--------------------------------------------------------------------------------------
//  �D��x���C���[AI�N���X�̒�`
//--------------------------------------------------------------------------------------
class PriorityLayerAI : public LayerAI
{
public:
	PriorityLayerAI( std::string name ,								//  �����t���R���X�g���N�^
					 CharacterAI* characterAI ,
					 NodeAI* parentNode ) :
					 LayerAI( CharacterAI::LAYER_TYPE::PRIORITY , 
							  name ,
							  characterAI ,
							  parentNode ){ }	  

	void			Run( void ) override;							//  ���s	

private:

};

#endif
