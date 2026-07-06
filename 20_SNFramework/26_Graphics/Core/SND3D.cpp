#include "SND3D.h"
#include "SNWindowsAPI.h"
#include "SNSystemConfig.h"
#include "SNUserConfig.h"
#include "SNWindow.h"
#include "SNCriticalSection.h"
#include "SNGraphics.h"
#include "SNGDI.h"
#include "SND3DShaderPS.h"
#include "SND3DShaderVS.h"
#include "SNApplication.h"

Handle SND3D::Device = nullptr;
Handle SND3D::DeviceContext = nullptr;
Handle SND3D::SwapChain = nullptr;
Handle SND3D::RenderTargetView = nullptr;
Handle SND3D::Surface = nullptr;
Handle SND3D::GDISurface = nullptr;
Handle SND3D::ShaderResourceView = nullptr;

Handle SND3D::VertexBuffer = nullptr;
Handle SND3D::InputLayout = nullptr;
Handle SND3D::VertexShader = nullptr;
Handle SND3D::PixelShader = nullptr;
Handle SND3D::SamplerState = nullptr;



// D3Dデバイス、コンテキスト生成
Void SND3D::CreateDevice()
{
    D3D_FEATURE_LEVEL fl;
    HRESULT hr;

    hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        (ID3D11Device**)&Device,
        &fl,
        (ID3D11DeviceContext**)&DeviceContext);

    return;
}

// スワップチェイン生成
Void SND3D::CreateSwapChain()
{
    IDXGIDevice* dxgi_dev = nullptr;
    IDXGIAdapter* adapter = nullptr;
    IDXGIFactory* factory = nullptr;
    DXGI_SWAP_CHAIN_DESC sd = {};
    HRESULT hr;

    ((ID3D11Device*)Device)->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_dev);
    dxgi_dev->GetAdapter(&adapter);
    adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory);

    sd.BufferCount = 2; // ダブルバッファ
    sd.BufferDesc.Width = SNSystemConfig::ScreenWidth;
    sd.BufferDesc.Height = SNSystemConfig::ScreenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)SNWindow::WindowHandle;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    hr = factory->CreateSwapChain((ID3D11Device*)Device, &sd, (IDXGISwapChain**)&SwapChain);

    factory->Release();
    adapter->Release();
    dxgi_dev->Release();

    return;
}

// RTV生成
Void SND3D::CreateRTV()
{
    ID3D11Texture2D* back_buf = nullptr;
    HRESULT hr;

    hr = ((IDXGISwapChain*)SwapChain)->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buf);
    if (!FAILED(hr))
    {
        hr = ((ID3D11Device*)Device)->CreateRenderTargetView(back_buf, nullptr, (ID3D11RenderTargetView**)&RenderTargetView);
        back_buf->Release();
    }

    return;
}

// サーフェス生成
Void SND3D::CreateSurface()
{
    HRESULT hr;

    // 共有サーフェス、SRV生成
    D3D11_TEXTURE2D_DESC td = {};

    td.Width = SNSystemConfig::ScreenWidth;
    td.Height = SNSystemConfig::ScreenHeight;
    td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.SampleDesc.Count = 1;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    td.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;

    hr = ((ID3D11Device*)Device)->CreateTexture2D(&td, nullptr, (ID3D11Texture2D**)&Surface);

    if (!FAILED(hr))
    {
        hr = ((ID3D11Texture2D*)Surface)->QueryInterface(__uuidof(IDXGISurface1), (void**)&GDISurface);
        if (!FAILED(hr))
        {
            hr = ((ID3D11Device*)Device)->CreateShaderResourceView((ID3D11Texture2D*)Surface, nullptr, (ID3D11ShaderResourceView**)&ShaderResourceView);
        }
    }

    return;
}

// SRV生成
Void SND3D::CreateSRV()
{
    HRESULT hr;

    if (Surface != nullptr)
    {
        hr = ((ID3D11Device*)Device)->CreateShaderResourceView((ID3D11Texture2D*)Surface, nullptr, (ID3D11ShaderResourceView**)&ShaderResourceView);
    }


    
    return;
}

// スクリーンクアッド生成
Void SND3D::CreateFullscreenQuad()
{
    struct VERTEX
    {
        float Pos[3];
        float Tex[2];
    };

    // 左上原点のテクスチャをそのまま貼る前提
    VERTEX vertices[4] =
    {
        // 左上
        { {-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f} },
        // 右上
        { { 1.0f,  1.0f, 0.0f}, {1.0f, 0.0f} },
        // 左下
        { {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} },
        // 右下
        { { 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} },
    };
    D3D11_BUFFER_DESC bd = {};
    D3D11_SUBRESOURCE_DATA init = {};
    ID3D11Device* dev = (ID3D11Device*)Device;
    ID3D11Buffer* vb = nullptr;
    HRESULT hr;

    if (dev != nullptr)
    {
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(vertices);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        init.pSysMem = vertices;
        hr = dev->CreateBuffer(&bd, &init, &vb);
        VertexBuffer = vb;
    }

    return;
}

// シェーダ生成
Void SND3D::CreateShaders()
{
    ID3D11Device* dev = (ID3D11Device*)Device;
    ID3D11VertexShader* vs = nullptr;
    ID3D11PixelShader* ps = nullptr;
    // 入力レイアウト
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12,
          D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    ID3D11InputLayout* il = nullptr;
    HRESULT hr;

    if (dev != nullptr)
    {
        hr = dev->CreateVertexShader(g_VS_FullscreenQuad, sizeof(g_VS_FullscreenQuad), nullptr, &vs);
        hr = dev->CreatePixelShader(g_PS_FullscreenQuad, sizeof(g_PS_FullscreenQuad), nullptr, &ps);
        VertexShader = vs;
        PixelShader = ps;

        hr = dev->CreateInputLayout(layout, _countof(layout),
            g_VS_FullscreenQuad,
            sizeof(g_VS_FullscreenQuad),
            &il);

        InputLayout = il;
    }
    return;
}

// サンプラ生成
Void SND3D::CreateSampler()
{
    ID3D11Device* dev = (ID3D11Device*)Device;
    D3D11_SAMPLER_DESC sd = {};
    ID3D11SamplerState* ss = nullptr;
    HRESULT hr;

    if (dev != nullptr)
    {
        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sd.MinLOD = 0;
        sd.MaxLOD = D3D11_FLOAT32_MAX;

        hr = dev->CreateSamplerState(&sd, &ss);
        SamplerState = ss;
    }
    return;
}

Void SND3D::ReleaseDevice()
{
    if (DeviceContext)
    {
        ((ID3D11DeviceContext*)DeviceContext)->Release();
        DeviceContext = nullptr;
    }
    if (Device)
    {
        ((ID3D11Device*)Device)->Release();
        Device = nullptr;
    }

    return;
}

Void SND3D::ReleaseSwapChain()
{
    if (SwapChain)
    {
        ((IDXGISwapChain*)SwapChain)->Release();
        SwapChain = nullptr;
    }
    return;
}

Void SND3D::ReleaseRTV()
{
    if (RenderTargetView)
    {
        ((ID3D11RenderTargetView*)RenderTargetView)->Release();
        RenderTargetView = nullptr;
    }
    return;
}

Void SND3D::ReleaseSurface()
{
    if (GDISurface)
    {
        ((IDXGISurface1*)GDISurface)->Release();
        GDISurface = nullptr;
    }

    if (Surface)
    {
        ((ID3D11Texture2D*)Surface)->Release();
        Surface = nullptr;
    }
    return;
}

Void SND3D::ReleaseSRV()
{
    if (ShaderResourceView)
    {
        ((ID3D11ShaderResourceView*)ShaderResourceView)->Release();
        ShaderResourceView = nullptr;
    }
    return;
}

Void SND3D::ReleaseFullscreenQuad()
{
    if (VertexBuffer)
    {
        ((ID3D11Buffer*)VertexBuffer)->Release();
        VertexBuffer = nullptr;
    }
    return;
}

Void SND3D::ReleaseShaders()
{
    if (InputLayout)
    {
        ((ID3D11InputLayout*)InputLayout)->Release();
        InputLayout = nullptr;
    }
    if (VertexShader)
    {
        ((ID3D11VertexShader*)VertexShader)->Release();
        VertexShader = nullptr;
    }

    if (PixelShader)
    {
        ((ID3D11PixelShader*)PixelShader)->Release();
        PixelShader = nullptr;
    }

    return;
}

Void SND3D::ReleaseSampler()
{
    if (SamplerState)
    {
        ((ID3D11SamplerState*)SamplerState)->Release();
        SamplerState = nullptr;
    }
    return;
}

// DC取得
Handle SND3D::GetDC()
{
    Handle ret = nullptr;
    HRESULT hr;
    HDC dc;

    if (GDISurface != nullptr)
    {
        hr = ((IDXGISurface1*)GDISurface)->GetDC(FALSE, &dc);
        if (SUCCEEDED(hr))
        {
            SNGDI gdi;

            gdi.InitDC((Handle)dc);
            ret = dc;
        }
    }

    return ret;
}

// DC解放
Void SND3D::ReleaseDC()
{
    ((IDXGISurface1*)GDISurface)->ReleaseDC(nullptr);

    return;
}

Void SND3D::Flip(SNRect* rect)
{
    ID3D11DeviceContext* ctx = (ID3D11DeviceContext*)DeviceContext;
    ID3D11RenderTargetView* rtv = (ID3D11RenderTargetView*)RenderTargetView;
    ID3D11Buffer* vb = (ID3D11Buffer*)VertexBuffer;
    ID3D11InputLayout* il = (ID3D11InputLayout*)InputLayout;
    ID3D11VertexShader* vs = (ID3D11VertexShader*)VertexShader;
    ID3D11PixelShader* ps = (ID3D11PixelShader*)PixelShader;
    ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)ShaderResourceView;
    ID3D11SamplerState* ss = (ID3D11SamplerState*)SamplerState;
    D3D11_VIEWPORT vp = {};

    vp.TopLeftX = (float)rect->PointX;
    vp.TopLeftY = (float)rect->PointY;
    vp.Width = (float)rect->Width;
    vp.Height = (float)rect->Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    // 0. Viewport設定
    ctx->RSSetViewports(1, &vp);

    // 1. RenderTarget をセット
    ctx->OMSetRenderTargets(1, &rtv, nullptr);

    // 2. バックバッファをクリア
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    ctx->ClearRenderTargetView(rtv, clearColor);

    // 3. フルスクリーンクアッド描画

    // 入力レイアウト
    ctx->IASetInputLayout(il);

    // トポロジ（四角形を三角形 2 枚で）
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // 頂点バッファ
    UINT stride = sizeof(float) * (3 + 2); // Pos(3) + Tex(2)
    UINT offset = 0;
    ctx->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

    // シェーダセット
    ctx->VSSetShader(vs, nullptr, 0);
    ctx->PSSetShader(ps, nullptr, 0);

    // SRV / サンプラセット
    ctx->PSSetShaderResources(0, 1, &srv);
    ctx->PSSetSamplers(0, 1, &ss);

    // 描画
    ctx->Draw(4, 0);

    // 4. Present（VSync ON）
    ((IDXGISwapChain*)SwapChain)->Present(1, 0);

    return;
}

Void SND3D::SetFullScreen(Boolean fs)
{
    IDXGISwapChain* sc = (IDXGISwapChain*)SwapChain;

    if (sc != nullptr)
    {
        sc->SetFullscreenState(fs, nullptr);
    }

    return;
}

Void SND3D::ResizeBuffer(SNSize* size)
{
    IDXGISwapChain* sc = (IDXGISwapChain*)SwapChain;

    if (sc != nullptr)
    {
        // バッファリサイズ（内部サーフェスは触らない）
        sc->ResizeBuffers(2, size->Width, size->Height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    }

    return;
}

Void SND3D::GetScreenRect(SNRect* rect)
{
    DXGI_SWAP_CHAIN_DESC desc;

    if (SwapChain != nullptr)
    {
        ((IDXGISwapChain*)SwapChain)->GetDesc(&desc);

        rect->PointX = 0;
        rect->PointY = 0;
        rect->Width = desc.BufferDesc.Width;
        rect->Height = desc.BufferDesc.Height;
    }
    else
    {
        rect->PointX = 0;
        rect->PointY = 0;
        rect->Width = 0;
        rect->Height = 0;
    }

    return;
}
