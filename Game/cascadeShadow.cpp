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
//  ���s�����V���h�E�}�b�v�̕����ʒu�̌v�Z
//--------------------------------------------------------------------------------------
void CascadeShadow::SetSplitShadowPosition( int numberSplit ,	
											float lamda , 
											float nearValue ,
											float farValue ,
											float* positions )
{

}