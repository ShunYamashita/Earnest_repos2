//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �t���[�_�b�V�� )  ( playerWinksFreeDash.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_FREE_DASH_H_
#define _PLAYER_WINKS_FREE_DASH_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �t���[�_�b�V�� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerWinksFreeDash : public PlayerWinksState
{
public:
	enum class MOTION_STATE
	{
		START = 0 ,
		END ,
	};

	PlayerWinksFreeDash( PlayerWinks* player )
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
