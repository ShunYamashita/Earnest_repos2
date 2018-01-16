//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���n )  ( playerWinksLanding.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_LANDING_H_
#define _PLAYER_LANDING_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���n ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerWinksLanding : public PlayerWinksState
{
public:
	enum class RIGID_TIME												//  �d������
	{
		SHORT = 0 ,														//  �Z��
		LONG ,															//  ����
	};

	PlayerWinksLanding( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;						//  ������
	void					Uninit( void ) override;					//  �I��
	void					Update( void ) override;					//  �X�V
	void					Draw( void ) override;						//  �`��
																		
private:					
	void					Action( void );								//  �A�N�V����
																		
	RIGID_TIME				m_rigidTimeType;							//  �d�����Ԃ̎��
};

#endif
