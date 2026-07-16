#include "SNDIB.h"
#include "SNImageCodec.h"
#include "SNWindowsAPI.h"
#include "SNGraphicsDevice.h"

// DIBクラス

// コンストラクタ
SNDIB::SNDIB()
{
    WICBitmap = nullptr;
    ImageSize = { 0 };
    TargetData = nullptr;
	return;
}

// デストラクタ
SNDIB::~SNDIB()
{
    DeleteDIB();

	return;
}

// DIB生成
Void SNDIB::CreateDIB(SNSize* size)
{
    IWICImagingFactory* factory = (IWICImagingFactory*)SNImageCodec::WICFactory;
    IWICBitmap* bmp;
    ID2D1RenderTarget* rt = nullptr;
    D2D1_RENDER_TARGET_PROPERTIES props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
            96.0f, 96.0f);

    DeleteDIB();

    factory->CreateBitmap(
        size->Width,
        size->Height,
        GUID_WICPixelFormat32bppBGRA,
        WICBitmapCacheOnLoad,
        &bmp);

    WICBitmap = bmp;
    ImageSize.Width = size->Width;
    ImageSize.Height = size->Height;

    return;
}


// DIB解放
Void SNDIB::DeleteDIB()
{
    if (WICBitmap)
    {
        ((IWICBitmap*)WICBitmap)->Release();
        WICBitmap = nullptr;
    }

    ImageSize.Width = 0;
    ImageSize.Height = 0;

    return;
}

// WICBitmap取得
Handle SNDIB::GetBitmap()
{
    return WICBitmap;
}

// ピクセル取得
SNDIBPixel* SNDIB::GetPixel()
{
    IWICBitmap* bmp = (IWICBitmap*)WICBitmap;
    IWICBitmapLock* lock;

    bmp->Lock(nullptr, WICBitmapLockWrite, &lock);

    return new SNDIBPixel(lock);
}

// ピクセル解放
Void SNDIB::ReleasePixel(SNDIBPixel* pixel)
{
    delete pixel;
}

// サイズ取得
Void SNDIB::GetSize(SNSize* size)
{
    *size = ImageSize;
}


// デコード(非同期実行)
// デコード対象データを渡す。呼び出し元で処理完了まで保持すること
Void SNDIB::Decode(SNMemory* in_data)
{
	TargetData = in_data;
	RequestOperation(SNImageCodecOperationDecode);
	return;
}

// エンコード(非同期実行)
// エンコード結果を受け取るメモリオブジェクトを指定する
Void SNDIB::Encode(SNMemory* out_data)
{
	TargetData = out_data;
	RequestOperation(SNImageCodecOperationEncode);
	return;
}

SNImageCodecResult SNDIB::OnOperationImageDecode()
{
	SNImageCodec::Decode(TargetData, this);
    return SNImageCodecResultNormal;
}

SNImageCodecResult SNDIB::OnOperationImageEncode()
{
	SNImageCodec::Encode(this, TargetData);
    return SNImageCodecResultNormal;
}
