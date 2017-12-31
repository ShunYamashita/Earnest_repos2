//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[ ( �V���h�E�}�b�v )�v���O����   ( shader3DShadowMap.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_3D_SHADOW_MAP_H_
#define _SHADER_3D_SHADOW_MAP_H_

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
//  3D�V�F�[�_�[ ( �V���h�E�}�b�v )�N���X�̒�`
//--------------------------------------------------------------------------------------
class Shader3DShadowMap : public Shader
{
public: 
	Shader3DShadowMap( ){ }														//  �R���X�g���N�^
	~Shader3DShadowMap( ){ }													//  �f�X�g���N�^

	void	Init( void ) override;												//  ������						
	void	Uninit( void ) override;											//  �I��
	void	SetVertexDeclaration( void ) override;								//  ���_�t�H�[�}�b�g�̒�`
	void	SetVertexShader( void ) override;									//  ���_�V�F�[�_�[�̐ݒ�
	void	SetPixelShader( void ) override;									//  �s�N�Z���V�F�[�_�[�̐ݒ�

	void	SetWorldMatrix( const D3DXMATRIX& matrix );							//  ���[���h�s��̐ݒ�
	void	SetLightViewProjectionMatrix( const D3DXMATRIX& matrix );			//  ���C�g���猩���r���[�v���W�F�N�V�����s��̐ݒ�
	void	SetFar( const float& fFar );										//  �t�@�[�̒l�ݒ�

	void	SetShaderInfo( const D3DXMATRIX &worldMatrix ,						//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
						   const D3DXMATRIX &lightViewProjectionMatrix ,
						   const float& fFar );

	void	DrawBegin( void );													//  �V�F�[�_�[�`��J�n

private:

};

#endif
