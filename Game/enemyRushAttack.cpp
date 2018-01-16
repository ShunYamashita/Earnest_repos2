//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( �ːi�U�� )   ( enemyRushAttack.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyRushAttack.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "effekseerManager.h"
#include "player.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float	ENEMY_TO_CAMERA_DISTANCE_Y		= 9.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Y2		= 10.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z		= 20.0f;
static const float	ENEMY_TO_CAMERA_DISTANCE_Z2		= 23.0f;

static const float	CAMERA_ROTATE_ANGLE				= 0.025f;

static const float	ENEMY_RUSH_ATTACK_SPEED			= 3.0f;
static const int	ENEMY_RUSH_ATTACK_DAMAGE		= 250;
static const float	ENEMY_RUSH_ATTACK_BLOW_POWER	= 7.0f;

static const float	ENEMY_HIT_SPHERE_POS_Y			= 3.0f;

static const float	ENEMY_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Init( void )
{
	//  �e�̏�����
	EnemyState::Init( );

	//  ���x�̏�����
	m_speed = ENEMY_RUSH_ATTACK_SPEED;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Uninit( void )
{
	m_speed = 0.0f;
}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Update( void )
{
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
void EnemyRushAttack::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyRushAttack::Action( void )
{
	//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
	GetEnemy( )->MovePos( GetEnemy( )->GetVecDirect( ) * m_speed );

	//  �����_�������Ă��������
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );

	//  �G�t�F�N�V�A���W�̐ݒ�
	D3DXVECTOR3 effekseerPos( GetEnemy( )->GetPos( ).x , GetEnemy( )->GetPos( ).y + ENEMY_HIT_SPHERE_POS_Y , GetEnemy( )->GetPos( ).z );
	effekseerPos += GetEnemy( )->GetVecDirect( ) * 8.0f;
	EffekseerManager::SetPosition( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_LANCE ) , effekseerPos );

	//  �L�[��������x�܂ł������ꍇ
	if( GetEnemy( )->GetAnimationKey( ) >= 11 )
	{
		//  ���x�̌���
		m_speed += ( 0.0f - m_speed ) * 0.06f;

		//  �G�t�F�N�g�̒�~
		EffekseerManager::Stop( GetEnemy( )->GetEffekseerHandle( EffekseerManager::TYPE_LANCE ) );
	}

	//  �U�����̓����蔻��̑��
	Utility::HIT_SPHERE attackHitSphere;
	attackHitSphere.position = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position.y += 2.0f;
	attackHitSphere.fLength = ENEMY_ATTACK_HIT_SPHERE_LENGTH;

	//  �V�[���N���X�̃|�C���^
	Scene* pScene = NULL;

	//  �L�[�����͈͓̔��ɂ���ꍇ
	if( GetEnemy( )->GetAnimationKey( ) >= 1 && GetEnemy( )->GetAnimationKey( ) <= 10 )
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

				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  �_�E���L���X�g
					Player* pPlayer = ( Player* )pScene;

					if( Utility::HitSphere( attackHitSphere , pPlayer->GetHitSphere( ) ) )
					{
						//  ������ѕ����̃x�N�g���Z�o
						D3DXVECTOR3 blowVecDirect = GetEnemy( )->GetVecDirect( );
						D3DXMATRIX mtxRot;
						D3DXMatrixIdentity( &mtxRot );
						D3DXMatrixRotationY( &mtxRot , D3DX_PI * 0.1f );
						D3DXVec3TransformNormal( &blowVecDirect , &blowVecDirect , &mtxRot );
						D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

						pPlayer->Damage( blowVecDirect , ENEMY_RUSH_ATTACK_BLOW_POWER , ENEMY_RUSH_ATTACK_DAMAGE );
					}
				}

				//  ���̃|�C���^����
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}
}