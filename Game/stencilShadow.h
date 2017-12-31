//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E  ( sceneModel.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _STENCIL_SHADOW_H_
#define _STENCIL_SHADOW_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�N���X�̒�`
//--------------------------------------------------------------------------------------
class StencilShadow : public Scene
{
public:
	enum class TYPE
	{
		SPHERE = 0,
		PILLAR ,
		THIEF ,
	};

	StencilShadow(  );													//  �f�t�H���g�R���X�g���N�^
	StencilShadow( D3DXVECTOR3 position ,								//  �R���X�g���N�^
				   D3DXVECTOR3 rot ,
				   D3DXVECTOR3 scale );				
	~StencilShadow( );													//  �f�X�g���N�^

	HRESULT					Init( void );								//  ������
	void					Uninit( void );								//  �I��
	void					Update( void );								//  �X�V
	void					Draw( void );								//  �`��
	void					DrawShadow( void );							//  �e�̕`��
	void					DrawDepth( void ){ }						//  �f�v�X�l�̏�������

	static StencilShadow*	Create( TYPE type ,							//  ����
									D3DXVECTOR3 position ,					
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 scale );

	void					SetScale( D3DXVECTOR3 scale );				//  �傫���̐ݒ�

protected:
	D3DXVECTOR3				GetScale( void);							//  �傫���̎擾

	D3DXVECTOR3				m_scale;									//  �傫���̔{��
	D3DXVECTOR3				m_rot;										//  ��]

	TYPE					m_type;										//  ���

	LPD3DXMESH				m_pMeshModel;								//  ���b�V�����

private:
	LPD3DXBUFFER			m_pBuffMatModel;							//  �}�e���A�����
	DWORD					m_nNumMatModel;								//  �}�e���A����

	char					m_aTextureName[ MAX_MATERIAL ][ 256 ];		//  �e�N�X�`�����ۑ��p

	LPDIRECT3DVERTEXBUFFER9	m_shadowVtxBuff;							//  ���_�o�b�t�@�ւ̃|�C���^
};

#endif