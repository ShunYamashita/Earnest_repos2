// ======== ======== ======== ======== ======== ======== ======== ========
//
//			[Wwise.h] 							Auter : KENSUKE WATANABE
//												Data  : 2017/09/06
//
// -------- -------- -------- -------- -------- -------- -------- --------
//��Update : 2017/09/06
//
// ======== ======== ======== ======== ======== ======== ======== ========
//
// �g�����I
// �@ CreateNoListenerGameObj()�ŃQ�[���I�u�W�F�N�g���쐬�I
// �A SetEvent()�ŃC�x���g���Đ��I
// �B Stop()�ŃC�x���g�̒�~�I
//
// �E Init(), Uninit(), Update() ��Manager.cpp�ɏ����ĉ�����
// �E ���X�i�[(m_pMainListener)�́A�v���C���[���̈ʒu������ɍ��킹�Ė��t���[���ݒ肵�����ĉ�����
// 
// -------- -------- -------- -------- -------- -------- -------- --------
#ifndef _WWISE_H_
#define _WWISE_H_

#include <windows.h>
#include <unordered_map>
#include <assert.h>

// Release���ɒ�`������
#ifndef _DEBUG
#define AK_OPTIMIZED
#endif

// Wwise(�T�E���h�G���W��)���g�p���邽�߂ɕK�v
#include <AK/SoundEngine/Common/AkSoundEngine.h>		// �T�E���h�G���W��
#include <AK/IBytes.h>

#include <AK/SoundEngine/Platforms/Windows/AkTypes.h>	// �^�C�v

#include "Wwise_IDs.h"									// �C�x���g��ID

using namespace AK;
using namespace AK::SoundEngine;

// ======== ======== ======== ======== ======== ======== ======== ========
// �\����
// -------- -------- -------- -------- -------- -------- -------- --------
// �Q�[���I�u�W�F�N�g
struct WWISE_GAMEOBJ
{
	D3DXVECTOR3 pos, rot;		// ���W, ��]�p
	D3DXVECTOR3 vel;			// ���x(�h�b�v���[���ʂŕK�v�H)
};

// �O���錾
class CAkFilePackageLowLevelIOBlocking;

// ======== ======== ======== ======== ======== ======== ======== ========
// �N���X
// -------- -------- -------- -------- -------- -------- -------- --------
class Wwise
{
public:
	Wwise(){}
	~Wwise(){}

	void Init();	// ������
	void Uninit();	// �I��
	void Update();	// �X�V

	// �Q�[���I�u�W�F�N�g�̐���
	WWISE_GAMEOBJ *CreateGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, void *thisPtr);				// Listener�t��
	WWISE_GAMEOBJ *CreateNoListenerGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, void *thisPtr);	// Listener����
	void DeleteGameObj(void *thisPtr);

	// �C�x���g���M
	void SetEvent(int eventID, void *thisPtr);		// �w��Q�[���I�u�W�F�N�g��ID�ŃC�x���g�𑗐M
	void SetEvent(char *eventName, void *thisPtr);	// �w��Q�[���I�u�W�F�N�g�ɖ��O�ŃC�x���g�𑗐M

	// �Q�[���I�u�W�F�N�g�̍Đ����~
	void Stop(void *thisPtr);

	// �C�x���g�̏�Ԃ�ύX
	void SetState(int stateGroup, int stateID);			// �X�e�[�g�̐ݒ�
	void SetGameParam(int rtpcID, float value);			// �Q�[���p�����[�^�[(RTPC)�̐ݒ�

	// �}�e���A��
	int GetMaterialId(char *name);						// ���O����}�e���A��ID�̎擾
//	void SetMaterial(void *gameObject, int materialid);	// �Q�[���I�u�W�F�N�g�Ƀ}�e���A��ID��ݒ�

	// ���C�����X�i�[�n
	void SetMainListenerEvent(int eventID);		// ���C�����X�i�[(�v���C���[)�ɃC�x���g�𑗐M
	void StopMainListener();
	void SetPos(const D3DXVECTOR3 &pos) { m_pMainListener->pos = pos; }		// ���W
	void SetRot(const D3DXVECTOR3 &rot) { m_pMainListener->rot = rot; }		// ��]
	void SetVel(const D3DXVECTOR3 &vel) { m_pMainListener->vel = vel; }		// ���x

	// �e�Q�[���I�u�W�F�N�g
	void SetPos(void *thisPtr, const D3DXVECTOR3 &pos) {			// ���W
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[thisPtr]->pos = pos;
	}
	void SetRot(void *thisPtr, const D3DXVECTOR3 &rot) {			// ��]
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[thisPtr]->rot = rot;
	}
	void SetVel(void *thisPtr, const D3DXVECTOR3 &vel) {			// ���x
		if (m_umapGameObj.count(thisPtr) < 1) { assert(!"�����̖��O���s��"); }
		m_umapGameObj[thisPtr]->vel = vel;
	}

	// �L���v�`�����X�g�փG���[���b�Z�[�W�𑗂�
	void PostMessage(char *szMessage);

	// D3DXVECTOR3��AkVector�̕ϊ�
	static void VecToAkVector(const D3DXVECTOR3 &vec, AkVector &vector);
	static AkVector Wwise::VecToAkVector(const D3DXVECTOR3 &vec);

private:
	// �Q�[���I�u�W�F�N�g�̓o�^(���ڂ͌Ăяo���Ȃ�)
	void registPtr(void *thisPtr, char *name = NULL);
	void registPtr(const AkGameObjectID &ID, char *pName);
	void unregistPtr(void *thisPtr);

	// �჌�x��I/O
	static CAkFilePackageLowLevelIOBlocking m_lowLevelIO;			

	// �Q�[���I�u�W�F�N�g
	std::unordered_map<void*, WWISE_GAMEOBJ*> m_umapGameObj;		// �Q�[���I�u�W�F�N�g
	WWISE_GAMEOBJ *m_pMainListener;									// ���C�����X�i�[�ւ̃|�C���^

	// �_�~�[�R�[���o�b�N
	static void dummyCallback(AkCallbackType type, AkCallbackInfo* pCallbackInfo);
	static std::list<int> dummies;
	static CAkLock dummiesLock;

	/*	void soundvol(int vol);
	void musicvol(int vol);
	void voicevol(int vol);*/				
};

#endif
