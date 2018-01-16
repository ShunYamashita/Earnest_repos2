//--------------------------------------------------------------------------------------
//  �f�v�X�V���h�E   ( stencilShadow.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "depthShadow.h"
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
LPDIRECT3DTEXTURE9	DepthShadow::m_depthShadowTexture = nullptr;			//  �e�N�X�`��
LPDIRECT3DSURFACE9	DepthShadow::m_depthShadowSurface = nullptr;			//  �����_�[�^�[�Q�b�g�p�T�[�t�F�C�X
LPDIRECT3DSURFACE9	DepthShadow::m_shadowMap = nullptr;						//  �e�N�X�`���p�̐[�x�o�b�t�@�[
float				DepthShadow::m_bias = 0.000003f;
const UINT			DepthShadow::TEXTURE_WIDTH = 2048;						//  �e�N�X�`���̕�
const UINT			DepthShadow::TEXTURE_HEIGHT = 2048;						//  �e�N�X�`���̍���

//--------------------------------------------------------------------------------------
//  �f�v�X�V���h�E�N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
DepthShadow::DepthShadow( )
{

}

//--------------------------------------------------------------------------------------
//  �f�v�X�V���h�E�̃f�X�g���N�^
//--------------------------------------------------------------------------------------
DepthShadow::~DepthShadow( )
{

}

//--------------------------------------------------------------------------------------
//  �f�v�X�V���h�E�̏���������
//--------------------------------------------------------------------------------------
HRESULT DepthShadow::Init( void )
{
	m_bias = 0.000003f;

	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  �����_�[�^�[�Q�b�g�p�̋�̃e�N�X�`������
	D3DXCreateTexture( pDevice ,						//  �f�o�C�X
					   TEXTURE_WIDTH ,					//  �X�N���[���̕�
					   TEXTURE_HEIGHT ,					//  �X�N���[���̍���
					   1 ,								//  �~�b�v�}�b�v�̐�
					   D3DUSAGE_RENDERTARGET ,			//  �g�p�p�r
					   D3DFMT_A32B32G32R32F ,			//  �F�̗v�f
					   D3DPOOL_DEFAULT ,				//  �������̔z�u���@
					   &m_depthShadowTexture );			//	�e�N�X�`���i�[�ꏊ

	//  �T�[�t�F�C�X�̎擾
	m_depthShadowTexture->GetSurfaceLevel( 0 , &m_depthShadowSurface );

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
//  �f�v�X�V���h�E�̏I������
//--------------------------------------------------------------------------------------
void DepthShadow::Uninit( void )
{
	//  �[�x�o�b�t�@�p�̃T�[�t�F�C�X�̔j��
	if( m_depthShadowSurface != nullptr )
	{
		m_depthShadowSurface->Release( );
		m_depthShadowSurface = nullptr;
	}

	//  �[�x�o�b�t�@�p�̃e�N�X�`���̔j��
	if( m_depthShadowTexture != nullptr )
	{
		m_depthShadowTexture->Release( );
		m_depthShadowTexture = nullptr;
	}

	//  �V���h�E�}�b�v�p�̃T�[�t�F�C�X�̔j��
	if( m_shadowMap != nullptr )
	{
		m_shadowMap->Release( );
		m_shadowMap = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  �f�v�X�V���h�E�̃o�b�t�@�̃N���A����
//--------------------------------------------------------------------------------------
void DepthShadow::DrawClearBuffer( void )
{
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	pDevice->Clear( 0 , NULL , ( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ) , D3DCOLOR_RGBA( 255 , 255 , 255 , 255 ) , 1.0f , 0 );
}

//--------------------------------------------------------------------------------------
//  �����_�[�^�[�Q�b�g�̐ݒ菈��
//--------------------------------------------------------------------------------------
void DepthShadow::SetRendererTarget( void )
{
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	if( m_depthShadowSurface != nullptr )
	{
		pDevice->SetRenderTarget( 0 , m_depthShadowSurface );
	}
}

//--------------------------------------------------------------------------------------
//  �[�x�o�b�t�@�̃T�[�t�F�[�X�̐ݒ菈��
//--------------------------------------------------------------------------------------
void DepthShadow::SetDepthSerface( void )
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
void DepthShadow::SetBias( float bias )
{
	m_bias = bias;
}

//--------------------------------------------------------------------------------------
//  �o�C�A�X�l�̎擾����
//--------------------------------------------------------------------------------------
float DepthShadow::GetBias( void )
{
	return m_bias;
}

//--------------------------------------------------------------------------------------
//  �����_�[�^�[�Q�b�g�̃e�N�X�`���擾
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 DepthShadow::GetRendereTargetTexture( void )
{
	return m_depthShadowTexture;
}

//--------------------------------------------------------------------------------------
//  �C���X�^���X����������֐�
//--------------------------------------------------------------------------------------
DepthShadow* DepthShadow::Create( void )
{
	DepthShadow *pDepthShadow;

	//  �C���X�^���X����
	pDepthShadow = new DepthShadow;

	//  ������
	pDepthShadow->Init( );

	return pDepthShadow;
}