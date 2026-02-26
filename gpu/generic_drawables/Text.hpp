#pragma once

#include <Drawable.hpp>

#include <SDL3_ttf/SDL_ttf.h>

class Text : public Drawable {
    SDL_GPUTexture* text_texture;
    SDL_GPUSampler* sampler;
    vector<pair<SDL_GPUTexture*, SDL_GPUViewport>> glyphs;
    string text = "";
    SDL_Color text_color = {0, 0, 0, 0};

public:
    /// @brief Default Constructor
    Text();

    /// @brief Constructor with additional arguments
    /// @param text_string string to be displayed by the Text
    /// @param font font to be used by the Text
    Text(std::string text_string);
    virtual ~Text() {
        this->free();
    }

    virtual void set_position(float x, float y);
    virtual SDL_FRect get_bounds();

    /// @brief set the text to be displayed by the Text
    /// @param text_string text to be displayed by the Text
    void set_text_string(string text_string);

    /// @brief changes the text_color to be displayed by the Text, not used by default
    /// @param text_color text color for the text, defaults to (0, 0, 0, 0)
    void set_text_color(SDL_Color text_color);


    void create_graphics_pipeline();

    /// @brief initialize the Text by generating the texture and viewport
    void init();

    /// @brief draw the Text onto the swapchaintexture
    void draw();

    /// @brief free the Text and its substructures
    void free();
};