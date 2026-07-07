#include "SND2D.h"
#include "SNWindowsAPI.h"
#include "SND3D.h"

Handle SND2D::D2DFactory = nullptr;
Handle SND2D::D2DRenderTarget = nullptr;
Handle SND2D::D2DBrush = nullptr;
Handle SND2D::DWriteFactory = nullptr;
Handle SND2D::DWriteTextFormat = nullptr;
Handle SND2D::WICFactory = nullptr;

// ‰Šú‰»
Void SND2D::Initialize()
{
    HRESULT hr;

    // 1. D2D Factory
    ID2D1Factory* factory = nullptr;
    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_MULTI_THREADED,
        &factory
    );
    if (FAILED(hr)) return;
    D2DFactory = factory;

    // 2. DXGI SurfaceiSND3D ‚ªì‚Á‚½‚à‚Ìj
    IDXGISurface1* dxgiSurface = nullptr;
    ((ID3D11Texture2D*)SND3D::Surface)->QueryInterface(&dxgiSurface);
    if (dxgiSurface == nullptr) return;

    // 3. D2D RenderTarget ì¬
    D2D1_RENDER_TARGET_PROPERTIES props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                D2D1_ALPHA_MODE_PREMULTIPLIED
            ),
            96.0f,
            96.0f
        );

    ID2D1RenderTarget* rt = nullptr;
    hr = ((ID2D1Factory*)D2DFactory)->CreateDxgiSurfaceRenderTarget(
        dxgiSurface,
        &props,
        &rt
    );
    dxgiSurface->Release();

    if (FAILED(hr)) return;
    D2DRenderTarget = rt;

    // 4. ƒuƒ‰ƒVì¬
    ID2D1SolidColorBrush* brush = nullptr;
    hr = ((ID2D1RenderTarget*)D2DRenderTarget)->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        &brush
    );
    if (FAILED(hr)) return;
    D2DBrush = brush;

    // 5. DirectWritei”CˆÓj
    IDWriteFactory* dwf = nullptr;
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        (IUnknown**)&dwf
    );
    if (SUCCEEDED(hr))
    {
        DWriteFactory = dwf;

        IDWriteTextFormat* tf = nullptr;
        dwf->CreateTextFormat(
            L"Meiryo",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            24.0f,
            L"ja-jp",
            &tf
        );
        DWriteTextFormat = tf;
    }

    // 6. WICi”CˆÓj
    IWICImagingFactory* wic = nullptr;
    CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&wic)
    );
    WICFactory = wic;
    return;
}

// I—¹ˆ—
Void SND2D::Terminate()
{
    if (D2DBrush)
    {
        ((ID2D1SolidColorBrush*)D2DBrush)->Release();
        D2DBrush = nullptr;
    }

    if (D2DRenderTarget)
    {
        ((ID2D1RenderTarget*)D2DRenderTarget)->Release();
        D2DRenderTarget = nullptr;
    }

    if (D2DFactory)
    {
        ((ID2D1Factory*)D2DFactory)->Release();
        D2DFactory = nullptr;
    }

    if (DWriteTextFormat)
    {
        ((IDWriteTextFormat*)DWriteTextFormat)->Release();
        DWriteTextFormat = nullptr;
    }

    if (DWriteFactory)
    {
        ((IDWriteFactory*)DWriteFactory)->Release();
        DWriteFactory = nullptr;
    }

    if (WICFactory)
    {
        ((IWICImagingFactory*)WICFactory)->Release();
        WICFactory = nullptr;
    }
    return;
}

Void SND2D::Draw()
{
    ID2D1RenderTarget* rt = (ID2D1RenderTarget*)D2DRenderTarget;
    ID2D1SolidColorBrush* brush = (ID2D1SolidColorBrush*)D2DBrush;

    if (rt == nullptr) return;

    rt->BeginDraw();

    // ”wŒiƒNƒŠƒA
 //   rt->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    // Ô‚¢lŠp
    brush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    rt->FillRectangle(
        D2D1::RectF(50, 50, 300, 300),
        brush
    );

    // •¶š•`‰æ
    if (DWriteTextFormat)
    {
        brush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
        rt->DrawText(
            L"Hello D2D + D3D11",
            20,
            (IDWriteTextFormat*)DWriteTextFormat,
            D2D1::RectF(50, 350, 600, 450),
            brush
        );
    }

    rt->EndDraw();
    return;
}

