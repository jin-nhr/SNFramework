#include "SNSoundEffect.h"
#include "SNSoundDevice.h"
#include "SNSoundResManager.h"
#include "SNWindowsAPI.h"


// コンストラクタ
SNSoundEffect::SNSoundEffect()
{
	UseResID = false;
	ResID = SNSoundResTop;	// 仮でTopを設定
	PCM = nullptr;
	SourceVoice = nullptr;

	return;
}

	// デストラクタ
SNSoundEffect:: ~SNSoundEffect()
{
	DeleteEffect();

	return;
}

	// Effect生成
Void SNSoundEffect::CreateEffect(SNPCM* pcm)
{
	DeleteEffect();

	PCM = pcm;

	// SourceVoice取得
	SourceVoice = SNSoundDevice::GetSourceVoice();

	return;
}

Void SNSoundEffect::CreateEffect(SNSoundResID res_id)
{
	DeleteEffect();

	// アクセス権を取得
	SNSoundResManager::AccessGet(res_id);
	UseResID = true;
	ResID = res_id;

	// SourceVoice取得
	SourceVoice = SNSoundDevice::GetSourceVoice();

	return;
}

	// Effect解放
Void SNSoundEffect::DeleteEffect()
{
	// リソースID使用時はアクセス権放棄
	if (UseResID)
	{
		SNSoundResManager::AccessRelease(ResID);
		ResID = SNSoundResTop;
		UseResID = false;
	}

	PCM = nullptr;

	if (SourceVoice != nullptr)
	{
		// SourceVoice解放
		SNSoundDevice::ReleaseSourceVoice(SourceVoice);
		SourceVoice = nullptr;
	}

	return;
}

// 再生
Void SNSoundEffect::Play()
{
	RequestOperation(SNSoundOperationPlay);
	return;
}

// 停止
Void SNSoundEffect::Stop()
{
	RequestOperation(SNSoundOperationStop);
	return;
}

// リクエスト実行関数
Void SNSoundEffect::OnOperationPlay()
{
	SNPCM* pcm;

	if (UseResID)
	{
		pcm = SNSoundResManager::GetResource(ResID);
	}
	else
	{
		pcm = PCM;
	}

	if ((SourceVoice != nullptr) && (pcm != nullptr))
	{
		SNSoundDevice::Play(SourceVoice, pcm);
	}
	return;
}

Void SNSoundEffect::OnOperationStop()
{
	if (SourceVoice != nullptr)
	{
		SNSoundDevice::Stop(SourceVoice);
	}
	return;
}
