//--------------------------------------------------------------------------------------
//  �v���C���[   ( playerWinks.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinks.h"
#include "keyboard.h"
#include "xboxController.h"
#include "joystick.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "scene2D.h"
#include "meshField.h"
#include "enemy.h"
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
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��( �X�e�[�g�n )
//--------------------------------------------------------------------------------------
#include "playerWinksIdle.h"
#include "playerWinksMove.h"
#include "playerWinksJump.h"
#include "playerWinksLanding.h"
#include "playerWinksAttack.h"
#include "playerWinksHPAttack.h"
#include "playerWinksFreeDash.h"
#include "playerWinksLockonDash.h"
#include "playerWinksGuard.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
#define PLAYER_TO_CAMERA_DISTANCE_Y				( 11.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Y2			( 15.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Z				( 25.0f )
#define PLAYER_TO_CAMERA_DISTANCE_Z2			( 35.0f )
#define PLAYER_TO_CUBE_DISTANCE					( 7.0f )
#define PLAYER_TO_CUBE_PUT_DISTANCE				( 2.5f )
#define PLAYER_SIZE								( 2.5f )
#define PLAYER_GOAL_SIZE						( 0.8f )
#define PLAYER_HIT_SIZE							( 2.0f )
#define PLAYER_WAIT_TIME						( 20 )
#define PLAYER_GRAVITY							( 0.1f )
#define PLAYER_GRAVITY2							( 0.2f )
#define PLAYER_ENABLE_NUM_JUMP					( 2 )
#define PLAYER_TO_CAMERA_RATE					( 0.5f )
#define PLAYER_LOCKON_DASH_SPEED				( 3.0f )
#define PLAYER_HIT_SPHERE_LENGTH				( 6.0f )
#define PLAYER_HEAL_LIFE						( 20 )

#define PLAYER_AVOID_SPEED						( 1.5f )

#define PLAYER_HIT_SPHERE_POS_Y					( 3.0f )

#define PLAYER_LIFE_POS_X						( 389.0f )
#define PLAYER_LIFE_POS_Y						( 661.0f )
#define PLAYER_LIFE_SIZE_X						( 350.0f )
#define PLAYER_LIFE_SIZE_Y						( 49.0f )

#define PLAYER_VS_LIFE_POS_X					( 125.0f )
#define PLAYER_VS_LIFE_POS_Y					( 646.0f )
#define PLAYER_VS_LIFE_SIZE_X					( 442.0f )
#define PLAYER_VS_LIFE_SIZE_Y					( 38.0f )

#define PLAYER_BRAVE_POS_X						( 440.0f )
#define PLAYER_BRAVE_POS_Y						( 570.0f )
#define PLAYER_BRAVE_SIZE_X						( 60.0f )
#define PLAYER_BRAVE_SIZE_Y						( 60.0f )

#define PLAYER_UI_POS_X							( SCREEN_WIDTH * 0.25f )
#define PLAYER_UI_POS_Y							( SCREEN_HEIGHT * 0.88f )
#define PLAYER_UI_SIZE_X						( 640.0f )
#define PLAYER_UI_SIZE_Y						( 200.0f )

#define PLAYER2_LIFE_POS_X						( 975.0f )
#define PLAYER2_LIFE_POS_Y						( 605.0f )
#define PLAYER2_LIFE_SIZE_X						( 255.0f )
#define PLAYER2_LIFE_SIZE_Y						( 30.0f )

#define PLAYER2_VS_LIFE_POS_X					( 715.0f )
#define PLAYER2_VS_LIFE_POS_Y					( 646.0f )
#define PLAYER2_VS_LIFE_SIZE_X					( 442.0f )
#define PLAYER2_VS_LIFE_SIZE_Y					( 38.0f )

#define PLAYER2_BRAVE_POS_X						( 990.0f )
#define PLAYER2_BRAVE_POS_Y						( 565.0f )
#define PLAYER2_BRAVE_SIZE_X					( 35.0f )
#define PLAYER2_BRAVE_SIZE_Y					( 35.0f )

#define PLAYER2_UI_POS_X						( SCREEN_WIDTH * 0.75f )
#define PLAYER2_UI_POS_Y						( SCREEN_HEIGHT * 0.88f )
#define PLAYER2_UI_SIZE_X						( 640.0f )
#define PLAYER2_UI_SIZE_Y						( 200.0f )

#define PLAYER_JUMP_							( 0.4f )

#define PLAYER_FINISH_SPEED						( 3.0f )

#define PLAYER_HOMIMG_TIME						( 90.0f )
#define PLAYER_ENABLE_HP_ATTACK_DIST			( 150.0f )
#define PLAYER_HP_ATTACK_SPEED					( 1.0f )

static const int	PLAYER_HP_DAMAGE			= 150;
static const int	PLAYER_FINISHER_DAMAGE		= 250;

static const float	PLAYER_HP_RANGE				= 30.0f;
static const float	PLAYER_HP_RANGE_HEIGHT		= 12.0f;

static const float	PLAYER_HP_BLOW_POWER		= 3.0f;
static const float	PLAYER_FINISHER_BLOW_POWER	= 7.0f;

static const int	PLAYER_GUARD_FRAME			= 5;

static const float	CAMERA_ROTATE_ANGLE			= 0.025f;

static const float	PLAYER_HEIGHT				= 10.0f;

static const float	PLAYER_SHADOW_SCALE			= 0.55f;

static const float	PLAYER_JUMP_POWER			= 2.0f;

//--------------------------------------------------------------------------------------
//  �C���X�^���X����
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �v���C���[�N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
PlayerWinks::PlayerWinks( ) : Scene( 3 )
{
	m_beforePos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_vecDirect = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	m_fSpeed = 0.0f;
	m_nWaitTime = 0;
	m_fCurrentJumpPower = 0.0f;
	m_nCntJump = 0;
	m_bLockOn = true;
	m_hitSphere.fLength = PLAYER_HIT_SPHERE_LENGTH * 0.5f;
	m_hitSphere.position = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_pLife = nullptr;
	m_pPlayerWinksUI = nullptr;
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
	m_fJumpPower = PLAYER_JUMP_POWER;
}

//--------------------------------------------------------------------------------------
//  �v���C���[�N���X�̃f�X�g���N�^
//--------------------------------------------------------------------------------------
PlayerWinks::~PlayerWinks( )
{

}

//--------------------------------------------------------------------------------------
//  �v���C���[�̏���������
//--------------------------------------------------------------------------------------
HRESULT PlayerWinks::Init( void )
{
	m_drawDepth = true;

	//  ���̂̎�ނ̐ݒ�
	Scene::SetObjType( Scene::OBJTYPE_PLAYER );

	if( m_nPlayerNo == 0 )
	{
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			//  �̗͂̐���
			m_pLife = Life::Create( Life::TYPE_LIFE000 ,
									D3DXVECTOR3( PLAYER_LIFE_POS_X , PLAYER_LIFE_POS_Y , 0.0f ) ,
									D3DXVECTOR3( PLAYER_LIFE_SIZE_X , PLAYER_LIFE_SIZE_Y , 0.0f ) ,
									D3DXVECTOR2( 0.0f , 0.0f ) ,
									D3DXVECTOR2( 1.0f , 1.0f ) ,
									D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) ,
									m_nLife );
		}
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  �v���C���[UI�̐���
			m_pPlayerWinksUI = PlayerUI::Create( PlayerUI::TYPE_PLAYER1 ,
												 D3DXVECTOR3( PLAYER_UI_POS_X , PLAYER_UI_POS_Y , 0.0f ) ,
												 D3DXVECTOR3( PLAYER_UI_SIZE_X , PLAYER_UI_SIZE_Y , 0.0f ) );

			//  �̗͂̐���
			m_pLife = Life::Create( Life::TYPE_LIFE002 ,
									D3DXVECTOR3( PLAYER_VS_LIFE_POS_X , PLAYER_VS_LIFE_POS_Y , 0.0f ) ,
									D3DXVECTOR3( PLAYER_VS_LIFE_SIZE_X , PLAYER_VS_LIFE_SIZE_Y , 0.0f ) ,
									D3DXVECTOR2( 0.0f , 0.0f ) ,
									D3DXVECTOR2( 1.0f , 1.0f ) ,
									D3DXCOLOR( 0.1f , 0.9f , 1.0f , 1.0f ) ,
									m_nLife );
		}

		//  �̗͂̐ݒ�
		m_pLife->SetEndLife( m_nLife );										 
	}
	else if( m_nPlayerNo == 1 )
	{
		if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
		{
			m_pLife = Life::Create( Life::TYPE_LIFE001 ,
									D3DXVECTOR3( PLAYER2_LIFE_POS_X , PLAYER2_LIFE_POS_Y , 0.0f ) ,
									D3DXVECTOR3( PLAYER2_LIFE_SIZE_X , PLAYER2_LIFE_SIZE_Y , 0.0f ) ,
									D3DXVECTOR2( 0.0f , 0.0f ) ,
									D3DXVECTOR2( 1.0f , 1.0f ) ,
									D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) ,
									m_nLife );
		}
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			//  �v���C���[UI�̐���
			m_pPlayerWinksUI = PlayerUI::Create( PlayerUI::TYPE_PLAYER2 ,
												 D3DXVECTOR3( PLAYER2_UI_POS_X , PLAYER2_UI_POS_Y , 0.0f ) ,
												 D3DXVECTOR3( PLAYER2_UI_SIZE_X , PLAYER2_UI_SIZE_Y , 0.0f ) );

			m_pLife = Life::Create( Life::TYPE_LIFE003 ,
									D3DXVECTOR3( PLAYER2_VS_LIFE_POS_X , PLAYER2_VS_LIFE_POS_Y , 0.0f ) ,
									D3DXVECTOR3( PLAYER2_VS_LIFE_SIZE_X , PLAYER2_VS_LIFE_SIZE_Y , 0.0f ) ,
									D3DXVECTOR2( 0.0f , 0.0f ) ,
									D3DXVECTOR2( 1.0f , 1.0f ) ,
									D3DXCOLOR( 0.1f , 0.9f , 1.0f , 1.0f ) ,
									m_nLife );
		}

		//  �̗͂̐ݒ�
		m_pLife->SetEndLife( m_nLife );
	}

	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( m_nPlayerNo );

	D3DXVECTOR3 cameraVecDirect = pCamera->GetCameraVecDirect( );

	D3DXVECTOR3 workPosEye = m_position;

	//  CPU�ΐ�̏ꍇ
	if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
	{
		workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y;
		workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z;
	}
	//  �v���C���[�ΐ�̏ꍇ
	else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
	{
		workPosEye.y += PLAYER_TO_CAMERA_DISTANCE_Y2;
		workPosEye -= cameraVecDirect * PLAYER_TO_CAMERA_DISTANCE_Z2;
	}

	//  �J�����̎��_�ݒ�
	pCamera->SetCameraPosEyeDirect( workPosEye );

	D3DXVECTOR3 workPosAt = m_position;
	workPosAt.z += 5.0f;

	//  �J�����̒����_�ݒ�
	pCamera->SetCameraPosAtDirect( workPosAt );

	//  �v���C���[�����_�̐ݒ�
	m_posAt = m_position + cameraVecDirect * 5.0f;

	//  �����x�N�g���̑��
	D3DXVec3Normalize( &m_vecDirect , &( m_posAt - m_position ) );

	//  �v���C���[�X�e�[�g�̐���
	m_allState[ static_cast< int >( PlayerWinks::STATE::IDLE ) ] = new PlayerWinksIdle( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::MOVE ) ] = new PlayerWinksMove( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::JUMP ) ] = new PlayerWinksJump( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::LANDING ) ] = new PlayerWinksLanding( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::ATTACK ) ] = new PlayerWinksAttack( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::HP_ATTACK ) ] = new PlayerWinksHPAttack( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::FREE_DASH ) ] = new PlayerWinksFreeDash( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::LOCKON_DASH ) ] = new PlayerWinksLockonDash( this );
	m_allState[ static_cast< int >( PlayerWinks::STATE::GUARD ) ] = new PlayerWinksGuard( this );

	//  �j���[�g������Ԃŏ�����
	m_playerState = m_allState[ static_cast< int >( PlayerWinks::STATE::IDLE ) ];

	//  FBX�f�[�^�̍쐬
	m_fbx = FBXLoader::Create( FBXLoader::TYPE::WINKS , m_position , m_posAt , 0.06f );
	m_fbx->SetMotion( FBXLoader::MOTION::IDLE );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  �v���C���[�̏I������
//--------------------------------------------------------------------------------------
void PlayerWinks::Uninit( void )
{
	if( m_pLife != nullptr )
	{
		m_pLife = nullptr;
	}

	if( m_pPlayerWinksUI != nullptr )
	{
		m_pPlayerWinksUI = nullptr;
	}

	if( m_fbx != nullptr )
	{
		m_fbx->Uninit( );
		m_fbx = nullptr;
	}

	//  �v���C���[�X�e�[�g�̐����̃��[�v
	for( int countState = 0; countState < static_cast< int >( PlayerWinks::STATE::MAX ) - 1; ++countState )
	{
		m_allState[ countState ]->Uninit( );
		delete m_allState[ countState ];
		m_allState[ countState ] = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  �v���C���[�̍X�V����
//--------------------------------------------------------------------------------------
void PlayerWinks::Update( void )
{
	//  �J�n�{�C�X���܂��Đ����Ă��Ȃ��ꍇ
	if( m_bVoiceStart == false )
	{
		m_bVoiceStart = true;
	}

	//  �󂯂鑤�̓����蔻��̑��
	m_hitSphere.position = m_position;
	m_hitSphere.position.y += PLAYER_HIT_SPHERE_POS_Y;

	//  �v���C���[�X�e�[�g�N���X�̍X�V
	if( m_playerState != nullptr )
	{
		m_playerState->Update( );
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
//  �v���C���[�̕`�揈��
//--------------------------------------------------------------------------------------
void PlayerWinks::Draw( void )
{
	Camera* camera = SceneManager::GetCamera( );

	D3DXVECTOR3 cameraVectorDirect;
	D3DXVECTOR3 cameraToPlayerWinks;

	if( camera != nullptr )
	{
		D3DXVECTOR3 cameraPosition = camera->GetCameraPosEye( );
		D3DXVECTOR3 playerWinksPosition = m_position;
		cameraPosition.y = 0.0f;
		playerWinksPosition.y = 0.0f;

		cameraToPlayerWinks = playerWinksPosition - cameraPosition;
		D3DXVec3Normalize( &cameraToPlayerWinks , &cameraToPlayerWinks );
		cameraVectorDirect = camera->GetCameraVecDirect( );
		cameraVectorDirect.y = 0.0f;
		D3DXVec3Normalize( &cameraVectorDirect , &cameraVectorDirect );
	}

	if( Utility::IntoView( cameraVectorDirect , cameraToPlayerWinks , D3DX_PI * 0.5f ) )
	{
		if( m_fbx != nullptr )
		{
			m_fbx->SetDrawInfo( m_position , m_posAt );
			m_fbx->Draw( );
		}
	}
}

//--------------------------------------------------------------------------------------
//  �v���C���[�̕`�揈��
//--------------------------------------------------------------------------------------
void PlayerWinks::DrawDepth( void )
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
bool PlayerWinks::JudgeHitGround( void )
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
bool PlayerWinks::JudgeHitEnemy( void )
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

				//  CPU�ΐ탂�[�h�̏ꍇ
				if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
				{
					//  ��ނ��G�̏ꍇ
					if( objType == Scene::OBJTYPE_ENEMY )
					{
						//  �_�E���L���X�g
						Enemy* pEnemy = ( Enemy* )pScene;

						float fDist = 0.0f;

						if( Utility::HitSphere( m_hitSphere , pEnemy->GetHitSphere( ) , &fDist ) )
						{
							//  ���x���Ȃ���
							m_fSpeed = 0.0f;

							//  ���˂���x�N�g�������߂�
							D3DXVECTOR3 reflectVec = m_hitSphere.position - pEnemy->GetHitSphere( ).position;
							reflectVec.y = 0.0f;

							D3DXVec3Normalize( &reflectVec , &reflectVec );

							//  �߂荞�񂾕����W��߂�
							m_position += reflectVec * fDist;

							return true;
						}
					}
				}
				//  �v���C���[�ΐ탂�[�h�̏ꍇ
				if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
				{
					//  ��ނ��v���C���[�̏ꍇ
					if( objType == Scene::OBJTYPE_PLAYER )
					{
						//  �_�E���L���X�g
						PlayerWinks* pPlayerWinks = ( PlayerWinks* )pScene;

						//  �����ȊO�̃v���C���[�ł��A�����蔻�������ꍇ
						if( m_nPlayerNo != pPlayerWinks->GetPlayerNo( ) && pPlayerWinks->GetJudgeHit( ) )
						{
							float fDist = 0.0f;

							if( Utility::HitSphere( m_hitSphere , pPlayerWinks->GetHitSphere( ) , &fDist ) )
							{
								//  ���x���Ȃ���
								m_fSpeed = 0.0f;

								//  ���˂���x�N�g�������߂�
								D3DXVECTOR3 reflectVec = m_hitSphere.position - pPlayerWinks->GetHitSphere( ).position;
								reflectVec.y = 0.0f;

								D3DXVec3Normalize( &reflectVec , &reflectVec );

								//  �߂荞�񂾕����W��߂�
								m_position += reflectVec * fDist;

								return true;
							}
						}
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
bool PlayerWinks::JudgeHitWall( void )
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
PlayerWinks* PlayerWinks::Create( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale , float fSpeed , MAGIC magic , int nPlayerWinksNo )
{
	PlayerWinks* pPlayerWinks;

	//  �C���X�^���X����
	pPlayerWinks = new PlayerWinks;

	//  ���W�̑��
	pPlayerWinks->m_position = position;

	//  �ŏ��̍��W�̑��
	pPlayerWinks->m_firstPos = position;

	//  �����_���W�̑��
	pPlayerWinks->m_posAt = position;

	//  ��]�p�̑��
	pPlayerWinks->m_rot = rot;

	//  �傫���{���̑��
	pPlayerWinks->m_scale = scale;

	//  ���x�̑��
	pPlayerWinks->m_fBaseSpeed = fSpeed;

	//  �������@�̑��
	pPlayerWinks->m_magic = magic;

	//  �v���C���[�ԍ��̑��
	pPlayerWinks->m_nPlayerNo = nPlayerWinksNo;

	//  ������
	pPlayerWinks->Init( );

	return pPlayerWinks;
}

//--------------------------------------------------------------------------------------
//  �v���C���[�̃_���[�W����
//--------------------------------------------------------------------------------------
void PlayerWinks::Damage( D3DXVECTOR3 blowVecDirect , float fBlowPower , int nDamage , bool bBlow )
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

	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( m_nPlayerNo );

	if( pCamera != nullptr )
	{
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.0000006f;

		//  �c�h�ꕪ�̑��
		pCamera->SetSwayVertical( fSwayVertical );
	}

	if( m_pPlayerWinksUI != nullptr )
	{
		float fSwaySide = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00003f;
		float fSwayVertical = ( float )nDamage * ( float )nDamage * ( float )nDamage * 0.00001f;

		if( m_nPlayerNo == 0 )
		{
			m_pPlayerWinksUI->SetSwaySide( fSwaySide );
		}
		else if( m_nPlayerNo == 1 )
		{
			m_pPlayerWinksUI->SetSwaySide( -fSwaySide );
		}
		
		m_pPlayerWinksUI->SetSwayVertical( fSwayVertical );
	}

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
			Result::SetResult( Result::GAME_OVER );
		}
		//  �v���C���[�ΐ�̏ꍇ
		else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
		{
			if( m_nPlayerNo == 0 )
			{
				//  �v���C���[2WIN��ʂ�
				Result::SetResult( Result::GAME_PLAYER2_WIN );
			}
			else if( m_nPlayerNo == 1 )
			{
				//  �v���C���[1WIN��ʂ�
				Result::SetResult( Result::GAME_PLAYER1_WIN );
			}
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
//  �v���C���[�̃u���C�u�_���[�W����
//--------------------------------------------------------------------------------------
int PlayerWinks::BraveDamage( int nDamage )
{
	if( m_nInvisibleTime != 0 )
	{
		return 0;
	}

	int nRealDamage = nDamage;	//  ���ۂ̃_���[�W�i�[
	int nOverDamage = 0;		//  �I�[�o�[�������̃_���[�W�i�[

	//  �J�����̎擾
	Camera* pCamera = SceneManager::GetCamera( m_nPlayerNo );

	if( pCamera != nullptr )
	{
		float fSwayVertical = ( float )nDamage * ( float )nDamage * 0.0006f;

		//  �c�h�ꕪ�̑��
		pCamera->SetSwayVertical( fSwayVertical );
	}

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
//  �v���C���[�S�ẴG�t�F�N�V�A�̍Đ��X�g�b�v����
//--------------------------------------------------------------------------------------
void PlayerWinks::StopAllEffekseer( void )
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
void PlayerWinks::ChangeLockon( void )
{
	m_bLockOn = !m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  ���݂̃W�����v�͂̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetCurrentJumpPower( float jumpPower )
{
	m_fCurrentJumpPower = jumpPower;
}

//--------------------------------------------------------------------------------------
//  �����x�N�g���̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetVecDirect( D3DXVECTOR3 vecDirect )
{
	m_vecDirect = vecDirect;
}

//--------------------------------------------------------------------------------------
//  �����_�̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetPositionAt( D3DXVECTOR3 positionAt )
{
	m_posAt = positionAt;
}

//--------------------------------------------------------------------------------------
//  ���x�̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetSpeed( float speed )
{
	m_fSpeed = speed;
}

//--------------------------------------------------------------------------------------
//  �ړ��͂̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetMovePower( float movePower )
{
	m_fMovePower = movePower;
}

//--------------------------------------------------------------------------------------
//  ������ї͂̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetBlowPower( float blowPower )
{
	m_fBlowPower = blowPower;
}

//--------------------------------------------------------------------------------------
//  ���G���Ԃ̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetInvisibleTime( int invisibleTime )
{
	m_nInvisibleTime = invisibleTime;
}

//--------------------------------------------------------------------------------------
//  �ǔ������̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetHomingDirect( bool homingDirect )
{
	m_bHomingLeft = homingDirect;
}

//--------------------------------------------------------------------------------------
//  �w�肵����ނ̃G�t�F�N�V�A���ʎq�̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetEffekseerHandle( EffekseerManager::TYPE type , ::Effekseer::Handle handle )
{
	m_handle[ static_cast< int >( type ) ] = handle;
}

//--------------------------------------------------------------------------------------
//  �G�Ɠ����蔻�肷�邩�̃t���O�ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetJudgeHit( bool judgeHit )
{
	m_bJudgeHit = judgeHit;
}

//--------------------------------------------------------------------------------------
//  �h��t���O�̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetGuardFlag( bool guardFlag )
{
	m_bGuard = guardFlag;
}

//--------------------------------------------------------------------------------------
//  ���@�N���X�̃|�C���^�̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetMagic( Magic* magic )
{
	m_pMagic = magic;
}

//--------------------------------------------------------------------------------------
//  ���[�V�����̐ݒ�
//--------------------------------------------------------------------------------------
void PlayerWinks::SetMotion( FBXLoader::MOTION motion )
{
	if( m_fbx != nullptr )
	{
		m_fbx->SetMotion( motion );
	}
}

//--------------------------------------------------------------------------------------
//  �̗͂̑���
//--------------------------------------------------------------------------------------
void PlayerWinks::AddLife( int addLife )
{
	m_nLife += addLife;
}

//--------------------------------------------------------------------------------------
//  ���@�̍폜
//--------------------------------------------------------------------------------------
void PlayerWinks::DeleteMagic( void )
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
D3DXVECTOR3	PlayerWinks::GetVecDirect( void )
{
	return m_vecDirect;
}

//--------------------------------------------------------------------------------------
//  �����_�̎擾
//--------------------------------------------------------------------------------------
D3DXVECTOR3	PlayerWinks::GetPositionAt( void )
{
	return m_posAt;
}

//--------------------------------------------------------------------------------------
//  ������ѕ����x�N�g���̎擾
//--------------------------------------------------------------------------------------
D3DXVECTOR3	PlayerWinks::GetBlowVecDirect( void )
{
	return m_blowVecDirect;
}

//--------------------------------------------------------------------------------------
//  ��Ԃ̕ω�
//--------------------------------------------------------------------------------------
void PlayerWinks::ChangeState( PlayerWinksState* playerState )
{
	//  �����̏ꍇ���^�[��
	if( m_playerState == playerState )
	{
		return;
	}

	if( m_playerState != nullptr )
	{
		//  �O��̃v���C���[�X�e�[�g�̏I������
		m_playerState->Uninit( );

		//  ���݂̃v���C���[�X�e�[�g�̏���������
		m_playerState = playerState;
		m_playerState->Init( );
	}
}

//--------------------------------------------------------------------------------------
//  �v���C���[�ԍ��̎擾
//--------------------------------------------------------------------------------------
int	PlayerWinks::GetPlayerNo( void )
{
	return m_nPlayerNo;
}

//--------------------------------------------------------------------------------------
//  ���G���Ԃ̎擾
//--------------------------------------------------------------------------------------
int	PlayerWinks::GetInvisibleTime( void )
{
	return m_nInvisibleTime;
}

//--------------------------------------------------------------------------------------
//  ���@�N���X�̃|�C���^�̎擾
//--------------------------------------------------------------------------------------
Magic* PlayerWinks::GetMagic( void )
{
	return m_pMagic;
}

//--------------------------------------------------------------------------------------
//  ���@�̎�ގ擾
//--------------------------------------------------------------------------------------
PlayerWinks::MAGIC PlayerWinks::GetMagicType( void )
{
	return m_magic;
}

//--------------------------------------------------------------------------------------
//  ���x�̎擾
//--------------------------------------------------------------------------------------
float PlayerWinks::GetSpeed( void )
{
	return m_fSpeed;
}

//--------------------------------------------------------------------------------------
//  ��{���x�̎擾
//--------------------------------------------------------------------------------------
float PlayerWinks::GetBaseSpeed( void )
{
	return m_fBaseSpeed;
}

//--------------------------------------------------------------------------------------
//  �W�����v�͂̎擾
//--------------------------------------------------------------------------------------
float PlayerWinks::GetJumpPower( void )
{
	return m_fJumpPower;
}

//--------------------------------------------------------------------------------------
//  ���݂̃W�����v�͂̎擾
//--------------------------------------------------------------------------------------
float PlayerWinks::GetCurrentJumpPower( void )
{
	return m_fCurrentJumpPower;
}

//--------------------------------------------------------------------------------------
//  �ڕW�܂ł̋����̎擾
//--------------------------------------------------------------------------------------
float PlayerWinks::GetTargetDistance( void )
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

			//  CPU�ΐ탂�[�h�̏ꍇ
			if( Game::GetModeVS( ) == Game::MODE_VS_CPU )
			{
				//  ��ނ��G�̏ꍇ
				if( objType == Scene::OBJTYPE_ENEMY )
				{
					//  �_�E���L���X�g
					Enemy* pEnemy = ( Enemy* )pScene;

					//  �G�̍��W�̑��
					D3DXVECTOR3 posEnemy = pEnemy->GetPos( );

					//  �����̎Z�o
					targetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
											( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
											( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

					return targetDistance;
				}
			}
			//  �v���C���[�ΐ탂�[�h�̏ꍇ
			else if( Game::GetModeVS( ) == Game::MODE_VS_PLAYER )
			{
				if( objType == Scene::OBJTYPE_PLAYER )
				{
					//  �_�E���L���X�g
					PlayerWinks* pPlayerWinks = ( PlayerWinks* )pScene;

					//  �����ȊO�̃v���C���[�ԍ��̏ꍇ
					if( m_nPlayerNo != pPlayerWinks->GetPlayerNo( ) )
					{
						//  �G�̍��W�̑��
						D3DXVECTOR3 posEnemy = pPlayerWinks->GetPos( );

						//  �����̎Z�o
						targetDistance = sqrtf( ( posEnemy.x - m_position.x ) * ( posEnemy.x - m_position.x ) +
												( posEnemy.y - m_position.y ) * ( posEnemy.y - m_position.y ) + 
												( posEnemy.z - m_position.z ) * ( posEnemy.z - m_position.z ) );

						return targetDistance;
					}
				}
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
float PlayerWinks::GetMovePower( void )
{
	return m_fMovePower;
}

//--------------------------------------------------------------------------------------
//  ������ї͂̎擾
//--------------------------------------------------------------------------------------
float PlayerWinks::GetBlowPower( void )
{
	return m_fBlowPower;
}

//--------------------------------------------------------------------------------------
//  �̗͂̎擾
//--------------------------------------------------------------------------------------
int PlayerWinks::GetLife( void )
{
	return m_nLife;
}

//--------------------------------------------------------------------------------------
//  �v���C���[�������蔻������邩�ǂ����̎擾����
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetJudgeHit( void )
{
	return m_bJudgeHit;
}

//--------------------------------------------------------------------------------------
//  �v���C���[���h�䂵�Ă��邩�ǂ����̎擾����
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetGuard( void )
{
	return m_bGuard;
}

//--------------------------------------------------------------------------------------
//  ���b�N�I�����̎擾
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetLockon( void )
{
	return m_bLockOn;
}

//--------------------------------------------------------------------------------------
//  �ǔ���������̎擾( false : �E , true : �� )
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetHomingDirect( void )
{
	return m_bHomingLeft;
}

//--------------------------------------------------------------------------------------
//  ���[�V�������I���������̎擾
//--------------------------------------------------------------------------------------
bool PlayerWinks::GetMotionFinish( void )
{
	if( m_fbx != nullptr )
	{
		return m_fbx->GetMotionFinish( );
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  ��Ԃ̎擾
//--------------------------------------------------------------------------------------
PlayerWinksState* PlayerWinks::GetPlayerState( PlayerWinks::STATE state )
{
	return m_allState[ static_cast< int >( state ) ];
}

//--------------------------------------------------------------------------------------
//  �G�t�F�N�V�A���ʎq�̎擾
//--------------------------------------------------------------------------------------
::Effekseer::Handle PlayerWinks::GetEffekseerHandle( EffekseerManager::TYPE type )
{
	return m_handle[ static_cast< int >( type ) ];
}

//--------------------------------------------------------------------------------------
//  �W�����v�ł̈ړ�
//--------------------------------------------------------------------------------------
void PlayerWinks::MoveHeight( float addMove )
{
	m_position.y += addMove;
}

//--------------------------------------------------------------------------------------
//  �W�����v�͂�������
//--------------------------------------------------------------------------------------
void PlayerWinks::DownCurrentJumpPower( float downPower )
{
	m_fCurrentJumpPower -= downPower;
}

//--------------------------------------------------------------------------------------
//  ���[�V�����t���[���擾
//--------------------------------------------------------------------------------------
int PlayerWinks::GetMotionFrame( void )
{
	if( m_fbx != nullptr )
	{
		return m_fbx->GetMotionFrame( );
	}
}

//--------------------------------------------------------------------------------------
//  ���[�V�����S�̂���t���[�������擾
//--------------------------------------------------------------------------------------
float PlayerWinks::GetMotionFrameRate( void )
{
	if( m_fbx != nullptr )
	{
		return m_fbx->GetMotionFrameRate( );
	}
}