cbuffer TextureCoords : register(b0, space1)
{
    float4 texture_coords;
    float rotation;
    float mirroring;
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

    // pos = float4(
    //     r * (sin(alpha) * (input.VertexIndex == 0) + sin(alpha + PI * 0.5f) * (input.VertexIndex == 1 || input.VertexIndex == 3)
    //         + sin(alpha + PI * 1.5f) * (input.VertexIndex == 2 || input.VertexIndex == 4) + sin(alpha + PI * 1.0f) * (input.VertexIndex == 5)
    //     ),
    //     r * (cos(alpha) * (input.VertexIndex == 0) + cos(alpha + PI * 0.5f) * (input.VertexIndex == 1 || input.VertexIndex == 3)
    //         + cos(alpha + PI * 1.5f) * (input.VertexIndex == 2 || input.VertexIndex == 4) + cos(alpha + PI * 1.0f) * (input.VertexIndex == 5)
    //     ),
    //     0.0f,
    //     1.0f
    // );

    // tex_coord = float2( tex_coord[0] * (input.VertexIndex % 2 == 0) + tex_coord[2] * !(input.VertexIndex % 2 == 0), 
    //                     tex_coord[1] * (input.VertexIndex < 2 || input.VertexIndex == 3) + tex_coord[3] * !(input.VertexIndex < 2 || input.VertexIndex == 3)
    //                     );

    // mirroring: 0 = no mirroring; 1 = horizontal; 2 = vertical; 3 = both;
    float u1 = mirroring == 2 || mirroring == 3 ? texture_coords[2] : texture_coords[0];
    float v1 = mirroring == 1 || mirroring == 3 ? texture_coords[3] : texture_coords[1];
    float u2 = mirroring == 2 || mirroring == 3 ? texture_coords[0] : texture_coords[2];
    float v2 = mirroring == 1 || mirroring == 3 ? texture_coords[1] : texture_coords[3];

    switch (input.VertexIndex) {
    case 0:
        pos = float4(r * sin(alpha), r * cos(alpha), 0.0f, 1.0f);
        tex_coord = float2(u1, v1);
        break;
    case 1:
    //intended
    case 3:
        pos = float4(r * sin(alpha + PI * 0.5f), r * cos(alpha + PI * 0.5f), 0.0f, 1.0f);
        tex_coord = float2(u2, v1);
        break;
    case 2:
    //intended
    case 4:
        pos = float4(r * sin(alpha + PI * 1.5f), r * cos(alpha + PI * 1.5f), 0.0f, 1.0f);
        tex_coord = float2(u1, v2);
        break;
    case 5:
        pos = float4(r * sin(alpha + PI * 1.0f), r * cos(alpha + PI * 1.0f), 0.0f, 1.0f);
        tex_coord = float2(u2, v2);
        break;
    }

    output.Position = pos;
    output.TexCoord = tex_coord;
    return output;
}