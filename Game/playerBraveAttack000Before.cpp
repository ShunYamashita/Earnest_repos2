//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( Brave�U��000( �O ) )   ( playerBraveAttack000Before.cpp )
//
//  Author : SHUN YAMASHITA
//
//  Update : 2017/11/25
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerBraveAttack000Before.h"
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
static const float	PLAYER_HOMIMG_TIME				= 90.0f;

//--------------------------------------------------------------------------------------
//  ����������������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Init( void )
{
	//  �e�̏�����
	PlayerState::Init( );

	m_homingTime = 0;
}

//--------------------------------------------------------------------------------------
//  �I������������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Update( void )
{
	//  �J��������
	ControlCamera( );

	//  �A�N�V����
	Action( );
}

//--------------------------------------------------------------------------------------
//  �`�揈��������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Draw( void )
{

}

//--------------------------------------------------------------------------------------
//  �A�N�V����������֐�
//--------------------------------------------------------------------------------------
void PlayerBraveAttack000Before::Action( void )
{
	//  ����ɓ�����Ȃ��l��
	GetPlayer( )->SetJudgeHit( false );

	//  �O��̍��W����
	m_beforePosition = GetPlayer( )->GetPos( );

	//  ���ʍ��W
	D3DXVECTOR2 resultPos;
	resultPos.x = GetPlayer( )->GetPos( ).x;
	resultPos.y = GetPlayer( )->GetPos( ).z;

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

			//  CPU�ΐ탂�[�h�̏ꍇ
			if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
			{
				//  ��ނ��G�̏ꍇ
				if( objType == Scene::OBJTYPE_ENEMY )
				{
					//  �_�E���L���X�g
					Enemy* pEnemy = ( Enemy* )pScene;

					//  �ڕW��3D���W�̎擾
					targetPos = pEnemy->GetPos( );

					//  �G�̍��W�̑��
					D3DXVECTOR2 posEnemy;
					posEnemy.x = pEnemy->GetPos( ).x;
					posEnemy.y = pEnemy->GetPos( ).z;

					//  �����̎Z�o
					fDistance = sqrtf( ( posEnemy.x - resultPos.x ) * ( posEnemy.x - resultPos.x ) +
										( posEnemy.y - resultPos.y ) * ( posEnemy.y - resultPos.y ) );

					D3DXVECTOR2 vecDirect;
					D3DXVec2Normalize( &vecDirect , &( posEnemy - resultPos ) );

					D3DXVECTOR2 controllPoint;
					controllPoint = resultPos + vecDirect * fDistance * 0.2f;

					if( GetPlayer( )->GetHomingDirect( ) == false )
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
					resultPos = Utility::BezierCurve2D( resultPos , posEnemy , controllPoint , m_homingTime );
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
						//  �ڕW��3D���W�̎擾
						targetPos = pPlayer->GetPos( );

						//  �G�̍��W�̑��
						D3DXVECTOR2 posEnemy;
						posEnemy.x = pPlayer->GetPos( ).x;
						posEnemy.y = pPlayer->GetPos( ).z;

						//  �����̎Z�o
						fDistance = sqrtf( ( posEnemy.x - resultPos.x ) * ( posEnemy.x - resultPos.x ) +
										   ( posEnemy.y - resultPos.y ) * ( posEnemy.y - resultPos.y ) );

						D3DXVECTOR2 vecDirect;
						D3DXVec2Normalize( &vecDirect , &( posEnemy - resultPos ) );

						D3DXVECTOR2 controllPoint;
						controllPoint = resultPos + vecDirect * fDistance * 0.2f;

						if( GetPlayer( )->GetHomingDirect( ) == false )
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
						resultPos = Utility::BezierCurve2D( resultPos , posEnemy , controllPoint , m_homingTime );
					}
				}
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	//  �v���C���[���W�̐ݒ�
	D3DXVECTOR3 playerPosition;
	playerPosition.x = resultPos.x;
	playerPosition.y += ( targetPos.y - GetPlayer( )->GetPos( ).y ) * 0.1f;
	playerPosition.z = resultPos.y;
	GetPlayer( )->SetPosition( playerPosition );

	//  �v���C���[�����x�N�g���̐ݒ�
	D3DXVECTOR3 playerVecDirect = GetPlayer( )->GetPos( ) - m_beforePosition;
	D3DXVec3Normalize( &playerVecDirect , &playerVecDirect );
	GetPlayer( )->SetVecDirect( playerVecDirect );

	//  �����_�������Ă��������
	GetPlayer( )->SetPositionAt( GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 5.0f );

	//  ���t���[��������Z�o���đ���
	m_homingTime += 1.0f / ( PLAYER_HOMIMG_TIME + fDistance * 0.8f );

	if( m_homingTime >= 0.8f || fDistance < 10.0f )
	{
		//  �ڕW�Ɍ��������x�N�g���̑��
		playerVecDirect = targetPos - GetPlayer( )->GetPos( );
		playerVecDirect.y = 0.0f;
		D3DXVec3Normalize( &playerVecDirect , &playerVecDirect );
		GetPlayer( )->SetVecDirect( playerVecDirect );

		m_homingTime = 0.8f;

		//  �u���C�u�U��000( �� )��ԂɕύX
		GetPlayer( )->SetAnimation( StateAnimator::MOTION_ATTACK_HP000_AFTER );
		GetPlayer( )->ChangeState( GetPlayer( )->GetPlayerState( Player::STATE::ATTACK_BRAVE_ATTACK000_AFTER ) );

		float fAngle = atan2f( GetPlayer( )->GetVecDirect( ).x , GetPlayer( )->GetVecDirect( ).z );

		//  �U�����̓����蔻��̑��
		D3DXVECTOR3 effekseerPosition = GetPlayer( )->GetPos( ) + GetPlayer( )->GetVecDirect( ) * 3.0f;
		effekseerPosition.y += 2.0f;

		//  �G�t�F�N�g�̐���
		::Effekseer::Handle handle = EffekseerManager::Create( EffekseerManager::TYPE_SLASH000 ,
															   D3DXVECTOR3( GetPlayer( )->GetPos( ).x , effekseerPosition.y , GetPlayer( )->GetPos( ).z ) ,
															   D3DXVECTOR3( 0.0f , fAngle , 0.0f ) , D3DXVECTOR3( 1.8f , 1.8f , 1.8f ) , 1.5f );
		GetPlayer( )->SetEffekseerHandle( EffekseerManager::TYPE_SLASH000 , handle );
	}
}
