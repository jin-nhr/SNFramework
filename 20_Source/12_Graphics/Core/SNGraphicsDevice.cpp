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
SNBitmap SNGraphicsDevice::ScreenSurface;

Handle SNGraphicsDevice::VertexBuffer = nullptr;
Handle SNGraphicsDevice::InputLayout = nullptr;
Handle SNGraphicsDevice::VertexShader = nullptr;
Handle SNGraphicsDevice::PixelShader = nullptr;
Handle SNGraphicsDevice::SamplerState = nullptr;

Handle SNGraphicsDevice::WorldVertexBuffer = nullptr;
Handle SNGraphicsDevice::WorldIndexBuffer = nullptr;
Handle SNGraphicsDevice::WorldConstantBuffer = nullptr;
Handle SNGraphicsDevice::WorldInstanceBuffer = nullptr;
Handle SNGraphicsDevice::WorldInstanceSRV = nullptr;
Handle SNGraphicsDevice::WorldVSShader = nullptr;
Handle SNGraphicsDevice::WorldPSShader = nullptr;
Handle SNGraphicsDevice::WorldInputLayout = nullptr;
Handle SNGraphicsDevice::WorldSampler = nullptr;
Handle SNGraphicsDevice::WorldBlendState = nullptr;

SNBitmap* SNGraphicsDevice::D3DSourceBitmap = nullptr;
SNSize SNGraphicsDevice::D3DTargetSize;

SNGraphicsDevice::SNTile SNGraphicsDevice::D3DDrawCommand[D3DDrawCommandMax];
UInt32 SNGraphicsDevice::D3DDrawCommandNum = 0;

Void SNGraphicsDevice::Initialize()
{
    // D2D, D3D初期化
    CreateDevice();
    CreateSwapChain();
    CreateRTV();

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

// ビットマップ生成
Void SNGraphicsDevice::CreateBitmap(SNBitmap* bmp, SNSize* size)
{
    // 共有サーフェス生成
    D3D11_TEXTURE2D_DESC td = {};
    ID3D11Texture2D* d3d_texture = nullptr;
    ID3D11ShaderResourceView* d3d_srv = nullptr;
    ID3D11RenderTargetView* d3d_rtv = nullptr;

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
        ((ID3D11Device*)SNGraphicsDevice::Device)->CreateShaderResourceView(d3d_texture, nullptr, &d3d_srv);

        if (d3d_srv != nullptr)
        {
            ((ID3D11Device*)SNGraphicsDevice::Device)->CreateRenderTargetView(d3d_texture, nullptr, &d3d_rtv);

            if (d3d_rtv != nullptr)
            {
                // ビットマップ設定
                bmp->SetBitmap(d3d_texture, d3d_srv, d3d_rtv);
            }
            else
            {
                d3d_srv->Release();
                d3d_texture->Release();
            }
        }
        else
        {
            d3d_texture->Release();
        }
    }

    return;
}


Void SNGraphicsDevice::CreateDIBFromBitmap(SNBitmap* src_bitmap, SNDIB* dst_dib)
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

Void SNGraphicsDevice::CreateBitmapFromDIB(SNDIB* src_dib, SNBitmap* dst_bitmap)
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


// サーフェス生成
Void SNGraphicsDevice::CreateSurface()
{
    SNSize size;

    size.Width = SNSystemConfig::ScreenWidth;
    size.Height = SNSystemConfig::ScreenHeight;

    // ビットマップ生成
    CreateBitmap(&ScreenSurface, &size);

    return;
}

// SRV生成
Void SNGraphicsDevice::CreateSRV()
{
    ID3D11Texture2D* d3d_texture = (ID3D11Texture2D*)(ScreenSurface.Get3DTexture());

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

    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    ID3D11ShaderResourceView* srv = nullptr;

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
        WorldVertexBuffer = buf;
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
        WorldIndexBuffer = buf;
        buf = nullptr;
    }

    // コンスタントバッファ生成
    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth = sizeof(SNTile);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;

    hr = dev->CreateBuffer(&desc, nullptr, &buf);

    if (buf != nullptr)
    {
        WorldConstantBuffer = buf;
    }

    // インスタンスデータバッファ
    ZeroMemory(&desc, sizeof(desc));
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.ByteWidth = sizeof(SNTile) * D3DDrawCommandMax;
    desc.StructureByteStride = sizeof(SNTile);
    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = dev->CreateBuffer(&desc, nullptr, &buf);
    if (buf != nullptr)
    {
        WorldInstanceBuffer = buf;

        // インスタンスバッファのSRV作成
        srv_desc.Format = DXGI_FORMAT_UNKNOWN;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srv_desc.Buffer.FirstElement = 0;
        srv_desc.Buffer.NumElements = D3DDrawCommandMax;

        hr = dev->CreateShaderResourceView(buf, &srv_desc, &srv);

        if (srv != nullptr)
        {
            WorldInstanceSRV = srv;
        }
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

Void SNGraphicsDevice::ReleaseSurface()
{
    ScreenSurface.DeleteBitmap();

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
    ID3D11Buffer* vertex = (ID3D11Buffer*)WorldVertexBuffer;
    ID3D11Buffer* index = (ID3D11Buffer*)WorldIndexBuffer;
    ID3D11Buffer* constbuf = (ID3D11Buffer*)WorldConstantBuffer;
    ID3D11Buffer* instbuf = (ID3D11Buffer*)WorldInstanceBuffer;
    ID3D11VertexShader* vs = (ID3D11VertexShader*)WorldVSShader;
    ID3D11PixelShader* ps = (ID3D11PixelShader*)WorldPSShader;
    ID3D11SamplerState* ss = (ID3D11SamplerState*)WorldSampler;
    ID3D11BlendState* bs = (ID3D11BlendState*)WorldBlendState;
    ID3D11ShaderResourceView* srv = (ID3D11ShaderResourceView*)WorldInstanceSRV;

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

    if (WorldInstanceSRV != nullptr)
    {
        srv->Release();
        WorldInstanceSRV = nullptr;
    }

    if (WorldInstanceBuffer != nullptr)
    {
        instbuf->Release();
        WorldInstanceBuffer = nullptr;
    }

    if (WorldConstantBuffer != nullptr)
    {
        constbuf->Release();
        WorldConstantBuffer = nullptr;
    }

    if (WorldIndexBuffer != nullptr)
    {
        index->Release();
        WorldIndexBuffer = nullptr;
    }

    if (WorldVertexBuffer != nullptr)
    {
        vertex->Release();
        WorldVertexBuffer = nullptr;
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

Void SNGraphicsDevice::Begin(SNBitmap* target)
{
    ID3D11DeviceContext* d3d_ctx = (ID3D11DeviceContext*)DeviceContext;
    ID3D11RenderTargetView* rtv = (ID3D11RenderTargetView*)target->GetRTV();
    Float32 color[4] = { 0, 0, 0, 0 };
    ID3D11InputLayout* input_layout = (ID3D11InputLayout*)SNGraphicsDevice::WorldInputLayout;
    ID3D11VertexShader* vs = (ID3D11VertexShader*)SNGraphicsDevice::WorldVSShader;
    ID3D11PixelShader* ps = (ID3D11PixelShader*)SNGraphicsDevice::WorldPSShader;
    ID3D11Buffer* vb = (ID3D11Buffer*)WorldVertexBuffer;
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

    target->GetSize(&D3DTargetSize);

    D3DSourceBitmap = nullptr;

    D3DDrawCommandNum = 0;

    return;
}


Void SNGraphicsDevice::DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect)
{
    SNColor color = { 255, 255, 255, 255 };
    DrawImageImp(dst_rect, src, src_rect, SNAlphaMax, &color);
    return;
}
Void SNGraphicsDevice::DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, SNColor* color)
{
    DrawImageImp(dst_rect, src, src_rect, SNAlphaMax, color);
    return;
}
Void SNGraphicsDevice::DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha)
{
    SNColor color = { 255, 255, 255, 255 };
    DrawImageImp(dst_rect, src, src_rect, alpha, &color);
    return;
}
Void SNGraphicsDevice::DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha, SNColor* color)
{
    DrawImageImp(dst_rect, src, src_rect, alpha, color);
    return;
}

Void SNGraphicsDevice::DrawImageImp(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha, SNColor* color)
{
    SNTile* tile;

    // ソースが変わる場合は描画する
    if (D3DSourceBitmap != src)
    {
        FlushD3DDrawCommand();

        D3DSourceBitmap = src;
    }

    tile = &D3DDrawCommand[D3DDrawCommandNum];

    // 描画コマンドを追加設定する
    tile->DstX = (Float32)dst_rect->PointX;
    tile->DstY = (Float32)dst_rect->PointY;
    tile->DstW = (Float32)dst_rect->Width;
    tile->DstH = (Float32)dst_rect->Height;

    tile->SrcX = (Float32)src_rect->PointX;
    tile->SrcY = (Float32)src_rect->PointY;
    tile->SrcW = (Float32)src_rect->Width;
    tile->SrcH = (Float32)src_rect->Height;

    tile->Alpha = (Float32)alpha / (Float32)SNAlphaMax;
    tile->ScreenWidth = (Float32)D3DTargetSize.Width;
    tile->ScreenHeight = (Float32)D3DTargetSize.Height;

    tile->MulR = color->Red / 255.0f;
    tile->MulG = color->Green / 255.0f;
    tile->MulB = color->Blue / 255.0f;
    tile->MulA = color->Alpha / 255.0f;

    D3DDrawCommandNum++;

    return;
}

Void SNGraphicsDevice::End()
{
    // 残っている描画コマンドを処理
    FlushD3DDrawCommand();

    D3DSourceBitmap = nullptr;

    return;
}


Void SNGraphicsDevice::FlushD3DDrawCommand()
{
    ID3D11DeviceContext* d3d_ctx = (ID3D11DeviceContext*)DeviceContext;
    ID3D11Buffer* cbh = (ID3D11Buffer*)WorldConstantBuffer;
    ID3D11ShaderResourceView* srv = nullptr;
    ID3D11SamplerState* ss = (ID3D11SamplerState*)WorldSampler;
    ID3D11Buffer* inst_buf = (ID3D11Buffer*)WorldInstanceBuffer;
    ID3D11ShaderResourceView* inst_srv = (ID3D11ShaderResourceView*)WorldInstanceSRV;
    D3D11_MAPPED_SUBRESOURCE mapped;

    if (D3DDrawCommandNum != 0)
    {
        srv = (ID3D11ShaderResourceView*)D3DSourceBitmap->GetSRV();

        // SRV設定
        d3d_ctx->PSSetShaderResources(0, 1, &srv);

        // サンプラ設定
        d3d_ctx->PSSetSamplers(0, 1, &ss);

        // インデックスバッファ設定
        d3d_ctx->IASetIndexBuffer((ID3D11Buffer*)WorldIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // GPUへインスタンスデータを転送
        d3d_ctx->Map(inst_buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        memcpy(mapped.pData, D3DDrawCommand, sizeof(SNTile) * D3DDrawCommandNum);
        d3d_ctx->Unmap(inst_buf, 0);

        // インスタンスデータのSRVをセット
        d3d_ctx->VSSetShaderResources(1, 1, &inst_srv);

        // 描画の実行
        d3d_ctx->DrawIndexedInstanced(6, D3DDrawCommandNum, 0, 0, 0);

        D3DDrawCommandNum = 0;
    }

    return;
}
