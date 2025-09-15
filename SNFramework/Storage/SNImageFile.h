#pragma once
#include "../Include/SNFramework.h"
#include "SNStorageAccessor.h"
#include "../Graphics/SNDeviceContext.h"

// イメージファイルクラス
class SNImageFile : public SNStorageAccessor, SNDeviceContext
{
public:
	// コンストラクタ
	SNImageFile();

	// デストラクタ
	virtual ~SNImageFile();

	// イメージロード
	Boolean Load();

	// イメージ保存
	Boolean Save();

	// イメージメモリ確保
	Void AllocImage(Int32 w, Int32 h);

	// メモリ解放
	Void Free();

	// ハンドル取得
	Handle GetHandle();

	// サイズ取得
	SNSize GetSize();

	// コールバック
	virtual Void Callback();

private:
	Handle Image;		// イメージハンドル
	SNSize Size;		// イメージサイズ
};
