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

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT o;

    float px = DstX + input.Pos.x * DstW;
    float py = DstY + input.Pos.y * DstH;

    float ndc_x = (px / ScreenWidth) * 2.0f - 1.0f;
    float ndc_y = 1.0f - (py / ScreenHeight) * 2.0f;

    o.Pos = float4(ndc_x, ndc_y, 0.0f, 1.0f);

    // UV はそのまま渡す（PSで切り抜き計算）
    o.Tex = input.Tex;

    return o;
}
