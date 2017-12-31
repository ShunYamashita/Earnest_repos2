//--------------------------------------------------------------------------------------
//  �v���C���[  ( player.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RESULT_H_
#define _PLAYER_RESULT_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModelAnim.h"
#include "utility.h"
#include "shadow.h"
#include "life.h"
#include "brave.h"
#include "stateAnimator.h"
#include "effekseerManager.h"
#include "magic.h"
#include "playerUI.h"

//--------------------------------------------------------------------------------------
//  �v���C���[�N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerResult : public SceneModelAnim
{
public:
	PlayerResult( int nPriority = 3 );								//  �R���X�g���N�^
	~PlayerResult( );												//  �f�X�g���N�^

	HRESULT					Init( void );							//  ������
	void					Uninit( void );							//  �I��
	void					Update( void );							//  �X�V
	void					Draw( void );							//  �`��

	void					Win( void );							//  ����
	void					Lose( void );							//  �s�k

	static PlayerResult*	Create( D3DXVECTOR3 position ,				//  ����
									D3DXVECTOR3 posAt ,				
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 scale ,
									float fSpeed ,
									int nPriority = 3 );

private:
	D3DXVECTOR3				m_firstPos;								//  �ŏ��̍��W
	D3DXVECTOR3				m_beforePos;							//  �O��̍��W
	float					m_fDistance;							//  ����
	D3DXVECTOR3				m_vecDirect;							//  �i�s����
	float					m_fSpeed;								//  ���x

	StateAnimator*			m_pStateAnimator;						//  �X�e�[�g�}�V��( �A�j���[�^�[ )�N���X�̃|�C���^
};

#endif

