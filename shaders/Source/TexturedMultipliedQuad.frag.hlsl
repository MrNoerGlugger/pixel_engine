cbuffer color_data : register(b0, space3)
{
    float color_mult;
}

Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

float4 main(float2 TexCoord : TEXCOORD0) : SV_Target0
{
    float4 color = Texture.Sample(Sampler, TexCoord);
    color[0] *= color_mult;
    color[1] *= color_mult;
    color[2] *= color_mult;
    color[3] *= color_mult;
    return color;
}