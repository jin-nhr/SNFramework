#pragma once
#include "SNFrameworkInternal.h"


class SND3D
{
public:

	////////////////////////////////////////////
	// 初期化
	// デバイス生成
	static Void CreateDevice();

	// SwapChain生成
	static Void CreateSwapChain();

	// RTV生成
	static Void CreateRTV();

	// サーフェス生成
	static Void CreateSurface();

	// SRV生成
	static Void CreateSRV();

	// クアッド頂点バッファ生成
	static Void CreateFullscreenQuad();

	// シェーダー生成
	static Void CreateShaders();

	// サンプラー生成
	static Void CreateSampler();

	////////////////////////////////////////////
	// 終了
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


	////////////////////////////////////////////
	// その他
	// DC取得
	// 使用後必ずReleaseDCすること
	static Handle GetDC();

	// DC解放
	static Void ReleaseDC();

	// フリップ処理
	static Void Flip(SNRect* rect);

	// フルスクリーン設定
	static Void SetFullScreen(Boolean fs);

	// リサイズ
	static Void ResizeBuffer(SNSize* size);

	// 画面サイズ取得
	static Void GetScreenRect(SNRect* rect);

public:

	static Handle Device;
	static Handle DeviceContext;
	static Handle SwapChain;
	static Handle RenderTargetView;
	static Handle Surface;
	static Handle GDISurface;
	static Handle ShaderResourceView;

	static Handle VertexBuffer;
	static Handle InputLayout;
	static Handle VertexShader;
	static Handle PixelShader;
	static Handle SamplerState;
};
