cbuffer color_buffer : register(b0, space1)
{
    float4 color;
}
struct Input
{
    uint VertexIndex : SV_VertexID;
};

struct Output
{
    float2 TexCoord : TEXCOORD0;
    float4 Color : TEXCOORD1;
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;
    float2 tex_coord;
    float4 pos;
    switch (input.VertexIndex) {
    case 0:
        pos = float4(-1.0f, 1.0f, 0.0f, 1.0f);
        tex_coord = float2(0.0f, 0.0f);
        break;
    case 1:
    //intended
    case 3:
        pos = float4(1.0f, 1.0f, 0.0f, 1.0f);
        tex_coord = float2(1.0f, 0.0f);
        break;
    case 2:
    //intended
    case 4:
        pos = float4(-1.0f, -1.0f, 0.0f, 1.0f);
        tex_coord = float2(0.0f, 1.0f);
        break;
    case 5:
        pos = float4(1.0f, -1.0f, 0.0f, 1.0f);
        tex_coord = float2(1.0f, 1.0f);
        break;
    }

    output.Position = pos;
    output.TexCoord = tex_coord;
    output.Color = color;
    return output;
}