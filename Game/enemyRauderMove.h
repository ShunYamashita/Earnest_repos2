//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( �ړ� )  ( enemyRauderMove.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_RAUDER_MOVE_H_
#define _ENEMY_RAUDER_MOVE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyRauderState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class EnemyRauder;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �ړ� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class EnemyRauderMove : public EnemyRauderState
{
public:
	EnemyRauderMove( EnemyRauder* enemy )
	{
		SetEnemy( enemy );
	};

	void					Init( void ) override;			//  ������
	void					Uninit( void ) override;		//  �I��
	void					Update( void ) override;		//  �X�V
	void					Draw( void ) override;			//  �`��

private:
	void					Action( void );					//  �A�N�V����
};

#endif