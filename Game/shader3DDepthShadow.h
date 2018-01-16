//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[ ( �[�x�o�b�t�@�ɂ��V���h�E )�v���O����   ( shader3DDepthShadow.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_3D_DEPTH_SHADOW_H_
#define _SHADER_3D_DEPTH_SHADOW_H_

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
//  3D�V�F�[�_�[ ( �[�x�o�b�t�@�ɂ��V���h�E )�N���X�̒�`
//--------------------------------------------------------------------------------------
class Shader3DDepthShadow : public Shader
{
public: 
	Shader3DDepthShadow( ){ }																//  �R���X�g���N�^
	~Shader3DDepthShadow( ){ }																//  �f�X�g���N�^

	void	Init( void ) override;															//  ������						
	void	Uninit( void ) override;														//  �I��
	void	SetVertexDeclaration( void ) override;											//  ���_�t�H�[�}�b�g�̒�`
	void	SetVertexShader( void ) override;												//  ���_�V�F�[�_�[�̐ݒ�
	void	SetPixelShader( void ) override;												//  �s�N�Z���V�F�[�_�[�̐ݒ�

	void	SetWorldMatrix( const D3DXMATRIX &worldMatrix );								//  ���[���h�s��̐ݒ�
	void	SetViewMatrix( const D3DXMATRIX &viewMatrix );									//  �r���[�s��̐ݒ�
	void	SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );						//  �v���W�F�N�V�����s��̐ݒ�
	void	SetLightDirectionLocal( const D3DXVECTOR3 &lightDirectionLocal );				//  ���C�g�̕����x�N�g���̐ݒ�
	void	SetLightViewProjectionMatrix( const D3DXMATRIX &lightViewProjectionMatrix );	//  ���C�g���猩���r���[�v���W�F�N�V�����s��̐ݒ�
	void	SetOffset( const D3DXVECTOR4 &offset );											//  �I�t�Z�b�g�l
	void	SetBias( const float& bias );													//  �o�C�A�X�l�̐ݒ�
	void	SetFar( const float& farValue );												//  �t�@�[�l�̐ݒ�
	UINT	GetSamplerTextureIndex( void );													//  �e�N�X�`���T���v���[�̎擾
	UINT	GetSamplerShadowIndex( void );													//  �V���h�E�T���v���[�̎擾

	void	SetShaderInfo( const D3DXMATRIX &worldMatrix ,									//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
						   const D3DXMATRIX &viewMatrix ,
						   const D3DXMATRIX &projectionMatrix ,
						   const D3DXVECTOR3 &lightDirectionLocal ,
						   const D3DXMATRIX &lightViewProjectionMatrix ,
						   const D3DXVECTOR4 &offset ,
						   const float& bias ,
						   const float& farValue );

	void	DrawBegin( void );																//  �V�F�[�_�[�`��J�n

private:

};

#endif
