#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoundResource.h"
#include "SNSoundFile.h"
#include "SNPCM.h"
#include "SNList.h"



enum SNSoundResMngState
{
	ResUnload,
	ResLoading,
	ResLoaded,
	ResUnloading,
};

enum SNSoundResMngEvent
{
	EvtAccessGet,
	EvtAccessRelease,
	EvtCycle
};

enum SNSoundResMngPhase
{
	PhaseIdle,
	PhaseLoad,
	PhaseWaitLoad,
	PhaseDecode,
	PhaseWaitDecode,
	PhaseCopyPCM
};

struct SNSoundResMngInfo
{
	SNSoundResMngState State;
	SNSoundResMngPhase Phase;
	Int32 RefCount;
	SNPCM Res;
};


class SNSoundResManager
{
public:
	static Void Initialize();

	static Void Terminate();

	static Void Update();

	// アクセス権取得
	static Void AccessGet(SNSoundResID id);

	// アクセス権解放
	static Void AccessRelease(SNSoundResID id);

	// リソース取得
	static SNPCM* GetResource(SNSoundResID id);

	// ロード完了判定
	static Boolean IsLoaded(SNSoundResID id);

private:
	// ステートマシン
	static SNSoundResMngState StateMachine(SNSoundResMngEvent evt, SNSoundResID id);
	static Void StateMachineResUnload(SNSoundResMngEvent evt, SNSoundResID id);
	static Void StateMachineResLoading(SNSoundResMngEvent evt, SNSoundResID id);
	static Void StateMachineResLoaded(SNSoundResMngEvent evt, SNSoundResID id);
	static Void StateMachineResUnloading(SNSoundResMngEvent evt, SNSoundResID id);
	static Void StateMachinePhaseProc(SNSoundResID id);

	// リソース管理情報
	static SNSoundResMngInfo ManageInfo[SNSoundResNum];

	// 処理中ID
	static SNSoundResID ProcID;

	// ローダー
	static SNSoundFile Loader;

	// ロードが間に合っていない場合のダミー(全透過)
	static SNPCM DummySound;
};
