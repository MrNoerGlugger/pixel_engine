struct Input
{
    uint VertexIndex : SV_VertexID;
};

struct Output
{
    float4 Color : TEXCOORD1;
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;
    output.Color = float4(1.0f, 1.0f, 1.0f, 0.2f);
    float4 pos;
    switch (input.VertexIndex) {
    case 0:
        pos = float4(-1.0f, 1.0f, 0.0f, 1.0f);
        break;
    case 1:
    //intended
    case 3:
        pos = float4(1.0f, 1.0f, 0.0f, 1.0f);
        break;
    case 2:
    //intended
    case 4:
        pos = float4(-1.0f, -1.0f, 0.0f, 1.0f);
        break;
    case 5:
        pos = float4(1.0f, -1.0f, 0.0f, 1.0f);
        break;
    }

    output.Position = pos;
    return output;
}