#pragma once
#include "SNFrameworkInternal.h"


// イベントクラス
// 各ブロックのイベントを統合管理する
// 内部イベントについては前回のApplication実行結果をループバックする
class SNEvent
{
public:
	// 初期化
	static Void Initialize();

	// 終了処理
	static Void Terminate();

	// 更新処理
	static Void Update();


	static Boolean EventExist[SNEventTypeNum];		// イベント有無
	static Boolean EventResult[SNEventResultNum];	// イベント処理結果
	static Boolean InternalEvent[SNEventResultNum];	// 内部イベント
private:


};
