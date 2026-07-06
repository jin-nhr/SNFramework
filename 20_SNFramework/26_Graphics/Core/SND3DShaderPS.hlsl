// 頂点入力
struct VS_INPUT
{
    float3 Pos  : POSITION;
    float2 Tex  : TEXCOORD0;
};

// 頂点出力 / ピクセル入力
struct VS_OUTPUT
{
    float4 Pos  : SV_POSITION;
    float2 Tex  : TEXCOORD0;
};

// テクスチャ / サンプラ
Texture2D g_GdiTex : register(t0);
SamplerState g_Sampler : register(s0);

// ピクセルシェーダ：GDI テクスチャをそのまま出力
float4 PS_FullscreenQuad(VS_OUTPUT input) : SV_TARGET
{
    return g_GdiTex.Sample(g_Sampler, input.Tex);
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return PS_FullscreenQuad(input);
}
