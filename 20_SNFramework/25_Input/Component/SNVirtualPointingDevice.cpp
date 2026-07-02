#include "SNVirtualPointingDevice.h"
#include "SNConfig.h"
#include "SNInput.h"
#include "SNMouse.h"


Boolean SNVirtualPointingDevice::Event[SNVirtualPointingEventNum];		// ポインティングイベント
SNPoint SNVirtualPointingDevice::Info[SNVirtualPointingInfoNum];			// ポインティング情報
Boolean SNVirtualPointingDevice::State[SNVirtualPointingStateNum];		// ポインティング状態

// イベント有無
Boolean SNVirtualPointingDevice::EventExist = false;

// 初期化
Void SNVirtualPointingDevice::Initialize()
{
	Int32 loop_cnt;

	EventExist = false;

	// 状態初期化
	for (loop_cnt = SNVirtualPointingStateTop; loop_cnt < SNVirtualPointingStateNum; loop_cnt++)
	{
		State[loop_cnt] = false;
	}

	// 座標初期化
	for (loop_cnt = SNVirtualPointingInfoTop; loop_cnt < SNVirtualPointingInfoNum; loop_cnt++)
	{
		Info[loop_cnt].X = 0;
		Info[loop_cnt].Y = 0;
	}

	// イベント初期化
	for (loop_cnt = SNVirtualPointingEventTop; loop_cnt < SNVirtualPointingEventNum; loop_cnt++)
	{
		Event[loop_cnt] = false;
	}

	return;
}

// 終了処理
Void SNVirtualPointingDevice::Terminate()
{
	return;
}

// 状態更新
Void SNVirtualPointingDevice::Update()
{
	// 更新前処理
	UpdatePrevProc();

	// 以降の処理については
	// 後方の処理で前方の処理の結果を参照するため
	// 処理順を守ること

	// 更新処理(決定)
	UpdateDecide();

	// 更新処理(キャンセル)
	UpdateCancel();

	// 更新処理(ホイール)
	UpdateWheel();

	// 更新処理(移動)
	UpdateMove();

	// 更新処理(ドラッグ)
	UpdateDrag();

	// 更新処理(ドロップ)
	UpdateDrop();

	// 更新処理(フリック)
	UpdateFlick();

	// 更新処理(拡張ボタン)
	UpdateExBtn();

	return;
}

// 状態更新前処理
Void SNVirtualPointingDevice::UpdatePrevProc()
{
	Int32 loop_cnt;

	// 前情報保存
	State[SNVirtualPointingStatePreviousDecide] = State[SNVirtualPointingStateDecide];
	State[SNVirtualPointingStatePreviousCancel] = State[SNVirtualPointingStateCancel];
	State[SNVirtualPointingStatePreviousExBtn] = State[SNVirtualPointingStateExBtn];
	State[SNVirtualPointingStatePreviousDrag] = Event[SNVirtualPointingEventDrag];
	State[SNVirtualPointingStatePreviousFlick] = Event[SNVirtualPointingEventFlick];
	Info[SNVirtualPointingInfoPrevious] = Info[SNVirtualPointingInfoCurrent];

	// 現在情報を設定
	State[SNVirtualPointingStateDecide] = SNMouse::ButtonState[SNMouseButtonDecide];
	State[SNVirtualPointingStateCancel] = SNMouse::ButtonState[SNMouseButtonCancel];
	State[SNVirtualPointingStateExBtn] = SNMouse::ButtonState[SNMouseButtonMiddle];
	Info[SNVirtualPointingInfoCurrent] = SNMouse::Position;

	// 差分情報
	Info[SNVirtualPointingInfoDelta].X = Info[SNVirtualPointingInfoCurrent].X - Info[SNVirtualPointingInfoPrevious].X;
	Info[SNVirtualPointingInfoDelta].Y = Info[SNVirtualPointingInfoCurrent].Y - Info[SNVirtualPointingInfoPrevious].Y;


	// イベント初期化
	// 本処理前半で前回のイベント情報を参照するため最後に初期化する
	for (loop_cnt = SNVirtualPointingEventTop; loop_cnt < SNVirtualPointingEventNum; loop_cnt++)
	{
		Event[loop_cnt] = false;
	}

	EventExist = false;

	return;
}

// 状態更新(決定)
Void SNVirtualPointingDevice::UpdateDecide()
{
	/////////////////////////////////////////////////
	// 決定判定
	// 前状態OFF→ON かつ クリップしていない (=画面範囲内)
	if ((!State[SNVirtualPointingStatePreviousDecide]) && State[SNVirtualPointingStateDecide] && (!SNMouse::PosClipping))
	{
		Event[SNVirtualPointingEventDecide] = true;
		EventExist = true;
		State[SNVirtualPointingStateClickEnable] = true;			// 決定ボタンの操作が有効で、以降ボタンを放すまでtrueを維持

		// ドラッグ、フリック開始点を保存
		Info[SNVirtualPointingInfoDragStart] = SNMouse::Position;

		// フリック関連情報を初期化
		Info[SNVirtualPointingInfoFlickVelocity].X = 0;
		Info[SNVirtualPointingInfoFlickVelocity].Y = 0;
	}

	// 決定OFF判定
	if (!State[SNVirtualPointingStateDecide])
	{
		State[SNVirtualPointingStateClickEnable] = false;
	}

	return;
}

// 更新処理(キャンセル)
Void SNVirtualPointingDevice::UpdateCancel()
{
	/////////////////////////////////////////////////
	// キャンセル判定
	// 前状態OFF→ON かつ クリップしていない (=画面範囲内)
	if ((!State[SNVirtualPointingStatePreviousCancel]) && State[SNVirtualPointingStateCancel] && (!SNMouse::PosClipping))
	{
		Event[SNVirtualPointingEventCancel] = true;
		EventExist = true;
	}

	return;
}

// 更新処理(ホイール)
Void SNVirtualPointingDevice::UpdateWheel()
{
	/////////////////////////////////////////////////
	// ホイール
	Event[SNVirtualPointingEventWheelUp] = SNMouse::ButtonState[SNMouseButtonWheelUp];
	Event[SNVirtualPointingEventWheelDown] = SNMouse::ButtonState[SNMouseButtonWheelDown];
	if (Event[SNVirtualPointingEventWheelUp] || Event[SNVirtualPointingEventWheelDown])
	{
		EventExist = true;
	}

	return;
}

// 更新処理(移動)
Void SNVirtualPointingDevice::UpdateMove()
{
	/////////////////////////////////////////////////
	// 移動判定
	// X or Yの移動検知
	if ((Info[SNVirtualPointingInfoDelta].X != 0) ||
		(Info[SNVirtualPointingInfoDelta].Y != 0))
	{
		Event[SNVirtualPointingEventMove] = true;
		EventExist = true;
	}

	return;
}

// 更新処理(ドラッグ)
Void SNVirtualPointingDevice::UpdateDrag()
{
	/////////////////////////////////////////////////
	// ドラッグ判定
	// ON継続中の移動検知またはドラッグ
	// Click有効時のみ
	if (State[SNVirtualPointingStateClickEnable] &&
		State[SNVirtualPointingStateDecide] &&
		State[SNVirtualPointingStatePreviousDecide] &&
		(Event[SNVirtualPointingEventMove] || State[SNVirtualPointingStatePreviousDrag]))
	{
		Event[SNVirtualPointingEventDrag] = true;
		EventExist = true;
	}

	return;
}

// 更新処理(ドロップ)
Void SNVirtualPointingDevice::UpdateDrop()
{
	/////////////////////////////////////////////////
	// ドロップ判定
	// ON→OFF検知かつDrag中
	if (!State[SNVirtualPointingStateDecide] &&
		State[SNVirtualPointingStatePreviousDecide] &&
		State[SNVirtualPointingStatePreviousDrag])
	{
		Event[SNVirtualPointingEventDrop] = true;
		EventExist = true;
	}

	return;
}

// 更新処理(フリック)
Void SNVirtualPointingDevice::UpdateFlick()
{
	/////////////////////////////////////////////////
	// フリック開始までの判定

	// フリック動作中以外
	if (!State[SNVirtualPointingStatePreviousFlick])
	{
		// ドラッグ中かつ移動中のみ計算
		if (Event[SNVirtualPointingEventDrag])
		{
			Info[SNVirtualPointingInfoFlickVelocity].X = ((Info[SNVirtualPointingInfoFlickVelocity].X * SNSystemConfig::FlickTraceGain) / 256) + Info[SNVirtualPointingInfoDelta].X;
			Info[SNVirtualPointingInfoFlickVelocity].Y = ((Info[SNVirtualPointingInfoFlickVelocity].Y * SNSystemConfig::FlickTraceGain) / 256) + Info[SNVirtualPointingInfoDelta].Y;
		}

		// ドラッグ以外のときは初期化(ドロップ時除く)
		else if (!Event[SNVirtualPointingEventDrop])
		{
			Info[SNVirtualPointingInfoFlickVelocity].X = 0;
			Info[SNVirtualPointingInfoFlickVelocity].Y = 0;
		}

		// ドロップ時(フリックによる移動開始)
		else
		{
			Event[SNVirtualPointingEventFlick] = true;
			EventExist = true;
			Info[SNVirtualPointingInfoFlickVelocity].X = ((Info[SNVirtualPointingInfoFlickVelocity].X * SNSystemConfig::FlickTraceGain) / 256) + Info[SNVirtualPointingInfoDelta].X;
			Info[SNVirtualPointingInfoFlickVelocity].Y = ((Info[SNVirtualPointingInfoFlickVelocity].Y * SNSystemConfig::FlickTraceGain) / 256) + Info[SNVirtualPointingInfoDelta].Y;
		}
	}

	// フリック中
	else
	{
		// 移動量減衰
		Info[SNVirtualPointingInfoFlickVelocity].X = (Info[SNVirtualPointingInfoFlickVelocity].X * SNSystemConfig::FlickFadeGain) / 256;
		Info[SNVirtualPointingInfoFlickVelocity].Y = (Info[SNVirtualPointingInfoFlickVelocity].Y * SNSystemConfig::FlickFadeGain) / 256;

		// XまたはYの移動量ありならFlick継続
		if ((Info[SNVirtualPointingInfoFlickVelocity].X) != 0 || (Info[SNVirtualPointingInfoFlickVelocity].Y != 0))
		{
			Event[SNVirtualPointingEventFlick] = true;
			EventExist = true;
		}
	}

	// 別の操作が入ったときはFlickを解除
	if (Event[SNVirtualPointingEventDecide] ||
		Event[SNVirtualPointingEventCancel]  ||
		Event[SNVirtualPointingEventWheelUp] ||
		Event[SNVirtualPointingEventWheelDown])
	{
		Event[SNVirtualPointingEventFlick] = false;
		Info[SNVirtualPointingInfoFlickVelocity].X = 0;
		Info[SNVirtualPointingInfoFlickVelocity].Y = 0;
	}

	return;
}

// 更新処理(拡張ボタン)
Void SNVirtualPointingDevice::UpdateExBtn()
{
	/////////////////////////////////////////////////
	// 拡張ボタン判定
	// 前状態OFF→ON かつ クリップしていない (=画面範囲内)
	if ((!State[SNVirtualPointingStatePreviousExBtn]) && State[SNVirtualPointingStateExBtn] && (!SNMouse::PosClipping))
	{
		Event[SNVirtualPointingEventExBtn] = true;
		EventExist = true;
	}

	return;
}
