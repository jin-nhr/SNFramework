#pragma once
#include "SNFrameworkInternal.h"
#include "SNStorageAccessor.h"
#include "SNSurface.h"
#include "SNDeviceContext.h"

// イメージファイルクラス
// GDIを使ってファイルのロードを行うのでフレーム処理に影響が出る可能性があり注意が必要
class SNImageFile : public SNStorageAccessor, public SNSurface
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

	// サーフェス生成
	Void CreateSurface(Int32 width, Int32 height);

	// サーフェス破棄
	Void DeleteSurface();

	// 幅取得
	Int32 GetWidth();

	// 高さ取得
	Int32 GetHeight();

	// ハンドル取得
	Handle GetHandle();

	// サイズ取得
	SNSize GetSize();

	// コールバック
	virtual Void Callback();

private:
	// イメージメモリ確保
	Void AllocImage(Int32 w, Int32 h);

	// メモリ解放
	Void Free();

	Handle Image;		// イメージハンドル
	SNSize Size;		// イメージサイズ
};
