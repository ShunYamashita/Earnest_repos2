//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g  ( playerState.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/28
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
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float	PLAYER_TO_CAMERA_DISTANCE_Y		= 9.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Y2	= 10.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z		= 20.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z2	= 23.0f;

static const float	PLAYER_GRAVITY2					= 0.2f;
static const float	CAMERA_ROTATE_ANGLE				= 0.025f;
static const float	ESCAPE_POWER					= 1.0f;

static const float	PLAYER_ENABLE_BRAVE_ATTACK_DIST	= 80.0f;

static const int	PLAYER_GUARD_FRAME				= 2;

static const float	PLAYER_HIT_SPHERE_POS_Y			= 3.0f;

//--------------------------------------------------------------------------------------
//  �d�͂�������֐�
//--------------------------------------------------------------------------------------
void PlayerState::Init( void )
{
	m_moveHeight = 0.0f;
	m_pressTime = 0;
}

//--------------------------------------------------------------------------------------
//  �v���C���[�f�[�^�̐ݒ������֐�
//--------------------------------------------------------------------------------------
void PlayerState::SetPlayer( Player* player )
{
	if( player == nullptr )
	{
		return;
	}

	m_player = player;
}

//--------------------------------------------------------------------------------------
//  �v���C���[�f�[�^�̎擾������֐�
//--------------------------------------------------------------------------------------
Player* PlayerState::GetPlayer( void )
{
	if( m_player == nullptr )
	{
		return nullptr;
	}

	return m_player;
}

//--------------------------------------------------------------------------------------
//  �d�͂�������֐�
//--------------------------------------------------------------------------------------
void PlayerState::AddGravity( void )
{
	//  �n�ʂƂ̓������Ă��Ȃ��ꍇ
	if( GetPlayer( )->JudgeHitGround( ) == false )
	{
		//  �������ɗ͂�������
		m_moveHeight -= PLAYER_GRAVITY2;

		//  Y���ړ�
		GetPlayer( )->MoveHeight( m_moveHeight );
	}
	else
	{
		//  �������̗͏�����
		m_moveHeight = 0.0f;
	}
}

//--------------------------------------------------------------------------------------
//  �J�������������֐�
//--------------------------------------------------------------------------------------
void PlayerState::ControlCamera( void )
{
	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	//  �V�[���N���X�̃|�C���^
	Scene* pScene = NULL;			

	//  �J���������x�N�g��
	D3DXVECTOR3 cameraVecDirect;

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	if( GetPlayer( )->GetLockon( ) == false )
	{
		D3DXVECTOR3 workPosEye = GetPlayer( )->GetPos( );

		//  CPU�ΐ�̏ꍇ
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y;
			workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
		}
		//  �v���C���[�ΐ�̏ꍇ
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
			workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
		}

		//  �J�����̎��_�ݒ�
		pCamera->SetCameraPosEye( workPosEye , 1.0f );

		//  �����_���W�̑��
		GetPlayer( )->GetPositionAt( ) = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f;

		D3DXVECTOR3 workPosAt = GetPlayer( )->GetPos( );

		//  CPU�ΐ�̏ꍇ
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			workPosAt.y += PLAYER_TO_CAMERA_DISTANCE_Y * 0.7f;
			workPosAt += cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
		}
		//  �v���C���[�ΐ�̏ꍇ
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			workPosAt.y += PLAYER_TO_CAMERA_DISTANCE_Y2 * 0.7f;
			workPosAt += cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
		}

		//  �J�����̒����_�ݒ�
		pCamera->SetCameraPosAt( workPosAt , 1.0f );
	}
	else
	{
		//  �D��x�̍ő吔���̃��[�v
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  �V�[���̐擪�A�h���X���擾
			pScene = Scene::GetScene( nCntPriority );

			//  �V�[������ł͂Ȃ��ԃ��[�v
			while( pScene != NULL )
			{
				Scene::OBJTYPE objType;						//  ���̂̎��

				//  ���̂̎�ނ̎擾
				objType = pScene->GetObjType( );
				
				//  CPU�ΐ탂�[�h�̏ꍇ
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  ��ނ��G�̏ꍇ
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						D3DXVECTOR3 posEnemy;						//  �G�̍��W

						//  �G���̎擾
						posEnemy = pScene->GetPos( );

						float fDistance = sqrtf( ( posEnemy.x - GetPlayer( )->GetPos( ).x ) * ( posEnemy.x - GetPlayer( )->GetPos( ).x ) +
												 ( posEnemy.z - GetPlayer( )->GetPos( ).z ) * ( posEnemy.z - GetPlayer( )->GetPos( ).z ) );

						//if( fDistance > 10.0f )
						{
							//  �J�����̒����_�ݒ�
							pCamera->SetCameraPosAt( posEnemy , 0.1f );
						}

						D3DXVECTOR3 workPos;
						D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
						workVec.y = 0.0f;

						D3DXVec3Normalize( &workVec , &workVec );

						workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
						workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;

						//  �J�����̎��_�ݒ�
						pCamera->SetCameraPosEye( workPos , 0.1f );

						break;
					}
				}
				//  �v���C���[�ΐ탂�[�h�̏ꍇ
				if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					//  ��ނ��v���C���[�̏ꍇ
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  �_�E���L���X�g
						Player* pPlayer = ( Player* )pScene;

						if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
						{
							D3DXVECTOR3 posEnemy;						//  �G�̍��W

							//  �G���̎擾
							posEnemy = pScene->GetPos( );

							float fDistance = sqrtf( ( posEnemy.x - GetPlayer( )->GetPos( ).x ) * ( posEnemy.x - GetPlayer( )->GetPos( ).x ) +
													 ( posEnemy.z - GetPlayer( )->GetPos( ).z ) * ( posEnemy.z - GetPlayer( )->GetPos( ).z ) );

							//  �J�����̒����_�ݒ�
							pCamera->SetCameraPosAt( posEnemy , 0.05f );

							D3DXVECTOR3 workPos;
							D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
							workVec.y = 0.0f;

							D3DXVec3Normalize( &workVec , &workVec );

							workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;

							//  �J�����̎��_�ݒ�
							pCamera->SetCameraPosEye( workPos , 0.1f );

							break;
						}
					}
				}

				//  ���̃|�C���^����
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}

	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = NULL;

	pPS4Input = SceneManager::GetPS4Input( );

	//  ���b�N�I����Ԃł͂Ȃ��ꍇ
	if( GetPlayer( )->GetLockon( ) == false )
	{
		//  �J���������_�؂�ւ�����
		if( pPS4Input->GetRightStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x > 300 ||
			pPS4Input->GetRightStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x < -300 ||
			pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) || pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
		{
			if( pPS4Input->GetRightStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x < 0 ||
				pKeyboard->GetKeyboardPress( DIK_LEFTARROW ) )
			{
				D3DXVECTOR3 cameraPosAt = pCamera->GetCameraBasePosAt( );
				D3DXVECTOR3 cameraPosEye = pCamera->GetCameraBasePosEye( );
				D3DXMATRIX mtxRot;
				float fAngle = atan2f( cameraPosAt.z - cameraPosEye.z , cameraPosAt.x - cameraPosEye.x );
				float fLength = sqrtf( ( cameraPosAt.z - cameraPosEye.z ) * ( cameraPosAt.z - cameraPosEye.z ) +
										( cameraPosAt.x - cameraPosEye.x ) * ( cameraPosAt.x - cameraPosEye.x ) );

				fAngle += CAMERA_ROTATE_ANGLE;

				cameraPosAt.x = cosf( fAngle ) * fLength + cameraPosEye.x;
				cameraPosAt.z = sinf( fAngle ) * fLength + cameraPosEye.z;

				//  �J�����̒����_�ݒ�
				pCamera->SetCameraPosAt( cameraPosAt , 1.0f );
			}
			else if( pPS4Input->GetRightStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x > 0 ||
					 pKeyboard->GetKeyboardPress( DIK_RIGHTARROW ) )
			{
				D3DXVECTOR3 cameraPosAt = pCamera->GetCameraBasePosAt( );
				D3DXVECTOR3 cameraPosEye = pCamera->GetCameraBasePosEye( );
				D3DXMATRIX mtxRot;
				float fAngle = atan2f( cameraPosAt.z - cameraPosEye.z , cameraPosAt.x - cameraPosEye.x );
				float fLength = sqrtf( ( cameraPosAt.z - cameraPosEye.z ) * ( cameraPosAt.z - cameraPosEye.z ) +
										( cameraPosAt.x - cameraPosEye.x ) * ( cameraPosAt.x - cameraPosEye.x ) );

				fAngle -= CAMERA_ROTATE_ANGLE;

				cameraPosAt.x = cosf( fAngle ) * fLength + cameraPosEye.x;
				cameraPosAt.z = sinf( fAngle ) * fLength + cameraPosEye.z;

				//  �J�����̒����_�ݒ�
				pCamera->SetCameraPosAt( cameraPosAt , 1.0f );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  ���b�N�I����Ԃ̕ύX������֐�
//--------------------------------------------------------------------------------------
void PlayerState::ChangeLockon( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_R;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_RETURN;
	}

#ifdef KEYBOARD_ENABLE

	//  ENTER�L�[�܂��́AL1�{�^�����������ꍇ
	if( ( pKeyboard->GetKeyboardTrigger( nKey ) ) )
	{
		//  ���b�N�I���̐؂�ւ�
		GetPlayer( )->ChangeLockon( );
	}

#else

	//  ENTER�L�[�܂��́AL1�{�^�����������ꍇ
	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_L1 ) )
	{
		//  ���b�N�I���̐؂�ւ�
		GetPlayer( )->ChangeLockon( );
	}

#endif

}

//--------------------------------------------------------------------------------------
//  �j���[�g������ԂɕύX���邩�̔��f������֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeNeutral( void )
{
	//  �A�j���[�V�������I�����Ă���ꍇ
	if( GetPlayer( )->GetAnimationFinish( ) )
	{
		//  �j���[�g�������[�V�����̕ύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_NEUTRAL );

		//  �j���[�g������Ԃ�
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::NEUTRAL ) );
	}
}

//--------------------------------------------------------------------------------------
//  �ړ���ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeMove( void )
{
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

	if( move.x != 0 ||
		move.z != 0 )
	{
		//  ���[�V�����̕ύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_MOVE );

		//  �X�e�[�g�̕ύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MOVE ) );
	}
}

//--------------------------------------------------------------------------------------
//  �W�����v��ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeJump( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

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

		//  �W�����v��Ԃ�
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::JUMP ) );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CROSS ) )
	{
		//  �W�����v�A�j���[�V������
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_JUMP );

		//  �W�����v��Ԃ�
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::JUMP ) );
	}

#endif

}

//--------------------------------------------------------------------------------------
//  HP�U��000��ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeHPAttack000( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_B;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_SEMICOLON;
	}

#ifdef KEYBOARD_ENABLE

	if( pKeyboard->GetKeyboardTrigger( nKey ) )
	{
		//  ���[�V�����̕ύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP001 );

		//  �u���C�u�U���ɕύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_HP_ATTACK000 ) );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_SQUARE ) )
	{
		//  ���[�V�����̕ύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP001 );

		//  �u���C�u�U���ɕύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_HP_ATTACK000 ) );
	}

#endif
}

//--------------------------------------------------------------------------------------
//  �u���C�u�U��000��ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeBraveAttack000( void )
{
	int nKey = 0;

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

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_C;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_K;
	}

#ifdef KEYBOARD_ENABLE

	if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_BRAVE_ATTACK_DIST )
	{ 
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  �u���C�u�U��000��ԂɕύX
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_BEFORE );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_BEFORE ) );

			if( move.x >= 0.0f )
			{
				//  �E���ǔ���Ԃ�
				GetPlayer( )->SetHomingDirect( false );
			}
			else
			{
				//  �����ǔ���Ԃ�
				GetPlayer( )->SetHomingDirect( true );
			}
		}
	}
	else
	{
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  �u���C�u�U��000��ԂɕύX
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  �U�����̓����蔻��̑��
			D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;

			//  �G�t�F�N�g�̐���
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
																   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , effekseerPosition.y , GetPlayer( )->GetPos( ).z ) ,
																   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );

		}
	}

#else

	if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_BRAVE_ATTACK_DIST )
	{
		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
		{
			//  �u���C�u�U��000��ԂɕύX
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_BEFORE );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_BEFORE ) );

			if( move.x >= 0.0f )
			{
				//  �E���ǔ���Ԃ�
				GetPlayer( )->SetHomingDirect( false );
			}
			else
			{
				//  �����ǔ���Ԃ�
				GetPlayer( )->SetHomingDirect( true );
			}
		}
	}
	else
	{
		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
		{
			//  �u���C�u�U��000��ԂɕύX
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  �U�����̓����蔻��̑��
			D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;

			//  �G�t�F�N�g�̐���
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
																   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , effekseerPosition.y , GetPlayer( )->GetPos( ).z ) ,
																   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );
		}
	}

#endif
}

//--------------------------------------------------------------------------------------
//  �����ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeEscape( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_G;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_P;
	}

	//  �ړ�����
	D3DXVECTOR3 move( 0.0f , 0.0f , 0.0f );

#ifdef KEYBOARD_ENABLE

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

#ifdef KEYBOARD_ENABLE

	if( pKeyboard->GetKeyboardTrigger( nKey ) && ( move.x != 0.0f || move.z != 0.0f ) )
	{
		//  �G�t�F�N�g�̒�~
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

		//  �G�t�F�N�g�̒�~
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

		//  ���@�̍폜
		GetPlayer( )->DeleteMagic( );

		D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

		//  ���x�̑��
		GetPlayer( )->SetSpeed( GetPlayer( )->GetBaseSpeed( ) );

		D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

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

		//  �ړ��͂̌v�Z
		GetPlayer( )->SetMovePower( ESCAPE_POWER );

		//  ���[�V�����̕ύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ESCAPE );

		//  ��Ԃ̕ύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ESCAPE ) );
	}
#else

	if( pPS4Input->GetPress( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CROSS ) && pPS4Input->GetPress( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_R1 ) &&
		( ( float )pPS4Input->GetLeftStickDisposition( GetPlayer( )->GetPlayerNo( ) ).x != 0.0f || ( float )pPS4Input->GetLeftStickDisposition( GetPlayer( )->GetPlayerNo( ) ).y != 0.0f ) )
	{
		//  �G�t�F�N�g�̒�~
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

		//  �G�t�F�N�g�̒�~
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

		//  ���@�̍폜
		GetPlayer( )->DeleteMagic( );

		D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

		//  ���x�̑��
		GetPlayer( )->SetSpeed( GetPlayer( )->GetBaseSpeed( ) );

		D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

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

		//  �ړ��͂̌v�Z
		GetPlayer( )->SetMovePower( ESCAPE_POWER );

		//  ���[�V�����̕ύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ESCAPE );

		//  ��Ԃ̕ύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ESCAPE ) );
	}

#endif
}

//--------------------------------------------------------------------------------------
//  �h���ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeGuard( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_F;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_O;
	}

#ifdef KEYBOARD_ENABLE

	//  R1���������ꍇ��
	if( pKeyboard->GetKeyboardPress( nKey ) )
	{
		//  ���t���[���ȏ�ɂȂ����ꍇ
		if( m_pressTime >= PLAYER_GUARD_FRAME )
		{
			//  ���[�V�����̕ύX
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_GUARD );

			//  ��Ԃ̕ύX
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::GUARD ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  �U�����̓����蔻��̑��
			D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
			effekseerPosition.y += 3.0f;

			//  �G�t�F�N�g�̐���
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
																	D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) ,
																	D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SHIELD , handle );
		}

		//  �������Ԃ̃J�E���g
		m_pressTime++;
	}
	else
	{
		//  �������Ԃ̏�����
		m_pressTime = 0;
	}

#else

	//  R1���������ꍇ��
	if( ( pPS4Input->GetPress( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_R1 ) ) )
	{
		//  ���t���[���ȏ�ɂȂ����ꍇ
		if( m_pressTime >= PLAYER_GUARD_FRAME )
		{
			//  ���[�V�����̕ύX
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_GUARD );

			//  ��Ԃ̕ύX
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::GUARD ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  �U�����̓����蔻��̑��
			D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
			effekseerPosition.y += 3.0f;

			//  �G�t�F�N�g�̐���
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
																	D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) ,
																	D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SHIELD , handle );
		}

		//  �������Ԃ̃J�E���g
		m_pressTime++;
	}
	else
	{
		//  �������Ԃ̏�����
		m_pressTime = 0;
	}

#endif
}

//--------------------------------------------------------------------------------------
//  ���b�N�I���_�b�V���ɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeLockonDash( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	//  ���b�N�I����Ԃ̏ꍇ
	if( GetPlayer( )->GetLockon( ) == true )
	{
		if( GetPlayer( )->GetPlayerNo( ) == 0 )
		{
			nKey = DIK_V;
		}
		else if( GetPlayer( )->GetPlayerNo( ) == 1 )
		{
			nKey = DIK_L;
		}

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

#ifdef KEYBOARD_ENABLE

		//  Y��RB�𒷉������Ă����
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			if( move.x != 0.0f || move.z != 0.0f )
			{
				//  ���[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

				//  ��Ԃ̕ύX
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::FREE_DASH ) );
			}
			else
			{
				//  ���[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

				//  ��Ԃ̕ύX
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::LOCKON_DASH ) );
			}
		}

#else

		//  Y��RB�𒷉������Ă����
		if( ( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_TRIANGLE ) ) )
		{
			if( move.x != 0.0f || move.z != 0.0f )
			{
				//  ���[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

				//  ��Ԃ̕ύX
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::FREE_DASH ) );
			}
			else
			{
				//  ���[�V�����̕ύX
				GetPlayer( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

				//  ��Ԃ̕ύX
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::LOCKON_DASH ) );
			}
		}

#endif
	}
}

//--------------------------------------------------------------------------------------
//  �ːi�U���ɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeRushAttack( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetPlayerNo( ) == 0 )
	{
		nKey = DIK_B;
	}
	else if( GetPlayer( )->GetPlayerNo( ) == 1 )
	{
		nKey = DIK_SEMICOLON;
	}

#ifdef KEYBOARD_ENABLE

	if( pKeyboard->GetKeyboardTrigger( nKey ) )
	{
		//  ���[�V�����̕ύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_RUSH_ATTACK );

		//  ��Ԃ̕ύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::RUSH_ATTACK ) );

		//  �p�x�̎Z�o
		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  �G�t�F�N�g�̐���
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_LANCE ,
																D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z ) ,
																D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.5f , 1.5f , 1.5f ) );
		GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_LANCE , handle );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_SQUARE ) )
	{
		//  ���[�V�����̕ύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_RUSH_ATTACK );

		//  ��Ԃ̕ύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::RUSH_ATTACK ) );

		//  �p�x�̎Z�o
		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  �G�t�F�N�g�̐���
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_LANCE ,
																D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z ) ,
																D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.5f , 1.5f , 1.5f ) );
		GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_LANCE , handle );
	}

#endif
}

//--------------------------------------------------------------------------------------
//  ���@������ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerState::JudgeChangeMagicReady( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	int nKey = 0;

	if( GetPlayer( )->GetMagic( ) == NULL )
	{
		if( GetPlayer( )->GetPlayerNo( ) == 0 )
		{
			nKey = DIK_H;
		}
		else if( GetPlayer( )->GetPlayerNo( ) == 1 )
		{
			nKey = DIK_AT;
		}

#ifdef KEYBOARD_ENABLE

		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  ���[�V�����̕ύX
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_READY );

			//  ��Ԃ̕ύX
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_READY ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  �G�t�F�N�g�̐���
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_AURA ,
																   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z ) ,
																   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.0f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_AURA , handle );

			//  �G�t�F�N�g�̐���
			handle = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE ,
											   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y , GetPlayer( )->GetPos( ).z ) ,
											   D3DXVECTOR3( -D3DX_PI * 0.5f , fAngle , 0.0f ) , D3DXVECTOR3( 5.0f , 5.0f , 5.0f ) , 1.0f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE , handle );

			D3DXVECTOR3 magicPos = GetPlayer( )->GetPos( );
			magicPos.y += 3.0f;

			Magic* magic = nullptr;

			if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::FIRE )
			{
				magic = MagicFire::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
										   D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}
			else if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::LIGHTNING )
			{
				magic = MagicLightning::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
												D3DXVECTOR3( 0.6f , 0.6f , 0.6f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}
			else if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::TORNADE )
			{
				magic = MagicTornade::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
											  D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}

			//  ���@�̐ݒ�
			GetPlayer( )->SetMagic( magic );
		}

#else

		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_R2 ) )
		{
			//  ���[�V�����̕ύX
			GetPlayer( )->SetAnimation( StateAnimator::MOTION_MAGIC_READY );

			//  ��Ԃ̕ύX
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::MAGIC_READY ) );

			float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

			//  �G�t�F�N�g�̐���
			::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_AURA ,
																   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z ) ,
																   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.0f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_AURA , handle );

			//  �G�t�F�N�g�̐���
			handle = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE ,
											   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y , GetPlayer( )->GetPos( ).z ) ,
											   D3DXVECTOR3( -D3DX_PI * 0.5f , fAngle , 0.0f ) , D3DXVECTOR3( 5.0f , 5.0f , 5.0f ) , 1.0f );
			GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE , handle );

			D3DXVECTOR3 magicPos = GetPlayer( )->GetPos( );
			magicPos.y += 3.0f;

			Magic* magic = nullptr;

			if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::FIRE )
			{
				magic = MagicFire::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
										   D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}
			else if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::LIGHTNING )
			{
				magic = MagicLightning::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
												D3DXVECTOR3( 0.6f , 0.6f , 0.6f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}
			else if( GetPlayer( )->GetMagicType( ) == Player::MAGIC::TORNADE )
			{
				magic = MagicTornade::Create( Magic::OWNER_PLAYER , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
											  D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetPlayer( )->GetVecDirect( ) , GetPlayer( )->GetPlayerNo( ) );
			}

			//  ���@�̐ݒ�
			GetPlayer( )->SetMagic( magic );
		}

#endif

	}
}