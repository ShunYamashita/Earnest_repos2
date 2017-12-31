//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( Brave�U��000( �� ) )  ( playerBraveAttack000Before.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_BRAVE_ATTACK000_BEFORE_H_
#define _PLAYER_BRAVE_ATTACK000_BEFORE_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerState.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( Brave�U��000( �O ) ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerBraveAttack000Before : public PlayerState
{
public:
	PlayerBraveAttack000Before( Player* player )
	{
		SetPlayer( player );
	};

	void								Init( void ) override;			//  ������
	void								Uninit( void ) override;		//  �I��
	void								Update( void ) override;		//  �X�V
	void								Draw( void ) override;			//  �`��

	static PlayerBraveAttack000Before*	Create( void );					//  ����

private:
	void								Action( void );					//  �A�N�V����

	D3DXVECTOR3							m_beforePosition;				//  �O��̍��W
	float								m_homingTime;					//  �ǔ�����
	float								m_speed;						//  ���x
};

#endif
