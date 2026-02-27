cbuffer origin_buffer : register(b0, space3)
{
    float2 origin;
    float2 scale;
}

Texture2D<float4> Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

float get_alpha(float2 pos)
{
    float4 pixel = Texture.Sample(Sampler, pos);
    return pixel[3];
}

float get_occlusion(float4 Position)
{
    float occlusion = 0.f;
    int distance = 0;
    distance += abs(Position[0] / scale[0] - origin[0]);
    distance += abs(Position[1] / scale[1] - origin[1]);
    float x;
    float y;
    Texture.GetDimensions(x, y);
    for (int i = 0; i < distance; i++) {
        float2 texture_pos;
        texture_pos[0] = float((floor(origin[0] + ((Position[0] / scale[0] - origin[0]) * i / distance))) / x);
        texture_pos[1] = float((floor(origin[1] + ((Position[1] / scale[1] - origin[1]) * i / distance))) / y);
        // texture_pos[0] = Position[0] / scale[0] / x;
        // texture_pos[1] = Position[1] / scale[1] / x;
        float alpha = get_alpha(texture_pos);
        if (occlusion < alpha) {
            occlusion = alpha;
        }
        if (occlusion >= 1.f) {
            occlusion = 1.f;
            break;
        }
    }
    return occlusion;
}

float4 main(float4 Color : TEXCOORD1, float4 Position : SV_Position) : SV_Target0
{
    // float pos_x = origin[0] + Position[0] * (Color[3] * 255.f);
    // float pos_y = origin[1] + Position[1] * (Color[3] * 255.f);
    float4 centered_position = float4(Position[0] + 0.5f, Position[1] + 0.5f, 0.0f, 1.0f);
    float distance = pow(pow(abs(centered_position[0] - origin[0] * scale[0]) / scale[0], 2) + pow(abs(centered_position[1] - origin[1] * scale[1]) / scale[1], 2), 0.5);
    float alpha = 1.f;
    alpha -= pow(distance, 1.2f) / (Color[3] * 255.f);
    float occlusion = get_occlusion(centered_position);
    alpha *= (1.f - occlusion);
    // if (true) {
    //     return float4(0.f, 0.f, 0.4f, 1.f);
    // }
    // else if (alpha > 1.f) {
    //     return float4(0.4f, 0.4f, 0.4f, 1.f);
    // }
    // else {
    // float2 TexturePos = {Position[0] / scale[0] / x, Position[1] / scale[1] / y};
    // float4 TexColor = Texture.Sample(Sampler, TexturePos);
    // if (TexColor[3] != 0) {
    //     return float4(0, 0, 0, 0);
    // }
    return float4(Color[0] * alpha, Color[1] * alpha, Color[2] * alpha, alpha);
    //     return float4(Color[0], Color[1], Color[2], 0.5f);
    // }
    // return alpha <= 0.f ? float4(0.f, 0.f, 0.4f, 1.f) : alpha <= 1.f ? float4(Color[0], Color[1], Color[2], 0.5f) : float4(0.4f, 0.f, 0.f, 1.f);
    // if (origin[0] > 0.f) {
    //     return float4(0.5f, 0.f, 0.f, 1.f);
    // }
    // else if (origin[0] <= 0.f) {
    //     return float4(0.f, 0.5f, 0.f, 1.f);
    // }
    // else {
    //     return float4(0.f, 0.f, 0.5f, 1.f);
    // }
}