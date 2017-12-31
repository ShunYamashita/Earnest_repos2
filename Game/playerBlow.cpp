//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ������� )   ( playerBlow.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerBlow.h"
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
void PlayerBlow::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );

	//  �v���C���[�������Ă���G�t�F�N�V�A�S�Ē�~
	GetPlayer( )->StopAllEffekseer( );

	//  ������ї͂̎擾
	m_blowPower = GetPlayer( )->GetBlowPower( );
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerBlow::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerBlow::Update( void )
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
void PlayerBlow::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerBlow::Action( void )
{
	//  ���G���Ԃ̐ݒ�
	GetPlayer( )->SetInvisibleTime( 2 );

	//  ���x�̌���
	m_blowPower += ( 0.0f - m_blowPower ) * 0.04f;

	//  ������ї͕��̈ړ�
	GetPlayer( )->MovePos( GetPlayer( )->GetBlowVecDirect( ) * m_blowPower );

	//  �v���C���[�����x�N�g���̐ݒ�
	D3DXVECTOR3 playerVecDirect;
	D3DXVec3Normalize( &playerVecDirect , &( GetPlayer( )->GetPositionAt( ) - GetPlayer( )->GetPos( ) ) );
	GetPlayer( )->SetVecDirect( playerVecDirect );

	//  �����_�̐ݒ�
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) - GetPlayer( )->GetBlowVecDirect( ) * 5.0f );

	//  �A�j���[�V�������I�����邩�A������ї͂�������x�܂ŗ������ꍇ
	if( GetPlayer( )->GetAnimationFinish( ) == true || m_blowPower < 1.0f )
	{
		//  ���݂̐�����ї͂̐ݒ�
		GetPlayer( )->SetBlowPower( m_blowPower );

		//  �󂯐g�̏�Ԃ�
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_PASSIVE );
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::PASSIVE ) );
	}
}