//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �U�� )  ( playerRauderAttack.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RAUDER_ATTACK_H_
#define _PLAYER_RAUDER_ATTACK_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerRauderState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerRauder;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �U�� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerRauderAttack : public PlayerRauderState
{
public:
	PlayerRauderAttack( PlayerRauder* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;			//  ������
	void					Uninit( void ) override;		//  �I��
	void					Update( void ) override;		//  �X�V
	void					Draw( void ) override;			//  �`��

private:
	void					Action( void );					//  �A�N�V����
	void					MoveTarget( void );				//  �^�[�Q�b�g�ɏ��X�ɋ߂Â�
	void					JudgeNextMotion( void );		//  ���̃��[�V�����Ɉڍs���邩�̔��f

	int						m_rigidTime;					//  �d������
	int						m_attackNo;						//  �U���ԍ�
};

#endif
