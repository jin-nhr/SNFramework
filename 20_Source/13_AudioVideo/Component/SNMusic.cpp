#include "SNMusic.h"
#include "SNSoundDevice.h"
#include "SNMusicResManager.h"
#include "SNWindowsAPI.h"
#include "SNAutoResource.h"

// XAUIDOとのIF用クラスのため、外部には公開しない
class SNXAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	SNXAudio2VoiceCallback(SNMusic* music)
	{
		Music = music;
		return;
	}

	virtual ~SNXAudio2VoiceCallback()
	{
		return;
	}

	virtual void OnVoiceProcessingPassStart(UINT32 BytesRequired)
	{
		return;
	}
	virtual void OnVoiceProcessingPassEnd()
	{
		return;
	}
	virtual void OnStreamEnd()
	{
		return;
	}
	virtual void OnBufferStart(void* pBufferContext)
	{
		return;
	}
	virtual void OnBufferEnd(void* pBufferContext)
	{
		Music->SubmitBuffer();
		return;
	}
	virtual void OnLoopEnd(void* pBufferContext)
	{
		return;
	}
	virtual void OnVoiceError(void* pBufferContext, HRESULT Error)
	{
		return;
	}

	SNMusic* Music;
};


// コンストラクタ
SNMusic::SNMusic()
{
	UseResID = false;
	ResID = SNMusicResTop;	// 仮でTopを設定
	PCMStream = nullptr;
	Operation = SNMusicOperationNo;
	Callback = new SNXAudio2VoiceCallback(this);
	SourceVoice = nullptr;

	DummyPCM.Allocate(32);
	DummyPCM.Clear();

	// SourceVoice生成
	SourceVoice = SNSoundDevice::CreateMusicVoice(Callback);

	return;
}

	// デストラクタ
SNMusic:: ~SNMusic()
{
	SNXAudio2VoiceCallback* callback = (SNXAudio2VoiceCallback*)Callback;

	DeleteMusic();

	// 事前にSTOPしていない場合、ここでスレッド終了を待つ(しかない)
	WaitForThreadEnd();

	if (SourceVoice != nullptr)
	{
		// SourceVoice破棄
		SNSoundDevice::DeleteMusicVoice(SourceVoice);
		SourceVoice = nullptr;
	}

	if (callback != nullptr)
	{
		delete callback;
		Callback = nullptr;
	}

	return;
}

	// 
Void SNMusic::CreateMusic(SNPCMStream* pcm)
{
	DeleteMusic();

	PCMStream = pcm;

	return;
}

Void SNMusic::CreateMusic(SNMusicResID res_id)
{
	DeleteMusic();

	// アクセス権を取得
	SNMusicResManager::AccessGet(res_id);
	UseResID = true;
	ResID = res_id;

	return;
}

	// Effect解放
Void SNMusic::DeleteMusic()
{
	Stop();

	// リソースID使用時はアクセス権放棄
	if (UseResID)
	{
		SNMusicResManager::AccessRelease(ResID);
		ResID = SNMusicResTop;
		UseResID = false;
	}

	PCMStream = nullptr;

	return;
}

// 再生
Void SNMusic::Play()
{
	Operation = SNMusicOperationPlay;
	// スレッド起動していなければ起動
	if (!GetRunStatus())
	{
		Run();
	}
	return;
}

// Resume
Void SNMusic::Resume()
{
	Operation = SNMusicOperationPlay;
	return;
}

// Pause
Void SNMusic::Pause()
{
	Operation = SNMusicOperationPause;
	return;
}

// 停止
Void SNMusic::Stop()
{
	Operation = SNMusicOperationStop;
	return;
}

Void SNMusic::PlayStartup()
{
	// バッファクリアのためにSTOPしておく
	SNSoundDevice::MusicStop(SourceVoice);

	// デコード(初回)
	PCMStream->Decode(true);

	// 最初のバッファを登録
	SubmitBuffer();

	return;
}

// バッファ枯渇コールバック
Void SNMusic::SubmitBuffer()
{
	SNMemory* pcm;

	SNAutoResource res(&PCMStream->CS);

	// ストリームのブロック取得
	pcm = PCMStream->GetStreamBlock();

	// もしデコードが追い付いていないときはダミーをセット
	if (pcm == nullptr)
	{
		pcm = &DummyPCM;
	}

	// ブロックをサブミット
	SNSoundDevice::SubmitMusicBuffer(SourceVoice, pcm);

	return;
}

Void SNMusic::UserMain()
{
	SNMusicOperation old_operation = SNMusicOperationNo;
	SNMusicOperation exe_operation = SNMusicOperationNo;
	Boolean stop_flg = false;

	// 初回デコード/バッファ設定
	PlayStartup();

	// 制御ループ
	while (true)
	{
		// 命令変化時
		exe_operation = Operation;

		if (old_operation != exe_operation)
		{
			switch (Operation)
			{
			case SNMusicOperationPlay:
				// XADUIOに再生指示
				SNSoundDevice::MusicPlay(SourceVoice);
				break;

			case SNMusicOperationPause:
				// XAUDIOに一時停止
				SNSoundDevice::MusicPause(SourceVoice);
				break;

			case SNMusicOperationStop:
				// XAUDIOに停止指示
				SNSoundDevice::MusicStop(SourceVoice);

				// 停止フラグを設定
				stop_flg = true;
				break;
			}

			// 前回オペを更新
			old_operation = exe_operation;
		}

		if (stop_flg)
		{
			break;
		}

		// デコード処理(継続)
		PCMStream->Decode(false);

		// 1msのスリープをはさんでおく
		::Sleep(1);
	}

	return;
}

