#include "SNMusic.h"
#include "SNSoundDevice.h"
#include "SNMusicResManager.h"
#include "SNWindowsAPI.h"
#include "SNAutoResource.h"
#include "SNSystemConfig.h"

// コンストラクタ
SNMusic::SNMusic()
{
	PCMStream = nullptr;
	Operation = SNMusicOperationNo;
	SourceVoice = nullptr;

	PlayBlockList.Allocate(SNSystemConfig::StreamingBlockNum);

	return;
}

	// デストラクタ
SNMusic:: ~SNMusic()
{
	DeleteMusic();

	// 事前にSTOPしていない場合、ここでスレッド終了を待つ(しかない)
	WaitForThreadEnd();

	return;
}

	// 
Void SNMusic::CreateMusic(SNPCMStream* pcm)
{
	DeleteMusic();

	SourceVoice = SNSoundDevice::GetSourceVoice();

	PCMStream = pcm;

	return;
}


	// Effect解放
Void SNMusic::DeleteMusic()
{
	Stop();

	if (SourceVoice != nullptr)
	{
		SNSoundDevice::ReleaseSourceVoice(SourceVoice);
		SourceVoice = nullptr;
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
	Int32 cnt;

	// バッファクリアのためにSTOPしておく
	SNSoundDevice::MusicStop(SourceVoice);

	// デコード開始処理
	PCMStream->StartDecode();

	// フルデコード
	PCMStream->DecodeFull();

	// 最初のバッファを登録
	for (cnt = 0; cnt < SNSystemConfig::StreamingBlockNum; cnt++)
	{
		SubmitBuffer();
	}

	return;
}

Void SNMusic::PlayEnd()
{
	Int32 cnt;
	Int32 rel_num = PlayBlockList.GetNum();


	SNSoundDevice::MusicStop(SourceVoice);

	// 再生リストにあるブロックを解放
	for (cnt = 0; cnt < rel_num; cnt++)
	{
		ReleasePlayBlock();
	}

	PCMStream->EndDecode();

	return;
}

Void SNMusic::ReleaseBuffer()
{
	Int32 xa_num = SNSoundDevice::GetBufferNum(SourceVoice);
	Int32 list_num = PlayBlockList.GetNum();
	Int32 rel_num = list_num - (xa_num + 1);
	Int32 cnt;

	// 再生が終了したブロックを解放
	for (cnt = 0; cnt < rel_num; cnt++)
	{
		ReleasePlayBlock();
	}
	return;
}

Void SNMusic::SubmitBuffer()
{
	SNListContainer* pcm;
	SNListContainer* it;

	// ストリームのブロック取得
	pcm = PCMStream->GetStreamBlock();

	if (pcm != nullptr)
	{
		// ブロックをサブミット
		SNSoundDevice::SubmitMusicBuffer(SourceVoice, (SNMemory*)pcm->UserData);

		it = PlayBlockList.InsertLast();
		it->UserData = pcm;
	}

	return;
}

Void SNMusic::ReleasePlayBlock()
{
	SNListContainer* it;

	it = PlayBlockList.GetTop();
	PCMStream->ReleaseStreamBlock((SNListContainer*)it->UserData);
	PlayBlockList.RemoveTop();

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
		PCMStream->Decode();

		// バッファ解放
		ReleaseBuffer();

		// バッファ登録
		SubmitBuffer();

		// 1msのスリープをはさんでおく
		::Sleep(1);
	}

	PlayEnd();

	return;
}

