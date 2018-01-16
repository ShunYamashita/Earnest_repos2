//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g  ( enemyState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_STATE_H_
#define _ENEMY_STATE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Enemy;

//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g�N���X�̒�`
//--------------------------------------------------------------------------------------
class EnemyState
{
public:
	EnemyState( ){ }														//  �f�t�H���g�R���X�g���N�^
	EnemyState( Enemy* enemy ) : m_enemy( enemy ){ }						//  �R���X�g���N�^

	virtual void	Init( void );											//  ������
	virtual void	Uninit( void ) = 0;										//  �I��
	virtual void	Update( void ) = 0;										//  �X�V
	virtual void	Draw( void ) = 0;										//  �`��

	/*  ���f�n�̏���  */
	void			JudgeChangeNeutral( void );								//  �j���[�g������ԂɕύX���邩�̔��f
	void			JudgeChangeMove( const D3DXVECTOR3& vecDirect );		//  �ړ���ԂɕύX���邩�̔��f
	void			JudgeChangeJump( void );								//  �W�����v��ԂɕύX���邩�̔��f
	void			JudgeChangeHPAttack000( void );							//  HP�U��000�ɕύX���邩�̔��f
	void			JudgeChangeBraveAttack000( bool left = true );			//  �u���C�u�U��000�ɕύX���邩�̔��f
	void			JudgeChangeEscape( const D3DXVECTOR3& vecDirect );		//  �����ԂɕύX���邩�̔��f
	void			JudgeChangeGuard( void );								//  �h���ԂɕύX���邩�̔��f
	void			JudgeChangeLockonDash( const D3DXVECTOR3& vecDirect );	//  ���b�N�I���_�b�V����ԂɕύX���邩�̔��f
	void			JudgeChangeFreeDash( const D3DXVECTOR3& vecDirect );	//  �t���[�_�b�V����ԂɕύX���邩�̔��f
	void			JudgeChangeRushAttack( void );							//  �ːi�U����ԂɕύX���邩�̔��f
	void			JudgeChangeMagicReady( void );							//  ���@������ԂɕύX���邩�̔��f
	void			JudgeChangeMagicActive( void );							//  ���@�A�N�e�B�u��ԂɕύX���邩�̔��f

protected:
	void			SetEnemy( Enemy* enemy );								//  �G�l�~�[�N���X�̐ݒ�
	Enemy*			GetEnemy( void );										//  �G�l�~�[�N���X�̎擾

	/*----------------------------
		�p����Ŏg����ėp�֐�
	----------------------------*/
	void			AddGravity( void );										//  �d�͂�������
	void			ChangeLockon( void );									//  ���b�N�I����Ԃ̕ύX

	/*----------------------------
		�p����Ŏg����ėp�ϐ�
	----------------------------*/
	float			m_moveHeight;										//  �����ړ���

private:
	Enemy*			m_enemy;											//  �G�l�~�[�N���X�̃|�C���^
	int				m_pressTime;										//  �����Ă��鎞��
};

#endif

