//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g  ( playerWinksState.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/28
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinks.h"
#include "playerWinksState.h"
#include "fbxLoader.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"
#include "enemyRauder.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float	PLAYER_TO_CAMERA_DISTANCE_Y		= 15.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Y2	= 17.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z		= 30.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z2	= 35.0f;

static const float	PLAYER_GRAVITY2					= 0.2f;
static const float	CAMERA_ROTATE_ANGLE				= 0.025f;
static const float	ESCAPE_POWER					= 1.0f;

static const float	PLAYER_ENABLE_BRAVE_ATTACK_DIST	= 80.0f;

static const int	PLAYER_GUARD_FRAME				= 2;

static const float	PLAYER_HIT_SPHERE_POS_Y			= 3.0f;

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  ����������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::Init( void )
{
	m_moveHeight = 0.0f;
	m_pressTime = 0;
}

//--------------------------------------------------------------------------------------
//  �v���C���[�f�[�^�̐ݒ������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::SetPlayer( PlayerWinks* playerWinks )
{
	if( playerWinks == nullptr )
	{
		return;
	}

	m_player = playerWinks;
}

//--------------------------------------------------------------------------------------
//  �v���C���[�f�[�^�̎擾������֐�
//--------------------------------------------------------------------------------------
PlayerWinks* PlayerWinksState::GetPlayer( void )
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
void PlayerWinksState::AddGravity( void )
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
void PlayerWinksState::ControlCamera( void )
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
							pCamera->SetCameraPosAt( posEnemy , 0.3f );
						}

						D3DXVECTOR3 workPos;
						D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
						workVec.y = 0.0f;

						D3DXVec3Normalize( &workVec , &workVec );

						workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
						workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;

						//  �J�����̎��_�ݒ�
						pCamera->SetCameraPosEye( workPos , 0.3f );

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
						PlayerWinks* pPlayer = ( PlayerWinks* )pScene;

						if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
						{
							D3DXVECTOR3 posEnemy;						//  �G�̍��W

							//  �G���̎擾
							posEnemy = pScene->GetPos( );

							float fDistance = sqrtf( ( posEnemy.x - GetPlayer( )->GetPos( ).x ) * ( posEnemy.x - GetPlayer( )->GetPos( ).x ) +
													 ( posEnemy.z - GetPlayer( )->GetPos( ).z ) * ( posEnemy.z - GetPlayer( )->GetPos( ).z ) );

							//  �J�����̒����_�ݒ�
							pCamera->SetCameraPosAt( posEnemy , 0.3f );

							D3DXVECTOR3 workPos;
							D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
							workVec.y = 0.0f;

							D3DXVec3Normalize( &workVec , &workVec );

							workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;

							//  �J�����̎��_�ݒ�
							pCamera->SetCameraPosEye( workPos , 0.3f );

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
void PlayerWinksState::ChangeLockon( void )
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
//  �^�[�Q�b�g�����Ɍ����֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::LookTarget( void )
{
	//  �^�[�Q�b�g�̕����ɐi�s�����x�N�g����ύX
	D3DXVECTOR3 workVecDirect = GetTargetPosition( ) - GetPlayer( )->GetPos( );
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetPlayer( )->SetVecDirect( workVecDirect );
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
}

//--------------------------------------------------------------------------------------
//  �^�[�Q�b�g����( Y���� )�Ɍ����֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::LookTargetXZ( void )
{
	//  �^�[�Q�b�g�̕����ɐi�s�����x�N�g����ύX
	D3DXVECTOR3 workVecDirect = GetTargetPosition( ) - GetPlayer( )->GetPos( );
	workVecDirect.y = 0.0f;
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetPlayer( )->SetVecDirect( workVecDirect );
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
}

//--------------------------------------------------------------------------------------
//  �^�[�Q�b�g���W�̎擾����֐�
//--------------------------------------------------------------------------------------
D3DXVECTOR3	PlayerWinksState::GetTargetPosition( void )
{
	//  �V�[���N���X�̃|�C���^
	Scene* pScene = NULL;

	//  �^�[�Q�b�g�̍��W
	D3DXVECTOR3 targetPosition;

#pragma omp parallel for
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
					//  �_�E���L���X�g
					EnemyRauder* enemy = ( EnemyRauder* )pScene;

					targetPosition = enemy->GetPos( );
				}
			}
			//  �v���C���[�ΐ탂�[�h�̏ꍇ
			else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
			{
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  �_�E���L���X�g
					PlayerWinks* player = ( PlayerWinks* )pScene;

					//  �����ȊO�̃v���C���[�ԍ��̏ꍇ
					if( m_player->GetPlayerNo( ) != player->GetPlayerNo( ) )
					{
						D3DXVECTOR3 targetPosition = player->GetPos( );
					}
				}
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	return targetPosition;
}

//--------------------------------------------------------------------------------------
//  �ҋ@��ԂɕύX���邩�̔��f������֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeIdle( void )
{
	//  ���[�V�������I�����Ă���ꍇ
	if( GetPlayer( )->GetMotionFinish( ) )
	{
		//  �ҋ@��Ԃ�
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::IDLE ) );
	}
}

//--------------------------------------------------------------------------------------
//  �ړ���ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeMove( void )
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
		//  �X�e�[�g�̕ύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::MOVE ) );
	}
}

//--------------------------------------------------------------------------------------
//  �W�����v��ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeJump( void )
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
		//  �W�����v��Ԃ�
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::JUMP ) );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CROSS ) )
	{
		//  �W�����v��Ԃ�
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::JUMP ) );
	}

#endif

}

//--------------------------------------------------------------------------------------
//  �U����ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeAttack( void )
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

	D3DXVECTOR3 workVecDirect( 0.0f , 0.0f , 0.0f );

	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	//  �J���������x�N�g��
	D3DXVECTOR3 cameraVecDirect;

	cameraVecDirect = pCamera->GetCameraVecDirect( );

	//  �����X�e�B�b�N��i�s�����x�N�g����
	if( move.x != 0.0f || move.z != 0.0f )
	{
		D3DXVec3Normalize( &move , &move );

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

		GetPlayer( )->SetVecDirect( workVecDirect );
	}

#ifdef KEYBOARD_ENABLE

	if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_BRAVE_ATTACK_DIST )
	{ 
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  �^�[�Q�b�g�����Ɍ�������
			LookTargetXZ( );

			//  �U����ԂɕύX
			GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK01 );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::ATTACK ) );

			//  �ǔ���Ԃ�
			GetPlayer( )->SetHomingDirect( true );
		}
	}
	else
	{
		if( pKeyboard->GetKeyboardTrigger( nKey ) )
		{
			//  �U����ԂɕύX
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::ATTACK ) );

			//  �ǔ���Ԃ�
			GetPlayer( )->SetHomingDirect( false );
		}
	}

#else

	if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_BRAVE_ATTACK_DIST )
	{
		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
		{
			//  �^�[�Q�b�g�����Ɍ�������
			LookTargetXZ( );

			//  �U����ԂɕύX
			GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK01 );
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::ATTACK ) );

			//  �ǔ���Ԃ�
			GetPlayer( )->SetHomingDirect( true );
		}
	}
	else
	{
		if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
		{
			//  �U����ԂɕύX
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::ATTACK ) );

			//  �ǔ���Ԃ�
			GetPlayer( )->SetHomingDirect( false );
		}
	}

#endif

}

//--------------------------------------------------------------------------------------
//  �_�b�V����ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeDash( void )
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
				//  ��Ԃ̕ύX
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::FREE_DASH ) );
			}
			else
			{
				//  ��Ԃ̕ύX
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::LOCKON_DASH ) );
			}
		}

#else

		//  Y��RB�𒷉������Ă����
		if( ( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_TRIANGLE ) ) )
		{
			if( move.x != 0.0f || move.z != 0.0f )
			{
				//  ��Ԃ̕ύX
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::FREE_DASH ) );
			}
			else
			{
				//  ��Ԃ̕ύX
				GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::LOCKON_DASH ) );
			}
		}

#endif
	}
}

//--------------------------------------------------------------------------------------
//  �h���ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeGuard( void )
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
			//  ��Ԃ̕ύX
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::GUARD ) );
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
			//  ��Ԃ̕ύX
			GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::GUARD ) );
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
//  HP�U����ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void PlayerWinksState::JudgeChangeHPAttack( void )
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
		//  HP�U���ɕύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::HP_ATTACK ) );
	}

#else

	if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_SQUARE ) )
	{
		//  HP�U���ɕύX
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( PlayerWinks::STATE::HP_ATTACK ) );
	}

#endif
}