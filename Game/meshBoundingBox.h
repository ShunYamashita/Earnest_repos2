////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�{�b�N�X  ( meshBoundingBox.h )
////
////  Author : SHUN YAMASHITA
////--------------------------------------------------------------------------------------
//#ifndef _MEESH_BOUNDING_BOX_H_
//#define _MEESH_BOUNDING_BOX_H_
//
////--------------------------------------------------------------------------------------
////  �w�b�_�[�t�@�C��
////--------------------------------------------------------------------------------------
//#include "main.h"
//#include "scene.h"
//
////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�{�b�N�X�N���X�̒�`
////--------------------------------------------------------------------------------------
//class MeshBoundingBox : public Scene
//{
//public:
//	MeshBoundingBox( );											//  �R���X�g���N�^					
//	~MeshBoundingBox( );										//  �f�X�g���N�^
//
//	HRESULT					Init( void );						//  ������
//	void					Uninit( void );						//  �I��
//	void					Update( void );						//  �X�V
//	void					Draw( void );						//  �`��
//	void					DrawDepth( void );					//  �f�v�X�l�̏�������
//
//	static MeshBoundingBox*	Create( const D3DXVECTOR3& min ,
//									const D3DXVECTOR3& max );		
//
//private:
//	void					MakeVertex( void );					//  ���_�̍쐬
//	void					SetVertex( void );					//  ���_�̐ݒ�
//
//	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							//  ���_�o�b�t�@�ւ̃|�C���^
//	LPDIRECT3DINDEXBUFFER9	m_pIndexBuff;						//  �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
//	D3DXVECTOR3				m_corners[ 8 ];						//  �p�̍��W
//};
//
//#endif
