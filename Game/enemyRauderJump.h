//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( �W�����v )  ( enemyRauderJump.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_RAUDER_JUMP_H_
#define _ENEMY_RAUDER_JUMP_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyRauderState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class EnemyRauder;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �W�����v ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class EnemyRauderJump : public EnemyRauderState
{
public:
	EnemyRauderJump( EnemyRauder* enemy )
	{
		SetEnemy( enemy );
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