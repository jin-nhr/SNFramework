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
	// コンストラクタ
	SNGamePad();

	// デストラクタ
	~SNGamePad();

	// 初期化処理
	// パラメータ：ゲームパッドID
	Void Initialize(UInt8 id);

	// 終了処理
	Void Terminate();

	// 更新処理
	Void Update();

	// 状態取得
	const Boolean* GetState();

private:
	// デバイス情報更新
	// パラメータ：ゲームパッドID
	Void GamePadInfoUpdate(UInt8 id);

	// ゲームパッド情報
	SNGamePadInfo GamePadInfo;

	// ボタン状態
	Boolean ButtonState[SNGamePadButtonNum];
};
