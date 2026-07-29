#include "SNAudioVideo.h"
#include "SNSoundResManager.h"
#include "SNSoundResource.h"

// オーディオビデオクラス

// 初期化処理
Void SNAudioVideo::Initialize()
{
	SNSound::Initialize();
	SNSoundResManager::Initialize();
	return;
}

// 起動準備
Void SNAudioVideo::Startup()
{


	return;
}

// 実行
Void SNAudioVideo::Run()
{
	return;
}

Void SNAudioVideo::Update()
{
	SNSoundResManager::Update();
	return;
}

// 終了前処理
Void SNAudioVideo::BeforeTerminate()
{


	return;
}

// 終了
Void SNAudioVideo::Terminate()
{
	SNSoundResManager::Terminate();
	SNSound::Terminate();
	return;
}


// サウンドリソースロード
Void SNAudioVideo::LoadSoundResource()
{
	Int32 res_id;

	for (res_id = SNSoundResTop; res_id <= SNSoundResEnd; res_id++)
	{
		SNSoundResManager::AccessGet((SNSoundResID)res_id);
	}

	return;
}

// サウンドリソースロード完了判定
Boolean SNAudioVideo::IsSoundResourceLoaded()
{
	Int32 res_id;
	Boolean loaded = true;

	for (res_id = SNSoundResTop; res_id <= SNSoundResEnd; res_id++)
	{
		loaded &= SNSoundResManager::IsLoaded((SNSoundResID)res_id);
	}

	return loaded;
}

// サウンドリソースアンロード
Void SNAudioVideo::UnloadSoundResource()
{
	Int32 res_id;

	for (res_id = SNSoundResTop; res_id <= SNSoundResEnd; res_id++)
	{
		SNSoundResManager::AccessRelease((SNSoundResID)res_id);
	}

	return;
}

