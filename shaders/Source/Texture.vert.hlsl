cbuffer rotation_buffer : register(b0, space1)
{
   float rotation;
}
struct Input
{
    uint VertexIndex : SV_VertexID;
};

struct Output
{
    float2 TexCoord : TEXCOORD0;
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;
    float2 tex_coord;
    float4 pos;

    const float PI = 3.14159265f;
    float r = sqrt(2);
    float alpha = PI * ((rotation - 45.f) / 180.f);

    switch (input.VertexIndex) {
    case 0:
        pos = float4(r * sin(alpha), r * cos(alpha), 0.0f, 1.0f);
        tex_coord = float2(0.0f, 0.0f);
        break;
    case 1:
    //intended
    case 3:
        pos = float4(r * sin(alpha + PI * 0.5f), r * cos(alpha + PI * 0.5f), 0.0f, 1.0f);
        tex_coord = float2(1.0f, 0.0f);
        break;
    case 2:
    //intended
    case 4:
        pos = float4(r * sin(alpha + PI * 1.5f), r * cos(alpha + PI * 1.5f), 0.0f, 1.0f);
        tex_coord = float2(0.0f, 1.0f);
        break;
    case 5:
        pos = float4(r * sin(alpha + PI * 1.0f), r * cos(alpha + PI * 1.0f), 0.0f, 1.0f);
        tex_coord = float2(1.0f, 1.0f);
        break;
    }

    output.Position = pos;
    output.TexCoord = tex_coord;
    return output;
}