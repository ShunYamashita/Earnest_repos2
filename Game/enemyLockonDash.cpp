//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( ���b�N�I���_�b�V�� )   ( enemyLockonDash.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyLockonDash.h"
#include "enemy.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "joystick.h"
#include "keyboard.h"
#include "particle.h"
#include "player.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
static const float ENEMY_LOCKON_DASH_SPEED = 2.5f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void EnemyLockonDash::Init( void )
{
	//  �e�̏�����
	EnemyState::Init( );

	//  ���x�̏�����
	m_speed = ENEMY_LOCKON_DASH_SPEED;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void EnemyLockonDash::Uninit( void )
{
	m_speed = 0.0f;
}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyLockonDash::Update( void )
{
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
void EnemyLockonDash::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyLockonDash::Action( void )
{
	D3DXVECTOR3 posPlayer( 0.0f , 0.0f , 0.0f );

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

			//  ��ނ��v���C���[�̏ꍇ
			if( objType == Scene::OBJTYPE_PLAYER )
			{
				//  �_�E���L���X�g
				Player* pPlayer = ( Player* )pScene;

				//  �v���C���[�̍��W���
				posPlayer = pPlayer->GetPos( );
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  �G����v���C���[�܂ł̕����x�N�g�������߂�
	D3DXVECTOR3 workVecDirect = posPlayer - GetEnemy( )->GetPos( );
	D3DXVec3Normalize( &workVecDirect , &workVecDirect );
	GetEnemy( )->SetVecDirect( workVecDirect );

	//  ���W�̍X�V( �i�s���� �~ ���x ���̈ړ� )
	GetEnemy( )->MovePos( GetEnemy( )->GetVecDirect( ) * m_speed );

	//  �����_�������Ă��������
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );

	//  ���x�����X�Ɍ��������Ă���
	m_speed += ( 0.0f - m_speed ) * 0.03f;

	//  �p�[�e�B�N���̐���
	Particle::Create( Particle::TYPE_NORMAL , GetEnemy( )->GetPos( ) , D3DXVECTOR3( 1.0f , 0.0f , 1.0f ) ,
					  D3DXCOLOR( 0.2f , 0.5f , 0.8f , 1.0f ) , 4.0f , 0.1f , 0.01f , 0.0f , 0.0f , 20 );
}