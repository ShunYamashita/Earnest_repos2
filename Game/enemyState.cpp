//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g  ( enemyState.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/28
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyJump.h"
#include "enemy.h"
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
static const float	ENEMY_TO_CAMERA_DISTANCE_Y		= 9.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Y2		= 10.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z		= 20.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z2		= 23.0f;

static const float	ENEMY_GRAVITY2					= 0.2f;
static const float	CAMERA_ROTATE_ANGLE				= 0.025f;
static const float	ESCAPE_POWER					= 1.0f;

static const float	ENEMY_ENABLE_BRAVE_ATTACK_DIST	= 80.0f;

static const int	ENEMY_GUARD_FRAME				= 2;

static const float	ENEMY_HIT_SPHERE_POS_Y			= 3.0f;

//--------------------------------------------------------------------------------------
//  �d�͂�������֐�
//--------------------------------------------------------------------------------------
void EnemyState::Init( void )
{
	m_moveHeight = 0.0f;
	m_pressTime = 0;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�f�[�^�̐ݒ������֐�
//--------------------------------------------------------------------------------------
void EnemyState::SetEnemy( Enemy* enemy )
{
	if( enemy == nullptr )
	{
		return;
	}

	m_enemy = enemy;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�f�[�^�̎擾������֐�
//--------------------------------------------------------------------------------------
Enemy* EnemyState::GetEnemy( void )
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
void EnemyState::AddGravity( void )
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
void EnemyState::ChangeLockon( void )
{
	//  ���b�N�I���̐؂�ւ�
	GetEnemy( )->ChangeLockon( );
}

//--------------------------------------------------------------------------------------
//  �j���[�g������ԂɕύX���邩�̔��f������֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeNeutral( void )
{
	//  �A�j���[�V�������I�����Ă���ꍇ
	if( GetEnemy( )->GetAnimationFinish( ) )
	{
		//  �j���[�g�������[�V�����̕ύX
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_NEUTRAL );

		//  �j���[�g������Ԃ�
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::NEUTRAL ) );
	}
}

//--------------------------------------------------------------------------------------
//  �ړ���ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeMove( const D3DXVECTOR3& vecDirect )
{
	// �L�[�{�[�h���̎擾
	Keyboard* pKeyboard = SceneManager::GetKeyboard( );

	//  PS4�R���g���[���[���̎擾
	PS4Controller* pPS4Input = SceneManager::GetPS4Input( );

	//  �i�s�����x�N�g���̑��
	GetEnemy( )->SetVecDirect( vecDirect );

	//  ���[�V�����̕ύX
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_MOVE );

	//  �X�e�[�g�̕ύX
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MOVE ) );
}

//--------------------------------------------------------------------------------------
//  �W�����v��ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeJump( void )
{
	//  �W�����v�A�j���[�V������
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_JUMP );

	//  �W�����v��Ԃ�
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::JUMP ) );
}

//--------------------------------------------------------------------------------------
//  HP�U��000��ԂɕύX���邩���f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeHPAttack000( void )
{
	//  ���[�V�����̕ύX
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP001 );

	//  �u���C�u�U���ɕύX
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ATTACK_HP_ATTACK000 ) );
}

//--------------------------------------------------------------------------------------
//  �u���C�u�U��000��ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeBraveAttack000( bool left )
{
	if( GetEnemy( )->GetTargetDistance( ) <= ENEMY_ENABLE_BRAVE_ATTACK_DIST )
	{ 
		//  �u���C�u�U��000��ԂɕύX
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_BEFORE );
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ATTACK_BRAVE_ATTACK000_BEFORE ) );

		//  �ǂ�������Œǔ����邩�̐ݒ�
		GetEnemy( )->SetHomingDirect( left );
	}
	else
	{
		//  �u���C�u�U��000��ԂɕύX
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

		float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

		//  �U�����̓����蔻��̑��
		D3DXVECTOR3 effekseerPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;

		//  �G�t�F�N�g�̐���
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
																D3DXVECTOR3( GetEnemy( )->GetPos( ).x , effekseerPosition.y , GetEnemy( )->GetPos( ).z ) ,
																D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
		GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );
	}
}

//--------------------------------------------------------------------------------------
//  �����ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeEscape( const D3DXVECTOR3& vecDirect )
{
	//  �G�t�F�N�g�̒�~
	EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

	//  �G�t�F�N�g�̒�~
	EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

	//  ���@�̍폜
	GetEnemy( )->DeleteMagic( );

	//  ���x�̑��
	GetEnemy( )->SetSpeed( GetEnemy( )->GetBaseSpeed( ) );

	//  �����x�N�g���̐ݒ�
	GetEnemy( )->SetVecDirect( vecDirect );

	//  �ړ��͂̌v�Z
	GetEnemy( )->SetMovePower( ESCAPE_POWER );

	//  ���[�V�����̕ύX
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_ESCAPE );

	//  ��Ԃ̕ύX
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ESCAPE ) );
}

//--------------------------------------------------------------------------------------
//  �h���ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeGuard( void )
{
	//  ���[�V�����̕ύX
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_GUARD );

	//  ��Ԃ̕ύX
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::GUARD ) );

	float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

	//  �U�����̓����蔻��̑��
	D3DXVECTOR3 effekseerPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
	effekseerPosition.y += 3.0f;

	//  �G�t�F�N�g�̐���
	::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SHIELD ,
															D3DXVECTOR3( effekseerPosition.x , effekseerPosition.y , effekseerPosition.z ) ,
															D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
	GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_SHIELD , handle );
}


//--------------------------------------------------------------------------------------
//  ���b�N�I���_�b�V���ɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeLockonDash( const D3DXVECTOR3& vecDirect )
{
	if( vecDirect.x != 0.0f || vecDirect.z != 0.0f )
	{
		//  �����x�N�g���̐ݒ�
		GetEnemy( )->SetVecDirect( vecDirect );

		//  ���[�V�����̕ύX
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

		//  ��Ԃ̕ύX
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::FREE_DASH ) );
	}
	else
	{
		//  ���[�V�����̕ύX
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_LOCKON_DASH );

		//  ��Ԃ̕ύX
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::LOCKON_DASH ) );
	}
}

//--------------------------------------------------------------------------------------
//  �ːi�U���ɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeRushAttack( void )
{
	//  ���[�V�����̕ύX
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_RUSH_ATTACK );

	//  ��Ԃ̕ύX
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::RUSH_ATTACK ) );

	//  �p�x�̎Z�o
	float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

	//  �G�t�F�N�g�̐���
	::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_LANCE ,
															D3DXVECTOR3( GetEnemy( )->GetPos( ).x , GetEnemy( )->GetPos( ).y + ENEMY_HIT_SPHERE_POS_Y , GetEnemy( )->GetPos( ).z ) ,
															D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.5f , 1.5f , 1.5f ) );
	GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_LANCE , handle );
}

//--------------------------------------------------------------------------------------
//  ���@������ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeMagicReady( void )
{
	//  ���[�V�����̕ύX
	GetEnemy( )->SetAnimation( StateAnimator::MOTION_MAGIC_READY );

	//  ��Ԃ̕ύX
	GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MAGIC_READY ) );

	float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

	//  �G�t�F�N�g�̐���
	::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_AURA ,
															D3DXVECTOR3( GetEnemy( )->GetPos( ).x , GetEnemy( )->GetPos( ).y + ENEMY_HIT_SPHERE_POS_Y , GetEnemy( )->GetPos( ).z ) ,
															D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 4.0f , 4.0f , 4.0f ) , 1.0f );
	GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_AURA , handle );

	//  �G�t�F�N�g�̐���
	handle = EffekseerManager::Create( EffekseerManager::TYPE_MAGIC_CIRCLE ,
										D3DXVECTOR3( GetEnemy( )->GetPos( ).x , GetEnemy( )->GetPos( ).y , GetEnemy( )->GetPos( ).z ) ,
										D3DXVECTOR3( -D3DX_PI * 0.5f , fAngle , 0.0f ) , D3DXVECTOR3( 5.0f , 5.0f , 5.0f ) , 1.0f );
	GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE , handle );

	D3DXVECTOR3 magicPos = GetEnemy( )->GetPos( );
	magicPos.y += 3.0f;

	Magic* magic = nullptr;

	if( GetEnemy( )->GetMagicType( ) == Enemy::MAGIC::FIRE )
	{
		magic = MagicFire::Create( Magic::OWNER_ENEMY , magicPos , D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) ,
									D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetEnemy( )->GetVecDirect( ) );
	}
	else if( GetEnemy( )->GetMagicType( ) == Enemy::MAGIC::LIGHTNING )
	{
		magic = MagicLightning::Create( Magic::OWNER_ENEMY , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
										D3DXVECTOR3( 0.6f , 0.6f , 0.6f ) , GetEnemy( )->GetVecDirect( ) );
	}
	else if( GetEnemy( )->GetMagicType( ) == Enemy::MAGIC::TORNADE )
	{
		magic = MagicTornade::Create( Magic::OWNER_ENEMY , magicPos , D3DXVECTOR3( 0.0f , fAngle , 0.0f ) ,
										D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) , GetEnemy( )->GetVecDirect( ) );
	}

	//  ���@�̐ݒ�
	GetEnemy( )->SetMagic( magic );
}

//--------------------------------------------------------------------------------------
//  ���@�A�N�e�B�u��ԂɕύX���邩�̔��f����֐�
//--------------------------------------------------------------------------------------
void EnemyState::JudgeChangeMagicActive( void )
{
	Magic* magic = GetEnemy( )->GetMagic( );

	if( magic != NULL )
	{
		Enemy::MAGIC magicType = GetEnemy( )->GetMagicType( );

		if( magicType == Enemy::MAGIC::FIRE )
		{
			//  ���@�������[�V�����̕ύX
			GetEnemy( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE );

			//  ���@������Ԃ�
			GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MAGIC_ACTIVE ) );

			//  �G�t�F�N�g�̒�~
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

			//  �G�t�F�N�g�̒�~
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

			//  ���s��Ԃ�
			magic->SetActive( );
		}
		else if( magicType == Enemy::MAGIC::LIGHTNING )
		{
			//  ���@�������[�V�����̕ύX
			GetEnemy( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE2 );

			//  ���@������Ԃ�
			GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MAGIC_ACTIVE ) );

			//  �G�t�F�N�g�̒�~
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

			//  �G�t�F�N�g�̒�~
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

			//  ���s��Ԃ�
			magic->SetActive( );
		}
		else if( magicType == Enemy::MAGIC::TORNADE )
		{
			//  ���@�������[�V�����̕ύX
			GetEnemy( )->SetAnimation( StateAnimator::MOTION_MAGIC_ACTIVE3 );

			//  ���@������Ԃ�
			GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::MAGIC_ACTIVE ) );

			//  �G�t�F�N�g�̒�~
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_AURA ) );

			//  �G�t�F�N�g�̒�~
			EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_MAGIC_CIRCLE ) );

			//  ���s��Ԃ�
			magic->SetActive( );
		}
	}
}