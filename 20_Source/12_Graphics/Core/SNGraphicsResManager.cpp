#include "SNGraphicsResManager.h"
#include "SNGraphicsDevice.h"
#include "SNSystemConfig.h"
#include "SNMath.h"

// リソース管理情報
SNGraphicsResMngInfo SNGraphicsResManager::ManageInfo[SNGraphicsResNum];

// 処理中ID
SNGraphicsResID SNGraphicsResManager::ProcID;

// ローダー
SNImageFile SNGraphicsResManager::Loader;

// ロードが間に合っていない場合のダミー(全透過)
SNBitmap SNGraphicsResManager::DummyBitmap;


Void SNGraphicsResManager::Initialize()
{
	SNGraphicsContext* grc = &SNGraphicsDevice::D2DGraphicsContext;
	Int32 cnt = 0;
	SNSize size = { SNSystemConfig::ScreenWidth, SNSystemConfig::ScreenHeight };
	SNColor color = { 0, 0, 0, 0 };

	for (cnt = 0; cnt < SNGraphicsResNum; cnt++)
	{
		ManageInfo[cnt].State = ResUnload;
		ManageInfo[cnt].Phase = PhaseIdle;
		ManageInfo[cnt].RefCount = 0;
		ManageInfo[cnt].Res.DeleteBitmap();
	}

	ProcID = (SNGraphicsResID)0;

	// ダミービットマップ生成
	grc->CreateBitmap(&DummyBitmap, &size);
	grc->Begin(&DummyBitmap);
	grc->Clear(&color);
	grc->End();

    return;
}


Void SNGraphicsResManager::Terminate()
{
	Int32 cnt = 0;

	for (cnt = 0; cnt < SNGraphicsResNum; cnt++)
	{
		ManageInfo[cnt].State = ResUnload;
		ManageInfo[cnt].Phase = PhaseIdle;
		ManageInfo[cnt].RefCount = 0;
		ManageInfo[cnt].Res.DeleteBitmap();
	}

	ProcID = (SNGraphicsResID)0;

	DummyBitmap.DeleteBitmap();

	// 念のため待ってから破棄
	Loader.WaitForOperationComplete();
	Loader.DeleteDIB();	// ローダー破棄
	
    return;
}

Void SNGraphicsResManager::Update()
{
	StateMachine(EvtCycle, ProcID);
}

// リソース取得(参照カウントアップ)
Void SNGraphicsResManager::AccessGet(SNGraphicsResID id)
{
	StateMachine(EvtAccessGet, id);
	return ;
}

// リソース解放(参照カウントダウン)
Void SNGraphicsResManager::AccessRelease(SNGraphicsResID id)
{
	StateMachine(EvtAccessRelease, id);

	return;
}

SNBitmap* SNGraphicsResManager::GetResource(SNGraphicsResID id)
{
	SNBitmap* bmp = &DummyBitmap;

	if (IsLoaded(id))
	{
		bmp = &ManageInfo[id].Res;
	}

	return bmp;
}

// ロード完了判定
Boolean SNGraphicsResManager::IsLoaded(SNGraphicsResID id)
{
	return (ManageInfo[id].State == ResLoaded);
}

SNResMngState SNGraphicsResManager::StateMachine(SNResMngEvent evt, SNGraphicsResID id)
{
    SNGraphicsResMngInfo* info = &ManageInfo[id];

	switch (info->State)
	{
	case ResUnload:
		StateMachineResUnload(evt, id);
		break;
	case ResLoading:
		StateMachineResLoading(evt, id);
		break;
	case ResLoaded:
		StateMachineResLoaded(evt, id); 
		break;
	case ResUnloading:
		StateMachineResUnloading(evt, id);
		break;

		// フェール処理
	default:
		info->State = ResUnload;
		break;
	}

	return info->State;
}

// 未ロード状態
Void SNGraphicsResManager::StateMachineResUnload(SNResMngEvent evt, SNGraphicsResID id)
{
	SNGraphicsResMngInfo* info = &ManageInfo[id];

	switch (evt)
	{
	case EvtAccessGet:
		// 参照カウント
		info->RefCount++;
		break;

	case EvtAccessRelease:
		// 参照カウント
		info->RefCount--;
		break;

	case EvtCycle:
		// 参照カウントがあればロード中にいく
		if (info->RefCount > 0)
		{
			info->State = ResLoading;
			info->Phase = PhaseIdle;
		}
		else
		{
			// 処理対象を次のIDに進める

			// 処理対象をインクリメント
			ProcID = (SNGraphicsResID)SNMath::Increment(ProcID, 0, SNGraphicsResNum - 1);
		}
		break;
	}

	return;
}

// ロード中
Void SNGraphicsResManager::StateMachineResLoading(SNResMngEvent evt, SNGraphicsResID id)
{
	SNGraphicsResMngInfo* info = &ManageInfo[id];

	switch (evt)
	{
	case EvtAccessGet:
		info->RefCount++;
		break;

	case EvtAccessRelease:
		info->RefCount--;
		break;

	case EvtCycle:
		StateMachinePhaseProc(id);
		break;
	}

	return;
}

// ロード完了
Void SNGraphicsResManager::StateMachineResLoaded(SNResMngEvent evt, SNGraphicsResID id)
{
	SNGraphicsResMngInfo* info = &ManageInfo[id];

	switch (evt)
	{
	case EvtAccessGet:
		info->RefCount++;
		break;

	case EvtAccessRelease:
		info->RefCount--;
		break;

	case EvtCycle:
		// 参照カウントが0になったらアンロードへ
		if (info->RefCount <= 0)
		{
			info->State = ResUnloading;
		}
		else
		{
			ProcID = (SNGraphicsResID)SNMath::Increment(ProcID, 0, SNGraphicsResNum - 1);
		}
		break;
	}

	return;
}

// アンロード中
Void SNGraphicsResManager::StateMachineResUnloading(SNResMngEvent evt, SNGraphicsResID id)
{
	SNGraphicsResMngInfo* info = &ManageInfo[id];

	switch (evt)
	{
	case EvtAccessGet:
		info->RefCount++;
		break;

	case EvtAccessRelease:
		info->RefCount--;
		break;

	case EvtCycle:
		// 参照カウントがあればロード済みへ
		if (info->RefCount > 0)
		{
			info->State = ResLoaded;
		}
		else
		{
			// ビットマップ解放し未ロードへ
			info->Res.DeleteBitmap();
			info->State = ResUnload;
		}
		break;
	}

	return;
}

// フェーズ処理
Void SNGraphicsResManager::StateMachinePhaseProc(SNGraphicsResID id)
{
	SNGraphicsResMngInfo* info = &ManageInfo[id];

	switch (info->Phase)
	{
	case PhaseIdle:
		// ファイル名セット
		Loader.SetFolderPath(SNSystemConfig::GraphicPath);
		Loader.SetFolderFileName(SNGraphicsResource::ResourceInfoTable[id]);
		info->Phase = PhaseLoad;
		break;
	case PhaseLoad:
		// ファイル読み込み開始
		Loader.Read();
		info->Phase = PhaseWaitLoad;
		break;
	case PhaseWaitLoad:
		// ファイル読み込み完了待ち
		if (Loader.IsOperationComplete())
		{
			info->Phase = PhaseDecode;
		}
		break;
	case PhaseDecode:
		// デコード開始
		Loader.Decode();
		info->Phase = PhaseWaitDecode;
		break;
	case PhaseWaitDecode:
		// デコード完了待ち
		if (Loader.IsOperationComplete())
		{
			info->Phase = PhaseEnd;
		}
		break;
	case PhaseEnd:
		// 処理完了時点での参照カウントにより遷移先変更
		if (info->RefCount > 0)
		{
			// ビットマップへの展開しロード済みへ
			SNGraphicsDevice::D2DGraphicsContext.CreateBitmapFromDIB(&Loader, &info->Res);
			info->State = ResLoaded;
		}
		else
		{
			// 参照カウントが0ならビットマップに展開せずに未ロードへ
			info->State = ResUnload;
		}
		info->Phase = PhaseIdle;
		break;
	}

	return;
}

