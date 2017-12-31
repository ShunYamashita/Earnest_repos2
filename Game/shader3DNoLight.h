//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[ ( ���C�g�Ȃ� )�v���O����   ( shaderNoTexture.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_3D_NO_LIGHT_H_
#define _SHADER_3D_NO_LIGHT_H_

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
//  3D�V�F�[�_�[ ( ���C�g�Ȃ� )�N���X�̒�`
//--------------------------------------------------------------------------------------
class Shader3DNoLight : public Shader
{
public: 
	Shader3DNoLight( ){ }															//  �R���X�g���N�^
	~Shader3DNoLight( ){ }															//  �f�X�g���N�^

	void				Init( void ) override;										//  ������						
	void				Uninit( void ) override;									//  �I��
	void				SetVertexDeclaration( void ) override;						//  ���_�t�H�[�}�b�g�̒�`
	void				SetVertexShader( void ) override;							//  ���_�V�F�[�_�[�̐ݒ�
	void				SetPixelShader( void ) override;							//  �s�N�Z���V�F�[�_�[�̐ݒ�

	void				SetWorldMatrix( const D3DXMATRIX &worldMatrix );			//  ���[���h�s��̐ݒ�
	void				SetViewMatrix( const D3DXMATRIX &viewMatrix );				//  �r���[�s��̐ݒ�
	void				SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );	//  �v���W�F�N�V�����s��̐ݒ�

	void				SetShaderInfo( const D3DXMATRIX &worldMatrix ,				//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
									   const D3DXMATRIX &viewMatrix ,
									   const D3DXMATRIX &projectionMatrix );

	void				DrawBegin( void );											//  �V�F�[�_�[�`��J�n

private:

};

#endif