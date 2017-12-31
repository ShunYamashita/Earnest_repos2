// ======== ======== ======== ======== ======== ======== ======== ========
//
//			[Wwise.cpp] 						Auter : KENSUKE WATANABE
//												Data  : 2017/09/06
//
// -------- -------- -------- -------- -------- -------- -------- --------
//■Update : 2017/09/06
//
// ======== ======== ======== ======== ======== ======== ======== ========
#include "main.h"
#include "Wwise.h"

// メモリマネージャに必要
#include <AK/SoundEngine/Common/AkMemoryMgr.h>				// メモリマネージャ
#include <AK/SoundEngine/Common/AkModule.h>					// デフォルトメモリ, ストリームマネージャ

// ストリーミング再生に必要
#include <AK/SoundEngine/Common/IAkStreamMgr.h>				// ストリームマネージャ
#include <AK/Tools/Common/AkPlatformFuncs.h>				// スレッド
#include "Wwise_Win32/AkFilePackageLowLevelIOBlocking.h"	// 低レベルI/O

// ミュージックエンジンに必要
#include <AK/MusicEngine/Common/AkMusicEngine.h>			// ミュージックエンジン

// #include <AK/Plugin/AkCompressorFXFactory.h>				// プラグイン
// #include <AK/Plugin/AllPluginsRegistrationHelpers.h>		// 

// Wwiseとゲームの間の通信に必要 - リリースバージョンでは不要
#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

#ifdef AK_MOTION
#include <AK/MotionEngine/Common/AkMotionEngine.h>	// モーションを使用するときに宣言
#endif

// ======== ======== ======== ======== ======== ======== ======== ========
//	メモリフック
// -------- -------- -------- -------- -------- -------- -------- --------
// カスタムalloc/free関数。
// これらはAkMemoryMgr.hの "extern"として宣言されている
// -------- -------- -------- -------- -------- -------- -------- --------
namespace AK
{
#ifdef WIN32
	void *AllocHook(size_t in_size) { return malloc(in_size); }

	void FreeHook(void *in_ptr) { free(in_ptr); }

	void *VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}

	void VirtualFreeHook(void *in_pMemAddress, size_t in_size, DWORD in_dwFreeType)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 名前省略
using namespace std;

// ======== ======== ======== ======== ======== ======== ======== ========
// マクロ
#define CHECK_SOUND_ENGINE if( !SoundEngine::IsInitialized() ) return;
#define CHECK_SOUND_ENGINE0 if( !SoundEngine::IsInitialized() ) return 0;
#define CHECK_NULLPTR(p) { if(p == nullptr) return; }

static const AkGameObjectID MainListenerID = 10001;

// ======== ======== ======== ======== ======== ======== ======== ========
// 静的メンバ
CAkFilePackageLowLevelIOBlocking Wwise::m_lowLevelIO;
std::list<int> Wwise::dummies;
CAkLock Wwise::dummiesLock;

// ======== ======== ======== ======== ======== ======== ======== ========
// 初期化
void Wwise::Init()
{
	// -------- -------- -------- --------
	// メモリマネージャ
	//
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (MemoryMgr::Init(&memSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the memory manager.");
		return;
	}

	// -------- -------- -------- --------
	// ストリーミングマネージャ
	//
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	if (!StreamMgr::Create(stmSettings))
	{
		AKASSERT(!"Could not create the Stream Manager");
		return;
	}

	// -------- -------- -------- --------
	// ストリーミングデバイス
	//
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (m_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		AKASSERT(!"Could not create the streaming device and Low-Level I/O system");
		return;
	}

	// -------- -------- -------- --------
	// サウンドエンジン
	//
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	// Setting pool sizes for this game. Here, allow for user content; every game should determine its own optimal values.
	initSettings.uDefaultPoolSize = 2 * 1024 * 1024;
	platformInitSettings.uLEngineDefaultPoolSize = 4 * 1024 * 1024;

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return;
	}

	// -------- -------- -------- --------
	// ミュージックエンジン
	//
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return;
	}

	/*
	// -------- -------- -------- --------
	// プラグイン
	//
	/// Note: This a convenience method for rapid prototyping. 
	/// To reduce executable code size register/link only the plug-ins required by your game 
	if (AK::SoundEngine::RegisterAllPlugins() != AK_Success)
	{
		AKASSERT(!"Error while registering plug-ins");
		return;
	}
	*/

	// -------- -------- -------- --------
	// 通信の初期化
	//
#ifndef AK_OPTIMIZED
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return;
	}
#endif // AK_OPTIMIZED

	// -------- -------- -------- --------
	// 低レベルI/O
	//
	// バンクスクリプトのパス
	m_lowLevelIO.SetBasePath(AKTEXT("data/wwiseProject/GeneratedSoundBanks/Windows/"));

	// ゲームの現在の言語の指定
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	// -------- -------- -------- --------
	// バンクの読み込み
	//
	AkBankID bankID;
	AKRESULT retValue;

	// 初期化バンク
	retValue = SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	// メインバンク
	retValue = SoundEngine::LoadBank("SoundBank.bnk", AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);

	/*
	// ボリュームの初期化
	musicvol(255);
	soundvol(255);
	voicevol(255);
	*/

	/*
	// Wwiseモーション
#ifdef AK_MOTION
	MotionEngine::AddPlayerMotionDevice(0, AKCOMPANYID_AUDIOKINETIC, AKMOTIONDEVICEID_RUMBLE, nullptr);
	MotionEngine::SetPlayerListener(0, 0);
	SoundEngine::SetListenerPipeline(0, true, true);
#endif
	*/

	// メインリスナーの設定
	m_pMainListener = Wwise::CreateGameObj(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), this);	// ゲームオブジェクトとして登録
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 終了
void Wwise::Uninit()
{
	// ゲームオブジェクトの消去
	for (auto it : m_umapGameObj)
	{
		if (it.first != nullptr)
		{
			Stop(it.first);
			unregistPtr(it.first);	// ゲームオブジェクトの登録取り消し
			delete it.second;
			it.second = nullptr;
		}
	}
	m_umapGameObj.clear();

	// 通信
#ifndef AK_OPTIMIZED
	Comm::Term();
#endif // AK_OPTIMIZED

	// ミュージックエンジン
	MusicEngine::Term();

	// サウンドエンジン
	SoundEngine::Term();

	// 低レベル入出力
	m_lowLevelIO.Term();
	if (IAkStreamMgr::Get())
		IAkStreamMgr::Get()->Destroy();

	// メモリマネージャ
	MemoryMgr::Term();
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 更新
void Wwise::Update()
{
	CHECK_SOUND_ENGINE;

	AkSoundPosition MainListener;
	AkSoundPosition soundPos;
	float fRadPitch, fCosPitch, fSinPitch;
	float fRadYaw, fCosYaw, fSinYaw;
	D3DXVECTOR3 front, top;
	AkVector position;
	// ======== ======== ======== ========
	// 各ゲームオブジェクトの設定
	for (auto gameObject : m_umapGameObj)
	{
		if (gameObject.second == m_pMainListener)
		{
			// ======== ======== ======== ========
			// リスナーの設定
			fRadPitch = m_pMainListener->rot.z;
			fCosPitch = cos(fRadPitch);
			fSinPitch = sin(fRadPitch);

			fRadYaw = m_pMainListener->rot.y - D3DX_PI * 0.5f;
			fCosYaw = cos(fRadYaw);
			fSinYaw = sin(fRadYaw);

			// 前方向ベクトル
			front.x = fSinYaw * fCosPitch;
			front.y = fSinPitch;
			front.z = fCosYaw * fCosPitch;
			D3DXVec3Normalize(&front, &front);

			// 上方向ベクトル
			top.x = -fCosYaw * fSinPitch;
			top.y = fCosPitch;
			top.z = -fSinYaw * fSinPitch;
			D3DXVec3Normalize(&top, &top);

			// 座標設定
			VecToAkVector(m_pMainListener->pos, position);

			// リスナーの設定
			MainListener.Set(position, VecToAkVector(front), VecToAkVector(top));
			SoundEngine::SetPosition((AkGameObjectID)gameObject.first, MainListener);

			continue;
		}
		fRadPitch = gameObject.second->rot.z;
		fCosPitch = cos(fRadPitch);
		fRadYaw = gameObject.second->rot.y - D3DX_PI * 0.5f;

		// 前方向ベクトル
		front.x = (float)(sin(fRadYaw) * fCosPitch);
		front.y = (float)(sin(fRadPitch));
		front.z = (float)(cos(fRadYaw) * fCosPitch);
		D3DXVec3Normalize(&front, &front);

		// 上方向ベクトル
		top = { 0.f, 1.f, 0.f };

		// 座標設定
		VecToAkVector(gameObject.second->pos, position);

		// ゲームオブジェクトの設定
		soundPos.Set(position, VecToAkVector(front), VecToAkVector(top));
		SoundEngine::SetPosition((AkGameObjectID)gameObject.first, soundPos);
	}

	// 設定されたイベントの実行
	SoundEngine::RenderAudio();
};

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトを登録
WWISE_GAMEOBJ *Wwise::CreateGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, void *thisPtr)
{
	WWISE_GAMEOBJ *pGameObject = new WWISE_GAMEOBJ;
	pGameObject->pos = pos;							// 座標
	pGameObject->rot = rot;							// 角度
	pGameObject->vel = D3DXVECTOR3(0.f, 0.f, 0.f);	// 速度(ドップラー用)
	
	// エンジンへ登録
	Wwise::registPtr(thisPtr);

	// リストへ追加
	if (m_umapGameObj.count(thisPtr) >= 1)
	{
		Stop(thisPtr);
		Wwise::unregistPtr(thisPtr);
		delete m_umapGameObj[thisPtr];
		m_umapGameObj[thisPtr] = nullptr;
	}
	m_umapGameObj[thisPtr] = pGameObject;
	AK::SoundEngine::RegisterGameObj(MainListenerID, "Listener (Default)");
	AK::SoundEngine::SetDefaultListeners(&MainListenerID, 1);

	return pGameObject;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// リスナー無しゲームオブジェクトを登録
WWISE_GAMEOBJ *Wwise::CreateNoListenerGameObj(D3DXVECTOR3 pos, D3DXVECTOR3 rot, void *thisPtr)
{
	WWISE_GAMEOBJ *pGameObject = new WWISE_GAMEOBJ;
	pGameObject->pos = pos;							// 座標
	pGameObject->rot = rot;							// 角度
	pGameObject->vel = D3DXVECTOR3(0.f, 0.f, 0.f);	// 速度(ドップラー用)

	// エンジンへ登録
	Wwise::registPtr(thisPtr);

	// リストへ追加
	if (m_umapGameObj.count(thisPtr) >= 1)
	{
		Stop(thisPtr);
		Wwise::unregistPtr(thisPtr);
		delete m_umapGameObj[thisPtr];
		m_umapGameObj[thisPtr] = nullptr;
	}
	m_umapGameObj[thisPtr] = pGameObject;

	return pGameObject;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトの登録(エンジンに登録)
void Wwise::registPtr(void *thisPtr, char *pName)
{
	CHECK_SOUND_ENGINE;

	if (pName == nullptr)
	{
		static int cEntity = 0;
		char name[256];
		sprintf_s(name, "Entity #%i", (int) ++cEntity);

		SoundEngine::RegisterGameObj((AkGameObjectID)thisPtr, name);
	}
	else
	{
		SoundEngine::RegisterGameObj((AkGameObjectID)thisPtr, pName);
	}
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトの登録(エンジンに登録)
void Wwise::registPtr(const AkGameObjectID &ID, char *pName)
{
	CHECK_SOUND_ENGINE;
	SoundEngine::RegisterGameObj(ID, pName);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトの消去
void Wwise::DeleteGameObj(void *thisPtr)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(thisPtr) == 0) { return; }

	// 指定ゲームオブジェクトの消去
	Wwise::unregistPtr(thisPtr);
	delete m_umapGameObj[thisPtr];
	m_umapGameObj[thisPtr] = nullptr;
	m_umapGameObj.erase(thisPtr);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトの消去(エンジンから削除)
void Wwise::unregistPtr(void *thisPtr)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::UnregisterGameObj((AkGameObjectID)thisPtr);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 
// イベントの設定
// 
// -------- -------- -------- -------- -------- -------- -------- --------
// メインリスナーにイベントを送信
void Wwise::SetMainListenerEvent(int eventID)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::PostEvent(eventID, MainListenerID);
}


// ======== ======== ======== ======== ======== ======== ======== ========
// ID, ゲームオブジェクト名
void Wwise::SetEvent(int eventID, void *thisPtr)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(thisPtr) == 0) { return; }

	SoundEngine::PostEvent(eventID, (AkGameObjectID)thisPtr);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// イベント名, ゲームオブジェクト名
void Wwise::SetEvent(char *eventName, void *thisPtr)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(thisPtr) == 0) { return; }

	SoundEngine::PostEvent(eventName, (AkGameObjectID)thisPtr);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 再生中イベントの停止
void Wwise::Stop(void *thisPtr)
{
	CHECK_SOUND_ENGINE;
	if (m_umapGameObj.count(thisPtr) == 0) { assert(!"引数の名前が不正"); }

	SoundEngine::StopAll((AkGameObjectID)thisPtr);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// メインリスナーの再生中イベントの停止
void Wwise::StopMainListener()
{
	CHECK_SOUND_ENGINE;
	if (m_pMainListener == nullptr) { return; }

	SoundEngine::StopAll(MainListenerID);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ステートの設定
void Wwise::SetState(int stateGroup, int stateID)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetState(stateGroup, stateID);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// RTPCの設定
void Wwise::SetGameParam(int rtpcID, float value)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(rtpcID, value);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// wwiseキャプチャーリストへエラーメッセージを送る
void Wwise::PostMessage(char *szMessage)
{
	Monitor::PostString(szMessage, Monitor::ErrorLevel_Message);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ダミーコールバック
void Wwise::dummyCallback(AkCallbackType type, AkCallbackInfo *pCallbackInfo)
{
	UNREFERENCED_PARAMETER(type);
	AkEventCallbackInfo *pInfo = (AkEventCallbackInfo *)pCallbackInfo;
	AkGameObjectID id = pInfo->gameObjID;

	dummiesLock.Lock();		// ロック

	// IDの有効性を確かめる
	auto it = std::find(dummies.begin(), dummies.end(), static_cast<int>(pInfo->playingID));
	if (it != dummies.end())
	{
		dummies.erase(it);
		AK::SoundEngine::UnregisterGameObj(id);
	}
	dummiesLock.Unlock();	// アンロック
}

// ======== ======== ======== ======== ======== ======== ======== ========
// 名前からマテリアルIDの取得
int Wwise::GetMaterialId(char *pName)
{
	CHECK_SOUND_ENGINE0;

	return SoundEngine::GetIDFromString(pName);
}

/*
// ======== ======== ======== ======== ======== ======== ======== ========
// ゲームオブジェクトにマテリアルIDを設定
void Wwise::SetMaterial(void *gameObject, int materialid)
{
CHECK_SOUND_ENGINE;

SoundEngine::SetSwitch(SWITCHES::MATERIAL::GROUP, materialid, (AkGameObjectID)gameObject);
}
*/

// ======== ======== ======== ======== ======== ======== ======== ========
// DirectXのVec3をWwiseのVec3に変換
void Wwise::VecToAkVector(const D3DXVECTOR3 &vec, AkVector &vector)
{
	vector.X = vec.x;
	vector.Y = vec.y;
	vector.Z = vec.z;
}

// ======== ======== ======== ======== ======== ======== ======== ========
// DirectXのVec3をWwiseのVec3に変換
AkVector Wwise::VecToAkVector(const D3DXVECTOR3 &vec)
{
	AkVector vector;
	vector.X = vec.x;
	vector.Y = vec.y;
	vector.Z = vec.z;
	return vector;
}

/*
// ======== ======== ======== ======== ======== ======== ======== ========
// 
// スクリプトコマンド
// 
// -------- -------- -------- -------- -------- -------- -------- --------
// ======== ======== ======== ======== ======== ======== ======== ========
// ボリューム設定 (0-255)
void Wwise::musicvol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::MUSICVOLUME, (AkRtpcValue)vol);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// SFX(エフェクト)ボリューム設定 (0-255)
void Wwise::soundvol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::SFXVOLUME, (AkRtpcValue)vol);
}

// ======== ======== ======== ======== ======== ======== ======== ========
// ボイスボリューム設定 (0-255)
void Wwise::voicevol(int vol)
{
	CHECK_SOUND_ENGINE;

	SoundEngine::SetRTPCValue(GAME_PARAMETERS::VOICEVOLUME, (AkRtpcValue)vol);
}
*/
