#pragma once
#include "SNFrameworkInternal.h"
#include "SNBitmap.h"
#include "SNDIB.h"

class SNGraphicsDevice
{
public:

	////////////////////////////////////////////
	// 初期化
	static Void Initialize();

	// デバイス生成
	static Void CreateDevice();

	// SwapChain生成
	static Void CreateSwapChain();

	// RTV生成
	static Void CreateRTV();

	static Void CreateBitmap(SNBitmap* bmp, SNSize* size);

	// BitmapからDIBを生成する
	static Void CreateDIBFromBitmap(SNBitmap* src_bitmap, SNDIB* dst_dib);

	// DIBからBitmapを生成する
	static Void CreateBitmapFromDIB(SNDIB* src_dib, SNBitmap* dst_bitmap);

	// 画面サーフェス生成
	static Void CreateSurface();

	// SRV生成
	static Void CreateSRV();

	// クアッド頂点バッファ生成
	static Void CreateFullscreenQuad();

	// シェーダー生成
	static Void CreateShaders();

	// サンプラー生成
	static Void CreateSampler();

	// WIC生成
	static Void CreateWIC();

	// D3D描画初期化
	static Void CreateDrawImageD3D();

	////////////////////////////////////////////
	// 終了
	static Void Terminate();

	// デバイス解放
	static Void ReleaseDevice();
	
	// SwapChain解放
	static Void ReleaseSwapChain();

	// RTV解放
	static Void ReleaseRTV();

	// サーフェス解放
	static Void ReleaseSurface();
	// 
	// SRV解放
	static Void ReleaseSRV();

	// クアッド頂点バッファ解放
	static Void ReleaseFullscreenQuad();

	// シェーダー解放
	static Void ReleaseShaders();

	// サンプラー解放
	static Void ReleaseSampler();

	// WIC解放
	static Void ReleaseWIC();

	// D3D描画解放
	static Void ReleaseDrawImageD3D();

	////////////////////////////////////////////
	// その他

	static Void Restore(SNSize* size);

	// ステートクリア
	static Void ClearState();

	// フリップ処理
	static Void Flip(SNRect* rect);

	// フルスクリーン設定
	static Void SetFullScreen(Boolean fs);

	// リサイズ
	static Void ResizeBuffer(SNSize* size);

	// 画面サイズ取得
	static Void GetWindowSize(SNSize* size);


	////////////////////////////////////////////
	// 描画

	static Void Begin(SNBitmap* target);

	static Void DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect);
	static Void DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, SNColor* color);
	static Void DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha);
	static Void DrawImage(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha, SNColor* color);

	static Void DrawImageImp(SNRect* dst_rect, SNBitmap* src, SNRect* src_rect, UInt8 alpha, SNColor* color);


	static Void End();

	static Void FlushD3DDrawCommand();

public:

	struct SNTileVertex
	{
		Float32 PosX;   // 画面上の X（後で拡大される）
		Float32 PosY;   // 画面上の Y（後で拡大される）
		Float32 PosZ;   // 2Dなので常に 0

		Float32 U;      // テクスチャの U（0〜1）
		Float32 V;      // テクスチャの V（0〜1）
	};

	struct SNTile
	{
		Float32 DstX;
		Float32 DstY;
		Float32 DstW;
		Float32 DstH;

		Float32 SrcX;
		Float32 SrcY;
		Float32 SrcW;
		Float32 SrcH;

		Float32 Alpha;
		Float32 ScreenWidth;
		Float32 ScreenHeight;
		Float32 reserve;

		Float32 MulR;
		Float32 MulG;
		Float32 MulB;
		Float32 MulA;
	};

	static constexpr Int64 D3DDrawCommandMax = 32767;

	static Handle Device;
	static Handle DeviceContext;
	static Handle SwapChain;
	static Handle RenderTargetView;
	static Handle ShaderResourceView;
	static SNBitmap ScreenSurface;

	// スクリーンサーフェス
	static Handle VertexBuffer;
	static Handle InputLayout;
	static Handle VertexShader;
	static Handle PixelShader;
	static Handle SamplerState;

	// D3D描画
	static Handle WorldVertexBuffer;
	static Handle WorldIndexBuffer;
	static Handle WorldConstantBuffer;
	static Handle WorldInstanceBuffer;
	static Handle WorldInstanceSRV;
	static Handle WorldVSShader;
	static Handle WorldPSShader;
	static Handle WorldInputLayout;
	static Handle WorldSampler;
	static Handle WorldBlendState;
	static SNBitmap* D3DSourceBitmap;
	static SNSize D3DTargetSize;
	static SNTile D3DDrawCommand[D3DDrawCommandMax];
	static UInt32 D3DDrawCommandNum;
};
