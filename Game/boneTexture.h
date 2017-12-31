//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`��   ( boneTexture.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _BONE_TEXTURE_H_
#define _BONE_TEXTURE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include <vector>

//--------------------------------------------------------------------------------------
//  �{�[�����i�[�e�N�X�`���N���X�̒�`
//--------------------------------------------------------------------------------------
class BoneTexture
{
public:
	BoneTexture(  );																			//  �f�t�H���g�R���X�g���N�^		
	~BoneTexture( );																			//  �f�X�g���N�^

	static HRESULT				Init( void );													//  ������
	static void					Uninit( void );													//  �I��
	static void					SetBoneInfoTexture( const std::vector< D3DXMATRIX >& matrix );	//  �{�[�����i�[�e�N�X�`���̐ݒ�
	static LPDIRECT3DTEXTURE9	GetBoneInfoTexture( void );										//  �{�[�����i�[�e�N�X�`���̎擾

	static const UINT			TEXTURE_WIDTH;													//  �f�v�X�V���h�E�̕�
	static const UINT			TEXTURE_HEIGHT;													//  �f�v�X�V���h�E�̍���

private:
	static LPDIRECT3DTEXTURE9	m_boneInfoTexture;												//  �{�[�����i�[�e�N�X�`��
};

#endif