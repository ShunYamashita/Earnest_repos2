//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( �u���C�u�U��000 )   ( enemyHPAttack000.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyHPAttack000.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "utility.h"
#include "player.h"
#include "bullet.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const int	ENEMY_HP_DAMAGE				= 400;
static const float	ENEMY_HP_BLOW_POWER			= 8.0f;
static const float	ENEMY_HP_RANGE					= 30.0f;
static const float	ENEMY_HP_RANGE_HEIGHT			= 12.0f;

static const int	ENEMY_HEAL_LIFE				= 20;

static const float	ENEMY_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Init( void )
{
	//  �e�̏�����
	EnemyState::Init( );

	m_slashEffekseer = false;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Update( void )
{
	//  �A�N�V����
	Action( );

	//  �j���[�g������ԂɕύX���邩�̔��f
	JudgeChangeNeutral( );

	//  �d�͂�������
	AddGravity( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyHPAttack000::Action( void )
{
	Utility::HIT_SPHERE	attackHitSphere;

	//  �U�����̓����蔻��̑��
	D3DXVECTOR3 attackPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.fLength = ENEMY_ATTACK_HIT_SPHERE_LENGTH;

	if( GetEnemy( )->GetAnimationKey( ) == 2 && m_slashEffekseer == false )
	{
		float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( attackHitSphere.position.x , GetEnemy( )->GetPos( ).y , attackHitSphere.position.z ) ,
								 D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( attackHitSphere.position.x , GetEnemy( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_FLAME , D3DXVECTOR3( attackHitSphere.position.x , GetEnemy( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) , 1.0f );

		m_slashEffekseer = true;
	}

	Scene* pScene = nullptr;

	if( GetEnemy( )->GetAnimationKey( ) >= 3 && GetEnemy( )->GetAnimationKey( ) <= 5 )
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
					Player* pPlayer= ( Player* )pScene;

					//  �v���C���[�̍��W�̑��
					D3DXVECTOR2 enemyPos;
					enemyPos.x  = pPlayer->GetPos( ).x;
					enemyPos.y  = pPlayer->GetPos( ).z;

					Utility::HIT_CIRCLE hitCircle;
					hitCircle.position.x = GetEnemy( )->GetPos( ).x;
					hitCircle.position.y = GetEnemy( )->GetPos( ).z;
					hitCircle.fLength = ENEMY_HP_RANGE;

					//  �v���C���[��3D���W�̑��
					D3DXVECTOR3 playerPos3D;
					playerPos3D = pPlayer->GetPos( );

					D3DXVECTOR3 blowVecDirect = playerPos3D - GetEnemy( )->GetPos( );
					D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

					if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
					{
						if( GetEnemy( )->GetPos( ).y + ENEMY_HP_RANGE_HEIGHT >= pPlayer->GetPos( ).y )
						{
							//  ���肪�h���Ԃł���ꍇ
							if( pPlayer->GetGuard( ) )
							{
								//  �̂������Ԃ�
								GetEnemy( )->SetAnimation( StateAnimator::MOTION_BEND );
								GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::BEND ) );
							}
							else
							{
								pPlayer->Damage( blowVecDirect , ENEMY_HP_BLOW_POWER , ENEMY_HP_DAMAGE , true );
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