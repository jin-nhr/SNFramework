#include "SNSoundResManager.h"
#include "SNSoundDevice.h"
#include "SNSystemConfig.h"
#include "SNMath.h"

// リソース管理情報
SNSoundResMngInfo SNSoundResManager::ManageInfo[SNSoundResNum];

// 処理中ID
SNSoundResID SNSoundResManager::ProcID;

// ローダー
SNSoundFile SNSoundResManager::Loader;

// ロードが間に合っていない場合のダミー
SNPCM SNSoundResManager::DummySound;


Void SNSoundResManager::Initialize()
{
	Int32 cnt = 0;

	for (cnt = 0; cnt < SNSoundResNum; cnt++)
	{
		ManageInfo[cnt].State = ResUnload;
		ManageInfo[cnt].Phase = PhaseIdle;
		ManageInfo[cnt].RefCount = 0;
		ManageInfo[cnt].Res.DeletePCM();
	}

	ProcID = (SNSoundResID)0;

	// ダミーPCM生成
	DummySound.CreatePCM(32);
	DummySound.Channels = 2;
	DummySound.SampleRate = 44100;
	DummySound.BitPerSample = 16;

    return;
}


Void SNSoundResManager::Terminate()
{
	Int32 cnt = 0;

	for (cnt = 0; cnt < SNSoundResNum; cnt++)
	{
		ManageInfo[cnt].State = ResUnload;
		ManageInfo[cnt].Phase = PhaseIdle;
		ManageInfo[cnt].RefCount = 0;
		ManageInfo[cnt].Res.DeletePCM();
	}

	ProcID = (SNSoundResID)0;

	DummySound.DeletePCM();

	// 念のため待ってから破棄
	Loader.WaitForOperationComplete();
	Loader.DeletePCM();	// ローダー破棄
	
    return;
}

Void SNSoundResManager::Update()
{
	StateMachine(EvtCycle, ProcID);
}

// リソース取得(参照カウントアップ)
Void SNSoundResManager::AccessGet(SNSoundResID id)
{
	StateMachine(EvtAccessGet, id);
	return ;
}

// リソース解放(参照カウントダウン)
Void SNSoundResManager::AccessRelease(SNSoundResID id)
{
	StateMachine(EvtAccessRelease, id);

	return;
}

SNPCM* SNSoundResManager::GetResource(SNSoundResID id)
{
	SNPCM* pcm = &DummySound;

	if (IsLoaded(id))
	{
		pcm = &ManageInfo[id].Res;
	}

	return pcm;
}

// ロード完了判定
Boolean SNSoundResManager::IsLoaded(SNSoundResID id)
{
	return (ManageInfo[id].State == ResLoaded);
}

SNResMngState SNSoundResManager::StateMachine(SNResMngEvent evt, SNSoundResID id)
{
    SNSoundResMngInfo* info = &ManageInfo[id];

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
Void SNSoundResManager::StateMachineResUnload(SNResMngEvent evt, SNSoundResID id)
{
	SNSoundResMngInfo* info = &ManageInfo[id];

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
			ProcID = (SNSoundResID)SNMath::Increment(ProcID, 0, SNSoundResNum - 1);
		}
		break;
	}

	return;
}

// ロード中
Void SNSoundResManager::StateMachineResLoading(SNResMngEvent evt, SNSoundResID id)
{
	SNSoundResMngInfo* info = &ManageInfo[id];

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
Void SNSoundResManager::StateMachineResLoaded(SNResMngEvent evt, SNSoundResID id)
{
	SNSoundResMngInfo* info = &ManageInfo[id];

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
			ProcID = (SNSoundResID)SNMath::Increment(ProcID, 0, SNSoundResNum - 1);
		}
		break;
	}

	return;
}

// アンロード中
Void SNSoundResManager::StateMachineResUnloading(SNResMngEvent evt, SNSoundResID id)
{
	SNSoundResMngInfo* info = &ManageInfo[id];

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
			info->Res.DeletePCM();
			info->State = ResUnload;
		}
		break;
	}

	return;
}

// フェーズ処理
Void SNSoundResManager::StateMachinePhaseProc(SNSoundResID id)
{
	SNSoundResMngInfo* info = &ManageInfo[id];

	switch (info->Phase)
	{
	case PhaseIdle:
		// ファイル名セット
		Loader.SetFolderPath(SNSystemConfig::SoundPath);
		Loader.SetFolderFileName(SNSoundResource::ResourceInfoTable[id]);
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
			// ローダーからリソースPCMへコピー
			info->Res.CreatePCM(Loader.PCMData.GetSize());
			info->Res.PCMData.Copy(Loader.PCMData.GetAddress(), Loader.PCMData.GetSize());
			info->Res.BitPerSample = Loader.BitPerSample;
			info->Res.SampleRate = Loader.SampleRate;
			info->Res.Channels = Loader.Channels;			
			info->State = ResLoaded;
		}
		else
		{
			// 参照カウントが0ならPCMに展開せずに未ロードへ
			info->State = ResUnload;
		}
		info->Phase = PhaseIdle;
		break;
	}

	return;
}

