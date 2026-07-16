#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNGUIBMTextLabel.h"
#include "SNGUIRect.h"
#include "SNLayerController.h"
#include "SNGUIFader.h"
#include "SNSoftTimer.h"

// アプリケーション(ローディング)
class SNApplicationLoading : public virtual SNLayerController
{
public:
	// コンストラクタ
	SNApplicationLoading();

	// デストラクタ
	virtual ~SNApplicationLoading();

protected:
	// 初期化
	virtual Void OnInitialize();

	// 終了処理
	virtual Void OnTerminate();

	// Entry
	virtual Void OnEntry();

	// Exit
	virtual Void OnExit();

	// 1フレーム実行
	// リターン：遷移先コード
	virtual SNTransitionCode OnStep(SNEvent* event);

	// 描画処理
	virtual Void OnDraw(SNSurface* grc);

private:
	static constexpr UInt32 InitialTime = 1000;
	static constexpr UInt32 FadeInTime = 3000;
	static constexpr UInt32 LogoTime = 3000;
	static constexpr UInt32 FadeOutTime = 3000;
	static constexpr UInt32 AfterTime = 1000;

	SNGUIBMTextLabel	Message;		// メッセージ
	SNGUIRect			BackGround;		// 背景
	SNGUIFader			Fader;			// フェーダー
	SNSoftTimer			PhaseTimer;		// タイマ
	UInt8				Phase;			// フェーズ
};
