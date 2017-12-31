//--------------------------------------------------------------------------------------
//  �I����ʏ���   ( readySelect.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _READY_SELECT_H_
#define _READY_SELECT_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene2D.h"

//--------------------------------------------------------------------------------------
//  �I����ʏ����N���X�̒�`
//--------------------------------------------------------------------------------------
class ReadySelect : public Scene2D
{
public:
	ReadySelect( );														//  �f�t�H���g�R���X�g���N�^
	~ReadySelect( );														//  �f�X�g���N�^

	HRESULT						Init( void );								//  ������
	void						Uninit( void );								//  �I��
	void						Update( void );								//  �X�V
	void						Draw( void );								//  �`��

	static ReadySelect*		Create( D3DXVECTOR3 position ,					//  ����			
										D3DXVECTOR3 size , 
										D3DXVECTOR2 posUV ,
										D3DXVECTOR2 divideUV );

private:

};

#endif
