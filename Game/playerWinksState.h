//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g  ( playerWinksState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_STATE_H_
#define _PLAYER_WINKS_STATE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g�N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerWinksState
{
public:
	PlayerWinksState( ){ }												//  �f�t�H���g�R���X�g���N�^
	PlayerWinksState( PlayerWinks* player ) : m_player( player ){ }		//  �R���X�g���N�^

	virtual void	Init( void );										//  ������
	virtual void	Uninit( void ) = 0;									//  �I��
	virtual void	Update( void ) = 0;									//  �X�V
	virtual void	Draw( void ) = 0;									//  �`��

protected:
	void			SetPlayer( PlayerWinks* player );						//  �v���C���[�N���X�̐ݒ�
	PlayerWinks*	GetPlayer( void );									//  �v���C���[�N���X�̎擾

	/*----------------------------
		�p����Ŏg����ėp�֐�
	----------------------------*/
	void			AddGravity( void );									//  �d�͂�������
	void			ControlCamera( void );								//  �J�����̐���
	void			ChangeLockon( void );								//  ���b�N�I����Ԃ̕ύX
	void			LookTarget( void );									//  �^�[�Q�b�g�����Ɍ���
	void			LookTargetXZ( void );								//  �^�[�Q�b�g�����Ɍ���( Y���� )
	D3DXVECTOR3		GetTargetPosition( void );							//  �^�[�Q�b�g���W�̎擾

	/*  ���f�n�̏���  */
	void			JudgeChangeIdle( void );							//  �ҋ@��ԂɕύX���邩�̔��f
	void			JudgeChangeMove( void );							//  �ړ���ԂɕύX���邩�̔��f
	void			JudgeChangeJump( void );							//  �W�����v��ԂɕύX���邩�̔��f
	void			JudgeChangeAttack( void );							//  �U���ɕύX���邩�̔��f
	void			JudgeChangeDash( void );							//  �_�b�V����ԂɕύX���邩�̔��f
	void			JudgeChangeGuard( void );							//  �h���ԂɕύX���邩�̔��f
	void			JudgeChangeHPAttack( void );						//  HP�U���ɕύX���邩�̔��f

	/*----------------------------
		�p����Ŏg����ėp�ϐ�
	----------------------------*/
	float			m_moveHeight;										//  �����ړ���

private:
	PlayerWinks*	m_player;											//  �v���C���[�N���X�̃|�C���^
	int				m_pressTime;										//  �����Ă��鎞��
};

#endif

