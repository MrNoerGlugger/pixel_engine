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
    float4 pos;
    if (input.VertexIndex == 0 || input.VertexIndex == 3)
    {
        pos = float4(-0.5f, 0.0f, 0.0f, 1.0f);
        output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f);
    }
    else
    {
        if (input.VertexIndex == 1 || input.VertexIndex == 4)
        {
            pos = float4(0.5f, 0.0f, 0.0f, 1.0f);
            output.Color = float4(1.0f, 0.0f, 1.0f, 1.0f);
        }
        else
        {
            if (input.VertexIndex == 2)
            {
                pos = float4(0.0f, 1.0f, 0.0f, 1.0f);
                output.Color = float4(1.0f, 1.0f, 1.0f, 0.5f);
            }
            else {
                pos = float4(0.0f, -1.0f, 0.0f, 1.0f);
                output.Color = float4(1.0f, 1.0f, 1.0f, 0.5f);
            }
        }
    }
    output.Position = pos;
    return output;
}