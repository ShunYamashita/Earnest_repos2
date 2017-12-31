//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���b�N�I���_�b�V�� )   ( playerLockonDash.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerLockonDash.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "particle.h"
#include "enemy.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float PLAYER_LOCKON_DASH_SPEED = 2.5f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );

	//  ���x�̏�����
	m_speed = PLAYER_LOCKON_DASH_SPEED;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Uninit( void )
{
	m_speed = 0.0f;
}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Update( void )
{
	//  �J��������
	ControlCamera( );

	//  �A�N�V����
	Action( );

	//  ���x�����������������ꍇ
	if( m_speed <= 0.9f )
	{
		//  �j���[�g������ԂɕύX���邩�̔��f
		JudgeChangeNeutral( );
	}

	//  �ːi�U����ԂɕύX���邩�̔��f
	JudgeChangeRushAttack( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerLockonDash::Action( void )
{
	D3DXVECTOR3 posEnemy( 0.0f , 0.0f , 0.0f );

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
					Enemy* pEnemy = ( Enemy* )pScene;
							
					//  �G�̍��W���
					posEnemy = pEnemy->GetPos( );
				}
			}
			//  �v���C���[�ΐ탂�[�h�̏ꍇ
			if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
			{
				//  ��ނ��G�̏ꍇ
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  �_�E���L���X�g
					Player* pPlayer = ( Player* )pScene;
							
					//  �����ȊO�̃v���C���[�ԍ��̏ꍇ
					if( GetPlayer( )->GetPlayerNo( ) != pPlayer->GetPlayerNo( ) )
					{
						//  �G�̍��W���
						posEnemy = pPlayer->GetPos( );
					}
				}
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  �G����v���C���[�܂ł̕����x�N�g�������߂�
	D3DXVECTOR3 workVecDirect = posEnemy - GetPlayer( )->GetPos( );
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetPlayer( )->SetVecDirect( workVecDirect );

	//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
	GetPlayer( )->MovePos( GetPlayer( )->GetVecDirect( ) * m_speed );

	//  �����_�������Ă��������
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  ���x�����X�Ɍ��������Ă���
	m_speed += ( 0.0f - m_speed ) * 0.03f;

	//  �p�[�e�B�N���̐���
	Particle::Create( Particle::TYPE_NORMAL , GetPlayer( )->GetPos( ) , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
					  D3DXCOLOR( 0.2f , 0.5f , 0.8f , 1.0f ) , 4.0f , 0.1f , 0.01f , 0.0f , 0.0f , 20 );
}