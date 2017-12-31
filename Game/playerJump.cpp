//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �W�����v )   ( playerJump.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerJump.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float	PLAYER_GRAVITY = 0.1f;
static const int	PLAYER_ENABLE_NUM_JUMP = 2;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerJump::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );

	//  �����o�ϐ��̏�����
	m_countJump = 1;
	m_jumpPower = GetPlayer( )->GetJumpPower( );
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerJump::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerJump::Update( void )
{
	//  �J��������
	ControlCamera( );

	//  �A�N�V����
	Action( );

	//  �����ԂɕύX���邩�̔��f
	JudgeChangeEscape( );

	//  HP�U��000��ԂɕύX���邩�̔���
	JudgeChangeHPAttack000( );

	//  �u���C�u�U��000��ԂɕύX���邩�̔���
	JudgeChangeBraveAttack000( );

	//  �h���ԂɕύX���邩�̔��f
	JudgeChangeGuard( );

	//  ���b�N�I���_�b�V����ԂɕύX���邩�̔��f
	JudgeChangeLockonDash( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerJump::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerJump::Action( void )
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

	//  PLAYER_ENABLE_NUM_JUMP( 2 )�i�W�����v�p����
	if( m_countJump < PLAYER_ENABLE_NUM_JUMP )
	{
		int nKey = 0;

		if( GetPlayer( )->GetPlayerNo( ) == 0 )
		{
			nKey = DIK_SPACE;
		}
		else if( GetPlayer( )->GetPlayerNo( ) == 1 )
		{
			nKey = DIK_RCONTROL;
		}

#ifdef KEYBOARD_ENABLE

		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  �W�����v�A�j���[�V������
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_JUMP );

			//  �W�����v�͂̏�����
			m_jumpPower = GetPlayer( )->GetJumpPower( );

			//  �W�����v�J�E���g
			m_countJump++;
		}

#else

		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CROSS ) )
		{
			//  �W�����v�A�j���[�V������
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_JUMP );

			//  �W�����v�͂̏�����
			m_jumpPower = GetPlayer( )->GetJumpPower( );

			//  �W�����v�J�E���g
			m_countJump++;
		}

#endif

	}

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
			D3DXVec3Normalize( &workVecDirect , &workVecDirect );
		}
		else
		{
			D3DXVec3Normalize( &workVecDirect , &move );
		}

		//  �����x�N�g���̐ݒ�
		GetPlayer( )->SetVecDirect( workVecDirect );

		//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
		GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * GetPlayer( )->GetSpeed( ) );
	}

	//  �����_�������Ă��������
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  �W�����v�͕���ɏオ��̂ƃW�����v�͂��d�͕�������
	GetPlayer( )->MoveHeight( m_jumpPower );
	m_jumpPower -= PLAYER_GRAVITY;

	//  �n�ʂɂ����ꍇ
	if( GetPlayer( )->JudgeHitGround( ) )
	{
		//  ���n�A�j���[�V������
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_LANDING_SHORT );
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::LANDING ) );
	}
}