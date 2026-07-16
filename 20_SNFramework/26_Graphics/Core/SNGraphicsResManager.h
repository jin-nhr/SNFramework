#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsResource.h"
#include "SNImageFile.h"
#include "SNBitmap.h"
#include "SNList.h"



enum SNGraphicsResMngState
{
	ResUnload,
	ResLoading,
	ResLoaded,
	ResUnloading,
};

enum SNGraphicsResMngEvent
{
	EvtAccessGet,
	EvtAccessRelease,
	EvtCycle
};

enum SNGraphicsResMngPhase
{
	PhaseIdle,
	PhaseLoad,
	PhaseWaitLoad,
	PhaseDecode,
	PhaseWaitDecode,
	PhaseCopyBitmap
};

struct SNGraphicsResMngInfo
{
	SNGraphicsResMngState State;
	SNGraphicsResMngPhase Phase;
	Int32 RefCount;
	SNBitmap Res;
};


class SNGraphicsResManager
{
public:
	static Void Initialize();

	static Void Terminate();

	static Void Update();

	// アクセス権取得
	static Void AccessGet(SNGraphicsResID id);

	// アクセス権解放
	static Void AccessRelease(SNGraphicsResID id);

	// リソース取得
	static SNBitmap* GetResource(SNGraphicsResID id);

	// ロード完了判定
	static Boolean IsLoaded(SNGraphicsResID id);

private:
	// ステートマシン
	static SNGraphicsResMngState StateMachine(SNGraphicsResMngEvent evt, SNGraphicsResID id);
	static Void StateMachineResUnload(SNGraphicsResMngEvent evt, SNGraphicsResID id);
	static Void StateMachineResLoading(SNGraphicsResMngEvent evt, SNGraphicsResID id);
	static Void StateMachineResLoaded(SNGraphicsResMngEvent evt, SNGraphicsResID id);
	static Void StateMachineResUnloading(SNGraphicsResMngEvent evt, SNGraphicsResID id);
	static Void StateMachinePhaseProc(SNGraphicsResID id);

	// リソース管理情報
	static SNGraphicsResMngInfo ManageInfo[SNGraphicsResNum];

	// 処理中ID
	static SNGraphicsResID ProcID;

	// ローダー
	static SNImageFile Loader;

	// ロードが間に合っていない場合のダミー(全透過)
	static SNBitmap DummyBitmap;
};
