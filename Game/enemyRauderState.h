//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g  ( enemyRauderState.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_RAUDER_STATE_H_
#define _ENEMY_RAUDER_STATE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class EnemyRauder;

//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g�N���X�̒�`
//--------------------------------------------------------------------------------------
class EnemyRauderState
{
public:
	enum class MOVE_TYPE
	{
		TARGET_CHASE = 0 ,
		TARGET_CHASE_CURVE ,
		ESCAPE ,
	};

	EnemyRauderState( ){ }												//  �f�t�H���g�R���X�g���N�^
	EnemyRauderState( EnemyRauder* enemy ) : m_enemy( enemy ){ }		//  �R���X�g���N�^

	virtual void	Init( void );										//  ������
	virtual void	Uninit( void ) = 0;									//  �I��
	virtual void	Update( void ) = 0;									//  �X�V
	virtual void	Draw( void ) = 0;									//  �`��

	/*  ���f�n�̏���  */
	void			ChangeIdle( void );									//  �ҋ@��ԂɕύX����
	void			ChangeMove( MOVE_TYPE moveType );					//  �ړ���ԂɕύX����
	void			ChangeJump( void );									//  �W�����v��ԂɕύX����

protected:
	void			SetEnemy( EnemyRauder* player );					//  �G�l�~�[�N���X�̐ݒ�
	EnemyRauder*	GetEnemy( void );									//  �G�l�~�[�N���X�̎擾

	/*----------------------------
		�p����Ŏg����ėp�֐�
	----------------------------*/
	void			AddGravity( void );									//  �d�͂�������
	void			ChangeLockon( void );								//  ���b�N�I����Ԃ̕ύX
	void			LookTargetXZ( void );									//  �^�[�Q�b�g�����Ɍ���
	D3DXVECTOR3		GetTargetPosition( void );							//  �^�[�Q�b�g���W�̎擾

	/*----------------------------
		�p����Ŏg����ėp�ϐ�
	----------------------------*/
	float			m_moveHeight;										//  �����ړ���
	MOVE_TYPE		m_moveType;											//  �ړ��^�C�v

private:
	EnemyRauder*	m_enemy;											//  �G�l�~�[�N���X�̃|�C���^
	int				m_pressTime;										//  �����Ă��鎞��
};

#endif

