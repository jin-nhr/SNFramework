#pragma once
#include "SNFrameworkInternal.h"
#include "SNSoundAccessor.h"
#include "SNList.h"
#include "SNPCM.h"
#include "SNSoundResource.h"

// サウンドエフェクト
class SNSoundEffect : public SNSoundAccessor
{
public:
	// コンストラクタ
	SNSoundEffect();

	// デストラクタ
	virtual ~SNSoundEffect();

	// Effect生成
	virtual Void CreateEffect(SNPCM* pcm);
	virtual Void CreateEffect(SNSoundResID res_id);
	
	// Effect解放
	virtual Void DeleteEffect();

	// 再生
	virtual Void Play();

	// 停止
	virtual Void Stop();

protected:
	// リクエスト実行関数
	virtual Void OnOperationPlay();
	virtual Void OnOperationStop();

private:
	Boolean UseResID;
	SNSoundResID ResID;
	SNPCM* PCM;
	SNListContainer* SourceVoice;

};
