#pragma once

#include <cmath>
#include <cstdlib>

#include <TextureSheet.hpp>

class LightSource : public TextureSheet {
    SDL_Point origin = {0, 0};
    SDL_FPoint origin_transformed = {0, 0};
    Uint32 color = 0;
    SDL_Point texture_size = {32, 32};

public:
    virtual ~LightSource() {
        this->free();
    }

    virtual void set_position(float x, float y);
    virtual void move(float x, float y);

    //set the origin (also known as the center) of the light source
    void set_origin(SDL_Point origin);

    //set the color of the light source
    void set_color(Uint32 color);

    void set_texture_size(SDL_Point texture_size);

    virtual void set_rotation(float rotation);

    //set the mirroring of the lightsource (see more: TextureSheet.hpp)
    virtual void set_mirroring(float mirroring);

    virtual void create_graphics_pipeline();

    virtual void init();

    virtual void draw();

    virtual void free();
};