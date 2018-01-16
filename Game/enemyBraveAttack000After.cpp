//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( Brave�U��000( �� ) )   ( enemyBraveAttack000After.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyBraveAttack000After.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "player.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float	ENEMY_ENABLE_BRAVE_ATTACK_DIST	= 150.0f;
static const float	ENEMY_BRAVE_ATTACK_SPEED		= 1.0f;
static const int	ENEMY_BRAVE_DAMAGE				= 150;
static const float	ENEMY_BRAVE_BLOW_POWER			= 3.0f;

static const float	ENEMY_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000After::Init( void )
{
	//  �e�̏�����
	EnemyState::Init( );

	m_speed = ENEMY_BRAVE_ATTACK_SPEED;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000After::Uninit( void )
{
	//  ����ɓ�����l��
	GetEnemy( )->SetJudgeHit( true );
}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000After::Update( void )
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
void EnemyBraveAttack000After::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000After::Action( void )
{
	Utility::HIT_SPHERE	attackHitSphere;		//  �U������p�̋���

	//  ����ɓ�����Ȃ��l��
	GetEnemy( )->SetJudgeHit( false );

	//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
	GetEnemy( )->MovePos( GetEnemy( )->GetVecDirect( ) * m_speed );

	//  �����_�������Ă��������
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );

	//  �U�����̓����蔻��̑��
	D3DXVECTOR3 attackPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.position.y += 2.0f;
	attackHitSphere.fLength = ENEMY_ATTACK_HIT_SPHERE_LENGTH;

	Scene* pScene = nullptr;

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

				//  �G�G�l�~�[��3D���W�̑��
				D3DXVECTOR3 playerPos3D;
				playerPos3D = pPlayer->GetPos( );

				D3DXVECTOR3 blowVecDirect = playerPos3D - GetEnemy( )->GetPos( );
				D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

				if( Utility::HitSphere( attackHitSphere , pPlayer->GetHitSphere( ) ) )
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
						pPlayer->Damage( blowVecDirect , ENEMY_BRAVE_BLOW_POWER , ENEMY_BRAVE_DAMAGE , true );
					}
				}
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

	EffekseerManager::SetPosition( GetEnemy( ) ->GetEffekseerHandle( EffekseerManager::TYPE_SLASH000 ) ,
								   D3DXVECTOR3( GetEnemy( )->GetPos( ).x , attackHitSphere.position.y , GetEnemy( )->GetPos( ).z ) );
	EffekseerManager::SetRot( GetEnemy( ) ->GetEffekseerHandle( EffekseerManager::TYPE_SLASH000 ) ,
							  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) );
}