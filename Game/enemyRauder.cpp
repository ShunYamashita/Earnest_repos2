//--------------------------------------------------------------------------------------
//  �G   ( enemyWinks.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "enemyRauder.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "scene2D.h"
#include "meshField.h"
#include "bullet.h"
#include "fade.h"
#include "bullet.h"
#include "particle.h"
#include "hitField.h"
#include "result.h"
#include "magicFire.h"
#include "magicLightning.h"
#include "magicTornade.h"
#include "magic.h"
#include "life.h"
#include "utility.h"
#include "fbxLoader.h"
#include <random>
#include "Wwise.h"
#include "playerWinks.h"
#include "imgui_impl_dx9.h"
#include "AIManager.h"

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��( �X�e�[�g�n )
//--------------------------------------------------------------------------------------
#include "EnemyRauderState.h"
#include "enemyRauderIdle.h"
#include "enemyRauderMove.h"
#include "enemyRauderJump.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
#define ENEMY_TO_CAMERA_DISTANCE_Y				( 11.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Y2				( 15.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Z				( 25.0f )
#define ENEMY_TO_CAMERA_DISTANCE_Z2				( 35.0f )
#define ENEMY_TO_CUBE_DISTANCE					( 7.0f )
#define ENEMY_TO_CUBE_PUT_DISTANCE				( 2.5f )
#define ENEMY_SIZE								( 2.5f )
#define ENEMY_GOAL_SIZE							( 0.8f )
#define ENEMY_HIT_SIZE							( 2.0f )
#define ENEMY_WAIT_TIME							( 20 )
#define ENEMY_GRAVITY							( 0.1f )
#define ENEMY_GRAVITY2							( 0.2f )
#define ENEMY_ENABLE_NUM_JUMP					( 2 )
#define ENEMY_TO_CAMERA_RATE					( 0.5f )
#define ENEMY_LOCKON_DASH_SPEED					( 3.0f )
#define ENEMY_HIT_SPHERE_LENGTH					( 6.0f )
#define ENEMY_HEAL_LIFE							( 20 )

#define ENEMY_AVOID_SPEED						( 1.5f )

#define ENEMY_HIT_SPHERE_POS_Y					( 3.0f )

#define ENEMY_LIFE_POS_X						( 389.0f )
#define ENEMY_LIFE_POS_Y						( 661.0f )
#define ENEMY_LIFE_SIZE_X						( 350.0f )
#define ENEMY_LIFE_SIZE_Y						( 49.0f )

#define ENEMY_VS_LIFE_POS_X						( 125.0f )
#define ENEMY_VS_LIFE_POS_Y						( 646.0f )
#define ENEMY_VS_LIFE_SIZE_X					( 442.0f )
#define ENEMY_VS_LIFE_SIZE_Y					( 38.0f )

#define ENEMY_BRAVE_POS_X						( 440.0f )
#define ENEMY_BRAVE_POS_Y						( 570.0f )
#define ENEMY_BRAVE_SIZE_X						( 60.0f )
#define ENEMY_BRAVE_SIZE_Y						( 60.0f )

#define ENEMY_UI_POS_X							( SCREEN_WIDTH * 0.25f )
#define ENEMY_UI_POS_Y							( SCREEN_HEIGHT * 0.88f )
#define ENEMY_UI_SIZE_X							( 640.0f )
#define ENEMY_UI_SIZE_Y							( 200.0f )

#define ENEMY2_LIFE_POS_X						( 975.0f )
#define ENEMY2_LIFE_POS_Y						( 605.0f )
#define ENEMY2_LIFE_SIZE_X						( 255.0f )
#define ENEMY2_LIFE_SIZE_Y						( 30.0f )

#define ENEMY2_VS_LIFE_POS_X					( 715.0f )
#define ENEMY2_VS_LIFE_POS_Y					( 646.0f )
#define ENEMY2_VS_LIFE_SIZE_X					( 442.0f )
#define ENEMY2_VS_LIFE_SIZE_Y					( 38.0f )

#define ENEMY2_BRAVE_POS_X						( 990.0f )
#define ENEMY2_BRAVE_POS_Y						( 565.0f )
#define ENEMY2_BRAVE_SIZE_X						( 35.0f )
#define ENEMY2_BRAVE_SIZE_Y						( 35.0f )

#define ENEMY2_UI_POS_X							( SCREEN_WIDTH * 0.75f )
#define ENEMY2_UI_POS_Y							( SCREEN_HEIGHT * 0.88f )
#define ENEMY2_UI_SIZE_X						( 640.0f )
#define ENEMY2_UI_SIZE_Y						( 200.0f )

#define ENEMY_JUMP_								( 0.4f )

#define ENEMY_FINISH_SPEED						( 3.0f )

#define ENEMY_HOMIMG_TIME						( 90.0f )
#define ENEMY_ENABLE_HP_ATTACK_DIST				( 150.0f )
#define ENEMY_HP_ATTACK_SPEED					( 1.0f )

static const int	ENEMY_HP_DAMAGE				= 150;
static const int	ENEMY_FINISHER_DAMAGE		= 250;

static const float	ENEMY_HP_RANGE				= 30.0f;
static const float	ENEMY_HP_RANGE_HEIGHT		= 12.0f;

static const float	ENEMY_HP_BLOW_POWER			= 3.0f;
static const float	ENEMY_FINISHER_BLOW_POWER	= 7.0f;

static const int	ENEMY_GUARD_FRAME			= 5;

static const float	CAMERA_ROTATE_ANGLE			= 0.025f;

static const float	ENEMY_HEIGHT				= 10.0f;

static const float	ENEMY_SHADOW_SCALE			= 0.55f;

static const float	ENEMY_JUMP_POWER			= 2.0f;

//--------------------------------------------------------------------------------------
//  �C���X�^���X����
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �G�l�~�[�N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
EnemyRauder::EnemyRauder( ) : Scene( 3 )
{
	m_beforePos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	m_fSpeed = 0.0f;
	m_nWaitTime = 0;
	m_fCurrentJumpPower = 0.0f;
	m_nCntJump = 0;
	m_bLockOn = true;
	m_hitSphere.fLength = ENEMY_HIT_SPHERE_LENGTH * 0.5f;
	m_hitSphere.position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_pLife = nullptr;
	m_pEnemyRauderUI = nullptr;
	m_pMagic = nullptr;
	m_nLife = BASE_LIFE;
	m_nBrave = BASE_BRAVE;
	m_bVoiceStart = false;
	m_bJudgeHit = true;
	m_bHomingLeft = false;
	m_bSlashEffekseer = false;
	m_fHomingTime = 0.0f;
	m_fBlowPower = 0.0f;
	m_fMoveY = 0.0f;
	m_nPressTime = 0;
	m_fJumpPower = ENEMY_JUMP_POWER;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�N���X�̃f�X�g���N�^
//--------------------------------------------------------------------------------------
EnemyRauder::~EnemyRauder( )
{

}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�̏���������
//--------------------------------------------------------------------------------------
HRESULT EnemyRauder::Init( void )
{
	m_drawDepth = true;

	//  ���̂̎�ނ̐ݒ�
	Scene::SetObjType( Scene::OBJTYPE_ENEMY );

	//  �̗͂̐���
	m_pLife = Life::Create( Life::TYPE_LIFE000 ,
							D3DXVECTOR3( ENEMY_LIFE_POS_X , ENEMY_LIFE_POS_Y , 0.0f ) ,
							D3DXVECTOR3( ENEMY_LIFE_SIZE_X , ENEMY_LIFE_SIZE_Y , 0.0f ) ,
							D3DXVECTOR2( 0.0f , 0.0f ) ,
							D3DXVECTOR2( 1.0f , 1.0f ) ,
							D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) ,
							m_nLife );

	//  �G�l�~�[�����_�̐ݒ�
	m_posAt = m_position - m_vecDirect * 5.0f;

	//  �����x�N�g���̑��
	D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

	//  �G�l�~�[�X�e�[�g�̐���
	m_allState[ static_cast< int >( EnemyRauder::STATE::IDLE ) ] = new EnemyRauderIdle( this );
	m_allState[ static_cast< int >( EnemyRauder::STATE::MOVE ) ] = new EnemyRauderMove( this );
	m_allState[ static_cast< int >( EnemyRauder::STATE::JUMP ) ] = new EnemyRauderJump( this );

	//  �j���[�g������Ԃŏ�����
	m_enemyState = m_allState[ static_cast< int >( EnemyRauder::STATE::IDLE ) ];

	//  FBX�f�[�^�̍쐬
	m_fbx = FBXLoader::Create( FBXLoader::TYPE::RAUDER , m_position , m_posAt , 0.07f );
	m_fbx->SetMotion( FBXLoader::MOTION::IDLE );

	//  AI�Ǘ��N���X�ɃL�����N�^�[AI�Ƃ��Ēǉ�
	m_characterAIIndex = AIManager::AddCharacterAI( "enemyRauder" , this );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�̏I������
//--------------------------------------------------------------------------------------
void EnemyRauder::Uninit( void )
{
	if( m_pLife != nullptr )
	{
		m_pLife = nullptr;
	}

	if( m_pEnemyRauderUI != nullptr )
	{
		m_pEnemyRauderUI = nullptr;
	}

	if( m_fbx != nullptr )
	{
		m_fbx->Uninit( );
		m_fbx = nullptr;
	}

	//  �G�l�~�[�X�e�[�g�̐����̃��[�v
	for( int countState = 0; countState < static_cast< int >( EnemyRauder::STATE::MAX ) - 1; ++countState )
	{
		m_allState[ countState ]->Uninit( );
		delete m_allState[ countState ];
		m_allState[ countState ] = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�̍X�V����
//--------------------------------------------------------------------------------------
void EnemyRauder::Update( void )
{
	//  �J�n�{�C�X���܂��Đ����Ă��Ȃ��ꍇ
	if( m_bVoiceStart == false )
	{
		m_bVoiceStart = true;
	}

	//  �󂯂鑤�̓����蔻��̑��
	m_hitSphere.position = m_position;
	m_hitSphere.position.y += ENEMY_HIT_SPHERE_POS_Y;

	//  �G�l�~�[�X�e�[�g�N���X�̍X�V
	if( m_enemyState != nullptr )
	{
		m_enemyState->Update( );
	}

	//  �G�Ƃ̓����蔻��
	JudgeHitEnemy( );

	//  �ǂƂ̓����蔻��
	JudgeHitWall( );

	//  �n�ʂƂ̓����蔻��
	JudgeHitGround( );

	//  �̗͂�����𒴂����ꍇ
	if( m_nLife > BASE_LIFE )
	{
		m_nLife = BASE_LIFE;
	}

	//  �̗̓|�C���^������ꍇ
	if( m_pLife != nullptr )
	{
		//  �̗͂̐ݒ�
		m_pLife->SetLife( m_nLife );
	}

	//  ���G���Ԃ̏ꍇ
	if( m_nInvisibleTime > 0 )
	{
		//  ���G���Ԃ̃J�E���g
		m_nInvisibleTime--;

		m_color.a = 0.5f;
	}
	else
	{
		m_color.a = 1.0f;
	}

	if( m_fbx != nullptr )
	{
		m_fbx->Update( );
	}
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�̕`�揈��
//--------------------------------------------------------------------------------------
void EnemyRauder::Draw( void )
{
	Camera* camera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVectorDirect;
	D3DXVECTOR3 cameraToEnemyRauder;

	if( camera != nullptr )
	{
		D3DXVECTOR3 cameraPosition = camera->GetCameraPosEye( );
		D3DXVECTOR3 enemyWinksPosition = m_position;
		cameraPosition.y = 0.0f;
		enemyWinksPosition.y = 0.0f;

		cameraToEnemyRauder = enemyWinksPosition - cameraPosition;
		D3DXVec3Normalize( &cameraToEnemyRauder , &cameraToEnemyRauder );
		cameraVectorDirect = camera->GetCameraVecDirect( );
		cameraVectorDirect.y = 0.0f;
		D3DXVec3Normalize( &cameraVectorDirect , &cameraVectorDirect );
	}

	if( Utility::IntoView( cameraVectorDirect , cameraToEnemyRauder , D3DX_PI * 0.5f ) )
	{
		if( m_fbx != nullptr )
		{
			m_fbx->SetDrawInfo( m_position , m_posAt );
			m_fbx->Draw( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�̕`�揈��
//--------------------------------------------------------------------------------------
void EnemyRauder::DrawDepth( void )
{
	if( m_fbx != nullptr )
	{
		m_fbx->SetDrawInfo( m_position , m_posAt );
		m_fbx->DrawDepth( );
	}
}

//--------------------------------------------------------------------------------------
//  �n�ʂƂ̓����蔻�������֐�
//--------------------------------------------------------------------------------------
bool EnemyRauder::JudgeHitGround( void )
{
	//  �V�[���N���X�̃|�C���^
	Scene* pScene = nullptr;											

	//  �����蔻��p�t�B�[���h�N���X
	HitField* pHitField = nullptr;

#pragma omp parallel for
	//  �D��x�̍ő吔���̃��[�v
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		//  �V�[���̐擪�A�h���X���擾
		pScene = Scene::GetScene( nCntPriority );

		//  �V�[������ł͂Ȃ��ԃ��[�v
		while( pScene != nullptr )
		{
			Scene::OBJTYPE objType;						//  ���̂̎��

			//  ���̂̎�ނ̎擾
			objType = pScene->GetObjType( );

			//  ��ނ������蔻��p�̃t�B�[���h�̏ꍇ
			if( objType == Scene::OBJTYPE_HIT_FIELD )
			{
				//  �����蔻��p�t�B�[���h�N���X�Ƀ_�E���L���X�g
				pHitField = ( HitField* )pScene;

				float fHeight = pHitField->GetHeight( m_position );

				if( !( fHeight == -100000.0f ) )
				{	
					if( m_position.y <= fHeight )
					{
						//  Y���W�t�B�[���h�ɍ��킹��
						m_position.y = pHitField->GetHeight( m_position );

						return true;
					}
				}
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  �G�Ƃ̓����蔻�������֐�
//--------------------------------------------------------------------------------------
bool EnemyRauder::JudgeHitEnemy( void )
{
	//  �V�[���N���X�̃|�C���^
	Scene* pScene = nullptr;	

	if( m_bJudgeHit )
	{
#pragma omp parallel for
		//  �D��x�̍ő吔���̃��[�v
		for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
		{
			//  �V�[���̐擪�A�h���X���擾
			pScene = Scene::GetScene( nCntPriority );

			//  �V�[������ł͂Ȃ��ԃ��[�v
			while( pScene != nullptr )
			{
				Scene::OBJTYPE objType;						//  ���̂̎��

				//  ���̂̎�ނ̎擾
				objType = pScene->GetObjType( );

				//  ��ނ��v���C���[�̏ꍇ
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  �_�E���L���X�g
					PlayerWinks* pPlayer = ( PlayerWinks* )pScene;

					float fDist = 0.0f;

					if( Utility::HitSphere( m_hitSphere , pPlayer->GetHitSphere( ) , &fDist ) )
					{
						//  ���x���Ȃ���
						m_fSpeed = 0.0f;

						//  ���˂���x�N�g�������߂�
						D3DXVECTOR3 reflectVec = m_hitSphere.position - pPlayer->GetHitSphere( ).position;
						reflectVec.y = 0.0f;

						D3DXVec3Normalize( &reflectVec , &reflectVec );

						//  �߂荞�񂾕����W��߂�
						m_position += reflectVec * fDist;

						return true;
					}
				}

				//  ���̃|�C���^����
				pScene = pScene->GetNextScene( pScene );
			}
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  �ǂƂ̓����蔻�������֐�
//--------------------------------------------------------------------------------------
bool EnemyRauder::JudgeHitWall( void )
{
	bool hit = false;

	//  �ǂƂ̓����蔻��
	if( m_position.x > Game::GetFieldMax( ).x * 0.999f )
	{
		m_position.x = Game::GetFieldMax( ).x * 0.999f;

		m_fSpeed = 0.0f;

		hit = true;
	}
	if( m_position.x < Game::GetFieldMin( ).x * 0.999f )
	{
		m_position.x = Game::GetFieldMin( ).x * 0.999f;

		m_fSpeed = 0.0f;

		hit = true;
	}
	if( m_position.z > Game::GetFieldMax( ).z * 0.999f )
	{
		m_position.z = Game::GetFieldMax( ).z * 0.999f;

		m_fSpeed = 0.0f;

		hit = true;
	}
	if( m_position.z < Game::GetFieldMin( ).z * 0.999f )
	{
		m_position.z = Game::GetFieldMin( ).z * 0.999f;

		m_fSpeed = 0.0f;

		hit = true;
	}

	return hit;
}

//--------------------------------------------------------------------------------------
//  �C���X�^���X����������֐�
//--------------------------------------------------------------------------------------
EnemyRauder* EnemyRauder::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed , MAGIC magic )
{
	EnemyRauder* pEnemyRauder;

	//  �C���X�^���X����
	pEnemyRauder = new EnemyRauder;

	//  ���W�̑��
	pEnemyRauder->m_position = position;

	//  �ŏ��̍��W�̑��
	pEnemyRauder->m_firstPos = position;

	//  �����_���W�̑��
	pEnemyRauder->m_posAt = position;

	//  ��]�p�̑��
	pEnemyRauder->m_rot = rot;

	//  �傫���{���̑��
	pEnemyRauder->m_scale = scale;

	//  ���x�̑��
	pEnemyRauder->m_fBaseSpeed = fSpeed;

	//  �������@�̑��
	pEnemyRauder->m_magic = magic;

	//  ������
	pEnemyRauder->Init( );

	return pEnemyRauder;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�̃_���[�W����
//--------------------------------------------------------------------------------------
void EnemyRauder::Damage( D3DXVECTOR3 blowVecDirect , float fBlowPower , int nDamage , bool bBlow )
{
	if( m_nInvisibleTime != 0 )
	{
		return;
	}

	//  ������ѕ����Ɨ͂̑��
	m_blowVecDirect = blowVecDirect;
	m_fBlowPower = fBlowPower;

	//  �_���[�W����
	m_nLife -= nDamage;

	//  ���G���Ԃ̐ݒ�
	m_nInvisibleTime = 60;

	//  �����̐錾
	std::random_device rd;

	//  �����̐ݒ�
	std::uniform_int_distribution< int > RandomSeekPos( 5 , 20 );

	float fSeekPosX = ( float )RandomSeekPos( rd ) * 0.1f;
	float fSeekPosZ = ( float )RandomSeekPos( rd ) * 0.1f;

	if( m_pLife != nullptr )
	{
		float fSwaySide = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00003f;
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00001f;

		m_pLife->SetSwaySide( fSwaySide );
		m_pLife->SetSwayVertical( fSwayVertical );
	}

	//  �G�t�F�N�g�̐���
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x + fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z + fSeekPosZ ) ,
							  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  �G�t�F�N�g�̐���
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x - fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z - fSeekPosZ ) ,
							  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  �G�t�F�N�g�̒�~
	EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_AURA ] );

	//  �G�t�F�N�g�̒�~
	EffekseerManager::Stop( m_handle[ EffekseerManager::TYPE_MAGIC_CIRCLE ] );

	//  ���@���������Ă���ꍇ
	if( m_pMagic != nullptr )
	{
		//  ���@�̍폜
		m_pMagic->Delete( );
		m_pMagic = nullptr;
	}

	//  �̗͂�0�ȉ��ɂȂ����ꍇ
	if( m_nLife <= 0 )
	{
		m_nLife = 0;

		m_pLife->SetLife( m_nLife );

		//  �������g�̍폜
		Release( );

		//  CPU�ΐ�̏ꍇ
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			//  �Q�[���I�[�o�[��ʂ�
			Result::SetResult( Result::GAME_CLEAR );
		}	

		//  ���ʉ�ʂւ̈ڍs�t���O�����Ă�
		Game::SetNextMode( Mode::MODE::RESULT );	
	}
	else
	{
		if( bBlow == true )
		{

		}
		else
		{

		}
	}
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�̃u���C�u�_���[�W����
//--------------------------------------------------------------------------------------
int EnemyRauder::BraveDamage( int nDamage )
{
	if( m_nInvisibleTime != 0 )
	{
		return 0;
	}

	int nRealDamage = nDamage;	//  ���ۂ̃_���[�W�i�[
	int nOverDamage = 0;		//  �I�[�o�[�������̃_���[�W�i�[

	//  �u���C�u�_���[�W����
	m_nBrave -= nDamage;

	//  �����̐錾
	std::random_device rd;

	//  �����̐ݒ�
	std::uniform_int_distribution< int > RandomSeekPos( 5 , 20 );

	float fSeekPosX = ( float )RandomSeekPos( rd ) * 0.1f;
	float fSeekPosZ = ( float )RandomSeekPos( rd ) * 0.1f;

	//  �G�t�F�N�g�̐���
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x + fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z + fSeekPosZ ) ,
							  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  �G�t�F�N�g�̐���
	EffekseerManager::Create( EffekseerManager::TYPE_HIT000 , D3DXVECTOR3( m_hitSphere.position.x - fSeekPosX , m_hitSphere.position.y , m_hitSphere.position.z - fSeekPosZ ) ,
							  D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , D3DXVECTOR3( 1.0f , 1.0f , 1.0f ) );

	//  �̗͂�0�ȉ��ɂȂ����ꍇ
	if( m_nBrave <= 0 )
	{
		//  �I�[�o�[�������̑��
		nOverDamage = abs( 0 - m_nBrave );

		m_nBrave = 0;
	}

	//  �I�[�o�[�����������ۂ̃_���[�W�������
	nRealDamage -= nOverDamage;

	return nRealDamage;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�S�ẴG�t�F�N�V�A�̍Đ��X�g�b�v����
//--------------------------------------------------------------------------------------
void EnemyRauder::StopAllEffekseer( void )
{
	for( int i = 0; i < EffekseerManager::TYPE_MAX; i++ )
	{
		if( m_handle[ i ] != -1 )
		{
			EffekseerManager::Stop( m_handle[ i ] );
		}
	}
}

//--------------------------------------------------------------------------------------
//  ���b�N�I����Ԃ̕ύX
//--------------------------------------------------------------------------------------
void EnemyRauder::ChangeLockon( void )
{
	m_bLockOn = !m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  ���݂̃W�����v�͂̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetCurrentJumpPower( float jumpPower )
{
	m_fCurrentJumpPower = jumpPower;
}

//--------------------------------------------------------------------------------------
//  �����x�N�g���̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetVecDirect( D3DXVECTOR3 vecDirect )
{
	m_vecDirect = vecDirect;
}

//--------------------------------------------------------------------------------------
//  �����_�̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetPositionAt( D3DXVECTOR3 positionAt )
{
	m_posAt = positionAt;
}

//--------------------------------------------------------------------------------------
//  ���x�̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetSpeed( float speed )
{
	m_fSpeed = speed;
}

//--------------------------------------------------------------------------------------
//  �ړ��͂̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetMovePower( float movePower )
{
	m_fMovePower = movePower;
}

//--------------------------------------------------------------------------------------
//  ������ї͂̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetBlowPower( float blowPower )
{
	m_fBlowPower = blowPower;
}

//--------------------------------------------------------------------------------------
//  ���G���Ԃ̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetInvisibleTime( int invisibleTime )
{
	m_nInvisibleTime = invisibleTime;
}

//--------------------------------------------------------------------------------------
//  �ǔ������̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetHomingDirect( bool homingDirect )
{
	m_bHomingLeft = homingDirect;
}

//--------------------------------------------------------------------------------------
//  �w�肵����ނ̃G�t�F�N�V�A���ʎq�̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetEffekseerHandle( EffekseerManager::TYPE type , ::Effekseer::Handle handle )
{
	m_handle[ static_cast< int >( type ) ] = handle;
}

//--------------------------------------------------------------------------------------
//  �G�Ɠ����蔻�肷�邩�̃t���O�ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetJudgeHit( bool judgeHit )
{
	m_bJudgeHit = judgeHit;
}

//--------------------------------------------------------------------------------------
//  �h��t���O�̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetGuardFlag( bool guardFlag )
{
	m_bGuard = guardFlag;
}

//--------------------------------------------------------------------------------------
//  ���@�N���X�̃|�C���^�̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetMagic( Magic* magic )
{
	m_pMagic = magic;
}

//--------------------------------------------------------------------------------------
//  ���[�V�����̐ݒ�
//--------------------------------------------------------------------------------------
void EnemyRauder::SetMotion( FBXLoader::MOTION motion )
{
	if( m_fbx != nullptr )
	{
		m_fbx->SetMotion( motion );
	}
}

//--------------------------------------------------------------------------------------
//  �̗͂̑���
//--------------------------------------------------------------------------------------
void EnemyRauder::AddLife( int addLife )
{
	m_nLife += addLife;
}

//--------------------------------------------------------------------------------------
//  ���@�̍폜
//--------------------------------------------------------------------------------------
void EnemyRauder::DeleteMagic( void )
{
	//  ���@���������Ă���ꍇ
	if( m_pMagic != nullptr )
	{
		//  ���@�̍폜
		m_pMagic->Delete( );
		m_pMagic = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  �����x�N�g���̎擾
//--------------------------------------------------------------------------------------
D3DXVECTOR3	EnemyRauder::GetVecDirect( void )
{
	return m_vecDirect;
}

//--------------------------------------------------------------------------------------
//  �����_�̎擾
//--------------------------------------------------------------------------------------
D3DXVECTOR3	EnemyRauder::GetPositionAt( void )
{
	return m_posAt;
}

//--------------------------------------------------------------------------------------
//  ������ѕ����x�N�g���̎擾
//--------------------------------------------------------------------------------------
D3DXVECTOR3	EnemyRauder::GetBlowVecDirect( void )
{
	return m_blowVecDirect;
}

//--------------------------------------------------------------------------------------
//  ��Ԃ̕ω�
//--------------------------------------------------------------------------------------
void EnemyRauder::ChangeState( EnemyRauderState* enemyState )
{
	//  �����̏ꍇ���^�[��
	if( m_enemyState == enemyState )
	{
		return;
	}

	if( m_enemyState != nullptr )
	{
		//  �O��̃G�l�~�[�X�e�[�g�̏I������
		m_enemyState->Uninit( );

		//  ���݂̃G�l�~�[�X�e�[�g�̏���������
		m_enemyState = enemyState;
		m_enemyState->Init( );
	}
}

//--------------------------------------------------------------------------------------
//  ��Ԃ̕ω�
//--------------------------------------------------------------------------------------
void EnemyRauder::ChangeState( EnemyRauder::STATE state )
{
	//  �����̏ꍇ���^�[��
	if( m_enemyState == m_allState[ ( int )state ] )
	{
		return;
	}

	if( m_enemyState != nullptr )
	{
		//  �O��̃G�l�~�[�X�e�[�g�̏I������
		m_enemyState->Uninit( );

		//  ���݂̃G�l�~�[�X�e�[�g�̏���������
		m_enemyState = m_allState[ ( int )state ];
		m_enemyState->Init( );
	}
}

//--------------------------------------------------------------------------------------
//  ���G���Ԃ̎擾
//--------------------------------------------------------------------------------------
int	EnemyRauder::GetInvisibleTime( void )
{
	return m_nInvisibleTime;
}

//--------------------------------------------------------------------------------------
//  ���@�N���X�̃|�C���^�̎擾
//--------------------------------------------------------------------------------------
Magic* EnemyRauder::GetMagic( void )
{
	return m_pMagic;
}

//--------------------------------------------------------------------------------------
//  ���@�̎�ގ擾
//--------------------------------------------------------------------------------------
EnemyRauder::MAGIC EnemyRauder::GetMagicType( void )
{
	return m_magic;
}

//--------------------------------------------------------------------------------------
//  ���x�̎擾
//--------------------------------------------------------------------------------------
float EnemyRauder::GetSpeed( void )
{
	return m_fSpeed;
}

//--------------------------------------------------------------------------------------
//  ��{���x�̎擾
//--------------------------------------------------------------------------------------
float EnemyRauder::GetBaseSpeed( void )
{
	return m_fBaseSpeed;
}

//--------------------------------------------------------------------------------------
//  �W�����v�͂̎擾
//--------------------------------------------------------------------------------------
float EnemyRauder::GetJumpPower( void )
{
	return m_fJumpPower;
}

//--------------------------------------------------------------------------------------
//  ���݂̃W�����v�͂̎擾
//--------------------------------------------------------------------------------------
float EnemyRauder::GetCurrentJumpPower( void )
{
	return m_fCurrentJumpPower;
}

//--------------------------------------------------------------------------------------
//  �ڕW�܂ł̋����̎擾
//--------------------------------------------------------------------------------------
float EnemyRauder::GetTargetDistance( void )
{
	//  �V�[���N���X�̃|�C���^
	Scene* pScene = nullptr;											

	//  �����蔻��p�t�B�[���h�N���X
	HitField* pHitField = nullptr;

	float targetDistance = -1.0f;

#pragma omp parallel for
	//  �D��x�̍ő吔���̃��[�v
	for( int nCntPriority = 0; nCntPriority < MAX_NUM_PRIORITY; nCntPriority++ )
	{
		//  �V�[���̐擪�A�h���X���擾
		pScene = Scene::GetScene( nCntPriority );

		//  �V�[������ł͂Ȃ��ԃ��[�v
		while( pScene != nullptr )
		{
			Scene::OBJTYPE objType;						//  ���̂̎��

			//  ���̂̎�ނ̎擾
			objType = pScene->GetObjType( );

			//  ��ނ��v���C���[�̏ꍇ
			if( objType == Scene::OBJTYPE_PLAYER )
			{
				//  �_�E���L���X�g
				PlayerWinks* pPlayer = ( PlayerWinks* )pScene;

				//  �G�̍��W�̑��
				D3DXVECTOR3 posEnemy = pPlayer->GetPos( );

				//  �����̎Z�o
				targetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
										( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
										( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

				return targetDistance;
			}

			//  ���̃|�C���^����
			pScene = pScene->GetNextScene( pScene );
		}
	}

	return targetDistance;
}

//--------------------------------------------------------------------------------------
//  �ړ��͂̎擾
//--------------------------------------------------------------------------------------
float EnemyRauder::GetMovePower( void )
{
	return m_fMovePower;
}

//--------------------------------------------------------------------------------------
//  ������ї͂̎擾
//--------------------------------------------------------------------------------------
float EnemyRauder::GetBlowPower( void )
{
	return m_fBlowPower;
}

//--------------------------------------------------------------------------------------
//  �̗͂̎擾
//--------------------------------------------------------------------------------------
int EnemyRauder::GetLife( void )
{
	return m_nLife;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[�������蔻������邩�ǂ����̎擾����
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetJudgeHit( void )
{
	return m_bJudgeHit;
}

//--------------------------------------------------------------------------------------
//  �G�l�~�[���h�䂵�Ă��邩�ǂ����̎擾����
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetGuard( void )
{
	return m_bGuard;
}

//--------------------------------------------------------------------------------------
//  ���b�N�I�����̎擾
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetLockon( void )
{
	return m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  �ǔ���������̎擾( false : �E , true : �� )
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetHomingDirect( void )
{
	return m_bHomingLeft;
}

//--------------------------------------------------------------------------------------
//  ���[�V�������I���������̎擾
//--------------------------------------------------------------------------------------
bool EnemyRauder::GetMotionFinish( void )
{
	if( m_fbx != nullptr )
	{
		return m_fbx->GetMotionFinish( );
	}
}

//--------------------------------------------------------------------------------------
//  ��Ԃ̎擾
//--------------------------------------------------------------------------------------
EnemyRauderState* EnemyRauder::GetEnemyState( EnemyRauder::STATE state )
{
	return m_allState[ static_cast< int >( state ) ];
}

//--------------------------------------------------------------------------------------
//  ���݂̏�Ԃ̎擾
//--------------------------------------------------------------------------------------
EnemyRauderState* EnemyRauder::GetCurrentEnemyState( void )
{
	return m_allState[ static_cast< int >( m_state ) ];
}

//--------------------------------------------------------------------------------------
//  �G�t�F�N�V�A���ʎq�̎擾
//--------------------------------------------------------------------------------------
::Effekseer::Handle EnemyRauder::GetEffekseerHandle( EffekseerManager::TYPE type )
{
	return m_handle[ static_cast< int >( type ) ];
}

//--------------------------------------------------------------------------------------
//  �W�����v�ł̈ړ�
//--------------------------------------------------------------------------------------
void EnemyRauder::MoveHeight( float addMove )
{
	m_position.y += addMove;
}

//--------------------------------------------------------------------------------------
//  �W�����v�͂�������
//--------------------------------------------------------------------------------------
void EnemyRauder::DownCurrentJumpPower( float downPower )
{
	m_fCurrentJumpPower -= downPower;
}