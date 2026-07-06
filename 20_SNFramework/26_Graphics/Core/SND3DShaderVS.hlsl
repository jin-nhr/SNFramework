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

// 頂点シェーダ：そのまま流すだけ
VS_OUTPUT VS_FullscreenQuad(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = float4(input.Pos, 1.0f);
    output.Tex = input.Tex;
    return output;
}

VS_OUTPUT main(VS_INPUT input)
{
    return VS_FullscreenQuad(input);
}
