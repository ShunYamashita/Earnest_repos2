//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���b�N�I���_�b�V�� )  ( playerWinksLockonDash.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_LOCKON_DASH_H_
#define _PLAYER_WINKS_LOCKON_DASH_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( ���b�N�I���_�b�V�� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerWinksLockonDash : public PlayerWinksState
{
public:
	enum class MOTION_STATE
	{
		START = 0 ,
		END ,
	};

	PlayerWinksLockonDash( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void						Init( void ) override;			//  ������
	void						Uninit( void ) override;		//  �I��
	void						Update( void ) override;		//  �X�V
	void						Draw( void ) override;			//  �`��

private:
	void						Action( void );					//  �A�N�V����

	float						m_speed;						//  ���x
	MOTION_STATE				m_motionState;					//  ���[�V�������
};

#endif
