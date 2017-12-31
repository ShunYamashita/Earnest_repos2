//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( Brave�U��000( �� ) )  ( playerBraveAttack000After.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_BRAVE_ATTACK000_AFTER_H_
#define _PLAYER_BRAVE_ATTACK000_AFTER_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( Brave�U��000( �� ) ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerBraveAttack000After : public PlayerState
{
public:
	PlayerBraveAttack000After( Player* player )
	{
		SetPlayer( player );
	};

	void								Init( void ) override;			//  ������
	void								Uninit( void ) override;		//  �I��
	void								Update( void ) override;		//  �X�V
	void								Draw( void ) override;			//  �`��

	static PlayerBraveAttack000After*	Create( void );					//  ����

private:
	void								Action( void );					//  �A�N�V����

	float								m_speed;						//  ���x
};

#endif
