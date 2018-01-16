//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �U�� )   ( playerRauderAttack.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerRauderAttack.h"
#include "playerRauder.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "fbxLoader.h"
#include "enemyRauder.h"

//--------------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------------
static const int	PLAYER_WINKS_ATTACK01_RIGIDTIME = 10;		//  �U��01�d������
static const int	PLAYER_WINKS_ATTACK02_RIGIDTIME = 20;		//  �U��02�d������
static const int	PLAYER_WINKS_ATTACK03_RIGIDTIME = 40;		//  �U��03�d������
static const int	PLAYER_WINKS_ATTACK_NUMBER = 3;				//  �U����

static const float	PLAYER_ENABLE_ATTACK_DIST = 80.0f;			//  �ǔ��\����

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Init( void )
{
	//  �e�̏�����
	PlayerRauderState::Init( );

	//  �d�����Ԃ̏�����
	m_rigidTime = PLAYER_WINKS_ATTACK01_RIGIDTIME;

	//  �U���ԍ��̏�����
	m_attackNo = 0;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Update( void )
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
void PlayerRauderAttack::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::Action( void )
{
	//  �^�[�Q�b�g�ɏ��X�ɋ߂Â���
	MoveTarget( );

	//  ���̃��[�V�����Ɉڍs���邩�̔��f
	JudgeNextMotion( );
}

//--------------------------------------------------------------------------------------
//  �^�[�Q�b�g�ɏ��X�ɋ߂Â��֐�
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::MoveTarget( void )
{
	//  �ǔ���Ԃ̏ꍇ
	if( GetPlayer( )->GetHomingDirect( ) )
	{
		//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
		D3DXVECTOR3 playerPosition = GetPlayer( )->GetPos( );

		//  �^�[�Q�b�g�̍��W
		D3DXVECTOR3 targetPosition;

		//  �V�[���N���X�̃|�C���^
		Scene* pScene = NULL;		

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
						PlayerRauder* player = ( PlayerRauder* )pScene;

						//  �����ȊO�̃v���C���[�ԍ��̏ꍇ
						if( GetPlayer( )->GetPlayerNo( ) != player->GetPlayerNo( ) )
						{
							D3DXVECTOR3 targetPosition = player->GetPos( );
						}
					}
				}

				//  ���̃|�C���^����
				pScene = pScene->GetNextScene( pScene );
			}
		}

		//  ���W�����X�ɋ߂Â���
		playerPosition.x += ( targetPosition.x - playerPosition.x ) * 0.01f;
		playerPosition.y += ( targetPosition.y - playerPosition.y ) * 0.003f;
		playerPosition.z += ( targetPosition.z - playerPosition.z ) * 0.01f;

		//  �v���C���[���W�ƒ����_�̐ݒ�
		GetPlayer( )->SetPosition( playerPosition );
		GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );
	}
}

//--------------------------------------------------------------------------------------
//  ���̃��[�V�����Ɉڍs���邩�̔��f������֐�
//--------------------------------------------------------------------------------------
void PlayerRauderAttack::JudgeNextMotion( void )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

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

			if( m_attackNo < PLAYER_WINKS_ATTACK_NUMBER - 1 )
			{
				if( pKeyboard->GetKeyboardTrigger( nKey ) )
				{
					if( m_attackNo == 0 )
					{
						//  ���̍U����ԂɕύX
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK02 );

						m_rigidTime = PLAYER_WINKS_ATTACK02_RIGIDTIME;
					}
					else if( m_attackNo == 1 )
					{
						//  ���̍U����ԂɕύX
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK03 );

						m_rigidTime = PLAYER_WINKS_ATTACK03_RIGIDTIME;
					}

					if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_ATTACK_DIST )
					{
						//  �ǔ���Ԃ�
						GetPlayer( )->SetHomingDirect( true );
					}
					else
					{
						//  �ǔ����Ȃ���Ԃ�
						GetPlayer( )->SetHomingDirect( false );
					}

					//  �U���ԍ���i�߂�
					m_attackNo++;
				}
			}

#else

			if( m_attackNo < PLAYER_WINKS_ATTACK_NUMBER - 1 )
			{
				if( pPS4Input->GetTrigger( GetPlayer( )->GetPlayerNo( ) , PS4Controller::DIJ_CIRCLE ) )
				{
					if( m_attackNo == 0 )
					{
						//  ���̍U����ԂɕύX
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK02 );

						m_rigidTime = PLAYER_WINKS_ATTACK02_RIGIDTIME;
					}
					else if( m_attackNo == 1 )
					{
						//  ���̍U����ԂɕύX
						GetPlayer( )->SetMotion( FBXLoader::MOTION::ATTACK03 );

						m_rigidTime = PLAYER_WINKS_ATTACK03_RIGIDTIME;
					}

					if( GetPlayer( )->GetTargetDistance( ) <= PLAYER_ENABLE_ATTACK_DIST )
					{
						//  �ǔ���Ԃ�
						GetPlayer( )->SetHomingDirect( true );
					}
					else
					{
						//  �ǔ����Ȃ���Ԃ�
						GetPlayer( )->SetHomingDirect( false );
					}

					//  �U���ԍ���i�߂�
					m_attackNo++;
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
}