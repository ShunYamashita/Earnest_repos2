//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �h�� )  ( playerWinksGuard.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_GUARD_H_
#define _PLAYER_WINKS_GUARD_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �h�� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerWinksGuard : public PlayerWinksState
{
public:
	enum class MOTION_STATE
	{
		START = 0,
		GUARD ,
		END ,
	};

	PlayerWinksGuard( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;			//  ������
	void					Uninit( void ) override;		//  �I��
	void					Update( void ) override;		//  �X�V
	void					Draw( void ) override;			//  �`��

private:
	void					Action( void );					//  �A�N�V����

	::Effekseer::Handle		m_effekseerHandle;				//  �G�t�F�N�V�A���ʎq
	MOTION_STATE			m_motionState;					//  ���[�V�������
	bool					m_ground;						//  �n��ɂ��邩�ǂ���
	int						m_guardTime;					//  �K�[�h����
};

#endif
