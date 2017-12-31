//--------------------------------------------------------------------------------------
//  �V�F�[�_�[3D�v���O����   ( shader3DRimLightTexture.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_3D_RIM_LIGHT_TEXTURE_H_
#define _SHADER_3D_RIM_LIGHT_TEXTURE_H_

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
//  �V�F�[�_�[3D�N���X�̒�`
//--------------------------------------------------------------------------------------
class Shader3DRimLightTexture : public Shader
{
public: 
	Shader3DRimLightTexture( ){ }																//  �R���X�g���N�^
	~Shader3DRimLightTexture( ){ }																//  �f�X�g���N�^

	void				Init( void ) override;													//  ������						
	void				Uninit( void ) override;												//  �I��
	void				SetVertexDeclaration( void ) override;									//  ���_�t�H�[�}�b�g�̒�`
	void				SetVertexShader( void ) override;										//  ���_�V�F�[�_�[�̐ݒ�
	void				SetPixelShader( void ) override;										//  �s�N�Z���V�F�[�_�[�̐ݒ�

	void				SetWorldMatrix( const D3DXMATRIX &worldMatrix );						//  ���[���h�s��̐ݒ�
	void				SetViewMatrix( const D3DXMATRIX &viewMatrix );							//  �r���[�s��̐ݒ�
	void				SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );				//  �v���W�F�N�V�����s��̐ݒ�
	void				SetWorldInverseTransposeMatrix( const D3DXMATRIX &matrix );				//  ���[���h�t�s��̓]�u�s��̐ݒ�
	void				SetWorldPositionEye( const D3DXVECTOR3 &position );						//  �J�����̃��[���h���W�̐ݒ�
	void				SetLightDirectWorld( const D3DXVECTOR3 &lightDirectWorld );				//  ���C�g�̃��[���h���W�n�̕����x�N�g���̐ݒ�
	void				SetSpecularPower( const float &specularPower );							//  �X�y�L�����[�̓x�����̐ݒ�
	void				SetMaterialDiffuseColor( const D3DXCOLOR &materialDiffuseColor );		//  �}�e���A���g�U���̐ݒ�

	void				SetShaderInfo( const D3DXMATRIX &worldMatrix ,							//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
									   const D3DXMATRIX &viewMatrix ,
									   const D3DXMATRIX &projectionMatrix ,
									   const D3DXMATRIX &worldInverseTransposeMatrix ,
									   const D3DXVECTOR3 &cameraPositionEye ,
									   const D3DXVECTOR3 &lightDirectWorld ,
									   const float &specularPower ,
									   const D3DXCOLOR &materialDiffuseColor  );

	void				DrawBegin( void );														//  �V�F�[�_�[�`��J�n

private:

};

#endif
