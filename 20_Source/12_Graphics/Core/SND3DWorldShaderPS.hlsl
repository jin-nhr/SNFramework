Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer TileCB : register(b0)
{
    float DstX;
    float DstY;
    float DstW;
    float DstH;

    float SrcX;
    float SrcY;
    float SrcW;
    float SrcH;

    float Alpha;
    float ScreenWidth;
    float ScreenHeight;
    float reserve;
    
    float reserve2[4];
};

struct VS_INPUT
{
    float3 Pos : POSITION; // 0〜1 の正規化矩形
    float2 Tex : TEXCOORD0; // 0〜1 の正規化UV
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    uint texW, texH;
    g_Tex.GetDimensions(texW, texH);

    float u = (SrcX + input.Tex.x * SrcW) / texW;
    float v = (SrcY + input.Tex.y * SrcH) / texH;

    float4 color = g_Tex.Sample(g_Sampler, float2(u, v));

    // Premultiplied の場合、RGB も Alpha を掛ける
    color.rgb *= Alpha;
    color.a *= Alpha;

    return color;
}
