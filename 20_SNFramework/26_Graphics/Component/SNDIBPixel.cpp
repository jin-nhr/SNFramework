#include "SNDIBPixel.h"
#include "SNWindowsAPI.h"


// コンストラクタ
SNDIBPixel::SNDIBPixel(Handle lock)
{
    IWICBitmapLock* l = (IWICBitmapLock*)lock;
    UINT size = 0;
    BYTE* ptr = nullptr;
    UINT stride = 0;

    l->GetDataPointer(&size, &ptr);
    l->GetStride(&stride);

    BitmapLock = lock;
    PixelPtr = (SNColor*)ptr;
    DataSize = (Int32)size;
    Stride = (Int32)stride;

    return;
}

// デストラクタ
SNDIBPixel::~SNDIBPixel()
{
    if (BitmapLock != nullptr)
    {
        ((IWICBitmapLock*)BitmapLock)->Release();
        BitmapLock = nullptr;
    }

    return;
}

// ピクセル参照
SNColor* SNDIBPixel::Ref(Int32 x, Int32 y)
{
    SNColor* p = (SNColor*)(((BYTE*)PixelPtr) + (Stride * y) + (x * 4));
    return p;
}

// ストライド取得
Int32 SNDIBPixel::GetStride()
{
    return Stride;
}

// データサイズ取得
Int32 SNDIBPixel::GetDataSize()
{
    return DataSize;
}
