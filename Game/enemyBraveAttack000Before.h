//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( Brave�U��000( �� ) )  ( enemyBraveAttack000Before.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _ENEMY_BRAVE_ATTACK000_BEFORE_H_
#define _ENEMY_BRAVE_ATTACK000_BEFORE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyState.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Enemy;

//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( Brave�U��000( �O ) ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class EnemyBraveAttack000Before : public EnemyState
{
public:
	EnemyBraveAttack000Before( Enemy* enemy )
	{
		SetEnemy( enemy );
	};

	void								Init( void ) override;			//  ������
	void								Uninit( void ) override;		//  �I��
	void								Update( void ) override;		//  �X�V
	void								Draw( void ) override;			//  �`��

	static EnemyBraveAttack000Before*	Create( void );					//  ����

private:
	void								Action( void );					//  �A�N�V����

	D3DXVECTOR3							m_beforePosition;				//  �O��̍��W
	float								m_homingTime;					//  �ǔ�����
	float								m_speed;						//  ���x
};

#endif
