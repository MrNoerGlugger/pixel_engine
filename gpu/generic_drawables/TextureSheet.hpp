#pragma once

#include <Texture.hpp>

class TextureSheet : public Texture {
protected:
    vector<SDL_FRect>* parts;
    int part = 0;
    float mirroring = 0;
    float color_mult = 1.f;
public:
    TextureSheet();
    virtual ~TextureSheet() {
        this->free();
    }

    /// @brief sets the part of the sheet
    /// @param part the part id, can be looked up in the Textures.hpp in init
    virtual void set_part(int part);

    /// @brief sets the mirror mode for the texturesheet
    /// @param mirroring the selected mirror mode (0 = none, 1 = horizontal, 2 = vertical, 3 = both)
    virtual void set_mirroring(float mirroring);

    /// @brief sets the color multiplier for the texturesheet
    /// @param color_mult the new color multiplier, also multiplies the alpha value
    virtual void set_color_multiplier(float color_mult);

    /// @brief get the bounds of the TextureSheet
    /// @return returns an SDL_FRect with the bounds
    SDL_FRect get_bounds();
    
    void create_graphics_pipeline();

    void init();

    void draw();

    void free();
};