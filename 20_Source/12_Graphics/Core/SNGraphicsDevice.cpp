#include "SNGraphicsDevice.h"
#include "SNWindowsAPI.h"
#include "SNSystemConfig.h"
#include "SNUserConfig.h"
#include "SNWindow.h"
#include "SNGraphics.h"
#include "SND3DShaderPS.h"
#include "SND3DShaderVS.h"
#include "SND3DWorldShaderPS.h"
#include "SND3DWorldShaderVS.h"
#include "SNApplication.h"

Handle SNGraphicsDevice::Device = nullptr;
Handle SNGraphicsDevice::DeviceContext = nullptr;
Handle SNGraphicsDevice::SwapChain = nullptr;
Handle SNGraphicsDevice::RenderTargetView = nullptr;
Handle SNGraphicsDevice::ShaderResourceView = nullptr;

Handle SNGraphicsDevice::VertexBuffer = nullptr;
Handle SNGraphicsDevice::InputLayout = nullptr;
Handle SNGraphicsDevice::VertexShader = nullptr;
Handle SNGraphicsDevice::PixelShader = nullptr;
Handle SNGraphicsDevice::SamplerState = nullptr;

Handle SNGraphicsDevice::D2DFactory = nullptr;
Handle SNGraphicsDevice::D2DDevice = nullptr;
SNGraphicsContext SNGraphicsDevice::D2DGraphicsContext;
SNBitmap SNGraphicsDevice::D2DTargetBitmap;

Handle SNGraphicsDevice::TileVertexBuffer = nullptr;
Handle SNGraphicsDevice::TileIndexBuffer = nullptr;
Handle SNGraphicsDevice::TileConstantBuffer = nullptr;
Handle SNGraphicsDevice::WorldVSShader = nullptr;
Handle SNGraphicsDevice::WorldPSShader = nullptr;
Handle SNGraphicsDevice::WorldInputLayout = nullptr;
Handle SNGraphicsDevice::WorldSampler = nullptr;
Handle SNGraphicsDevice::WorldBlendState = nullptr;

Void SNGraphicsDevice::Initialize()
{
    // D2D, D3D初期化
    CreateDevice();
    CreateSwapChain();
    CreateRTV();

    CreateD2DFactory();
    CreateDeviceContext();

    CreateSurface();
    CreateSRV();
    CreateFullscreenQuad();
    CreateShaders();
    CreateSampler();

    CreateDrawImageD3D();

    return;
}


// D3Dデバイス、コンテキスト生成
Void SNGraphicsDevice::CreateDevice()
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
Void SNGraphicsDevice::CreateSwapChain()
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

    factory->MakeWindowAssociation((HWND)SNWindow::WindowHandle, DXGI_MWA_NO_ALT_ENTER);

    factory->Release();
    adapter->Release();
    dxgi_dev->Release();

    return;
}

// RTV生成
Void SNGraphicsDevice::CreateRTV()
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

Void SNGraphicsDevice::CreateD2DFactory()
{
    HRESULT hr;
    ID2D1Factory1* factory = nullptr;

    hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_MULTI_THREADED,
        __uuidof(ID2D1Factory1),
        (Void**)&factory
    );
    D2DFactory = factory;

    return;
}

Void SNGraphicsDevice::CreateDeviceContext()
{
    IDXGIDevice* dxgi_dev = nullptr;
    ID2D1Device* d2d_dev = nullptr;

    ((ID3D11Device*)Device)->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_dev);

    if (dxgi_dev != nullptr)
    {
        ((ID2D1Factory1*)D2DFactory)->CreateDevice(dxgi_dev, &d2d_dev);

        D2DDevice = d2d_dev;

        // D2DのDC生成
        D2DGraphicsContext.CreateDeviceContext();

        dxgi_dev->Release();
    }

    return;
}

// サーフェス生成
Void SNGraphicsDevice::CreateSurface()
{
    SNSize size;

    size.Width = SNSystemConfig::ScreenWidth;
    size.Height = SNSystemConfig::ScreenHeight;

    // ビットマップ生成
    D2DGraphicsContext.CreateBitmap(&D2DTargetBitmap, &size);

    return;
}

// SRV生成
Void SNGraphicsDevice::CreateSRV()
{
    ID3D11Texture2D* d3d_texture = (ID3D11Texture2D*)(D2DTargetBitmap.Get3DTexture());

    ((ID3D11Device*)Device)->CreateShaderResourceView(d3d_texture, nullptr, (ID3D11ShaderResourceView**)&ShaderResourceView);

    return;
}

// スクリーンクアッド生成
Void SNGraphicsDevice::CreateFullscreenQuad()
{
    // 左上原点のテクスチャをそのまま貼る前提
    SNTileVertex vertices[4] =
    {
        // 左上
        { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f },
        // 右上
        {  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
        // 左下
        { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
        // 右下
        {  1.0f, -1.0f, 0.0f, 1.0f, 1.0f },
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
Void SNGraphicsDevice::CreateShaders()
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
Void SNGraphicsDevice::CreateSampler()
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

Void SNGraphicsDevice::CreateDrawImageD3D()
{
    ID3D11Device* dev = (ID3D11Device*)Device;
    ID3D11Buffer* buf = nullptr;

    SNTileVertex quad[4] =
    {
        { 0.0f, 0.0f, 0.0f,   0.0f, 0.0f },   // 左上
        { 1.0f, 0.0f, 0.0f,   1.0f, 0.0f },   // 右上
        { 0.0f, 1.0f, 0.0f,   0.0f, 1.0f },   // 左下
        { 1.0f, 1.0f, 0.0f,   1.0f, 1.0f },   // 右下
    };

    // 4頂点 → 三角形2枚（6インデックス）
    UInt16 indices[6] = { 0, 1, 2, 2, 1, 3 };
    D3D11_BUFFER_DESC desc = {};
    D3D11_SUBRESOURCE_DATA init_data = {};
    ID3D11VertexShader* vs_shader = nullptr;
    ID3D11PixelShader* ps_shader = nullptr;
    ID3D11InputLayout* input_layout = nullptr;

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
          0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
          0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    D3D11_SAMPLER_DESC sd = {};
    ID3D11SamplerState* ss = nullptr;
    ID3D11BlendState* bs = nullptr;
    D3D11_BLEND_DESC bd = {};
    D3D11_RENDER_TARGET_BLEND_DESC* rt;

    HRESULT hr;

    // 頂点バッファ生成
    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth = sizeof(SNTileVertex) * 4;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    init_data.pSysMem = quad;

    hr = dev->CreateBuffer(&desc, &init_data, &buf);

    if (buf != nullptr)
    {
        TileVertexBuffer = buf;
        buf = nullptr;
    }

    // インデックスバッファ生成
    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth = sizeof(indices);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    init_data.pSysMem = indices;

    hr = dev->CreateBuffer(&desc, &init_data, &buf);

    if (buf != nullptr)
    {
        TileIndexBuffer = buf;
        buf = nullptr;
    }

    // コンスタントバッファ生成
    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth = sizeof(SNTileCB);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;

    hr = dev->CreateBuffer(&desc, nullptr, &buf);

    if (buf != nullptr)
    {
        TileConstantBuffer = buf;
    }


    hr = dev->CreateVertexShader(
        g_WorldShaderVS,
        sizeof(g_WorldShaderVS),
        nullptr,
        &vs_shader);
    if (vs_shader != nullptr)
    {
        WorldVSShader = vs_shader;
    }

    hr = dev->CreatePixelShader(
        g_WorldShaderPS,
        sizeof(g_WorldShaderPS),
        nullptr,
        &ps_shader);
    if (ps_shader != nullptr)
    {
        WorldPSShader = ps_shader;
    }

    hr = dev->CreateInputLayout(
        layout,
        2,
        g_WorldShaderVS,
        sizeof(g_WorldShaderVS),
        &input_layout);
    if (input_layout != nullptr)
    {
        WorldInputLayout = input_layout;
    }

    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sd.MinLOD = 0;
    sd.MaxLOD = D3D11_FLOAT32_MAX;

    hr = dev->CreateSamplerState(&sd, &ss);
    if (ss != nullptr)
    {
        WorldSampler = ss;
    }

    bd.AlphaToCoverageEnable = FALSE;
    bd.IndependentBlendEnable = FALSE;

    rt = &bd.RenderTarget[0];
    rt->BlendEnable = TRUE;

    // Premultiplied Alpha 用
    rt->SrcBlend = D3D11_BLEND_ONE;
    rt->DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rt->BlendOp = D3D11_BLEND_OP_ADD;

    // アルファはそのまま
    rt->SrcBlendAlpha = D3D11_BLEND_ONE;
    rt->DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    rt->BlendOpAlpha = D3D11_BLEND_OP_ADD;

    rt->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = dev->CreateBlendState(&bd, &bs);
    if (bs != nullptr)
    {
        WorldBlendState = bs;
    }

    return;
}


Void SNGraphicsDevice::Terminate()
{
    // D2D, D3D関連
    ReleaseDrawImageD3D();

    ReleaseSampler();
    ReleaseShaders();
    ReleaseFullscreenQuad();
    ReleaseSRV();
    ReleaseSurface();

    ReleaseDeviceContext();
    ReleaseFactory();

    ReleaseRTV();
    ReleaseSwapChain();
    ReleaseDevice();

    return;
}

Void SNGraphicsDevice::ReleaseDevice()
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

Void SNGraphicsDevice::ReleaseSwapChain()
{
    if (SwapChain)
    {
        ((IDXGISwapChain*)SwapChain)->Release();
        SwapChain = nullptr;
    }
    return;
}

Void SNGraphicsDevice::ReleaseRTV()
{
    if (RenderTargetView)
    {
        ((ID3D11RenderTargetView*)RenderTargetView)->Release();
        RenderTargetView = nullptr;
    }
    return;
}

Void SNGraphicsDevice::ReleaseFactory()
{
    if (D2DFactory != nullptr)
    {
        ((ID2D1Factory1*)D2DFactory)->Release();
        D2DFactory = nullptr;
    }

    return;
}

Void SNGraphicsDevice::ReleaseDeviceContext()
{
    D2DGraphicsContext.DeleteDeviceContext();

    if (D2DDevice != nullptr)
    {
        ((ID2D1Device*)D2DDevice)->Release();
    }

    return;
}

Void SNGraphicsDevice::ReleaseSurface()
{
    D2DTargetBitmap.DeleteBitmap();
    return;
}

Void SNGraphicsDevice::ReleaseSRV()
{
    if (ShaderResourceView)
    {
        ((ID3D11ShaderResourceView*)ShaderResourceView)->Release();
        ShaderResourceView = nullptr;
    }
    return;
}

Void SNGraphicsDevice::ReleaseFullscreenQuad()
{
    if (VertexBuffer)
    {
        ((ID3D11Buffer*)VertexBuffer)->Release();
        VertexBuffer = nullptr;
    }
    return;
}

Void SNGraphicsDevice::ReleaseShaders()
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

Void SNGraphicsDevice::ReleaseSampler()
{
    if (SamplerState)
    {
        ((ID3D11SamplerState*)SamplerState)->Release();
        SamplerState = nullptr;
    }
    return;
}

Void SNGraphicsDevice::ReleaseDrawImageD3D()
{
    ID3D11Buffer* vertex = (ID3D11Buffer*)TileVertexBuffer;
    ID3D11Buffer* index = (ID3D11Buffer*)TileIndexBuffer;
    ID3D11Buffer* constbuf = (ID3D11Buffer*)TileConstantBuffer;
    ID3D11VertexShader* vs = (ID3D11VertexShader*)WorldVSShader;
    ID3D11PixelShader* ps = (ID3D11PixelShader*)WorldPSShader;
    ID3D11SamplerState* ss = (ID3D11SamplerState*)WorldSampler;
    ID3D11BlendState* bs = (ID3D11BlendState*)WorldBlendState;

    if (WorldBlendState != nullptr)
    {
        bs->Release();
        WorldBlendState = nullptr;
    }

    if (WorldSampler != nullptr)
    {
        ss->Release();
        WorldSampler = nullptr;
    }

    if (WorldPSShader != nullptr)
    {
        ps->Release();
        WorldPSShader = nullptr;
    }

    if (WorldVSShader != nullptr)
    {
        vs->Release();
        WorldVSShader = nullptr;
    }

    if (TileConstantBuffer != nullptr)
    {
        constbuf->Release();
        TileConstantBuffer = nullptr;
    }

    if (TileIndexBuffer != nullptr)
    {
        index->Release();
        TileIndexBuffer = nullptr;
    }

    if (TileVertexBuffer != nullptr)
    {
        vertex->Release();
        TileVertexBuffer = nullptr;
    }

    return;
}


Void SNGraphicsDevice::Restore(SNSize* size)
{
    // RTVの破棄
    ReleaseRTV();

    // ステートクリア
    ClearState();

    // バックバッファ再構築
    ResizeBuffer(size);

    // RTVの再生成
    CreateRTV();

    return;
}

Void SNGraphicsDevice::ClearState()
{
    ID3D11DeviceContext* ctx = (ID3D11DeviceContext*)DeviceContext;
    ctx->ClearState();

    return;
}

Void SNGraphicsDevice::Flip(SNRect* rect)
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
    Int32 wait_vsync = 0;

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

    // 4. Present
    if (SNUserConfig::Data.VSync)
    {
        wait_vsync = 1;
    }

    ((IDXGISwapChain*)SwapChain)->Present(wait_vsync, 0);

    SNGraphicsContext::DrawImageCounter = (SNGraphicsContext::DrawImageCounterWork + SNGraphicsContext::DrawImageCounter) / 2;
    SNGraphicsContext::DrawImageCounterWork = 0;

    SNGraphicsContext::DrawPixelCounter = (SNGraphicsContext::DrawPixelCounterWork + SNGraphicsContext::DrawPixelCounter) / 2;
    SNGraphicsContext::DrawPixelCounterWork = 0;

    return;
}

Void SNGraphicsDevice::SetFullScreen(Boolean fs)
{
    IDXGISwapChain* sc = (IDXGISwapChain*)SwapChain;
    BOOL pre_sts;

    if (sc != nullptr)
    {
        // 現在の設定を取得
        sc->GetFullscreenState(&pre_sts, nullptr);

        // 変更必要なら設定
        if (pre_sts != fs)
        {
            sc->SetFullscreenState(fs, nullptr);
        }
    }

    return;
}

Void SNGraphicsDevice::ResizeBuffer(SNSize* size)
{
    IDXGISwapChain* sc = (IDXGISwapChain*)SwapChain;

    if (sc != nullptr)
    {
        // バッファリサイズ（内部サーフェスは触らない）
        sc->ResizeBuffers(2, size->Width, size->Height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
    }

    return;
}

Void SNGraphicsDevice::GetWindowSize(SNSize* size)
{
    RECT rect;

    // 状態によらずウインドウのサイズを取得する
    GetClientRect((HWND)SNWindow::WindowHandle, &rect);
    size->Width = rect.right - rect.left;
    size->Height = rect.bottom - rect.top;

    return;
}

Void SNGraphicsDevice::D3DBegin(Handle ctx, SNBitmap* target)
{
    ID3D11DeviceContext* d3d_ctx = (ID3D11DeviceContext*)ctx;
    ID3D11RenderTargetView* rtv = (ID3D11RenderTargetView*)target->GetRTV();
    Float32 color[4] = { 0, 0, 0, 0 };
    ID3D11InputLayout* input_layout = (ID3D11InputLayout*)SNGraphicsDevice::WorldInputLayout;
    ID3D11VertexShader* vs = (ID3D11VertexShader*)SNGraphicsDevice::WorldVSShader;
    ID3D11PixelShader* ps = (ID3D11PixelShader*)SNGraphicsDevice::WorldPSShader;
    ID3D11Buffer* vb = (ID3D11Buffer*)TileVertexBuffer;
    UINT stride = sizeof(SNTileVertex);
    UINT offset = 0;
    D3D11_VIEWPORT vp = {0};
    SNSize size;
    UINT sampleMask = 0xFFFFFFFF;
    float blendFactor[4] = { 0, 0, 0, 0 };

    target->GetSize(&size);

    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = (Float32)size.Width;
    vp.Height = (Float32)size.Height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    // ViewPport設定
    d3d_ctx->RSSetViewports(1, &vp);

    // RenderTarget設定
    d3d_ctx->OMSetRenderTargets(1, &rtv, nullptr);

    // バッファクリア
    d3d_ctx->ClearRenderTargetView(rtv, color);

    // 入力レイアウト
    d3d_ctx->IASetInputLayout(input_layout);

    // トポロジ
    d3d_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ブレンドステート
    d3d_ctx->OMSetBlendState((ID3D11BlendState*)WorldBlendState, blendFactor, sampleMask);

    // 頂点バッファ設定
    d3d_ctx->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

    // シェーダ設定
    d3d_ctx->VSSetShader(vs, nullptr, 0);
    d3d_ctx->PSSetShader(ps, nullptr, 0);

    return;
}

Void SNGraphicsDevice::DrawImageD3D(Handle ctx, SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha, SNSize* size)
{
    ID3D11DeviceContext* d3d_ctx = (ID3D11DeviceContext*)ctx;
    SNTileCB cb = {0};
    ID3D11Buffer* cbh = (ID3D11Buffer*)TileConstantBuffer;
    ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)src->GetSRV();
    D3D11_VIEWPORT vp;
    ID3D11SamplerState* ss = (ID3D11SamplerState*)WorldSampler;
    UINT vp_num = 1;

    d3d_ctx->RSGetViewports(&vp_num, &vp);

    // --- 定数バッファ更新（座標とUVとアルファ） ---
    cb.DstX = (Float32)dst_rect->PointX;
    cb.DstY = (Float32)dst_rect->PointY;
    cb.DstW = (Float32)dst_rect->Width;
    cb.DstH = (Float32)dst_rect->Height;

    cb.SrcX = (Float32)src_rect->PointX;
    cb.SrcY = (Float32)src_rect->PointY;
    cb.SrcW = (Float32)src_rect->Width;
    cb.SrcH = (Float32)src_rect->Height;

    cb.Alpha = (Float32)alpha / (Float32)SNAlphaMax;
    cb.ScreenWidth = vp.Width;
    cb.ScreenHeight = vp.Height;

    // 定数バッファ設定
    d3d_ctx->UpdateSubresource(cbh, 0, nullptr, &cb, 0, 0);
    d3d_ctx->VSSetConstantBuffers(0, 1, &cbh);
    d3d_ctx->PSSetConstantBuffers(0, 1, &cbh);

    // SRV設定
    d3d_ctx->PSSetShaderResources(0, 1, &srv);

    // サンプラ設定
    d3d_ctx->PSSetSamplers(0, 1, &ss);

    // インデックスバッファ設定
    d3d_ctx->IASetIndexBuffer((ID3D11Buffer*)TileIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // 描画
    d3d_ctx->DrawIndexed(6, 0, 0);

    return;
}

Void SNGraphicsDevice::D3DEnd(Handle ctx)
{
    return;
}


