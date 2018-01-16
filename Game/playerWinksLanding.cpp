//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���n )   ( playerWinksLanding.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksLanding.h"
#include "playerWinks.h"
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
void PlayerWinksLanding::Init( void )
{
	//  �e�̏�����
	PlayerWinksState::Init( );

	//  ���n�A�j���[�V������
	GetPlayer( )->SetMotion( FBXLoader::MOTION::LANDING );
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLanding::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLanding::Update( void )
{
	//  �ҋ@��ԂɕύX���邩�̔��f
	JudgeChangeIdle( );

	//  �ړ���ԂɕύX���邩�̔��f
	JudgeChangeMove( );

	//  �J��������
	ControlCamera( );

	//  �d�͂�������
	AddGravity( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLanding::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLanding::Action( void )
{

}