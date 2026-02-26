#pragma once

#include <SDL3_image/SDL_image.h>

#include <Drawable.hpp>

class Texture : public Drawable {
protected:
    SDL_GPUSampler* sampler;
    std::string* image_path = nullptr;
    bool shader_texture = false;
    float rotation = 0.f;

public:
    Texture();
    Texture(std::string* image_path);
    virtual ~Texture() {
        this->free();
    }

    virtual void set_rotation(float rotation);
    void set_image_path(std::string* image_path, bool shader_texture = false);


    void create_graphics_pipeline();

    virtual void update_geometry();


    /// @brief initialize the Texture by registering the graphics pipeline, texture and sampler
    virtual void init();

    /// @brief draw the Texture onto the swapchaintexture
    virtual void draw();

    /// @brief free the Texture and its substructures
    virtual void free();
};