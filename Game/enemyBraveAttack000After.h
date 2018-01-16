//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( Brave�U��000( �� ) )  ( enemyBraveAttack000After.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_BRAVE_ATTACK000_AFTER_H_
#define _ENEMY_BRAVE_ATTACK000_AFTER_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Enemy;

//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( Brave�U��000( �� ) ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class EnemyBraveAttack000After : public EnemyState
{
public:
	EnemyBraveAttack000After( Enemy* enemy )
	{
		SetEnemy( enemy );
	};

	void								Init( void ) override;			//  ������
	void								Uninit( void ) override;		//  �I��
	void								Update( void ) override;		//  �X�V
	void								Draw( void ) override;			//  �`��

	static EnemyBraveAttack000After*	Create( void );					//  ����

private:
	void								Action( void );					//  �A�N�V����

	float								m_speed;						//  ���x
};

#endif
