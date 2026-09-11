struct Tile
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

    float MulR;
    float MulG;
    float MulB;
    float MulA;
};

StructuredBuffer<Tile> Inst : register(t1);

struct VS_INPUT
{
    float3 Pos : POSITION; // 0〜1 の正規化矩形
    float2 Tex : TEXCOORD0; // 0〜1 の正規化UV
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float Alpha : TEXCOORD1;
    float4 ColorMul : TEXCOORD2;
};

VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
    VS_OUTPUT o;

    Tile t = Inst[instanceID];

    float px = t.DstX + input.Pos.x * t.DstW;
    float py = t.DstY + input.Pos.y * t.DstH;

    float ndc_x = (px / t.ScreenWidth) * 2.0f - 1.0f;
    float ndc_y = 1.0f - (py / t.ScreenHeight) * 2.0f;

    o.Pos = float4(ndc_x, ndc_y, 0.0f, 1.0f);

    o.Tex.x = (t.SrcX + input.Tex.x * t.SrcW);
    o.Tex.y = (t.SrcY + input.Tex.y * t.SrcH);
    o.Alpha = t.Alpha;
    
    o.ColorMul = float4(t.MulR, t.MulG, t.MulB, t.MulA);

    return o;
}
