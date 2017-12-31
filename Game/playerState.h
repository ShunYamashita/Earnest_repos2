//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g  ( playerState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_STATE_H_
#define _PLAYER_STATE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g�N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerState
{
public:
	PlayerState( ){ }											//  �f�t�H���g�R���X�g���N�^
	PlayerState( Player* player ) : m_player( player ){ }		//  �R���X�g���N�^

	virtual void	Init( void );								//  ������
	virtual void	Uninit( void ) = 0;							//  �I��
	virtual void	Update( void ) = 0;							//  �X�V
	virtual void	Draw( void ) = 0;							//  �`��

protected:
	void			SetPlayer( Player* player );				//  �v���C���[�N���X�̐ݒ�
	Player*			GetPlayer( void );							//  �v���C���[�N���X�̎擾

	/*----------------------------
		�p����Ŏg����ėp�֐�
	----------------------------*/
	void			AddGravity( void );							//  �d�͂�������
	void			ControlCamera( void );						//  �J�����̐���
	void			ChangeLockon( void );						//  ���b�N�I����Ԃ̕ύX

	/*  ���f�n�̏���  */
	void			JudgeChangeNeutral( void );					//  �j���[�g������ԂɕύX���邩�̔��f
	void			JudgeChangeMove( void );					//  �ړ���ԂɕύX���邩�̔��f
	void			JudgeChangeJump( void );					//  �W�����v��ԂɕύX���邩�̔��f
	void			JudgeChangeHPAttack000( void );				//  HP�U��000�ɕύX���邩�̔��f
	void			JudgeChangeBraveAttack000( void );			//  �u���C�u�U��000�ɕύX���邩�̔��f
	void			JudgeChangeEscape( void );					//  �����ԂɕύX���邩�̔��f
	void			JudgeChangeGuard( void );					//  �h���ԂɕύX���邩�̔��f
	void			JudgeChangeLockonDash( void );				//  ���b�N�I���_�b�V����ԂɕύX���邩�̔��f
	void			JudgeChangeFreeDash( void );				//  �t���[�_�b�V����ԂɕύX���邩�̔��f
	void			JudgeChangeRushAttack( void );				//  �ːi�U����ԂɕύX���邩�̔��f
	void			JudgeChangeMagicReady( void );				//  ���@������ԂɕύX���邩�̔��f

	/*----------------------------
		�p����Ŏg����ėp�ϐ�
	----------------------------*/
	float			m_moveHeight;								//  �����ړ���

private:
	Player*			m_player;									//  �v���C���[�N���X�̃|�C���^
	int				m_pressTime;								//  �����Ă��鎞��
};

#endif

