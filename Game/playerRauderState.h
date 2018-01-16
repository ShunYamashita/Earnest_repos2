//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g  ( playerRauderState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RAUDER_STATE_H_
#define _PLAYER_RAUDER_STATE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerRauder;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g�N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerRauderState
{
public:
	PlayerRauderState( ){ }												//  �f�t�H���g�R���X�g���N�^
	PlayerRauderState( PlayerRauder* player ) : m_player( player ){ }		//  �R���X�g���N�^

	virtual void	Init( void );										//  ������
	virtual void	Uninit( void ) = 0;									//  �I��
	virtual void	Update( void ) = 0;									//  �X�V
	virtual void	Draw( void ) = 0;									//  �`��

protected:
	void			SetPlayer( PlayerRauder* player );						//  �v���C���[�N���X�̐ݒ�
	PlayerRauder*	GetPlayer( void );									//  �v���C���[�N���X�̎擾

	/*----------------------------
		�p����Ŏg����ėp�֐�
	----------------------------*/
	void			AddGravity( void );									//  �d�͂�������
	void			ControlCamera( void );								//  �J�����̐���
	void			ChangeLockon( void );								//  ���b�N�I����Ԃ̕ύX

	/*  ���f�n�̏���  */
	void			JudgeChangeIdle( void );							//  �ҋ@��ԂɕύX���邩�̔��f
	void			JudgeChangeMove( void );							//  �ړ���ԂɕύX���邩�̔��f
	void			JudgeChangeJump( void );							//  �W�����v��ԂɕύX���邩�̔��f
	void			JudgeChangeAttack( void );							//  �U���ɕύX���邩�̔��f

	/*----------------------------
		�p����Ŏg����ėp�ϐ�
	----------------------------*/
	float			m_moveHeight;										//  �����ړ���

private:
	PlayerRauder*	m_player;											//  �v���C���[�N���X�̃|�C���^
	int				m_pressTime;										//  �����Ă��鎞��
};

#endif

