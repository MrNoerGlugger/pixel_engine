#pragma once

#include <SDL3/SDL.h>

#include <ShaderHandling.hpp>
#include <TextureInit.hpp>
#include <Logger.hpp>

class Drawable {
protected:
    SDL_GPUGraphicsPipeline* pipeline;
    string pipeline_id;
    SDL_GPUViewport viewport{0.f, 0.f, 0.f, 0.f, 0.f, 1.f};
    SDL_FPoint position = {0.0f, 0.0f};
    SDL_FPoint scale = {1.0f, 1.0f};

    bool geometry_update_needed = true;

public:
    virtual ~Drawable() {
        this->free();
    }

    virtual void set_position(float x, float y);
    virtual void move(float x, float y);
    virtual void set_scale(float x, float y);
    virtual void set_scale(float scale);
    virtual SDL_FRect get_bounds();

    virtual void create_graphics_pipeline();

    virtual void update_geometry();

    virtual void init();

    virtual void draw();

    virtual void free();
};