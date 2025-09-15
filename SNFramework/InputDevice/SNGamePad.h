#pragma once
#include "../Include/SNFramework.h"


// ゲームパッド軸
enum SNGamePadAxis
{
	SNGamePadAxisX,
	SNGamePadAxisY,
	SNGamePadAxisZ,
	SNGamePadAxisR,
	SNGamePadAxisU,
	SNGamePadAxisV,
	SNGamePadAxisNum
};

// 軸情報
struct SNAxisInfo
{
	Boolean Enable;			// 有効フラグ
	UInt32 High;			// 上限
	UInt32 Low;				// 下限
	UInt32 Center;			// 中央値
	UInt32 High_Threshold;	// 上限側閾値
	UInt32 Low_Threshold;	// 下限側閾値
};

// ゲームパッド情報
struct SNGamePadInfo
{
	UInt8 ID;								// ゲームパッドID
	Boolean Enable;							// 有効フラグ
	UInt8 ButtonNum;						// ボタン数
	SNAxisInfo AxisInfo[SNGamePadAxisNum];	// 軸情報
};

// ゲームパッドクラス
class SNGamePad
{
// インスタンスメンバ
public:
	// 初期化処理
	static Void Initialize();

	// 終了処理
	static Void Terminate();

	// 更新処理
	static Void Update();

	// ゲームパッド情報
	static SNGamePadInfo GamePadInfo[SNGamePadIDNum];

	// ボタン状態
	static Boolean ButtonState[SNGamePadIDNum][SNGamePadButtonNum];

private:
	// 初期化処理
	// パラメータ：ゲームパッドID
	static Void OnInitialize(UInt8 id);

	// 終了処理
	static Void OnTerminate(UInt8 id);

	// 更新処理
	static Void OnUpdate(UInt8 id);

	// デバイス情報更新
	// パラメータ：ゲームパッドID
	static Void GamePadInfoUpdate(UInt8 id);
};
