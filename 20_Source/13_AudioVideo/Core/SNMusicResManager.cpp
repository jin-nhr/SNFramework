#include "SNMusicResManager.h"
#include "SNSoundDevice.h"
#include "SNSystemConfig.h"
#include "SNMath.h"

// リソース管理情報
SNMusicResMngInfo SNMusicResManager::ManageInfo[SNMusicResNum];

// 処理中ID
SNMusicResID SNMusicResManager::ProcID;

// ローダー
SNFile SNMusicResManager::Loader;

// ロードが間に合っていない場合のダミー
SNPCMStream SNMusicResManager::DummyMusic;

SNMemory SNMusicResManager::DummyData;


Void SNMusicResManager::Initialize()
{
	Int32 cnt = 0;

	for (cnt = 0; cnt < SNMusicResNum; cnt++)
	{
		ManageInfo[cnt].State = ResUnload;
		ManageInfo[cnt].Phase = PhaseIdle;
		ManageInfo[cnt].RefCount = 0;
		ManageInfo[cnt].Res.ClosePCMStream();
	}

	ProcID = (SNMusicResID)0;

	// ダミーPCM生成
	DummyData.Allocate(32);
	DummyMusic.OpenPCMStream(&DummyData);
	DummyMusic.Channels = 2;
	DummyMusic.SampleRate = 44100;
	DummyMusic.BitPerSample = 16;

    return;
}


Void SNMusicResManager::Terminate()
{
	Int32 cnt = 0;

	for (cnt = 0; cnt < SNMusicResNum; cnt++)
	{
		// 念のため終了を待ってから解放する
		ManageInfo[cnt].Res.WaitForOperationComplete();

		ManageInfo[cnt].State = ResUnload;
		ManageInfo[cnt].Phase = PhaseIdle;
		ManageInfo[cnt].RefCount = 0;
		ManageInfo[cnt].Res.ClosePCMStream();
	}

	ProcID = (SNMusicResID)0;

	DummyMusic.ClosePCMStream();
	DummyData.Free();

	// 念のため待ってから破棄
	Loader.WaitForOperationComplete();
	Loader.ReleaseMemory();
	
    return;
}

Void SNMusicResManager::Update()
{
	StateMachine(EvtCycle, ProcID);
}

// リソース取得(参照カウントアップ)
Void SNMusicResManager::AccessGet(SNMusicResID id)
{
	StateMachine(EvtAccessGet, id);
	return ;
}

// リソース解放(参照カウントダウン)
Void SNMusicResManager::AccessRelease(SNMusicResID id)
{
	StateMachine(EvtAccessRelease, id);

	return;
}

SNPCMStream* SNMusicResManager::GetResource(SNMusicResID id)
{
	SNPCMStream* pcm = &DummyMusic;

	if (IsLoaded(id))
	{
		pcm = &ManageInfo[id].Res;
	}

	return pcm;
}

// ロード完了判定
Boolean SNMusicResManager::IsLoaded(SNMusicResID id)
{
	return (ManageInfo[id].State == ResLoaded);
}

SNResMngState SNMusicResManager::StateMachine(SNResMngEvent evt, SNMusicResID id)
{
    SNMusicResMngInfo* info = &ManageInfo[id];

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
Void SNMusicResManager::StateMachineResUnload(SNResMngEvent evt, SNMusicResID id)
{
	SNMusicResMngInfo* info = &ManageInfo[id];

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
			ProcID = (SNMusicResID)SNMath::Increment(ProcID, 0, SNMusicResNum - 1);
		}
		break;
	}

	return;
}

// ロード中
Void SNMusicResManager::StateMachineResLoading(SNResMngEvent evt, SNMusicResID id)
{
	SNMusicResMngInfo* info = &ManageInfo[id];

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
Void SNMusicResManager::StateMachineResLoaded(SNResMngEvent evt, SNMusicResID id)
{
	SNMusicResMngInfo* info = &ManageInfo[id];

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
			ProcID = (SNMusicResID)SNMath::Increment(ProcID, 0, SNMusicResNum - 1);
		}
		break;
	}

	return;
}

// アンロード中
Void SNMusicResManager::StateMachineResUnloading(SNResMngEvent evt, SNMusicResID id)
{
	SNMusicResMngInfo* info = &ManageInfo[id];

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
			// PCM解放し未ロードへ
			info->Res.ClosePCMStream();
			info->State = ResUnload;
		}
		break;
	}

	return;
}

// フェーズ処理
Void SNMusicResManager::StateMachinePhaseProc(SNMusicResID id)
{
	SNMusicResMngInfo* info = &ManageInfo[id];

	switch (info->Phase)
	{
	case PhaseIdle:
		// ファイル名セット
		Loader.SetFolderPath(SNSystemConfig::MusicPath);
		Loader.SetFolderFileName(SNMusicResource::ResourceInfoTable[id]);
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
		// オープン
		info->Res.OpenPCMStream(Loader.GetMemoryObject());
		info->Phase = PhaseWaitDecode;
		break;
	case PhaseWaitDecode:
		// オープン待ち
		if (info->Res.IsOperationComplete())
		{
			info->Phase = PhaseEnd;
		}
		break;
	case PhaseEnd:
		info->State = ResLoaded;
		info->Phase = PhaseIdle;
		break;
	}

	return;
}

