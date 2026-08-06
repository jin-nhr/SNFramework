#pragma once
#include "SNFrameworkInternal.h"
#include "SNGraphicsResource.h"
#include "SNImageFile.h"
#include "SNBitmap.h"
#include "SNList.h"



struct SNGraphicsResMngInfo
{
	SNResMngState State;
	SNResMngPhase Phase;
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
	static SNResMngState StateMachine(SNResMngEvent evt, SNGraphicsResID id);
	static Void StateMachineResUnload(SNResMngEvent evt, SNGraphicsResID id);
	static Void StateMachineResLoading(SNResMngEvent evt, SNGraphicsResID id);
	static Void StateMachineResLoaded(SNResMngEvent evt, SNGraphicsResID id);
	static Void StateMachineResUnloading(SNResMngEvent evt, SNGraphicsResID id);
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
