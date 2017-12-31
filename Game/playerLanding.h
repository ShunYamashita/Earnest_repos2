//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���n )  ( playerLanding.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_LANDING_H_
#define _PLAYER_LANDING_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���n ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerLanding : public PlayerState
{
public:
	enum class RIGID_TIME												//  �d������
	{
		SHORT = 0 ,														//  �Z��
		LONG ,															//  ����
	};

	PlayerLanding( Player* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;						//  ������
	void					Uninit( void ) override;					//  �I��
	void					Update( void ) override;					//  �X�V
	void					Draw( void ) override;						//  �`��

	static PlayerLanding*	Create( void );								//  ����
																		
private:					
	void					Action( void );								//  �A�N�V����
																		
	RIGID_TIME				m_rigidTimeType;							//  �d�����Ԃ̎��
};

#endif
