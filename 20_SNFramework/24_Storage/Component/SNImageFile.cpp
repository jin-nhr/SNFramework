#include "SNImageFile.h"
#include "SNStorage.h"
#include "SNWindowsAPI.h"
#include "SNWindow.h"


// ファイルクラス

// コンストラクタ
SNImageFile::SNImageFile()
{
	// 変数初期化
	Image = nullptr;
	Size = { 0 };

	// DC生成
	CreateDeviceContext();

	return;
}

// デストラクタ
SNImageFile::~SNImageFile()
{
	// 選択解除
	ReleaseBitmap();

	// DC破棄
	DeleteDeviceContext();

	Free();
	return;
}

// イメージロード
Boolean SNImageFile::Load()
{
	Boolean ret = true;

	// 非同期
	if (AsyncMode)
	{
		RequestOperation(SNStorageOperationReadFile);
	}
	else
	{
		// ビットマップ選択解除
		ReleaseBitmap();

		// ロード前にいったん解放
		Free();

		// 生成
		if (SNStorage::LoadImageFile(FullFolderFileName.GetString(), &Image, &Size))
		{
			// ビットマップ選択
			SetBitmap(Image);
			ret = true;
		}
	}

	return ret;
}

// イメージ保存
Boolean SNImageFile::Save()
{
	Boolean ret = true;

	// 非同期
	if (AsyncMode)
	{
		RequestOperation(SNStorageOperationWriteFile);
	}
	else
	{
		ret = SNStorage::SaveImageFile(FullFolderFileName.GetString(), Image);
	}

	return ret;
}

// サーフェス生成
Void SNImageFile::CreateSurface(Int32 width, Int32 height)
{
	// イメージメモリ確保とDC選択
	AllocImage(width, height);

	return;
}

// サーフェス破棄
Void SNImageFile::DeleteSurface()
{
	// DC選択解除とイメージメモリ解放
	Free();
}

// 幅取得
Int32 SNImageFile::GetWidth()
{
	return Size.Width;
}

// 高さ取得
Int32 SNImageFile::GetHeight()
{
	return Size.Height;
}


// イメージメモリ確保
Void SNImageFile::AllocImage(Int32 w, Int32 h)
{
	// ビットマップ選択解除
	ReleaseBitmap();

	// いったん解放
	Free();

	// ビットマップ生成
	Image = (Handle)CreateCompatibleBitmap((HDC)SNWindow::WindowDC,w, h);

	// サイズ設定
	Size.Width = w;
	Size.Height = h;

	// ビットマップ選択
	SetBitmap(Image);

	return;
}

// メモリ解放
Void SNImageFile::Free()
{
	if (Image != nullptr)
	{
		// ビットマップ選択解除
		ReleaseBitmap();

		DeleteObject((HGDIOBJ)Image);
		Image = nullptr;
	}

	return;
}

// ハンドル取得
Handle SNImageFile::GetHandle()
{
	return Image;
}

// サイズ取得
SNSize SNImageFile::GetSize()
{
	return Size;
}

// コールバック
Void SNImageFile::Callback()
{
	SNStorageResult ret = SNStorageResultIdle;

	// 処理中の場合
	if (Result == SNStorageResultProcessing)
	{
		switch (Operation)
		{
		case SNStorageOperationReadFile:
			// ビットマップ選択解除
			ReleaseBitmap();

			// ロード前にいったん解放
			Free();

			// ロードイメージ
			if (SNStorage::LoadImageFile(FullFolderFileName.GetString(), &Image, &Size))
			{
				// ビットマップ選択
				SetBitmap(Image);
				ret = SNStorageResultNormal;
			}
			else
			{
				ret = SNStorageResultError;
			}
			break;

		case SNStorageOperationWriteFile:
			// セーブイメージ
			if (SNStorage::SaveImageFile(FullFolderFileName.GetString(), Image))
			{
				ret = SNStorageResultNormal;
			}
			else
			{
				ret = SNStorageResultError;
			}
			break;

		default:
			break;
		}
	}

	// 処理結果をセット
	Result = ret;

	return;
}
