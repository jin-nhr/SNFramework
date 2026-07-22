#include "SNGraphicsContext.h"
#include "SNGraphicsDevice.h"
#include "SNBitmap.h"
#include "SNWindowsAPI.h"
#include "SNDIB.h"
#include "SNUserConfig.h"
#include "SNColorMatrix.h"

// コンストラクタ
SNGraphicsContext::SNGraphicsContext()
{
	DeviceContext = nullptr;
	return;
}

// デストラクタ
SNGraphicsContext::~SNGraphicsContext()
{
	DeleteDeviceContext();
	return;
}

// コンテキスト生成
Void SNGraphicsContext::CreateDeviceContext()
{
    ID2D1Device* device = (ID2D1Device*)SNGraphicsDevice::D2DDevice;
    ID2D1DeviceContext* dc = nullptr;

    // 事前にDC破棄
    DeleteDeviceContext();

    // DC生成
    device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &dc);

    DeviceContext = dc;
    return;
}

// コンテキスト削除
Void SNGraphicsContext::DeleteDeviceContext()
{
    if (DeviceContext != nullptr)
    {
        ((ID2D1DeviceContext*)DeviceContext)->Release();
        DeviceContext = nullptr;
    }

    return;
}

// 描画開始
Void SNGraphicsContext::Begin(SNBitmap* bmp)
{
    ID2D1DeviceContext* dc = (ID2D1DeviceContext*)GetDC();
    ID2D1Bitmap1* d2dbitmap = (ID2D1Bitmap1*)bmp->GetTargetBitmap();

    if (dc != nullptr)
    {
        dc->SetTarget(d2dbitmap);
        dc->BeginDraw();
    }

    return;
}

// 描画終了
Void SNGraphicsContext::End()
{
    ID2D1DeviceContext* dc = (ID2D1DeviceContext*)GetDC();
    dc->EndDraw();

    return;
}

// DC取得
Handle SNGraphicsContext::GetDC()
{
    return DeviceContext;
}

// ビットマップ生成
Void SNGraphicsContext::CreateBitmap(SNBitmap* bmp, SNSize* size)
{
    // 共有サーフェス生成
    D3D11_TEXTURE2D_DESC td = {};
    ID3D11Texture2D* d3d_texture = nullptr;
    IDXGISurface* dxgi_surface = nullptr;
    ID2D1Bitmap1* d2d_target = nullptr;
    ID2D1Bitmap1* d2d_source = nullptr;
    ID2D1DeviceContext* d2d_dc = (ID2D1DeviceContext*)DeviceContext;

    D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
        96.0f, 96.0f);

    // 事前に削除
    bmp->DeleteBitmap();

    td.Width = size->Width;
    td.Height = size->Height;
    td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    td.MiscFlags = 0;

    ((ID3D11Device*)SNGraphicsDevice::Device)->CreateTexture2D(&td, nullptr, &d3d_texture);

    if (d3d_texture != nullptr)
    {
        d3d_texture->QueryInterface(&dxgi_surface);

        if (dxgi_surface != nullptr)
        {
            // ターゲット生成
            d2d_dc->CreateBitmapFromDxgiSurface(dxgi_surface, &props, &d2d_target);
            
            // ソース生成
            props.bitmapOptions = D2D1_BITMAP_OPTIONS_NONE;
            d2d_dc->CreateBitmapFromDxgiSurface(dxgi_surface, &props, &d2d_source);

            dxgi_surface->Release();

            // ビットマップ設定
            bmp->SetBitmap(d3d_texture, d2d_target, d2d_source);
        }
    }

    return;
}

// 矩形描画
Void SNGraphicsContext::DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha)
{
    ID2D1DeviceContext* dc = (ID2D1DeviceContext*)GetDC();
    ID2D1Bitmap1* bmp = (ID2D1Bitmap1*)src->GetSourceBitmap();
    D2D1_BITMAP_INTERPOLATION_MODE draw_mode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;

    D2D1_RECT_F dst_rc = D2D1::RectF(
        (FLOAT)dst_rect->PointX,
        (FLOAT)dst_rect->PointY,
        (FLOAT)(dst_rect->PointX + dst_rect->Width),
        (FLOAT)(dst_rect->PointY + dst_rect->Height)
    );

    D2D1_RECT_F src_rc = D2D1::RectF(
        (FLOAT)src_rect->PointX,
        (FLOAT)src_rect->PointY,
        (FLOAT)(src_rect->PointX + src_rect->Width),
        (FLOAT)(src_rect->PointY + src_rect->Height)
    );

    FLOAT a = alpha / 255.0f;

    if (SNUserConfig::Data.DrawFilter)
    {
        draw_mode = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
    }

    dc->DrawBitmap(
        bmp,
        dst_rc,
        a,
        draw_mode,
        &src_rc);

    return;
}

// クリア
Void SNGraphicsContext::Clear(SNColor* color)
{
    ID2D1DeviceContext* dc = (ID2D1DeviceContext*)GetDC();
    D2D1_COLOR_F col = D2D1::ColorF(
        color->Red / 255.0f,
        color->Green / 255.0f,
        color->Blue / 255.0f,
        color->Alpha / 255.0f);

    dc->Clear(&col);

    return;
}

// カラーマトリクス変換
Void SNGraphicsContext::ColorMatrixEffect(SNPoint* dst_pnt, SNRect* src_rect, SNColorMatrix* color_matrix)
{
    ID2D1DeviceContext* dc = (ID2D1DeviceContext*)SNGraphicsDevice::D2DGraphicsContext.DeviceContext;
    ID2D1Effect* effect = (ID2D1Effect*)color_matrix->ColorMatrix;

    D2D1_POINT_2F pnt =
    {
        (FLOAT)dst_pnt->X,
        (FLOAT)dst_pnt->Y
    };

    D2D1_RECT_F rect =
    {
        (FLOAT)src_rect->PointX,
        (FLOAT)src_rect->PointY,
        (FLOAT)(src_rect->PointX + src_rect->Width),
        (FLOAT)(src_rect->PointY + src_rect->Height),
    };

    dc->DrawImage(effect, &pnt, &rect);

    return;
}

Void SNGraphicsContext::CreateDIBFromBitmap(SNBitmap* src_bitmap, SNDIB* dst_dib)
{
    ID3D11Device* dev = (ID3D11Device*)SNGraphicsDevice::Device;
    ID3D11DeviceContext* ctx = (ID3D11DeviceContext*)SNGraphicsDevice::DeviceContext;
    ID3D11Texture2D* d3d_texture = (ID3D11Texture2D*)(src_bitmap->Get3DTexture());
    ID3D11Texture2D* staging = nullptr;
    D3D11_TEXTURE2D_DESC staging_desc;
    D3D11_MAPPED_SUBRESOURCE mapped = {};
    BYTE* gpu_pixels;
    UINT gpu_pitch;
    BYTE* wic_pixels = nullptr;
    UINT wic_pitch = 0;
    INT y;
    SNSize snsize;
    SNDIBPixel* dst_dib_pix;

    // 1. Surface の情報取得
    D3D11_TEXTURE2D_DESC desc = {};
    d3d_texture->GetDesc(&desc);

    snsize.Width = desc.Width;
    snsize.Height = desc.Height;

    // 2. CPU 読み込み用の staging texture を作成
    staging_desc = desc;
    staging_desc.Usage = D3D11_USAGE_STAGING;
    staging_desc.BindFlags = 0;
    staging_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    staging_desc.MiscFlags = 0;

    dev->CreateTexture2D(&staging_desc, nullptr, &staging);

    // 3. Surface → staging へコピー
    ctx->CopyResource(staging, d3d_texture);

    // 4. staging を Map して CPU からピクセル取得
    ctx->Map(staging, 0, D3D11_MAP_READ, 0, &mapped);

    gpu_pixels = (BYTE*)mapped.pData;
    gpu_pitch = mapped.RowPitch;

    // 5. WICBitmap を作成（BGRA32）
    dst_dib->CreateDIB(&snsize);


    // 6. WICBitmap を Lock して書き込み
    dst_dib_pix = dst_dib->GetPixel();

    wic_pixels = (BYTE*)dst_dib_pix->Ref(0, 0);
    wic_pitch = dst_dib_pix->GetStride();

    // 7. 行ごとに memcpy（BGRA32 前提）
    for (y = 0; y < snsize.Height; ++y)
    {
        memcpy(
            wic_pixels + y * wic_pitch,
            gpu_pixels + y * gpu_pitch,
            (size_t)(snsize.Width * 4));
    }

    // 8. 後片付け
    dst_dib->ReleasePixel(dst_dib_pix);
    ctx->Unmap(staging, 0);
    staging->Release();

    return;
}

Void SNGraphicsContext::CreateBitmapFromDIB(SNDIB* src_dib, SNBitmap* dst_bitmap)
{
    ID3D11DeviceContext* ctx = (ID3D11DeviceContext*)SNGraphicsDevice::DeviceContext;
    ID3D11Texture2D* d3d_tex = nullptr;
    SNDIBPixel* src_dib_pix;
    BYTE* wic_pixels = nullptr;
    UINT wic_pitch = 0;
    SNSize snsize;

    src_dib->GetSize(&snsize);

    // 転送先ビットマップを生成
    CreateBitmap(dst_bitmap, &snsize);
    d3d_tex = (ID3D11Texture2D*)(dst_bitmap->Get3DTexture());

    // 2. WICBitmap を Lock して CPU ピクセル取得
    src_dib_pix = src_dib->GetPixel();
    wic_pixels = (BYTE*)src_dib_pix->Ref(0, 0);
    wic_pitch = src_dib_pix->GetStride();

    // 3. Surface に書き込む（GPU にアップロード）
    ctx->UpdateSubresource(
        d3d_tex,
        0,
        nullptr,
        wic_pixels,
        wic_pitch,
        0
    );

    src_dib->ReleasePixel(src_dib_pix);

    return;
}

// カラーマトリクス生成
Void SNGraphicsContext::CreateColorMatrix(SNColorMatrix* color_matrix)
{
    ID2D1DeviceContext* dc = (ID2D1DeviceContext*)GetDC();
    ID2D1Effect* effect = nullptr;

    color_matrix->DeleteColorMatrix();

    dc->CreateEffect(CLSID_D2D1ColorMatrix, &effect);

    // エフェクトオブジェクトをセット
    color_matrix->SetColorMatrix(effect);

    return;
}
