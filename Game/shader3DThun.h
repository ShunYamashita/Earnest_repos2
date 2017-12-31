//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[ ( �g�D�[���V�F�[�_�[ )�v���O����   ( shader3DThun.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER3D_THUN_H_
#define _SHADER3D_THUN_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "shader.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[ ( �g�D�[���V�F�[�_�[ )�N���X�̒�`
//--------------------------------------------------------------------------------------
class Shader3DThun : public Shader
{
public: 
	Shader3DThun( ){ }																			//  �R���X�g���N�^
	~Shader3DThun( ){ }																			//  �f�X�g���N�^

	void				Init( void ) override;													//  ������						
	void				Uninit( void ) override;												//  �I��
	void				SetVertexDeclaration( void ) override;									//  ���_�t�H�[�}�b�g�̒�`
	void				SetVertexShader( void ) override;										//  ���_�V�F�[�_�[�̐ݒ�
	void				SetPixelShader( void ) override;										//  �s�N�Z���V�F�[�_�[�̐ݒ�

	void				SetWorldMatrix( const D3DXMATRIX &worldMatrix );						//  ���[���h�s��̐ݒ�
	void				SetViewMatrix( const D3DXMATRIX &viewMatrix );							//  �r���[�s��̐ݒ�
	void				SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );				//  �v���W�F�N�V�����s��̐ݒ�
	void				SetLightDirectionLocal( const D3DXVECTOR3 &lightDirectionLocal );		//  ���C�g�̕����x�N�g���̐ݒ�

	void				SetShaderInfo( const D3DXMATRIX &worldMatrix ,							//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
									   const D3DXMATRIX &viewMatrix ,
									   const D3DXMATRIX &projectionMatrix ,
									   const D3DXVECTOR3 &lightDirectionLocal );

	UINT				GetSamplerTextureIndex( void );											//  �e�N�X�`���T���v���[�̎擾
	UINT				GetSamplerThunIndex( void );											//  �g�D�[���}�b�v�p�̃T���v���[�̎擾

	void				DrawBegin( void );														//  �V�F�[�_�[�`��J�n

private:

};

#endif
