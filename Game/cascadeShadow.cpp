//--------------------------------------------------------------------------------------
//  �f�v�X�V���h�E   ( stencilShadow.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "cascadeShadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "shaderManager.h"
#include "light.h"
#include "camera.h"
#include "game.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �C���X�^���X����
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9	CascadeShadow::m_cascadeShadowTexture[ MAX_CASCADE ] = { nullptr };		//  �e�N�X�`��
LPDIRECT3DSURFACE9	CascadeShadow::m_cascadeShadowSurface[ MAX_CASCADE ] = { nullptr };		//  �����_�[�^�[�Q�b�g�p�T�[�t�F�C�X
LPDIRECT3DSURFACE9	CascadeShadow::m_shadowMap = nullptr;									//  �e�N�X�`���p�̐[�x�o�b�t�@�[
float				CascadeShadow::m_bias = 0.000013f;										//  �o�C�A�X�l
D3DXMATRIX			CascadeShadow::m_shadowMatrix[ MAX_CASCADE ];							//  �V���h�E�s��
float				CascadeShadow::m_splitPosition[ MAX_CASCADE ];							//  �������Ƃ̍��W

const UINT			CascadeShadow::TEXTURE_WIDTH = 2048;									//  �e�N�X�`���̕�
const UINT			CascadeShadow::TEXTURE_HEIGHT = 2048;									//  �e�N�X�`���̍���

//--------------------------------------------------------------------------------------
//  �J�X�P�[�h�V���h�E�N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
CascadeShadow::CascadeShadow( )
{

}

//--------------------------------------------------------------------------------------
//  �J�X�P�[�h�V���h�E�̃f�X�g���N�^
//--------------------------------------------------------------------------------------
CascadeShadow::~CascadeShadow( )
{

}

//--------------------------------------------------------------------------------------
//  �J�X�P�[�h�V���h�E�̏���������
//--------------------------------------------------------------------------------------
HRESULT CascadeShadow::Init( void )
{
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	for( int countCascade = 0; countCascade < MAX_CASCADE; countCascade++ )
	{
		//  �����_�[�^�[�Q�b�g�p�̋�̃e�N�X�`������
		D3DXCreateTexture( pDevice ,										//  �f�o�C�X
						   TEXTURE_WIDTH ,									//  �X�N���[���̕�
						   TEXTURE_HEIGHT ,									//  �X�N���[���̍���
						   1 ,												//  �~�b�v�}�b�v�̐�
						   D3DUSAGE_RENDERTARGET ,							//  �g�p�p�r
						   D3DFMT_A32B32G32R32F ,							//  �F�̗v�f
						   D3DPOOL_DEFAULT ,								//  �������̔z�u���@
						   &m_cascadeShadowTexture[ countCascade ] );		//	�e�N�X�`���i�[�ꏊ

		//  �T�[�t�F�C�X�̎擾
		m_cascadeShadowTexture[ countCascade ]->GetSurfaceLevel( 0 , &m_cascadeShadowSurface[ countCascade ] );
	}

	// �e�N�X�`���ւ̃����_�����O�Ɏg���[�x�o�b�t�@�[�̍쐬
	if( FAILED( pDevice->CreateDepthStencilSurface( TEXTURE_WIDTH ,
													TEXTURE_HEIGHT ,
													D3DFMT_D24S8,
													D3DMULTISAMPLE_NONE,
													0,
													TRUE,
													&m_shadowMap,
													NULL ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  �J�X�P�[�h�V���h�E�̏I������
//--------------------------------------------------------------------------------------
void CascadeShadow::Uninit( void )
{
	for( int countCascade = 0; countCascade < MAX_CASCADE; countCascade++ )
	{
		//  �[�x�o�b�t�@�p�̃T�[�t�F�C�X�̔j��
		if( m_cascadeShadowSurface[ countCascade ] != nullptr )
		{
			m_cascadeShadowSurface[ countCascade ]->Release( );
			m_cascadeShadowSurface[ countCascade ] = nullptr;
		}

		//  �[�x�o�b�t�@�p�̃e�N�X�`���̔j��
		if( m_cascadeShadowTexture[ countCascade ] != nullptr )
		{
			m_cascadeShadowTexture[ countCascade ]->Release( );
			m_cascadeShadowTexture[ countCascade ] = nullptr;
		}
	}

	//  �V���h�E�}�b�v�p�̃T�[�t�F�C�X�̔j��
	if( m_shadowMap != nullptr )
	{
		m_shadowMap->Release( );
		m_shadowMap = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  �J�X�P�[�h�V���h�E�̍X�V����
//--------------------------------------------------------------------------------------
void CascadeShadow::Update( void )
{
	//  �J�������W�̎擾
	D3DXVECTOR3 cameraPosition = SceneManager::GetCamera( )->GetCameraPosEye( );

	//  �J���������x�N�g���̎擾
	D3DXVECTOR3 cameraVectorDirection = SceneManager::GetCamera( )->GetCameraVecDirect( );

	//  �t�B�[���h�T�C�Y��AABB�쐬
	AABB aabb;
	aabb.min = Game::GetFieldMin( );
	aabb.min = Game::GetFieldMax( );

	//  near/far�i�[�ꏊ
	float nearValue = 1.0f;
	float farValue = 1.0f;

	AdjustClipPlane( aabb , cameraPosition , cameraVectorDirection , nearValue , farValue );

	//  ���������̍��W�i�[
	float splitPositions[ MAX_CASCADE + 1 ];
	SetSplitShadowPosition( MAX_CASCADE , 0.5f , nearValue , farValue , splitPositions );

	//  ���C�g���̎擾
	Light* light = SceneManager::GetLight( );

	//  �J�X�P�[�h����
	for( int countSplit = 0; countSplit < MAX_CASCADE; countSplit++ )
	{
		//  ���C�g�̃r���[�v���W�F�N�V�����s��
		m_shadowMatrix[ countSplit ] = light->GetViewMatrix( ) * light->GetProjectionMatrix( );

		//  AABB���v�Z����
		AABB workAABB = CalculateAABB( splitPositions[ countSplit ] , splitPositions[ countSplit + 1 ] , m_shadowMatrix[ countSplit ] );

		//  �N���b�v�s������߂�
		D3DXMATRIX cropMatrix = CreateCropMatrix( workAABB );

		//  �V���h�E�}�b�v�s��ƕ������Ƃ̍��W�̐ݒ�
		m_shadowMatrix[ countSplit ] = m_shadowMatrix[ countSplit ] * cropMatrix;
		m_splitPosition[ countSplit ] = m_splitPosition[ countSplit + 1 ];
	}
}

//--------------------------------------------------------------------------------------
//  �J�X�P�[�h�V���h�E�̃o�b�t�@�̃N���A����
//--------------------------------------------------------------------------------------
void CascadeShadow::DrawClearBuffer( void )
{
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear( 0 , NULL , ( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ) , D3DCOLOR_RGBA( 255 , 255 , 255 , 255 ) , 1.0f , 0 );
}

//--------------------------------------------------------------------------------------
//  �����_�[�^�[�Q�b�g�̐ݒ菈��
//--------------------------------------------------------------------------------------
void CascadeShadow::SetRendererTarget( int index )
{
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( m_cascadeShadowSurface != nullptr )
	{
		pDevice->SetRenderTarget( 0 , m_cascadeShadowSurface[ index ] );
	}
}

//--------------------------------------------------------------------------------------
//  �[�x�o�b�t�@�̃T�[�t�F�[�X�̐ݒ菈��
//--------------------------------------------------------------------------------------
void CascadeShadow::SetDepthSerface( void )
{
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( m_shadowMap != nullptr )
	{
		//  �e�N�X�`���p�̐[�x�o�b�t�@��ݒ�
		pDevice->SetDepthStencilSurface( m_shadowMap );
	}
}

//--------------------------------------------------------------------------------------
//  �o�C�A�X�l�̐ݒ菈��
//--------------------------------------------------------------------------------------
void CascadeShadow::SetBias( float bias )
{
	m_bias = bias;
}

//--------------------------------------------------------------------------------------
//  �o�C�A�X�l�̎擾����
//--------------------------------------------------------------------------------------
float CascadeShadow::GetBias( void )
{
	return m_bias;
}

//--------------------------------------------------------------------------------------
//  �����_�[�^�[�Q�b�g�̃e�N�X�`���擾
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CascadeShadow::GetRendereTargetTexture( int index )
{
	return m_cascadeShadowTexture[ index ];
}

//--------------------------------------------------------------------------------------
//  �V���h�E�s��̎擾
//--------------------------------------------------------------------------------------
const D3DXMATRIX& CascadeShadow::GetShadowMatrix( int index )
{
	return m_shadowMatrix[ index ];
}

//--------------------------------------------------------------------------------------
//  �������Ƃ̍��W�̎擾
//--------------------------------------------------------------------------------------
const float& CascadeShadow::GetSplitPosition( int index )
{
	return m_splitPosition[ index ];
}

//--------------------------------------------------------------------------------------
//  ���s�����V���h�E�}�b�v�̕����ʒu�̌v�Z
//--------------------------------------------------------------------------------------
void CascadeShadow::SetSplitShadowPosition( int numberSplit ,	
											float lamda , 
											float nearValue ,
											float farValue ,
											float* positions )
{
	//  ��������1�ŁA���ʂ̃V���h�E�}�b�v�̏ꍇ
	if( numberSplit == 1 )
	{
		positions[ 0 ] = nearValue;
		positions[ 1 ] = farValue;
	}

	float rateSplit = 1.0f / ( float )numberSplit;

	//  �[�����Z�̖h�~
	if( nearValue == 0.0f )
	{
		return;
	}

	float farDivisionNear = farValue / nearValue;
	float farSubtractNear = farValue - nearValue;

	//  ���p�����X�L�[���̓K�p
	for( int countSplit = 1; countSplit < MAX_CASCADE + 1; ++countSplit )
	{
		//  �ΐ������X�L�[���v�Z
		float splitLog = nearValue * powf( farDivisionNear , rateSplit * countSplit );

		//  ��l�����X�L�[���̌v�Z
		float splitUni = nearValue + farSubtractNear * countSplit * rateSplit;

		//  ��̒l����`���
		positions[ countSplit ] = lamda * splitLog + splitUni * ( 1.0f - lamda );
	}

	//  �ŏ��ƍŌ��near/far�̑��
	positions[ 0 ] = nearValue;
	positions[ numberSplit ] = farValue;
}

//--------------------------------------------------------------------------------------
//  �N���b�v���ʂ̋����𒲐�
//--------------------------------------------------------------------------------------
void CascadeShadow::AdjustClipPlane( const AABB& aabb ,
									 const D3DXVECTOR3& position ,
									 const D3DXVECTOR3& vecDirect ,
									 float& nearValue , 
									 float& farValue )
{
	//  near/far�̈ꎞ���
	float minZ = nearValue;
	float maxZ = farValue;

	//  �p���W
	D3DXVECTOR3 cornerPosition[ 8 ];

	//  �t�B�[���h�̍ŏ��l�ƍő�l����p�̍��W����
	cornerPosition[ 0 ] = D3DXVECTOR3( aabb.min.x , aabb.max.y , aabb.max.z );
	cornerPosition[ 1 ] = D3DXVECTOR3( aabb.max.x , aabb.max.y , aabb.max.z );
	cornerPosition[ 2 ] = D3DXVECTOR3( aabb.max.x , aabb.min.y , aabb.max.z );
	cornerPosition[ 3 ] = D3DXVECTOR3( aabb.min.x , aabb.min.y , aabb.max.z );
	cornerPosition[ 4 ] = D3DXVECTOR3( aabb.min.x , aabb.max.y , aabb.min.z );
	cornerPosition[ 5 ] = D3DXVECTOR3( aabb.max.x , aabb.max.y , aabb.min.z );
	cornerPosition[ 6 ] = D3DXVECTOR3( aabb.max.x , aabb.min.y , aabb.min.z );
	cornerPosition[ 7 ] = D3DXVECTOR3( aabb.min.x , aabb.min.y , aabb.min.z );

	//  �p����œK��near/far�l�̎Z�o
	for( int countCorner = 0; countCorner < 8; countCorner++ )
	{
		//  �J��������p�ւ̃x�N�g���̎Z�o
		D3DXVECTOR3 cameraPositionToCorner = cornerPosition[ countCorner ] - position;

		//  Z�l�̎Z�o
		float workZ = D3DXVec3Dot( &cameraPositionToCorner , &vecDirect );

		//  �Œ�ƍő�l�����߂�
		minZ = min( workZ , minZ );
		maxZ = max( workZ , maxZ );
	}

	//  �œK��near/far�l�̎Z�o
	nearValue = max( minZ , nearValue );
	farValue = max( maxZ , nearValue + 1.0f );
}

//--------------------------------------------------------------------------------------
//  �p����AABB�����߂�
//--------------------------------------------------------------------------------------
const CascadeShadow::AABB& CascadeShadow::CalculateAABB( const float nearValue ,
														 const float farValue ,
														 const D3DXMATRIX& viewProjection )
{
	//  �J�������̎擾
	Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

	//  AABB�̏�����
	AABB aabb;
	aabb.min = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	aabb.max = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	if( camera == nullptr )
	{
		return aabb;
	}

	//  3�����̃x�N�g�����̎Z�o
	D3DXVECTOR3 vectorZ = camera->GetCameraVecDirect( );
	D3DXVec3Normalize( &vectorZ , &vectorZ );
	D3DXVECTOR3 vectorX;
	D3DXVec3Cross( &vectorX , &camera->GetCameraVectorUp( ) , &vectorZ );
	D3DXVec3Normalize( &vectorX , &vectorX );
	D3DXVECTOR3 vectorY;
	D3DXVec3Cross( &vectorY , &vectorZ , &vectorX );
	D3DXVec3Normalize( &vectorY , &vectorY );

	//  �A�X�y�N�g������߂�
	float aspect = SCREEN_WIDTH / SCREEN_HEIGHT;

	//  �J���������p���̎擾
	float fov = camera->GetCameraFov( );

	//  near/far�̔����̕��ƍ����A�������W�����߂�
	float nearPlaneHalfHeight = tanf( fov * 0.5f ) * nearValue;
	float nearPlaneHalfWidth = nearPlaneHalfHeight * aspect;
	float farPlaneHalfHeight = tanf( fov * 0.5f ) * farValue;
	float farPlaneHalfWidth = farPlaneHalfHeight * aspect;
	D3DXVECTOR3 nearPlaneCenter = camera->GetCameraPosEye( ) + vectorZ * nearValue;
	D3DXVECTOR3 farPlaneCenter = camera->GetCameraPosEye( ) + vectorZ * farValue;

	//  8�̊p
	D3DXVECTOR3 corners[ 8 ];

    corners[ 0 ] = D3DXVECTOR3( nearPlaneCenter - vectorX * nearPlaneHalfWidth - vectorY * nearPlaneHalfHeight );
    corners[ 1 ] = D3DXVECTOR3( nearPlaneCenter - vectorX * nearPlaneHalfWidth + vectorY * nearPlaneHalfHeight );
    corners[ 2 ] = D3DXVECTOR3( nearPlaneCenter + vectorX * nearPlaneHalfWidth + vectorY * nearPlaneHalfHeight );
    corners[ 3 ] = D3DXVECTOR3( nearPlaneCenter + vectorX * nearPlaneHalfWidth - vectorY * nearPlaneHalfHeight );
    corners[ 4 ] = D3DXVECTOR3( farPlaneCenter - vectorX * farPlaneHalfWidth - vectorY * farPlaneHalfHeight );
    corners[ 5 ] = D3DXVECTOR3( farPlaneCenter - vectorX * farPlaneHalfWidth + vectorY * farPlaneHalfHeight );
    corners[ 6 ] = D3DXVECTOR3( farPlaneCenter + vectorX * farPlaneHalfWidth + vectorY * farPlaneHalfHeight );
    corners[ 7 ] = D3DXVECTOR3( farPlaneCenter + vectorX * farPlaneHalfWidth - vectorY * farPlaneHalfHeight );

	//  ��ʏ�̍��W
	D3DXVECTOR3 point;
	D3DXVec3TransformCoord( &point , &corners[ 0 ] , &viewProjection );

	//  �ŏ��l�ƍő�l�Ɉꎞ���
	D3DXVECTOR3 mini = point;
	D3DXVECTOR3 maxi = point;

	//  8�p�̒�����ŏ��l�ƍő�l��AABB�����߂�
    for( int countCorner = 1; countCorner < 8; ++countCorner )
    {
		D3DXVec3TransformCoord( &point , &corners[ countCorner ] , &viewProjection );
        mini  = min( point, mini );
        maxi  = max( point, maxi );
    }

	//  AABB�̑��
	aabb.min = mini;
	aabb.max = maxi;

	return aabb;
}

//--------------------------------------------------------------------------------------
//  �N���b�v�s��̍쐬
//--------------------------------------------------------------------------------------
const D3DXMATRIX& CascadeShadow::CreateCropMatrix( const AABB& aabb )
{
	//  ������
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	float scaleZ = 1.0f;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float offsetZ = 0.0f;

	//  AABB����ŏ��l�ƍő�l�̎擾
	D3DXVECTOR3 mini = aabb.min;
	D3DXVECTOR3 maxi = aabb.max;

	scaleX = 2.0f / ( maxi.x - mini.x );
	scaleY = 2.0f / ( maxi.y - mini.y );

    offsetX = -0.5f * ( maxi.x + mini.x ) * scaleX;
    offsetY = -0.5f * ( maxi.y + mini.y ) * scaleY;

    //  1.0�ȉ��ɂȂ�Ȃ��l�ɒ���
    scaleX = max( 1.0f, scaleX );
    scaleY = max( 1.0f, scaleY );

	//  �N���b�v�s��̍쐬
	D3DXMATRIX cropMatrix;
	cropMatrix._11 = scaleX;
	cropMatrix._12 = 0.0f;
	cropMatrix._13 = 0.0f;
	cropMatrix._14 = 0.0f;
	cropMatrix._21 = 0.0f;
	cropMatrix._22 = scaleY;
	cropMatrix._23 = 0.0f;
	cropMatrix._24 = 0.0f;
	cropMatrix._31 = 0.0f;
	cropMatrix._32 = 0.0f;
	cropMatrix._33 = scaleZ;
	cropMatrix._34 = 0.0f;
	cropMatrix._41 = offsetX;
	cropMatrix._42 = offsetY;
	cropMatrix._43 = offsetZ;
	cropMatrix._44 = 1.0f;

    return cropMatrix;
}