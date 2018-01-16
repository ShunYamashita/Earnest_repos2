//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �h�� )   ( playerWinksGuard.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksGuard.h"
#include "playerWinks.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------------
static const int GUARD_TIME = 30;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Init( void )
{
	//  �e�̏�����
	PlayerWinksState::Init( );

	//  �n�ʂɂ��邩�ǂ����Ń��[�V�����ύX
	if( GetPlayer( )->JudgeHitGround( ) )
	{
		//  ���[�V�����̕ύX
		GetPlayer( )->SetMotion( FBXLoader::MOTION::GROUND_GUARD_START );
		m_ground = true;
	}
	else
	{
		//  ���[�V�����̕ύX
		GetPlayer( )->SetMotion( FBXLoader::MOTION::AIR_GUARD_START );
		m_ground = false;
	}

	float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

	//  �U�����̓����蔻��̑��
	D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f;
	effekseerPosition.y += 6.0f;

	//  �G�t�F�N�g�̐���
	m_effekseerHandle = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
												  D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) ,
												  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.5f );

	//  �K�[�h���Ԃ̏�����
	m_guardTime = GUARD_TIME;

	//  ���[�V������Ԃ̏�����
	m_motionState = MOTION_STATE::START;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Uninit( void )
{
	//  �h�������Ԃ�
	GetPlayer( )->SetGuardFlag( false );
}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Update( void )
{
	//  �J��������
	ControlCamera( );

	//  �A�N�V����
	Action( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksGuard::Action( void )
{
	switch( m_motionState )
	{
		case MOTION_STATE::START:
		{
			//  �h���Ԃ�
			GetPlayer( )->SetGuardFlag( true );

			//  ���[�V�������I�����Ă���ꍇ
			if( GetPlayer( )->GetMotionFinish( ) )
			{
				m_motionState = MOTION_STATE::GUARD;
			}

			break;
		}
		case MOTION_STATE::GUARD:
		{
			//  �h���Ԃ�
			GetPlayer( )->SetGuardFlag( true );

			//  �K�[�h���Ԓ��̏ꍇ
			if( m_guardTime > 0 )
			{
				m_guardTime--;
			}
			else
			{
				if( m_ground )
				{
					//  ���[�V�����̕ύX
					GetPlayer( )->SetMotion( FBXLoader::MOTION::GROUND_GUARD_END );
				}
				else
				{
					//  ���[�V�����̕ύX
					GetPlayer( )->SetMotion( FBXLoader::MOTION::AIR_GUARD_END );
				}

				m_motionState = MOTION_STATE::END;
			}

			break;
		}
		case MOTION_STATE::END:
		{
			//  �h�������Ԃ�
			GetPlayer( )->SetGuardFlag( false );

			//  �G�t�F�N�V�A�̍Đ��X�g�b�v
			EffekseerManager::Stop( m_effekseerHandle );

			//  ���[�V�������I���������͑ҋ@��ԂɕύX
			JudgeChangeIdle( );

			break;
		}
	}

	//  �G�t�F�N�V�A���W�̐ݒ�
	D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f;
	effekseerPosition.y += 6.0f;
	EffekseerManager::SetPosition( m_effekseerHandle , D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) );
}