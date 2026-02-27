Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

float4 main(float2 TexCoord : TEXCOORD0, float4 Color : TEXCOORD1) : SV_Target0
{
    if (Texture.Sample(Sampler, TexCoord).a == 0) {
        return float4(0, 0, 0, 0);
    }
    else {
        return float4(Color.r, Color.g, Color.b, Color.a);
    }
}