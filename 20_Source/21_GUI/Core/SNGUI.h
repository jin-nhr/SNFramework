#pragma once
#include "SNFrameworkInternal.h"
#include "SNSystemConfig.h"
#include "SNGraphicsContext.h"
#include "SNBitmap.h"
#include "SNBitmapFont.h"
#include "SNGraphicsResource.h"

// GUIクラス
class SNGUI
{
public:
	static Void Tiling(SNGraphicsContext* grc, SNRect* dst_rect, SNBitmap* src_bmp, SNRect* src_rect);
	static Void Tiling9(SNGraphicsContext* grc, SNRect* dst_rect, SNGraphicsResID res_id, const SNPoint* blockdef, const SNPoint* offset);


public:
	// 初期化処理
	static Void Initialize();

	// 起動準備
	static Void Startup();

	// 実行
	static Void Run();

	// 終了前処理
	static Void BeforeTerminate();

	// 終了
	static Void Terminate();

};
