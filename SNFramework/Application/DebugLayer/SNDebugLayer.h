#pragma once
#include "../../Include/SNFramework.h"
#include "../Controller/SNScene.h"
#include "../GUI/SNGUITextLabelEx.h"

// デバッグレイヤクラス
class SNDebugLayer : public virtual SNScene
{
public:
	// スタティックメンバ
	static const UInt32 SystemInfoNum = 3;	// システム情報表示数
	static const String SystemInfoTitle;	// システム情報タイトル
	static const String FPSDispText;		// FPS表示テキスト
	static const String ProcTimeDispText;	// 処理時間テキスト

	static const UInt32 InputDeviceDispNum = SNInputButtonNum + 1; // 入力表示数(ボタン数+タイトル行)
	static const String InputDeviceTitle[SNInputDeviceNum];	// 入力表示タイトル
	static const String InputDeviceText[SNInputButtonNum];	// 入力表示テキスト

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
	virtual Void OnDraw(SNSurface* surface);

private:
	//SNGUIContainer SystemInfoContainer;		// システム情報コンテナ
	//SNGUIBase* SystemInfoList[SystemInfoNum];	// システム情報リスト
	SNGUITextLabel SystemInfoTitleDisp;		// システム情報タイトル表示
	SNGUITextLabelEx FPSDisp;				// PFS表示
	SNGUITextLabelEx ProcTimeDisp;			// 処理時間表示

	//SNGUIContainer InputDeviceContainer[SNInputDeviceNum];		// 入力表示コンテナ
	//SNGUIBase* InputDeviceList[SNInputDeviceNum][InputDeviceDispNum];	// 入力表示リスト
	SNGUITextLabel InputDeviceTitleDisp[SNInputDeviceNum];		// 入力表示タイトル
	SNGUITextLabelEx InputDeviceDisp[SNInputDeviceNum][SNInputButtonNum];	// 入力表示

	//SNGUIContainer PointingContainer;		// ポインティングコンテナ
	//SNGUIBase* PointingList[PointingDispNum];		// ポインティング表示リスト
	SNGUITextLabelEx PointingDisp[PointingDispNum];	// ポインティング表示

	

};

