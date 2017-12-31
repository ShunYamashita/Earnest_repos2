//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �t���[�_�b�V�� )  ( playerFreeDash.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_FREE_DASH_H_
#define _PLAYER_FREE_DASH_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �t���[�_�b�V�� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerFreeDash : public PlayerState
{
public:
	PlayerFreeDash( Player* player )
	{
		SetPlayer( player );
	};

	void						Init( void ) override;			//  ������
	void						Uninit( void ) override;		//  �I��
	void						Update( void ) override;		//  �X�V
	void						Draw( void ) override;			//  �`��

	static PlayerFreeDash*		Create( void );					//  ����

private:
	void						Action( void );					//  �A�N�V����

	float						m_speed;
};

#endif