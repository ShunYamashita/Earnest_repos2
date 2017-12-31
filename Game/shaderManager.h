//--------------------------------------------------------------------------------------
//  �V�F�[�_�[�Ǘ��N���X   ( shaderManager.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "shader3D.h"
#include "shader3DNoTexture.h"
#include "shader3DNoLight.h"
#include "shader3DRimLightTexture.h"
#include "shader3DDepthShadow.h"
#include "shader3DShadowMap.h"
#include "shader3DThun.h"
#include "shader3DOutline.h"
#include "shader3DPlayer.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------------
class Shader;

//--------------------------------------------------------------------------------------
//  �V�F�[�_�[�Ǘ��N���X�̒�`
//--------------------------------------------------------------------------------------
class ShaderManager
{
public: 
	enum class TYPE 
	{
		SHADER_3D = 0 ,
		SHADER_3D_NO_TEXTURE ,
		SHADER_3D_NO_LIGHT ,
		SHADER_3D_RIM_LIGHT_TEXTURE ,
		SHADER_3D_DEPTH_SHADOW ,
		SHADER_3D_SHADOW_MAP ,
		SHADER_3D_THUN ,
		SHADER_3D_OUTLINE ,
		SHADER_3D_PLAYER ,
		SHADER_MAX ,
	};

	ShaderManager( );										//  �R���X�g���N�^
	~ShaderManager( );										//  �f�X�g���N�^

	static void		Init( void );							//  ������						
	static void		Uninit( void );							//  �I��

	static Shader*	GetShader( TYPE type );					//  �V�F�[�_�[���̎擾
	static void		DrawEnd( void );						//  �V�F�[�_�[�`��I��

private:
	static Shader*	m_shader[ ( int )TYPE::SHADER_MAX ];	//  �V�F�[�_�[�|�C���^
};

#endif
