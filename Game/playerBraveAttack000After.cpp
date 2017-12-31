//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( Brave�U��000( �� ) )   ( playerBraveAttack000After.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerBraveAttack000After.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "enemy.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float	PLAYER_ENABLE_BRAVE_ATTACK_DIST	= 150.0f;
static const float	PLAYER_BRAVE_ATTACK_SPEED		= 1.0f;
static const int	PLAYER_BRAVE_DAMAGE				= 150;
static const float	PLAYER_BRAVE_BLOW_POWER			= 3.0f;

static const float	PLAYER_ATTACK_HIT_SPHERE_LENGTH = 6.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000After::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );

	m_speed = PLAYER_BRAVE_ATTACK_SPEED;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000After::Uninit( void )
{
	//  ����ɓ�����l��
	GetPlayer( )->SetJudgeHit( true );
}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000After::Update( void )
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
void PlayerBraveAttack000After::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000After::Action( void )
{
	Utility::HIT_SPHERE	attackHitSphere;		//  �U������p�̋���

	//  ����ɓ�����Ȃ��l��
	GetPlayer( )->SetJudgeHit( false );

	//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
	GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * m_speed );

	//  �����_�������Ă��������
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  �U�����̓����蔻��̑��
	D3DXVECTOR3 attackPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
	attackHitSphere.position = attackPosition;
	attackHitSphere.position.y += 2.0f;
	attackHitSphere.fLength = PLAYER_ATTACK_HIT_SPHERE_LENGTH;

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
						pEnemy->Damage( PLAYER_BRAVE_DAMAGE );
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
						//  �G�v���C���[��3D���W�̑��
						D3DXVECTOR3 enemyPos3D;
						enemyPos3D = pPlayer->GetPos( );

						D3DXVECTOR3 blowVecDirect = enemyPos3D - GetPlayer( )->GetPos( );
						D3DXVec3Normalize( &blowVecDirect , &blowVecDirect );

						if( Utility::HitSphere( attackHitSphere , pPlayer->GetHitSphere( ) ) )
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
								pPlayer->Damage( blowVecDirect , PLAYER_BRAVE_BLOW_POWER , PLAYER_BRAVE_DAMAGE , true );
							}
						}
					}
				}
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

	EffekseerManager::SetPosition( GetPlayer( ) ->GetEffekseerHandle( EffekseerManager::TYPE_SLASH000 ) ,
								   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , attackHitSphere.position.y , GetPlayer( )->GetPos( ).z ) );
	EffekseerManager::SetRot( GetPlayer( ) ->GetEffekseerHandle( EffekseerManager::TYPE_SLASH000 ) ,
							  D3DXVECTOR3( 0.0f , fAngle , 0.0f ) );
}