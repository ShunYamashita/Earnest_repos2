//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���@���� )   ( playerMagicReady.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerMagicReady.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Update( void )
{
	//  �J��������
	ControlCamera( );

	//  �A�N�V����
	Action( );

	//  �d�͂�������
	AddGravity( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerMagicReady::Action( void )
{
	//  �ړ�����
	D3DXVECTOR3 move( 0.0f , 0.0f , 0.0f );

	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

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

	//  �J���������x�N�g��
	D3DXVECTOR3 cameraVecDirect;

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	Magic* magic = GetPlayer( )->GetMagic( );

	if( magic != NULL )
	{
		Player::MAGIC magicType = GetPlayer( )->GetMagicType( );

		//  �����@�������Ă���ꍇ
		if( magicType == Player::MAGIC::LIGHTNING )
		{
			D3DXVECTOR3 vecDirect( move.x , 0.0f , move.z );

			cameraVecDirect = pCamera->GetCameraVecDirect( );

			if( pCamera != NULL )
			{
				float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

				D3DXMATRIX mtxRot;
				D3DXMatrixIdentity( &mtxRot );
				D3DXMatrixRotationY( &mtxRot , fAngle );

				D3DXVec3TransformNormal( &vecDirect , &move , &mtxRot );
				D3DXVec3Normalize( &vecDirect , &vecDirect );
			}
			else
			{
				D3DXVec3Normalize( &vecDirect , &move );
			}

			//  ���@�w�𓮂���
			MagicLightning* pMagicLightning = ( MagicLightning* )GetPlayer( )->GetMagic( );
			pMagicLightning->MoveMagicPos( vecDirect );
		}	

		//  �������@�������Ă���{
		if( magicType == Player::MAGIC::TORNADE )
		{
			D3DXVECTOR3 vecDirect( move.x , 0.0f , move.z );

			cameraVecDirect = pCamera->GetCameraVecDirect( );

			if( pCamera != NULL )
			{
				float fAngle = atan2f( cameraVecDirect.x , cameraVecDirect.z );

				D3DXMATRIX mtxRot;
				D3DXMatrixIdentity( &mtxRot );
				D3DXMatrixRotationY( &mtxRot , fAngle );

				D3DXVec3TransformNormal( &vecDirect , &move , &mtxRot );
				D3DXVec3Normalize( &vecDirect , &vecDirect );
			}
			else
			{
				D3DXVec3Normalize( &vecDirect , &move );
			}

			//  �ړ������x�N�g���̐ݒ�
			MagicTornade* pMagicToranade = ( MagicTornade* )GetPlayer( )->GetMagic( );

			if( move.x == 0.0f && move.z == 0.0f )
			{
				vecDirect = GetPlayer( )->GetVecDirect( );
			}

			pMagicToranade->SetMoveVecDirect( vecDirect );
		}
				
		int nKey= 0;

		if( GetPlayer( )->GetPlayerNo( ) == 0 )
		{
			nKey = DIK_H;
		}
		else if( GetPlayer( )->GetPlayerNo( ) == 1 )
		{
			nKey = DIK_AT;
		}

#ifdef KEYBOARD_ENABLE

		if( pKeyboard->GetKeyboardRelease( nKey ) )
		{
			if( magicType == Player::MAGIC::FIRE )
			{
				//  ���@�������[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE );

				//  ���@������Ԃ�
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  ���s��Ԃ�
				magic->SetActive( );
			}
			else if( magicType == Player::MAGIC::LIGHTNING )
			{
				//  ���@�������[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE2 );

				//  ���@������Ԃ�
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  ���s��Ԃ�
				magic->SetActive( );
			}
			else if( magicType == Player::MAGIC::TORNADE )
			{
				//  ���@�������[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE3 );

				//  ���@������Ԃ�
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  ���s��Ԃ�
				magic->SetActive( );
			}
		}

#else

		if( pPS4Input->GetRelease( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_R2 ) )
		{
			if( magicType == Player::MAGIC::FIRE )
			{
				//  ���@�������[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE );

				//  ���@������Ԃ�
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  ���s��Ԃ�
				magic->SetActive( );
			}
			else if( magicType == Player::MAGIC::LIGHTNING )
			{
				//  ���@�������[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE2 );

				//  ���@������Ԃ�
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  ���s��Ԃ�
				magic->SetActive( );
			}
			else if( magicType == Player::MAGIC::TORNADE )
			{
				//  ���@�������[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE3 );

				//  ���@������Ԃ�
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_ACTIVE ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

				//  �G�t�F�N�g�̒�~
				EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

				//  ���s��Ԃ�
				magic->SetActive( );
			}
		}

#endif

	}
}