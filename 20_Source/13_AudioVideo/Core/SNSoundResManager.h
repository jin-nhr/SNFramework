#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoundResource.h"
#include "SNSoundFile.h"
#include "SNPCM.h"
#include "SNList.h"



struct SNSoundResMngInfo
{
	SNResMngState State;
	SNResMngPhase Phase;
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
	static SNResMngState StateMachine(SNResMngEvent evt, SNSoundResID id);
	static Void StateMachineResUnload(SNResMngEvent evt, SNSoundResID id);
	static Void StateMachineResLoading(SNResMngEvent evt, SNSoundResID id);
	static Void StateMachineResLoaded(SNResMngEvent evt, SNSoundResID id);
	static Void StateMachineResUnloading(SNResMngEvent evt, SNSoundResID id);
	static Void StateMachinePhaseProc(SNSoundResID id);

	// リソース管理情報
	static SNSoundResMngInfo ManageInfo[SNSoundResNum];

	// 処理中ID
	static SNSoundResID ProcID;

	// ローダー
	static SNSoundFile Loader;

	// ロードが間に合っていない場合のダミー
	static SNPCM DummySound;
};
