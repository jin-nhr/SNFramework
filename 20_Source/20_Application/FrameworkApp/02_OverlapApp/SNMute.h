#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNTimerSequencer.h"

// Muteアプリ
class SNMute : public SNScene
{
public:
	// ミュート設定(on/off, fade有無)
	static Void SetMute(Boolean mute, Boolean fade);

	// ミュート状態取得
	static Boolean GetNowSts();

public:
	// コンストラクタ
	SNMute();

	// デストラクタ
	virtual ~SNMute();

protected:

	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// Entry
	virtual Void OnEntry();

	// Exit
	virtual Void OnExit();

	// フレーム処理
	virtual Void OnCycle();

	// 描画処理
	virtual Void OnDraw(SNGraphicsContext* grc);

	virtual SNPhaseResult PhaseStepFunc(Int32 ch, Int32 phase_idx, Int32 call_count);

private:
	static SNBitmap Surface;
	static Boolean RequestMute;
	static Boolean RequestFade;
	static Boolean NowMuteSts;
	static Float32 Alpha;

	static SNTimerSequencer FadeInSeq;
	static SNTimerSequencer FadeOutSeq;


};

