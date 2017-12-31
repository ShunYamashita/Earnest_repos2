//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �ړ� )   ( playerMove.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerMove.h"
#include "player.h"
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
void PlayerMove::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerMove::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerMove::Update( void )
{
	//  �J��������
	ControlCamera( );

	//  �A�N�V����
	Action( );

	//  �d�͂�������
	AddGravity( );

	//  �j���[�g������ԂɕύX���邩�̔��f
	JudgeChangeNeutral( );

	//  �W�����v��ԂɕύX���邩���f
	JudgeChangeJump( );

	//  �����ԂɕύX���邩�̔��f
	JudgeChangeEscape( );

	//  HP�U����ԂɕύX���邩�̔��f
	JudgeChangeHPAttack000( );

	//  �u���C�u�U��000��ԂɕύX���邩�̔���
	JudgeChangeBraveAttack000( );

	//  �h���ԂɕύX���邩�̔��f
	JudgeChangeGuard( );

	//  ���b�N�I���_�b�V����ԂɕύX���邩�̔��f
	JudgeChangeLockonDash( );

	//  ���@������ԂɕύX���邩�̔��f
	JudgeChangeMagicReady( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerMove::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerMove::Action( void )
{
	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	//  �V�[���N���X�̃|�C���^
	Scene* pScene = NULL;			

	//  �J���������x�N�g��
	D3DXVECTOR3 cameraVecDirect;

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	//  �ړ�����
	D3DXVECTOR3 move( 0.0f , 0.0f , 0.0f );

#ifdef KEYBOARD_ENABLE

	//  �ړ��ʂ̏�����
	move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		if( pKeyboard->GetKeyboardPress( DIK_W ) )
		{
			move.z += 1.0f;
		}
		else if( pKeyboard->GetKeyboardPress( DIK_S ) )
		{
			move.z -= 1.0f;
		}

		if( pKeyboard->GetKeyboardPress( DIK_A ) )
		{
			move.x -= 1.0f;
		}
		else if( pKeyboard->GetKeyboardPress( DIK_D ) )
		{
			move.x += 1.0f;
		}
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		if( pKeyboard->GetKeyboardPress( DIK_UPARROW ) )
		{
			move.z += 1.0f;
		}
		else if( pKeyboard->GetKeyboardPress( DIK_DOWNARROW ) )
		{
			move.z -= 1.0f;
		}

		if( pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) )
		{
			move.x -= 1.0f;
		}
		else if( pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
		{
			move.x += 1.0f;
		}
	}

#else

	//  ���X�e�B�b�N���̎擾
	move.x = ( float )pPS4Input->GetLeftStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x;
	move.z = -( float )pPS4Input->GetLeftStickDisposition( GetPlayer( )->GetPlayerNo( ) ).y;

#endif 

	D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

	if( move.x != 0 ||
		move.z != 0 )
	{
		//  ���x�̑��
		GetPlayer( )->SetSpeed( GetPlayer( )->GetBaseSpeed( ) );

		if( pCamera != NULL )
		{
			float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

			D3DXMATRIX mtxRot;
			D3DXMatrixIdentity( &mtxRot );
			D3DXMatrixRotationY( &mtxRot , fAngle );

			D3DXVec3TransformNormal( &workVecDirect , &move , &mtxRot );
			workVecDirect.y = 0.0f;
			D3DXVec3Normalize( &workVecDirect , &workVecDirect );
		}
		else
		{
			workVecDirect.y = 0.0f;
			D3DXVec3Normalize( &workVecDirect , &move );
		}

		//  �����x�N�g���̐ݒ�
		GetPlayer( )->SetVecDirect( workVecDirect );

		//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
		GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * GetPlayer( )->GetSpeed( ) );
	}
	else
	{
		//  �j���[�g������Ԃ�
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_NEUTRAL );
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::NEUTRAL ) );
	}

	//  �����_�������Ă��������
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
}

//--------------------------------------------------------------------------------------
//  �j���[�g������ԂɕύX���邩�̔��f������֐�
//--------------------------------------------------------------------------------------
void PlayerMove::JudgeChangeNeutral( void )
{

}