//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �ːi�U�� )  ( playerRushAttack.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RUSH_ATTACK_H_
#define _PLAYER_RUSH_ATTACK_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Player;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �ːi�U�� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerRushAttack : public PlayerState
{
public:
	PlayerRushAttack( Player* player )
	{
		SetPlayer( player );
	};

	void						Init( void ) override;			//  ������
	void						Uninit( void ) override;		//  �I��
	void						Update( void ) override;		//  �X�V
	void						Draw( void ) override;			//  �`��

	static PlayerRushAttack*	Create( void );					//  ����

private:
	void						Action( void );					//  �A�N�V����
	void						ControlCameraExclusive( void );	//  ��p�̃J��������

	float						m_speed;
};

#endif
