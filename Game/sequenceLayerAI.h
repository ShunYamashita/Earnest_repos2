//--------------------------------------------------------------------------------------
//  �V�[�N�G���X���C���[AI�N���X   ( sequenceLayerAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SEQUENCE_LAYER_AI_H_
#define _SEQUENCE_LAYER_AI_H_

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
//  �V�[�N�G���X���C���[AI�N���X�̒�`
//--------------------------------------------------------------------------------------
class SequenceLayerAI : public LayerAI
{
public:
	SequenceLayerAI( std::string name ,								//  �����t���R���X�g���N�^
					 CharacterAI* characterAI ,
					 NodeAI* parentNode ) :
					 LayerAI( CharacterAI::LAYER_TYPE::SEQUENCE ,
							  name ,
							  characterAI ,
							  parentNode ){ }	  

	void			Run( void ) override;							//  ���s	

private:

};

#endif
