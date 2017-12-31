//--------------------------------------------------------------------------------------
//  �����_�����O����   ( scene2D.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _GOAL_SCENE_2D_H_
#define _GOAL_SCENE_2D_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------------
//  �����_���[�N���X�̒�`
//--------------------------------------------------------------------------------------
class CStageSelect1 : public CScene2D
{
public:
	CStageSelect1( );														//  �f�t�H���g�R���X�g���N�^
	CStageSelect1( D3DXVECTOR3 pos ,
				   D3DXVECTOR3 size ,
				   D3DXVECTOR2 divideUV ,
				   D3DXVECTOR2 posUV ,
				   D3DXCOLOR color );										//  �R���X�g���N�^
	~CStageSelect1( );														//  �f�X�g���N�^

	HRESULT						Init( void );								//  ������
	void						Uninit( void );								//  �I��
	void						Update( void );								//  �X�V
	void						Draw( void );								//  �`��

	static CStageSelect1*		Create( D3DXVECTOR3 pos,					//  ����
										D3DXVECTOR3 size , 
										D3DXVECTOR2 posUV ,
										D3DXVECTOR2 divideUV );
};

#endif
