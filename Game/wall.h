//--------------------------------------------------------------------------------------
//  ��  ( wall.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModel.h"
#include "utility.h"
#include "shadow.h"
#include "life.h"

//--------------------------------------------------------------------------------------
//  �ǃN���X�̒�`
//--------------------------------------------------------------------------------------
class Wall : public SceneModel
{
public:
	Wall( );											//  �R���X�g���N�^
	~Wall( );											//  �f�X�g���N�^

	HRESULT				Init( void );					//  ������
	void				Uninit( void );					//  �I��
	void				Update( void );					//  �X�V
	void				Draw( void );					//  �`��

	static Wall*		Create(	D3DXVECTOR3 position ,	//  ����	
								D3DXVECTOR3 rot ,
								D3DXVECTOR3 scale );

private:

};

#endif

#pragma once
