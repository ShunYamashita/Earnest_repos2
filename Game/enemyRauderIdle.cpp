//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( �ҋ@ )   ( enemyRauderIdle.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyRauderIdle.h"
#include "player.h"
#include "manager.h"
#include "keyboard.h"
#include "joystick.h"
#include "game.h"
#include "camera.h"
#include "effekseerManager.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void EnemyRauderIdle::Init( void )
{
	//  �e�̏�����
	EnemyRauderState::Init( );
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void EnemyRauderIdle::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyRauderIdle::Update( void )
{
	//  �d�͂�������
	AddGravity( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void EnemyRauderIdle::Draw( void )
{

}