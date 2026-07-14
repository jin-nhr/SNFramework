#include "SNScene.h"
#include "SNConfig.h"

// シーンクラス

// コンストラクタ
SNScene::SNScene()
{
	Enable = true;
	Visible = true;
	ParentPosition = {0};
	ParentSize = {
		(Int32)SNSystemConfig::ScreenWidth,
		(Int32)SNSystemConfig::ScreenHeight };
	LocalPosition = {0};
	Size = {
		(Int32)SNSystemConfig::ScreenWidth,
		(Int32)SNSystemConfig::ScreenHeight };
	TransCode = SNTransitionCodeStay;
	return;
}

// デストラクタ
SNScene::~SNScene()
{
	return;
}

// 初期化
Void SNScene::Initialize()
{
	OnInitialize();
	return;
}

// 終了処理
Void SNScene::Terminate()
{
	OnTerminate();
	return;
}

// Entry
Void SNScene::Entry()
{
	OnEntry();
	return;
}

// Exit
Void SNScene::Exit()
{
	OnExit();
	return;
}

// 1フレーム実行
Void SNScene::Step()
{
	Int32 evt;

	// 有効時のみ処理
	if (Enable)
	{

		// 遷移コードを初期化しておく
		TransCode = SNTransitionCodeStay;

		for (evt = 0; evt < SNEventTypeNum; evt++)
		{
			if (SNEvent::EventExist[evt])
			{
				EventProc((SNEventType)evt);
			
				// 遷移判定時は抜ける
				if (TransCode != SNTransitionCodeStay)
				{
					break;
				}
			}
		}

		// 遷移なしのときは周期処理を実行
		if (TransCode == SNTransitionCodeStay)
		{
			OnCycle();
		}
	}

	return;
}

// イベント処理
Boolean SNScene::EventProc(SNEventType evt)
{
	Boolean ret = false;

	// 指定イベントあり
	switch (evt)
	{
	case SNEventTypeNotifyEvent:
		ret = OnNotifyEvent();
		break;
	case SNEventTypeInternalEvent:
		ret = OnInternalEvent();
		break;
	case SNEventTypeVirtualGamePad1:
		ret = OnGamePad1();
		break;
	case SNEventTypeVirtualGamePad2:
		ret = OnGamePad2();
		break;
	case SNEventTypeVirtualPointing:
		ret = OnPointingDevice();
		break;
	case SNEventTypeVirtualDirectKeyboard:
		ret = OnDirectKeyboard();
		break;
	case SNEventTypeVirtualDirectGamePad:
		ret = OnDirectGamePad();
		break;
	}

	// イベント消費時はクリア
	if (ret)
	{
		SNEvent::EventExist[evt] = false;
	}

	return ret;
}

// 描画処理
Void SNScene::Draw(SNGraphicsContext* grc)
{
	// 有効時のみ処理
	if (Enable)
	{
		// 表示状態時のみDraw実行
		if (Visible)
		{
			OnDraw(grc);
		}
	}
	
	return;
}

// 座標/サイズ設定
Void SNScene::SetRect(Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 座標、サイズ設定
	LocalPosition.X = x;
	LocalPosition.Y = y;
	Size.Width = w;
	Size.Height = h;

	return;
}

// オフセット座標/サイズ取得
SNRect SNScene::GetRect()
{
	SNRect rect;

	// ローカル座標、サイズを返す
	rect.PointX = LocalPosition.X;
	rect.PointY = LocalPosition.Y;
	rect.Width = Size.Width;
	rect.Height = Size.Height;

	return rect;
}

// 親座標/サイズ設定
Void SNScene::SetParentRect(Int32 x, Int32 y, Int32 w, Int32 h)
{
	// 座標、サイズ設定
	ParentPosition.X = x;
	ParentPosition.Y = y;
	ParentSize.Width = w;
	ParentSize.Height = h;

	return;
}

// 親座標/サイズ設定
SNRect SNScene::GetParentRect()
{
	SNRect rect;

	// ローカル座標、サイズを返す
	rect.PointX = ParentPosition.X;
	rect.PointY = ParentPosition.Y;
	rect.Width = ParentSize.Width;
	rect.Height = ParentSize.Height;

	return rect;
}

// 移動
// パラメータ：移動量を指定する
Void SNScene::Move(Int32 x, Int32 y)
{
	// 移動量を加算
	LocalPosition.X += x;
	LocalPosition.Y += y;

	return;
}

// リサイズ
Void SNScene::Resize(Int32 w, Int32 h)
{
	Size.Width = w;
	Size.Height = h;

	return;
}

// センタリング処理
Void SNScene::Centering(Boolean v, Boolean h)
{
	// 垂直方向
	if (v)
	{
		LocalPosition.Y = (ParentSize.Height - Size.Height) / 2;
	}

	// 水平方向
	if (h)
	{
		LocalPosition.X = (ParentSize.Width - Size.Width) / 2;
	}

	return;
}

// グローバル座標/サイズ取得
SNRect SNScene::CalcGlobalRect()
{
	SNRect rect;

	// グローバル座標に変換
	rect.PointX = ParentPosition.X + LocalPosition.X;
	rect.PointY = ParentPosition.Y + LocalPosition.Y;
	rect.Width = Size.Width;
	rect.Height = Size.Height;

	return rect;
}

// 初期化
Void SNScene::OnInitialize()
{
	return;
}

// 終了処理
Void SNScene::OnTerminate()
{
	return;
}

// Entry
Void SNScene::OnEntry()
{
	return;
}

// Exit
Void SNScene::OnExit()
{
	return;
}

// アプリ通知
Boolean SNScene::OnNotifyEvent()
{
	return false;
}

// 内部イベント
Boolean SNScene::OnInternalEvent()
{
	return false;
}

// ゲームパッド
Boolean SNScene::OnGamePad1()
{
	return false;
}

// ゲームパッド
Boolean SNScene::OnGamePad2()
{
	return false;
}

// ポインティングデバイス
Boolean SNScene::OnPointingDevice()
{
	return false;
}

// ダイレクトキーボード
Boolean SNScene::OnDirectKeyboard()
{
	return false;
}

// ダイレクトゲームパッド
Boolean SNScene::OnDirectGamePad()
{
	return false;
}


// 1フレーム実行
Void SNScene::OnCycle()
{
	return;
}

// 描画処理
Void SNScene::OnDraw(SNGraphicsContext* grc)
{
	return;
}
