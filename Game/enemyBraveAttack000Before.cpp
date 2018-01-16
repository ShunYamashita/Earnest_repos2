//--------------------------------------------------------------------------------------
//  �G�l�~�[�X�e�[�g( Brave�U��000( �O ) )   ( enemyBraveAttack000Before.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyBraveAttack000Before.h"
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
static const float	ENEMY_HOMIMG_TIME				= 90.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Init( void )
{
	//  �e�̏�����
	EnemyState::Init( );

	m_homingTime = 0;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Update( void )
{
	//  �A�N�V����
	Action( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void EnemyBraveAttack000Before::Action( void )
{
	//  ����ɓ�����Ȃ��l��
	GetEnemy( )->SetJudgeHit( false );

	//  �O��̍��W����
	m_beforePosition = GetEnemy( )->GetPos( );

	//  ���ʍ��W
	D3DXVECTOR2 resultPos;
	resultPos.x = GetEnemy( )->GetPos( ).x;
	resultPos.y = GetEnemy( )->GetPos( ).z;

	D3DXVECTOR3 targetPos;

	//  �V�[���N���X�̃|�C���^
	Scene* pScene = NULL;			

	float fDistance = 0.0f;

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

				//  �ڕW��3D���W�̎擾
				targetPos = pPlayer->GetPos( );

				//  �G�̍��W�̑��
				D3DXVECTOR2 posPlayer;
				posPlayer.x = pPlayer->GetPos( ).x;
				posPlayer.y = pPlayer->GetPos( ).z;

				//  �����̎Z�o
				fDistance = sqrtf( ( posPlayer.x - resultPos.x ) * ( posPlayer.x - resultPos.x ) +
								   ( posPlayer.y - resultPos.y ) * ( posPlayer.y - resultPos.y ) );

				D3DXVECTOR2 vecDirect;
				D3DXVec2Normalize( &vecDirect , &( posPlayer - resultPos ) );

				D3DXVECTOR2 controllPoint;
				controllPoint = resultPos + vecDirect * fDistance * 0.2f;

				if( GetEnemy( )->GetHomingDirect( ) == false )
				{					
					controllPoint.x += vecDirect.y * fDistance * 0.12f;
					controllPoint.y -= vecDirect.x * fDistance * 0.12f;
				}
				else
				{
					controllPoint.x -= vecDirect.y * fDistance * 0.12f;
					controllPoint.y += vecDirect.x * fDistance * 0.12f;
				}

				//  �x�W�F�Ȑ����猋�ʍ��W�̎Z�o
				resultPos = Utility::BezierCurve2D( resultPos , posPlayer , controllPoint , m_homingTime );
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  �G�l�~�[���W�̐ݒ�
	D3DXVECTOR3 enemyPosition;
	enemyPosition.x = resultPos.x;
	enemyPosition.y += ( targetPos.y - GetEnemy( )->GetPos( ).y ) * 0.1f;
	enemyPosition.z = resultPos.y;
	GetEnemy( )->SetPosition( enemyPosition );

	//  �G�l�~�[�����x�N�g���̐ݒ�
	D3DXVECTOR3 enemyVecDirect = GetEnemy( )->GetPos( ) - m_beforePosition;
	D3DXVec3Normalize( &enemyVecDirect , &enemyVecDirect );
	GetEnemy( )->SetVecDirect( enemyVecDirect );

	//  �����_�������Ă��������
	GetEnemy( )->SetPositionAt( GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 5.0f );

	//  ���t���[��������Z�o���đ���
	m_homingTime += 1.0f / ( ENEMY_HOMIMG_TIME + fDistance * 0.8f );

	if( m_homingTime >= 0.8f || fDistance < 10.0f )
	{
		//  �ڕW�Ɍ��������x�N�g���̑��
		enemyVecDirect = targetPos - GetEnemy( )->GetPos( );
		enemyVecDirect.y = 0.0f;
		D3DXVec3Normalize( &enemyVecDirect , &enemyVecDirect );
		GetEnemy( )->SetVecDirect( enemyVecDirect );

		m_homingTime = 0.8f;

		//  �u���C�u�U��000( �� )��ԂɕύX
		GetEnemy( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
		GetEnemy( )->ChangeState( GetEnemy( )->GetEnemyState( Enemy::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

		float fAngle = atan2f( GetEnemy( )->GetVecDirect( ).x , GetEnemy( )->GetVecDirect( ).z );

		//  �U�����̓����蔻��̑��
		D3DXVECTOR3 effekseerPosition = GetEnemy( )->GetPos( ) + GetEnemy( )->GetVecDirect( ) * 3.0f;
		effekseerPosition.y += 2.0f;

		//  �G�t�F�N�g�̐���
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
															   D3DXVECTOR3( GetEnemy( )->GetPos( ).x , effekseerPosition.y , GetEnemy( )->GetPos( ).z ) ,
															   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
		GetEnemy( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );
	}
}
