struct animation_data {
    uint time_fraction;
    float x, y, w, h;
};

cbuffer time_buffer : register(b1, space3)
{
    animation_data data : packoffset(c0);
}

float abs(float number) {
    if (number >= 0) {
        return number;
    } 
    else {
        return number * -1.0f;
    }
}

float4 main(float4 Color : TEXCOORD1, float4 Position : SV_Position) : SV_Target0
{
    // Color = float4(abs((((time_fraction + 10000) % 10001) - 5000.f) / 5000.f), abs((((time_fraction + 0) % 10001) - 5000.f) / 5000.f), abs((((time_fraction + 2000) % 10001) - 5000.f) / 5000.f), 0.5f);
    
    // if (data.w == 480) {
    //     return Color;
    // }
    float pos_x = (Position.x - data.x) / data.w;
    float pos_y = (Position.y - data.y) / data.h;
    // Color.r = abs((((int)((10000 - data.time_fraction) + (pos_x * 10000.f)) % 10001) - 5000.f) / 5000.f);
    // Color.g = abs((((int)((10000 - data.time_fraction) + (pos_y * 10000.f)) % 10001) - 5000.f) / 5000.f);
    // Color.b = abs((((int)((10000 - data.time_fraction) + ((pos_x + pos_y) * 10000.f)) % 10001) - 5000.f) / 5000.f);


    Color.r = abs((((int)((10000 - data.time_fraction) + ((pos_x + pos_y) * 5000.f)) % 10001) - 5000.f) / 5000.f);
    Color.g = abs((((int)((10000 - data.time_fraction) + ((pos_x + pos_y) * 5000.f) + 3333) % 10001) - 5000.f) / 5000.f);
    Color.b = abs((((int)((10000 - data.time_fraction) + ((pos_x + pos_y) * 5000.f) + 6667) % 10001) - 5000.f) / 5000.f);
    // Color.r = 1.0f;
    return Color;
}