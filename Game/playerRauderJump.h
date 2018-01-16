//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �W�����v )  ( playerRauderJump.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RAUDER_JUMP_H_
#define _PLAYER_RAUDER_JUMP_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerRauderState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerRauder;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �W�����v ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerRauderJump : public PlayerRauderState
{
public:
	PlayerRauderJump( PlayerRauder* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;			//  ������
	void					Uninit( void ) override;		//  �I��
	void					Update( void ) override;		//  �X�V
	void					Draw( void ) override;			//  �`��

private:
	void					Action( void );					//  �A�N�V����

	float					m_jumpPower;					//  �W�����v��
	int						m_countJump;					//  �W�����v��
};

#endif
