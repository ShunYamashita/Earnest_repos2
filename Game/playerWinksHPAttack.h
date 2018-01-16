//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( HP�U�� )  ( playerWinksHPAttack.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_HP_ATTACK_H_
#define _PLAYER_WINKS_HP_ATTACK_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( HP�U�� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerWinksHPAttack : public PlayerWinksState
{
public:
	PlayerWinksHPAttack( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void							Init( void ) override;			//  ������
	void							Uninit( void ) override;		//  �I��
	void							Update( void ) override;		//  �X�V
	void							Draw( void ) override;			//  �`��

private:
	void							Action( void );					//  �A�N�V����

	bool							m_slashEffekseer;				//  �X���b�V���G�t�F�N�V�A���o�����̃t���O
};

#endif
