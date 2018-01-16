//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g  ( playerState.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/28
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyRauder.h"
#include "enemyRauderState.h"
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
#include "playerWinks.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float	ENEMY_TO_CAMERA_DISTANCE_Y		= 10.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Y2	= 13.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z		= 25.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z2	= 30.0f;

static const float	ENEMY_GRAVITY2					= 0.2f;
static const float	CAMERA_ROTATE_ANGLE				= 0.025f;
static const float	ESCAPE_POWER					= 1.0f;

static const float	ENEMY_ENABLE_BRAVE_ATTACK_DIST	= 80.0f;

static const int	ENEMY_GUARD_FRAME				= 2;

static const float	ENEMY_HIT_SPHERE_POS_Y			= 3.0f;

//--------------------------------------------------------------------------------------
//  �d�͂�������֐�
//--------------------------------------------------------------------------------------
void EnemyRauderState::Init( void )
{
	m_moveHeight = 0.0f;
	m_pressTime = 0;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�f�[�^�̐ݒ������֐�
//--------------------------------------------------------------------------------------
void EnemyRauderState::SetEnemy( EnemyRauder* enemyRauder )
{
	if( enemyRauder == nullptr )
	{
		return;
	}

	m_enemy = enemyRauder;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�f�[�^�̎擾������֐�
//--------------------------------------------------------------------------------------
EnemyRauder* EnemyRauderState::GetEnemy( void )
{
	if( m_enemy == nullptr )
	{
		return nullptr;
	}

	return m_enemy;
}

//--------------------------------------------------------------------------------------
//  �d�͂�������֐�
//--------------------------------------------------------------------------------------
void EnemyRauderState::AddGravity( void )
{
	//  �n�ʂƂ̓������Ă��Ȃ��ꍇ
	if( GetEnemy( )->JudgeHitGround( ) == false )
	{
		//  �������ɗ͂�������
		m_moveHeight -= ENEMY_GRAVITY2;

		//  Y���ړ�
		GetEnemy( )->MoveHeight( m_moveHeight );
	}
	else
	{
		//  �������̗͏�����
		m_moveHeight = 0.0f;
	}
}

//--------------------------------------------------------------------------------------
//  ���b�N�I����Ԃ̕ύX������֐�
//--------------------------------------------------------------------------------------
void EnemyRauderState::ChangeLockon( void )
{
	//  ���b�N�I���̐؂�ւ�
	GetEnemy( )->ChangeLockon( );
}

//--------------------------------------------------------------------------------------
//  �^�[�Q�b�g�����Ɍ����֐�
//--------------------------------------------------------------------------------------
void EnemyRauderState::LookTargetXZ( void )
{
	//  �^�[�Q�b�g�̕����ɐi�s�����x�N�g����ύX
	D3DXVECTOR3 workVecDirect = GetTargetPosition( ) - GetEnemy( )->GetPos( );
	workVecDirect.y = 0.0f;
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetEnemy( )->SetVecDirect( workVecDirect );
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );
}

//--------------------------------------------------------------------------------------
//  �^�[�Q�b�g���W�̎擾����֐�
//--------------------------------------------------------------------------------------
D3DXVECTOR3	EnemyRauderState::GetTargetPosition( void )
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

			//  ��ނ��G�̏ꍇ
			if( objType == Scene::OBJTYPE_PLAYER )
			{
				//  �_�E���L���X�g
				PlayerWinks* player = ( PlayerWinks* )pScene;

				targetPosition = player->GetPos( );
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	return targetPosition;
}

//--------------------------------------------------------------------------------------
//  �ҋ@��ԂɕύX����֐�
//--------------------------------------------------------------------------------------
void EnemyRauderState::ChangeIdle( void )
{
	//  ���[�V�������I�����Ă���ꍇ
	if( GetEnemy( )->GetMotionFinish( ) )
	{
		//  �ҋ@���[�V�����ɕύX
		GetEnemy( )->SetMotion( FBXLoader::MOTION::IDLE );

		//  �j���[�g������Ԃ�
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( EnemyRauder::STATE::IDLE ) );
	}
}

//--------------------------------------------------------------------------------------
//  �ړ���ԂɕύX����֐�
//--------------------------------------------------------------------------------------
void EnemyRauderState::ChangeMove( MOVE_TYPE moveType )
{
	m_moveType = moveType;

	//  �ړ����[�V�����ɕύX
	GetEnemy( )->SetMotion( FBXLoader::MOTION::RUN );

	//  �X�e�[�g�̕ύX
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( EnemyRauder::STATE::MOVE ) );
}

//--------------------------------------------------------------------------------------
//  �W�����v��ԂɕύX����֐�
//--------------------------------------------------------------------------------------
void EnemyRauderState::ChangeJump( void )
{
	//  �W�����v�A�j���[�V������
	GetEnemy( )->SetMotion( FBXLoader::MOTION::JUMP );

	//  �W�����v��Ԃ�
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( EnemyRauder::STATE::JUMP ) );
}