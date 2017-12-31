//--------------------------------------------------------------------------------------
//  �u���b�N  ( block.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _BLOCK_H_
#define _BLOCK_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModel.h"
#include "utility.h"
#include "shadow.h"
#include "life.h"

//--------------------------------------------------------------------------------------
//  �u���b�N�N���X�̒�`
//--------------------------------------------------------------------------------------
class Block : public SceneModel
{
public:
	Block( );											//  �R���X�g���N�^
	~Block( );											//  �f�X�g���N�^

	HRESULT				Init( void );					//  ������
	void				Uninit( void );					//  �I��
	void				Update( void );					//  �X�V
	void				Draw( void );					//  �`��

	static Block*		Create(	D3DXVECTOR3 position ,	//  ����	
								D3DXVECTOR3 rot ,
								D3DXVECTOR3 scale );

private:
};

#endif

