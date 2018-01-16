//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �U�� )  ( playerWinksAttack.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_WINKS_ATTACK_H_
#define _PLAYER_WINKS_ATTACK_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "playerWinksState.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class PlayerWinks;

//--------------------------------------------------------------------------------------
//  �v���C���[�X�e�[�g( �U�� ) �N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerWinksAttack : public PlayerWinksState
{
public:
	enum class MOTION_STATE
	{
		ATTACK01 = 0 ,
		ATTACK02 ,
		ATTACK03 ,
		MAX ,
	};

	PlayerWinksAttack( PlayerWinks* player )
	{
		SetPlayer( player );
	};

	void					Init( void ) override;			//  ������
	void					Uninit( void ) override;		//  �I��
	void					Update( void ) override;		//  �X�V
	void					Draw( void ) override;			//  �`��

private:
	void					Action( void );					//  �A�N�V����
	void					MoveTarget( void );				//  �^�[�Q�b�g�ɏ��X�ɋ߂Â�
	void					JudgeNextMotion( void );		//  ���̃��[�V�����Ɉڍs���邩�̔��f

	MOTION_STATE			m_motionState;					//  ���[�V�������
	int						m_rigidTime;					//  �d������
	bool					m_effekseerFrame;				//  �t���[���𔭐��������ǂ����̃t���O
	Effekseer::Handle		m_effekseerHandle[ 2 ];			//  �G�t�F�N�V�A���ʎq
};

#endif
