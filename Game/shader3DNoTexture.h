//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[ ( �e�N�X�`���Ȃ� )�v���O����   ( shaderNoTexture.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER3D_NO_TEXTURE_H_
#define _SHADER3D_NO_TEXTURE_H_

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
//  3D�V�F�[�_�[ ( �e�N�X�`���Ȃ� )�N���X�̒�`
//--------------------------------------------------------------------------------------
class Shader3DNoTexture : public Shader
{
public: 
	Shader3DNoTexture( ){ }																	//  �R���X�g���N�^
	~Shader3DNoTexture( ){ }																	//  �f�X�g���N�^

	void				Init( void ) override;										//  ������						
	void				Uninit( void ) override;									//  �I��
	void				SetVertexDeclaration( void ) override;						//  ���_�t�H�[�}�b�g�̒�`
	void				SetVertexShader( void ) override;							//  ���_�V�F�[�_�[�̐ݒ�
	void				SetPixelShader( void ) override;							//  �s�N�Z���V�F�[�_�[�̐ݒ�

	void				SetWorldMatrix( const D3DXMATRIX &worldMatrix );			//  ���[���h�s��̐ݒ�
	void				SetViewMatrix( const D3DXMATRIX &viewMatrix );				//  �r���[�s��̐ݒ�
	void				SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );	//  �v���W�F�N�V�����s��̐ݒ�
	void				SetLightDirection( const D3DXVECTOR3 &lightDirection );		//  ���C�g�̕����x�N�g���̐ݒ�
	void				SetLightDiffuseColor( const D3DXCOLOR &color );				//  ���C�g�̊g�U���̐ݒ�

	void				SetShaderInfo( const D3DXMATRIX &worldMatrix ,				//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
									   const D3DXMATRIX &viewMatrix ,
									   const D3DXMATRIX &projectionMatrix ,
									   const D3DXVECTOR3 &lightDirection ,
									   const D3DXCOLOR &lightDiffuseColor );

	void				DrawBegin( void );											//  �V�F�[�_�[�`��J�n

private:

};

#endif
