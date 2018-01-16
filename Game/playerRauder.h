//--------------------------------------------------------------------------------------
//  �v���C���[  ( playerRauder.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_RAUDER_H_
#define _PLAYER_RAUDER_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModelAnim.h"
#include "effekseerManager.h"
#include "fbxLoader.h"

//--------------------------------------------------------------------------------------
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class Life;
class Magic;
class PlayerUI;
class PlayerRauderState;

//--------------------------------------------------------------------------------------
//  �v���C���[�N���X�̒�`
//--------------------------------------------------------------------------------------
class PlayerRauder : public Scene
{
public:
	enum class STATE														//  ���
	{
		IDLE = 0 ,															//  �ҋ@
		MOVE ,																//  �ړ�
		JUMP ,																//  �W�����v
		ATTACK ,															//  �U��
		MAX ,																//  �ő吔
	};

	enum class MAGIC														//  ���@
	{
		FIRE = 0 ,															//  ��
		LIGHTNING ,															//  ��
		TORNADE ,															//  ����
	};

	PlayerRauder( );														//  �R���X�g���N�^
	~PlayerRauder( );														//  �f�X�g���N�^

	HRESULT					Init( void );									//  ������
	void					Uninit( void );									//  �I��
	void					Update( void );									//  �X�V
	void					Draw( void );									//  �`��
	void					DrawDepth( void ) override;						//  �[�x�l�`��

	static PlayerRauder*		Create( D3DXVECTOR3 position ,				//  ����
										D3DXVECTOR3 rot ,
										D3DXVECTOR3 scale ,
										float fSpeed ,
										MAGIC magic , 
										int nPlayerRauderNo = 0 );

	/*-----------------------
		�ݒ�/�ύX�n�̊֐�
	-----------------------*/
	void				Damage( D3DXVECTOR3 blowVecDirect ,					//  �_���[�W����
								float fBlowPower = 0.0f ,					
								int nDamage = 250 ,
								bool bBlow = true );						
	int					BraveDamage( int nDamage = 100 );					//  �u���C�u�_���[�W����
	void				StopAllEffekseer( void );							//  �S�ẴG�t�F�N�V�A�̍Đ��X�g�b�v
	void				ChangeState( PlayerRauderState* playerState );		//  ��Ԃ̕ω�
	void				ChangeLockon( void );								//  ���b�N�I����Ԃ̕ύX
	void				SetCurrentJumpPower( float jumpPower );				//  ���݂̃W�����v�͂ɒl��ݒ�
	void				SetVecDirect( D3DXVECTOR3 vecDirect );				//  �����x�N�g���̐ݒ�
	void				SetPositionAt( D3DXVECTOR3 positionAt );			//  �����_�̐ݒ�
	void				SetSpeed( float speed );							//  ���x�̐ݒ�
	void				SetMovePower( float movePower );					//  �ړ��͂̐ݒ�
	void				SetBlowPower( float blowPower );					//  ������ї͂̐ݒ�
	void				SetInvisibleTime( int invisibleTime );				//  ���G���Ԃ̐ݒ�
	void				SetHomingDirect( bool homingDirect );				//  �ǔ������̐ݒ�
	void				SetEffekseerHandle( EffekseerManager::TYPE type ,	//  �w�肵����ނ̃G�t�F�N�V�A���ʎq�̐ݒ�
											::Effekseer::Handle handle );			
	void				SetJudgeHit( bool judgeHit );						//  �G�Ɠ����蔻�肷�邩�̃t���O�ݒ�
	void				SetGuardFlag( bool guardFlag );						//  �h��t���O�̐ݒ�
	void				SetMagic( Magic* magic );							//  ���@�N���X�̃|�C���^�̐ݒ�
	void				SetMotion( FBXLoader::MOTION motion );				//  ���[�V�����̐ݒ�
	void				AddLife( int addLife );								//  �̗͂̑���
	void				DeleteMagic( void );								//  ���@�̍폜

	/*-----------------------
		�擾�n�̊֐�
	-----------------------*/
	D3DXVECTOR3			GetVecDirect( void );								//  �����x�N�g���̎擾
	D3DXVECTOR3			GetPositionAt( void );								//  �����_�̎擾
	D3DXVECTOR3			GetBlowVecDirect( void );							//  ������ѕ����x�N�g���̎擾
	int					GetPlayerNo( void );								//  �v���C���[�ԍ��̎擾
	int					GetLife( void );									//  �̗͂̎擾
	int					GetInvisibleTime( void );							//  ���G���Ԃ̎擾
	float				GetSpeed( void );									//  ���x�̎擾
	float				GetBaseSpeed( void );								//  ��{���x�̎擾
	float				GetJumpPower( void );								//  �W�����v�͂̎擾
	float				GetCurrentJumpPower( void );						//  ���݂̃W�����v�͂̎擾
	float				GetTargetDistance( void );							//  �ڕW�܂ł̋����̎擾
	float				GetMovePower( void );								//  �ړ��͂̎擾
	float				GetBlowPower( void );								//  ������ї͂̎擾
	bool				GetJudgeHit( void );								//  �����蔻������邩�ǂ����̎擾
	bool				GetGuard( void );									//  �h�䂵�Ă��邩�ǂ����̎擾
	bool				GetLockon( void );									//  ���b�N�I�����̎擾
	bool				GetHomingDirect( void );							//  �ǔ���������̎擾( false : �E , true : �� )
	bool				GetMotionFinish( void );							//  ���[�V�������I���������̎擾
	Magic*				GetMagic( void );									//  ���@�N���X�̃|�C���^�̎擾
	MAGIC				GetMagicType( void );								//  ���@�̎�ގ擾
	PlayerRauderState*	GetPlayerState( STATE state );						//  ��Ԃ̎擾
	::Effekseer::Handle GetEffekseerHandle( EffekseerManager::TYPE type );	//  �G�t�F�N�V�A���ʎq�̎擾
	Utility::HIT_SPHERE GetHitSphere( void ){ return m_hitSphere; }			//  �����蔻��̎擾

	/*-----------------------
		�A�N�V�����n�̊֐�
	-----------------------*/
	void				MoveHeight( float addMove );						//  �W�����v�ł̈ړ�
	void				DownCurrentJumpPower( float downPower );			//  �W�����v�͂�������

	/*-----------------------
		�����蔻��n�̊֐�
	-----------------------*/
	bool				JudgeHitGround( void );								//  �n�ʂƂ̓����蔻��
	bool				JudgeHitEnemy( void );								//  �G�Ƃ̓����蔻��
	bool				JudgeHitWall( void );								//  �ǂƂ̓����蔻��

private:
	D3DXVECTOR3			m_firstPos;											//  �ŏ��̍��W
	D3DXVECTOR3			m_beforePos;										//  �O��̍��W
	D3DXVECTOR3			m_posAt;											//  �����_
	D3DXVECTOR3			m_rot;												//  ��]
	D3DXVECTOR3			m_scale;											//  �傫��
	float				m_fDistance;										//  ����
	D3DXVECTOR3			m_vecDirect;										//  �i�s����
	D3DXVECTOR3			m_blowVecDirect;									//  ������ѐi�s����
	float				m_fSpeed;											//  ���x
	float				m_fBaseSpeed;										//  ����x
	float				m_fCurrentJumpPower;								//  ���݂̃W�����v��
	float				m_fRotateY;											//  ��]Y��
	float				m_fMovePower;										//  �ړ���
	float				m_fHomingTime;										//  �ǔ�����
	float				m_fBlowPower;										//  ������ї�
	float				m_fMoveY;											//  ���Ɉړ������
	float				m_fJumpPower;										//  �W�����v��
	int					m_nWaitTime;										//  �҂�����
	int					m_nCntJump;											//  �W�����v�񐔃J�E���g�p
	int					m_nLife;											//  �̗�
	int					m_nBrave;											//  �u���C�u
	int					m_nInvisibleTime;									//  ���G����
	int					m_nPlayerNo;										//  �v���C���[�ԍ�
	int					m_nPressTime;										//  �v���X����
	bool				m_bLockOn;											//  ���b�N�I���t���O
	bool				m_bVoiceStart;										//  �J�n�{�C�X�𒝂������ǂ���
	bool				m_bJudgeHit;										//  �����蔻������邩�ǂ���
	bool				m_bHomingLeft;										//  �ǔ����������͂���Ă������ǂ���
	bool				m_bGuard;											//  �h�䒆���ǂ���
	bool				m_bSlashEffekseer;									//  �U���G�t�F�N�g
	MAGIC				m_magic;											//  ���@
	STATE				m_state;											//  ���
	Utility::HIT_SPHERE	m_hitSphere;										//  �����蔻��

	Life*				m_pLife;											//  �̗̓N���X�̃|�C���^
	PlayerUI*			m_pPlayerRauderUI;									//  �v���C���[UI�N���X�̃|�C���^
	Magic*				m_pMagic;											//  ���@�N���X�̃|�C���^
	FBXLoader*			m_fbx;												//  FBX�f�[�^�̃|�C���^

	PlayerRauderState*	m_playerState;										//  �v���C���[��ԃN���X�̃|�C���^

	PlayerRauderState*	m_allState[ ( int )STATE::MAX ];					//  ��ԑS�Ă̕ۑ��p

	::Effekseer::Handle m_handle[ EffekseerManager::TYPE_MAX ];				//  �G�t�F�N�V�A�n���h��

	const int			BASE_LIFE = 3000;									//  �̗͏����l
	const int			BASE_BRAVE = 1000;									//  �u���C�u�����l
};

#endif

