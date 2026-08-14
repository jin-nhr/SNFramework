#include "SNMusicPlayer.h"
#include "SNMusicResManager.h"
#include "SNSoundDevice.h"
#include "SNSystemConfig.h"
#include "SNMath.h"


SNMusic SNMusicPlayer::Player;
SNMusicLayer SNMusicPlayer::PlayingLayer;
SNMusicResID SNMusicPlayer::PlayingResID;

SNMusicResID SNMusicPlayer::ResID[];
Boolean SNMusicPlayer::PlayState[];
SNTimerSequencer SNMusicPlayer::Seq;



SNMusicPlayer::SNMusicPlayer()
{
	return;
}

SNMusicPlayer::~SNMusicPlayer()
{
	return;
}

Void SNMusicPlayer::Play(SNMusicLayer layer, SNMusicResID res_id)
{
	ResID[layer] = res_id;
	PlayState[layer] = true;

	return;
}

Void SNMusicPlayer::Stop(SNMusicLayer layer)
{
	ResID[layer] = SNMusicResMute;
	PlayState[layer] = false;

	return;
}



Void SNMusicPlayer::OnInitialize()
{
	Int32 layer;

	PlayingLayer = SNMusicLayerInit;
	PlayingResID = SNMusicResMute;

	for (layer = 0; layer < SNMusicLayerNum; layer++)
	{
		ResID[layer] = SNMusicResMute;
		PlayState[layer] = false;
	}

	Seq.Initialize(this, 0, 7);
	Seq.SetWait(0, 0);
	Seq.SetWait(1, 0);
	Seq.SetWait(2, 0);
	Seq.SetWait(3, 0);
	Seq.SetWait(4, 0);
	Seq.SetWait(5, 0);
	Seq.SetWait(6, 0);

	return;
}

Void SNMusicPlayer::OnTerminate()
{
	Player.Stop();

	return;
}



Void SNMusicPlayer::OnCycle()
{
	// 処理中？
	if (Seq.IsProc())
	{
		Seq.Step();
	}

	// 切替確認
	else
	{
		// 再生中レイヤに変更あり？
		if (PlayingLayer != GetPlayingLayer())
		{
			Seq.Start();
		}
	}

	return;
}

SNMusicLayer SNMusicPlayer::GetPlayingLayer()
{
	Int32 layer;

	// 高優先レイヤ側から再生レイヤを検索
	for (layer = SNMusicLayerNum - 1; layer > SNMusicLayerInit; layer--)
	{
		if (PlayState[layer])
		{
			break;
		}
	}

	return (SNMusicLayer)layer;
}


SNPhaseResult SNMusicPlayer::PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count)
{
	SNPhaseResult ret = SNPhaseResultStay;
	Float32 fade_gain;

	switch (ch)
	{
	case 0:
		switch (phase_idx)
		{
		// フェードアウト
		case 0:
			if (PlayingLayer != SNMusicLayerInit)
			{
				fade_gain = (((1000.0F / SNSystemConfig::FPS) + 1) / SNSystemConfig::BGMFadeTime) * call_count;
				SNSoundDevice::SetFade(1.0F - SNMath::SaturateF(fade_gain, 0.0F, 1.0F));

				if (fade_gain >= 1.0F)
				{
					ret = SNPhaseResultNext;
				}
			}
			else
			{
				ret = SNPhaseResultNext;
			}

			break;

		// 停止
		case 1:
			if (PlayingLayer != SNMusicLayerInit)
			{
				Player.Stop();

			}
			ret = SNPhaseResultNext;
			break;

		// リソース解放
		case 2:
			SNMusicResManager::AccessRelease(PlayingResID);
			ret = SNPhaseResultNext;
			break;

		// リソース取得
		case 3:
			PlayingLayer = GetPlayingLayer();

			if (PlayingLayer != SNMusicLayerInit)
			{
				SNMusicResManager::AccessGet(ResID[PlayingLayer]);
			}
			ret = SNPhaseResultNext;
			break;

		// ロード
		case 4:
			if (PlayingLayer != SNMusicLayerInit)
			{
				if (SNMusicResManager::IsLoaded(ResID[PlayingLayer]))
				{
					PlayingResID = ResID[PlayingLayer];
					Player.CreateMusic(SNMusicResManager::GetResource(PlayingResID));
					SNSoundDevice::SetFade(0.0F);
					Player.Play();
					ret = SNPhaseResultNext;
				}
			}
			else
			{
				ret = SNPhaseResultNext;
			}
			break;

		// フェードイン
		case 5:
			if (PlayingLayer != SNMusicLayerInit)
			{
				fade_gain = (((1000.0F / SNSystemConfig::FPS) + 1) / SNSystemConfig::BGMFadeTime) * call_count;
				SNSoundDevice::SetFade(SNMath::SaturateF(fade_gain, 0.0F, 1.0F));

				if (fade_gain >= 1.0F)
				{
					ret = SNPhaseResultNext;
				}
			}
			else
			{
				ret = SNPhaseResultNext;
			}
			break;

		// 再生
		case 6:
			SNSoundDevice::SetFade(1.0F);
			ret = SNPhaseResultNext;
			break;
		}
		break;
	}

	return ret;
}
