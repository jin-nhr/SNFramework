#pragma once
#include "SNFramework.h"

#if 0

class TestApplication : public SNUserApplication
{
public:
	// コンストラクタ
	TestApplication();

	// デストラクタ
	virtual ~TestApplication();

	// 初期化
	virtual Void Initialize();

	// 終了処理
	virtual Void Terminate();

	// Entry
	virtual Void Entry();

	// Exit
	virtual Void Exit();

	// 1フレーム実行
	// リターン：遷移先コード
	virtual SNTransitionCode Step(SNEvent* event);

	// 描画処理
	virtual Void Draw(SNSurface* surface);

private:
	SNGUIFader		fader;
	SNGUITextLabel	text;
	SNImageFile		bg;
	SNGUIRect		rect;
	SNGUICircle		circle;
	SNGUILine		line;
	SNGUIImage		image;
};

#endif
