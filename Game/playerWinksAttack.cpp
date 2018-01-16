//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �U�� )   ( playerWinksAttack.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksAttack.h"
#include "playerWinks.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "fbxLoader.h"
#include "enemyRauder.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------------
static const int	PLAYER_WINKS_ATTACK01_RIGIDTIME = 20;		//  �U��01�d������
static const int	PLAYER_WINKS_ATTACK02_RIGIDTIME = 30;		//  �U��02�d������
static const int	PLAYER_WINKS_ATTACK03_RIGIDTIME = 40;		//  �U��03�d������
static const int	PLAYER_WINKS_ATTACK_NUMBER = 3;				//  �U����

static const float	PLAYER_ENABLE_ATTACK_DIST = 80.0f;			//  �ǔ��\����

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;		//  �U�������蔻��

static const float	PLAYER_ATTACK_MOVE_RATE_XZ = 0.03f;			//  �U���ǔ����̈ړ�����( XZ���W )
static const float	PLAYER_ATTACK_MOVE_RATE_Y = 0.001f;			//  �U���ǔ����̈ړ�����( Y���W )

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::Init( void )
{
	//  �e�̏�����
	PlayerWinksState::Init( );

	//  �d�����Ԃ̏�����
	m_rigidTime = PLAYER_WINKS_ATTACK01_RIGIDTIME;

	//  �U���ԍ��̏�����
	m_motionState = MOTION_STATE::ATTACK01;

	//  �t���[���𔭐��������ǂ����̃t���O�̏�����
	m_effekseerFrame = false;

	//  �U�����[�V�����ɕύX
	GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK01 );

	Utility::HIT_SPHERE	attackHitSphere;

	//  �U�����̓����蔻��̑��
	D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

	float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

	//  �G�t�F�N�g�̐���
	m_effekseerHandle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
													   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );

	//  �G�t�F�N�g�̐���
	m_effekseerHandle[ 1 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
													   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::Update( void )
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
void PlayerWinksAttack::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::Action( void )
{
	if( GetPlayer( )->GetMotionFrameRate( ) < 0.3f )
	{
		//  �^�[�Q�b�g�ɏ��X�ɋ߂Â���
		MoveTarget( );
	}

	//  ���̃��[�V�����Ɉڍs���邩�̔��f
	JudgeNextMotion( );

	if( m_motionState == MOTION_STATE::ATTACK03 && GetPlayer( )->GetMotionFrame( ) == 20 )
	{
		Utility::HIT_SPHERE	attackHitSphere;

		//  �U�����̓����蔻��̑��
		D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 10.0f;
		attackHitSphere.position = attackPosition;
		attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  �G�t�F�N�g�̐���
		m_effekseerHandle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH003 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
															D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );
	}

	if( m_motionState == MOTION_STATE::ATTACK03 && GetPlayer( )->GetMotionFrameRate( ) > 0.8f && m_effekseerFrame == false )
	{
		Utility::HIT_SPHERE	attackHitSphere;

		//  �U�����̓����蔻��̑��
		D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 10.0f;
		attackHitSphere.position = attackPosition;
		attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_THUNDER , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) , 1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_SYLPH , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , 1.0f );

		//  �G�t�F�N�g������Ԃ�
		m_effekseerFrame = true;
	}
}

//--------------------------------------------------------------------------------------
//  �^�[�Q�b�g�ɏ��X�ɋ߂Â��֐�
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::MoveTarget( void )
{
	//  �ǔ���Ԃ̏ꍇ
	if( GetPlayer( )->GetHomingDirect( ) )
	{
		//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
		D3DXVECTOR3 playerPosition = GetPlayer( )->GetPos( );

		//  �^�[�Q�b�g�̍��W
		D3DXVECTOR3 targetPosition = GetTargetPosition( );

		//  ���W�����X�ɋ߂Â���
		playerPosition.x += ( targetPosition.x - playerPosition.x ) * PLAYER_ATTACK_MOVE_RATE_XZ;
		playerPosition.y += ( targetPosition.y - playerPosition.y ) * PLAYER_ATTACK_MOVE_RATE_Y;
		playerPosition.z += ( targetPosition.z - playerPosition.z ) * PLAYER_ATTACK_MOVE_RATE_XZ;

		//  �v���C���[���W�ƒ����_�̐ݒ�
		GetPlayer( )->SetPosition( playerPosition );
		GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
	}
}

//--------------------------------------------------------------------------------------
//  ���̃��[�V�����Ɉڍs���邩�̔��f������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksAttack::JudgeNextMotion( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	Utility::HIT_SPHERE	attackHitSphere;

	//  �U�����̓����蔻��̑��
	D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

	//  ���[�V�������I������ꍇ
	if( GetPlayer( )->GetMotionFinish( ) )
	{
		//  �d�����Ԃ�����ꍇ
		if( m_rigidTime > 0 )
		{

#ifdef KEYBOARD_ENABLE

			int nKey = 0;

			if( GetPlayer( )->GetPlayerNo( ) == 0 )
			{
				nKey = DIK_C;
			}
			else if( GetPlayer( )->GetPlayerNo( ) == 1 )
			{
				nKey = DIK_K;
			}

			if( ( int )m_motionState < ( int )MOTION_STATE::MAX - 1 )
			{
				if( pKeyboard->GetKeyboardTrigger( nKey ) )
				{
					if( m_motionState == MOTION_STATE::ATTACK01 )
					{
						//  ���̍U����ԂɕύX
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK02 );
						m_motionState = MOTION_STATE::ATTACK02;

						m_rigidTime = PLAYER_WINKS_ATTACK02_RIGIDTIME;

						float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

						//  �G�t�F�N�g�̐���
						m_effekseerHandle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
																		   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );

						//  �G�t�F�N�g�̐���
						m_effekseerHandle[ 1 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
																		   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );
					}
					else if( m_motionState == MOTION_STATE::ATTACK02 )
					{
						//  ���̍U����ԂɕύX
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK03 );
						m_motionState = MOTION_STATE::ATTACK03;

						m_rigidTime = PLAYER_WINKS_ATTACK03_RIGIDTIME;
					}

					if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_ATTACK_DIST )
					{
						//  �^�[�Q�b�g�����Ɍ�������
						LookTargetXZ( );

						//  �ǔ���Ԃ�
						GetPlayer( )->SetHomingDirect( true );
					}
					else
					{
						//  �ǔ����Ȃ���Ԃ�
						GetPlayer( )->SetHomingDirect( false );
					}
				}
			}

#else

			if( ( int )m_motionState < ( int )MOTION_STATE::MAX - 1 )
			{
				if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
				{
					if( m_motionState == MOTION_STATE::ATTACK01 )
					{
						//  ���̍U����ԂɕύX
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK02 );
						m_motionState = MOTION_STATE::ATTACK02;

						m_rigidTime = PLAYER_WINKS_ATTACK02_RIGIDTIME;

						float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

						//  �G�t�F�N�g�̐���
						m_effekseerHandle[ 0 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
																		   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );

						//  �G�t�F�N�g�̐���
						m_effekseerHandle[ 1 ] = EffekseerManager::Create( EffekseerManager::TYPE_SLASH001 , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
																		   D3DXVECTOR3( 0.0f , fAngle - D3DX_PI * 0.5f , 0.0f ) , D3DXVECTOR3( 3.5f , 3.5f , 3.5f ) , 1.0f );
					}
					else if( m_motionState == MOTION_STATE::ATTACK02 )
					{
						//  ���̍U����ԂɕύX
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK03 );
						m_motionState = MOTION_STATE::ATTACK03;

						m_rigidTime = PLAYER_WINKS_ATTACK03_RIGIDTIME;
					}

					if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_ATTACK_DIST )
					{
						//  �^�[�Q�b�g�����Ɍ�������
						LookTargetXZ( );

						//  �ǔ���Ԃ�
						GetPlayer( )->SetHomingDirect( true );
					}
					else
					{
						//  �ǔ����Ȃ���Ԃ�
						GetPlayer( )->SetHomingDirect( false );
					}
				}
			}

#endif

			//  �d�����Ԃ̃J�E���g
			m_rigidTime--;
		}
		else
		{
			//  �ҋ@��Ԃ�
			JudgeChangeIdle( );
		}
	}

	//  �G�t�F�N�V�A�̍��W����
	EffekseerManager::SetPosition( m_effekseerHandle[ 0 ] , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) );
	EffekseerManager::SetPosition( m_effekseerHandle[ 1 ] , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) );
}