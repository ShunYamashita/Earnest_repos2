//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( ���@���� )   ( enemyMagicActive.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyMagicActive.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void EnemyMagicActive::Init( void )
{
	//  �e�̏�����
	EnemyState::Init( );

	//  ���@�����
	GetEnemy( )->SetMagic( nullptr );
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void EnemyMagicActive::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyMagicActive::Update( void )
{
	//  �d�͂�������
	AddGravity( );

	//  �j���[�g������ԂɕύX���邩�̔��f
	JudgeChangeNeutral( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void EnemyMagicActive::Draw( void )
{

}