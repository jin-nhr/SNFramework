#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"


// Muteアプリ

enum SNMuteSts
{
	SNMuteStsOn,
	SNMuteStsFadeIn,
	SNMuteStsOff,
	SNMuteStsFadeOut,
	SNMuteStsNum
};


class SNMute : public SNScene
{
public:
	// ミュート設定(on/off, fade有無, 色)
	static Void SetMute(Boolean mute, Boolean fade);

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

private:
	static SNBitmap Surface;
	static Boolean UpdateFlag;
	static SNMuteSts NowSts;
	static Float32 Alpha;
	static Float32 FadeStep;
	static Boolean RequestMute;
	static Boolean RequestFade;

};

