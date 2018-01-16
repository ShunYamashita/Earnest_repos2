//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���b�N�I���_�b�V�� )   ( playerWinksLockonDash.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksLockonDash.h"
#include "playerWinks.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "particle.h"
#include "enemy.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float PLAYER_LOCKON_DASH_SPEED = 3.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Init( void )
{
	//  �e�̏�����
	PlayerWinksState::Init( );

	//  ���x�̏�����
	m_speed = PLAYER_LOCKON_DASH_SPEED;

	//  ���[�V������Ԃ̏�����
	m_motionState = MOTION_STATE::START;

	//  ���[�V�����̕ύX
	GetPlayer( )->SetMotion( FBXLoader::MOTION::DASH_START );

	//  �p�����Œ�ł͂Ȃ���Ԃ�
	FBXLoader* fbx = GetPlayer( )->GetFBXLoader( );

	if( fbx != nullptr )
	{
		fbx->SetFixedPose( false );
	}
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Uninit( void )
{
	m_speed = 0.0f;

	//  �p�����Œ��Ԃ�
	FBXLoader* fbx = GetPlayer( )->GetFBXLoader( );

	if( fbx != nullptr )
	{
		fbx->SetFixedPose( true );
	}
}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Update( void )
{
	//  �J��������
	ControlCamera( );

	//  �A�N�V����
	Action( );

	//  �_�b�V���J�n��Ԃ̏ꍇ
	if( m_motionState == MOTION_STATE::START )
	{
		//  ���x�����������������ꍇ
		if( m_speed <= 0.9f )
		{
			//  ���[�V�����ύX
			GetPlayer( )->SetMotion( FBXLoader::MOTION::DASH_END );
			m_motionState = MOTION_STATE::END;
		}
	}
	//  �_�b�V���I����Ԃ̏ꍇ
	else if( m_motionState == MOTION_STATE::END )
	{
		//  ���x���������������Ă��A���[�V�������I�������ꍇ
		if( m_speed <= 0.3f && GetPlayer( )->GetMotionFinish( ) )
		{
			//  �ҋ@���[�V�����ɕύX
			JudgeChangeIdle( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksLockonDash::Action( void )
{
	//  �^�[�Q�b�g�����ɐi�s�����x�N�g����ύX
	LookTarget( );

	//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
	GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * m_speed );

	//  �����_�������Ă��������
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  ���x�����X�Ɍ��������Ă���
	m_speed += ( 0.0f - m_speed ) * 0.03f;

	//  �p�[�e�B�N���̐���
	Particle::Create( Particle::TYPE_NORMAL , GetPlayer( )->GetPos( ) , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
					  D3DXCOLOR( 0.2f , 0.5f , 0.8f , 1.0f ) , 4.0f , 0.1f , 0.01f , 0.0f , 0.0f , 20 );
}