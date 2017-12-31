//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �ːi�U�� )   ( playerRushAttack.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerRushAttack.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "effekseerManager.h"
#include "enemy.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float	PLAYER_TO_CAMERA_DISTANCE_Y		= 9.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Y2	= 10.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z		= 20.0f;
static const float	PLAYER_TO_CAMERA_DISTANCE_Z2	= 23.0f;

static const float	CAMERA_ROTATE_ANGLE				= 0.025f;

static const float	PLAYER_RUSH_ATTACK_SPEED		= 3.0f;
static const int	PLAYER_RUSH_ATTACK_DAMAGE		= 250;
static const float	PLAYER_RUSH_ATTACK_BLOW_POWER	= 7.0f;

static const float	PLAYER_HIT_SPHERE_POS_Y			= 3.0f;

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );

	//  ���x�̏�����
	m_speed = PLAYER_RUSH_ATTACK_SPEED;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Uninit( void )
{
	m_speed = 0.0f;
}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Update( void )
{
	//  ��p�̃J��������
	ControlCameraExclusive( );

	//  �A�N�V����
	Action( );

	//  �d�͂�������
	AddGravity( );

	//  �j���[�g������ԂɕύX���邩�̔��f
	JudgeChangeNeutral( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerRushAttack::Action( void )
{
	//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
	GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * m_speed );

	//  �����_�������Ă��������
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  �G�t�F�N�V�A���W�̐ݒ�
	D3DXVECTOR3 effekseerPos( GetPlayer( )->GetPos( ).x , GetPlayer( )->GetPos( ).y + PLAYER_HIT_SPHERE_POS_Y , GetPlayer( )->GetPos( ).z );
	effekseerPos += GetPlayer( )->GetVecDirect( ) * 8.0f;
	EffekseerManager::SetPosition( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_LANCE ) , effekseerPos );

	//  �L�[��������x�܂ł������ꍇ
	if( GetPlayer( )->GetAnimationKey( ) >= 11 )
	{
		//  ���x�̌���
		m_speed += ( 0.0f - m_speed ) * 0.06f;

		//  �G�t�F�N�g�̒�~
		EffekseerManager::Stop( GetPlayer( )->GetEffekseerHandle( EffekseerManager::TYPE_LANCE ) );
	}

	//  �U�����̓����蔻��̑��
	Utility::HIT_SPHERE attackHitSphere;
	attackHitSphere.position = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position.y += 2.0f;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

	//  �V�[���N���X�̃|�C���^
	Scene* pScene = NULL;

	//  �L�[�����͈͓̔��ɂ���ꍇ
	if( GetPlayer( )->GetAnimationKey( ) >= 1 && GetPlayer( )->GetAnimationKey( ) <= 10 )
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
						//  �_�E���L���X�g
						Enemy* pEnemy = ( Enemy* )pScene;

						if( Utility::HitSphere( attackHitSphere , pEnemy->GetHitSphere( ) ) )
						{
							pEnemy->Damage( PLAYER_RUSH_ATTACK_DAMAGE );
						}
					}
				}
				//  �v���C���[�ΐ탂�[�h�̏ꍇ
				else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  �_�E���L���X�g
						Player* pPlayer = ( Player* )pScene;

						//  �����ȊO�̃v���C���[�ԍ��̏ꍇ
						if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
						{
							if( Utility::HitSphere( attackHitSphere , pPlayer->GetHitSphere( ) ) )
							{
								//  ������ѕ����̃x�N�g���Z�o
								D3DXVECTOR3 blowVecDirect = GetPlayer( )->GetVecDirect( );
								D3DXMATRIX mtxRot;
								D3DXMatrixIdentity( &mtxRot );
								D3DXMatrixRotationY( &mtxRot , D3DX_PI * 0.1f );
								D3DXVec3TransformNormal( &blowVecDirect , &blowVecDirect , &mtxRot );
								D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

								pPlayer->Damage( blowVecDirect , PLAYER_RUSH_ATTACK_BLOW_POWER , PLAYER_RUSH_ATTACK_DAMAGE );
							}
						}
					}
				}

				//  ���̃|�C���^����
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  ��p�̃J�������������֐�
//--------------------------------------------------------------------------------------
void PlayerRushAttack::ControlCameraExclusive( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = NULL;

	pPS4Input = SceneManager::GetPS4Input( );

	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( GetPlayer( )->GetPlayerNo( ) );

	//  �V�[���N���X�̃|�C���^
	Scene* pScene = NULL;			

	//  �V�[���N���X�̃|�C���^
	Scene *pScene2 =NULL;										

	//  �J���������x�N�g��
	D3DXVECTOR3 cameraVecDirect;

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

					//  �J�����̒����_�ݒ�
					pCamera->SetCameraPosAt( posEnemy , 0.1f );

					D3DXVECTOR3 workPos;
					D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
					workVec.y = 0.0f;

					D3DXVec3Normalize( &workVec , &workVec );

					//  CPU�ΐ�̏ꍇ
					if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
					{
						workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
						workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
					}
					//  �v���C���[�ΐ�̏ꍇ
					else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
					{
						workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
						workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
					}

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

						//if( fDistance > 8.0f )
						{
							//  �J�����̒����_�ݒ�
							pCamera->SetCameraPosAt( posEnemy , 0.8f );
						}

						D3DXVECTOR3 workPos;
						D3DXVECTOR3 workVec = posEnemy - GetPlayer( )->GetPos( );
						workVec.y = 0.0f;

						D3DXVec3Normalize( &workVec , &workVec );

						//  CPU�ΐ�̏ꍇ
						if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
						{
							workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y;
						}
						//  �v���C���[�ΐ�̏ꍇ
						else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
						{
							workPos = GetPlayer( )->GetPos( ) - workVec * PLAYER_TO_CAMERA_DISTANCE_Z2;
							workPos.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
						}

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