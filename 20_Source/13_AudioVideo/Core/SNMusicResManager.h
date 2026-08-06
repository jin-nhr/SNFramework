#pragma once
#include "SNFrameworkInternal.h"
#include "SNMusicResource.h"
#include "SNFile.h"
#include "SNPCMStream.h"
#include "SNList.h"


struct SNMusicResMngInfo
{
	SNResMngState State;
	SNResMngPhase Phase;
	Int32 RefCount;
	SNPCMStream Res;
};


class SNMusicResManager
{
public:
	static Void Initialize();

	static Void Terminate();

	static Void Update();

	// アクセス権取得
	static Void AccessGet(SNMusicResID id);

	// アクセス権解放
	static Void AccessRelease(SNMusicResID id);

	// リソース取得
	static SNPCMStream* GetResource(SNMusicResID id);

	// ロード完了判定
	static Boolean IsLoaded(SNMusicResID id);

private:
	// ステートマシン
	static SNResMngState StateMachine(SNResMngEvent evt, SNMusicResID id);
	static Void StateMachineResUnload(SNResMngEvent evt, SNMusicResID id);
	static Void StateMachineResLoading(SNResMngEvent evt, SNMusicResID id);
	static Void StateMachineResLoaded(SNResMngEvent evt, SNMusicResID id);
	static Void StateMachineResUnloading(SNResMngEvent evt, SNMusicResID id);
	static Void StateMachinePhaseProc(SNMusicResID id);

	// リソース管理情報
	static SNMusicResMngInfo ManageInfo[SNMusicResNum];

	// 処理中ID
	static SNMusicResID ProcID;

	// ローダー
	static SNFile Loader;

	// ロードが間に合っていない場合のダミー
	static SNPCMStream DummyMusic;
	static SNMemory DummyData;
};
