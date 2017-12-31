//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`��   ( boneTexture.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "boneTexture.h"
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
LPDIRECT3DTEXTURE9	BoneTexture::m_boneInfoTexture = nullptr;			//  �e�N�X�`��
const UINT			BoneTexture::TEXTURE_WIDTH = 256;					//  �e�N�X�`���̕�
const UINT			BoneTexture::TEXTURE_HEIGHT = 256;					//  �e�N�X�`���̍���

//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`���N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
BoneTexture::BoneTexture( )
{

}

//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`���̃f�X�g���N�^
//--------------------------------------------------------------------------------------
BoneTexture::~BoneTexture( )
{

}

//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`���̏���������
//--------------------------------------------------------------------------------------
HRESULT BoneTexture::Init( void )
{
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  �����_�[�^�[�Q�b�g�p�̋�̃e�N�X�`������
	D3DXCreateTexture( pDevice ,						//  �f�o�C�X
					   TEXTURE_WIDTH ,					//  �X�N���[���̕�
					   TEXTURE_HEIGHT ,					//  �X�N���[���̍���
					   1 ,								//  �~�b�v�}�b�v�̐�
					   D3DUSAGE_DYNAMIC ,				//  �g�p�p�r
					   D3DFMT_A32B32G32R32F ,			//  �F�̗v�f
					   D3DPOOL_DEFAULT ,				//  �������̔z�u���@
					   &m_boneInfoTexture );			//	�e�N�X�`���i�[�ꏊ

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`���̏I������
//--------------------------------------------------------------------------------------
void BoneTexture::Uninit( void )
{
	//  �{�[�����i�[�e�N�X�`���̔j��
	if( m_boneInfoTexture != nullptr )
	{
		m_boneInfoTexture->Release( );
		m_boneInfoTexture = nullptr;
	}
}

//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`���̐ݒ�
//--------------------------------------------------------------------------------------
void BoneTexture::SetBoneInfoTexture( const std::vector< D3DXMATRIX >& matrix )
{
	D3DLOCKED_RECT lockRect;

	//  �e�N�X�`���������݊J�n
	m_boneInfoTexture->LockRect( 0 ,
								 &lockRect ,
								 nullptr ,
								 D3DLOCK_DISCARD );

	//  ���g�̏�����
	FillMemory( lockRect.pBits ,
				lockRect.Pitch * TEXTURE_HEIGHT , 
				0 );

	//  �{�[�����ƃI�t�Z�b�g�l�̎擾
	int numberBone = matrix.size( );
	int offset = lockRect.Pitch / sizeof( D3DXMATRIX );

	//  �{�[�������̃��[�v
	for( int countBone = 0; countBone < numberBone; ++countBone )
	{
		memcpy( ( D3DXMATRIX* )lockRect.pBits + offset * countBone ,
				&matrix[ countBone ] ,
				sizeof( D3DXMATRIX ) );
	}

	//  �e�N�X�`���������ݏI��
	m_boneInfoTexture->UnlockRect( 0 );
}

//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`���擾
//--------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 BoneTexture::GetBoneInfoTexture( void )
{
	return m_boneInfoTexture;
}