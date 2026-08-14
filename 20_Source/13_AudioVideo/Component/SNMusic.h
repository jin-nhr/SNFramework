#pragma once
#include "SNFrameworkInternal.h"
#include "SNList.h"
#include "SNPCMStream.h"
#include "SNMusicResource.h"
#include "SNThread.h"

// ミュージック
// ミュージック再生用にVoiceを生成する
// 再生制御はワーカースレッドで行う
// ワーカースレッドは再生中のみ動作させる

class SNMusic : public SNThread
{
public:
	// コンストラクタ
	SNMusic();

	// デストラクタ
	virtual ~SNMusic();

	// Music生成
	virtual Void CreateMusic(SNPCMStream* pcm);
	
	// Music解放
	virtual Void DeleteMusic();

	// 再生
	virtual Void Play();

	// Resume
	virtual Void Resume();

	// 一時停止
	virtual Void Pause();

	// 停止
	virtual Void Stop();

	// 再生準備
	virtual Void PlayStartup();

	// 再生終了
	virtual Void PlayEnd();

	// バッファ解放
	virtual Void ReleaseBuffer();

	// バッファ登録
	virtual Void SubmitBuffer();

	// 再生ブロック解放
	virtual Void ReleasePlayBlock();

private:
	// ユーザー実行関数(派生先で実装する)
	virtual Void UserMain();

	SNPCMStream* PCMStream;
	volatile SNMusicOperation Operation;
	SNListContainer* SourceVoice;

	SNList PlayBlockList;
};
