#pragma once
#include "SNFrameworkInternal.h"
#include "SNScene.h"
#include "SNLayerController.h"
#include "SNGUIBMTextLabelEx.h"

// デバッグレイヤクラス
class SNDebugLayer : public virtual SNScene
{
public:
	// スタティックメンバ
	static const UInt32 SystemInfoNum = 3;	// システム情報表示数
	static const String SystemInfoTitle;	// システム情報タイトル
	static const String FPSDispText;		// FPS表示テキスト
	static const String ProcTimeDispText;	// 処理時間テキスト

	static const UInt32 PointingDispNum = 13;				// ポインティングデバイス表示数
	static const String PointingText[PointingDispNum];	// ポインティングデバイステキスト

public:
	// コンストラクタ
	SNDebugLayer();

	// デストラクタ
	virtual ~SNDebugLayer();

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
	SNLayerController ContainerSystemInfo;		// システム情報コンテナ
	SNGUIBMTextLabel LabelTitleSysInfo;			// システム情報タイトル表示
	SNGUIBMTextLabelEx LabelFPS;				// PFS表示
	SNGUIBMTextLabelEx LabelProcTime;			// 処理時間表示
};

