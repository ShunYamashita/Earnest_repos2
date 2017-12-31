//--------------------------------------------------------------------------------------
//  �J�X�P�[�h�V���h�E   ( cascadeShadow.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _CASCADE_SHADOW_H_
#define _CASCADE_SHADOW_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------------
static const int MAX_CASCADE = 4;

//--------------------------------------------------------------------------------------
//  �J�X�P�[�h�V���h�E�N���X�̒�`
//--------------------------------------------------------------------------------------
class CascadeShadow
{
public:
	CascadeShadow(  );														//  �f�t�H���g�R���X�g���N�^		
	~CascadeShadow( );														//  �f�X�g���N�^

	static HRESULT				Init( void );								//  ������
	static void					Uninit( void );								//  �I��
	static void					DrawClearBuffer( void );					//  �o�b�t�@�̃N���A
	static void					SetRendererTarget( int index );				//  �����_�[�^�[�Q�b�g�̐ݒ�
	static void					SetDepthSerface( void );					//  �[�x�o�b�t�@�̐ݒ�
	static void					SetBias( float bias );						//  �o�C�A�X�l�̐ݒ�
	static float				GetBias( void );							//  �o�C�A�X�l�̎擾
	static LPDIRECT3DTEXTURE9	GetRendereTargetTexture( int index );		//  �����_�[�^�[�Q�b�g�e�N�X�`���̎擾
	static void					SetSplitShadowPosition( int numberSplit ,	//  ���s�����V���h�E�}�b�v�̕����ʒu�̌v�Z
														float lamda , 
														float nearValue ,
														float farValue ,
														float* positions );

	static const UINT			TEXTURE_WIDTH;								//  �J�X�P�[�h�V���h�E�}�b�v�̕�
	static const UINT			TEXTURE_HEIGHT;								//  �J�X�P�[�h�V���h�E�}�b�v�̍���

private:
	static LPDIRECT3DTEXTURE9	m_cascadeShadowTexture[ MAX_CASCADE ];		//  �e�N�X�`��
	static LPDIRECT3DSURFACE9	m_cascadeShadowSurface[ MAX_CASCADE ];		//  �����_�[�^�[�Q�b�g�p�T�[�t�F�C�X
	static LPDIRECT3DSURFACE9	m_shadowMap;								//  �e�N�X�`���p�̐[�x�o�b�t�@�[
	static float				m_bias;										//  �o�C�A�X�l
};

#endif