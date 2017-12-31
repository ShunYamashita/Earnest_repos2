//--------------------------------------------------------------------------------------
//  �O�Ճ��b�V�� ( meshTracing.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _MEESH_TRACING_H_
#define _MEESH_TRACING_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include <vector>

//--------------------------------------------------------------------------------------
//  ���b�V���t�B�[���h�N���X�̒�`
//--------------------------------------------------------------------------------------
class MeshTracing : public Scene
{
public:
	MeshTracing( );																	//  �R���X�g���N�^					
	~MeshTracing( );																//  �f�X�g���N�^

	HRESULT						Init( void );										//  ������
	void						Uninit( void );										//  �I��
	void						Update( void );										//  �X�V
	void						Draw( void );										//  �`��

	void						SetNewVertexPositionUp( D3DXVECTOR3 position );		//  �V�������_���W�̃g�b�v���W�̐ݒ�
	void						SetNewVertexPositionDown( D3DXVECTOR3 position );	//  �V�������_���W�̃g�b�v���W�̐ݒ�

	static MeshTracing*			Create( D3DXCOLOR color ,							//  ���� 	
										D3DXVECTOR3 positionUp ,
										D3DXVECTOR3 positionDown );								

private:
	void						MakeVertex( void );									//  ���_�̍쐬
	void						SetVertex( void );									//  ���_�̐ݒ�

	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;											//  ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIndexBuff;										//  �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

	D3DXVECTOR3					m_rot;

	std::vector< D3DXVECTOR3 >	m_vertex;											//  ���_���W

	D3DXVECTOR3					m_vertexPosition[ 10 ];								//  ���W
	D3DXVECTOR3					m_newVertexPositionUp;								//  �V�������_���W�̃g�b�v���W
	D3DXVECTOR3					m_newVertexPositionDown;							//  �V�������_���W�̃_�E�����W

	int							m_nDivideSide;										//  ��������
	int							m_nDivideVertical;									//  �c������

	int							m_countFrame;										//  �t���[���J�E���g
};

#endif
