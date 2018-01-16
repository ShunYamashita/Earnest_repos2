//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �W�����v )  ( playerWinksJump.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_JUMP_H_
#define _PLAYER_WINKS_JUMP_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �W�����v ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerWinksJump : public PlayerWinksState
{
public:
	PlayerWinksJump( PlayerWinks* player )
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
