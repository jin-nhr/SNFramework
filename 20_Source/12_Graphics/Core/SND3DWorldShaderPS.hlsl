Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float Alpha : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    uint texW, texH;
    g_Tex.GetDimensions(texW, texH);

    float2 uv = float2(input.Tex.x / texW, input.Tex.y / texH);

    float4 color = g_Tex.Sample(g_Sampler, uv);

    color.rgb *= input.Alpha;
    color.a *= input.Alpha;

    return color;
}
