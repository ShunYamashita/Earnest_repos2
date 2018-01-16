//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �u���C�u�U��000 )   ( playerHPAttack000.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerHPAttack000.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "utility.h"
#include "enemy.h"
#include "bullet.h"
#include "magic.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const int	PLAYER_HP_DAMAGE				= 400;
static const float	PLAYER_HP_BLOW_POWER			= 8.0f;
static const float	PLAYER_HP_RANGE					= 30.0f;
static const float	PLAYER_HP_RANGE_HEIGHT			= 12.0f;

static const int	PLAYER_HEAL_LIFE				= 20;

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerHPAttack000::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );

	m_slashEffekseer = false;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerHPAttack000::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerHPAttack000::Update( void )
{
	//  �J��������
	ControlCamera( );

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
void PlayerHPAttack000::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerHPAttack000::Action( void )
{
	Utility::HIT_SPHERE	attackHitSphere;

	//  �U�����̓����蔻��̑��
	D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

	if( GetPlayer( )->GetAnimationKey( ) == 2 && m_slashEffekseer == false )
	{
		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								 D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_SMASH , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.5f , 2.5f , 2.5f ) , 1.0f );

		//  �G�t�F�N�g�̐���
		EffekseerManager::Create( EffekseerManager::TYPE_FLAME , D3DXVECTOR3( attackHitSphere.position.x , GetPlayer( )->GetPos( ).y , attackHitSphere.position.z ) ,
								  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 2.0f , 2.0f , 2.0f ) , 1.0f );

		m_slashEffekseer = true;
	}

	Scene* pScene = nullptr;

	if( GetPlayer( )->GetAnimationKey( ) >= 3 && GetPlayer( )->GetAnimationKey( ) <= 5 )
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

						//  �G�v���C���[�̍��W�̑��
						D3DXVECTOR2 enemyPos;
						enemyPos.x  = pEnemy->GetPos( ).x;
						enemyPos.y  = pEnemy->GetPos( ).z;

						Utility::HIT_CIRCLE hitCircle;
						hitCircle.position.x = GetPlayer( )->GetPos( ).x;
						hitCircle.position.y = GetPlayer( )->GetPos( ).z;
						hitCircle.fLength = PLAYER_HP_RANGE;

						//  �G�v���C���[��3D���W�̑��
						D3DXVECTOR3 enemyPos3D;
						enemyPos3D = pEnemy->GetPos( );

						D3DXVECTOR3 blowVecDirect = enemyPos3D - GetPlayer( )->GetPos( );
						D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

						if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
						{
							if( GetPlayer( )->GetPos( ).y + PLAYER_HP_RANGE_HEIGHT >= pEnemy->GetPos( ).y )
							{
								//  ���肪�h���Ԃł���ꍇ
								if( pEnemy->GetGuard( ) )
								{
									//  �̂������Ԃ�
									GetPlayer( )->SetAnimation( StateAnimator::MOTION_BEND );
									GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::BEND ) );
								}
								else
								{
									pEnemy->Damage( blowVecDirect , PLAYER_HP_BLOW_POWER , PLAYER_HP_DAMAGE , true );
								}
							}
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
							//  �G�v���C���[�̍��W�̑��
							D3DXVECTOR2 enemyPos;
							enemyPos.x  = pPlayer->GetPos( ).x;
							enemyPos.y  = pPlayer->GetPos( ).z;

							Utility::HIT_CIRCLE hitCircle;
							hitCircle.position.x = GetPlayer( )->GetPos( ).x;
							hitCircle.position.y = GetPlayer( )->GetPos( ).z;
							hitCircle.fLength = PLAYER_HP_RANGE;

							//  �G�v���C���[��3D���W�̑��
							D3DXVECTOR3 enemyPos3D;
							enemyPos3D = pPlayer->GetPos( );

							D3DXVECTOR3 blowVecDirect = enemyPos3D - GetPlayer( )->GetPos( );
							D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

							if( Utility::HitCirclePoint( hitCircle , enemyPos ) )
							{
								if( GetPlayer( )->GetPos( ).y + PLAYER_HP_RANGE_HEIGHT >= pPlayer->GetPos( ).y )
								{
									//  ���肪�h���Ԃł���ꍇ
									if( pPlayer->GetGuard( ) )
									{
										//  �̂������Ԃ�
										GetPlayer( )->SetAnimation( StateAnimator::MOTION_BEND );
										GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::BEND ) );
									}
									else
									{
										pPlayer->Damage( blowVecDirect , PLAYER_HP_BLOW_POWER , PLAYER_HP_DAMAGE , true );
									}
								}
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