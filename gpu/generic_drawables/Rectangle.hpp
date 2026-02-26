#pragma once

#include <Drawable.hpp>

class Rectangle : public Drawable {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Point size = {480, 270};
    
public:
    virtual ~Rectangle() {
        this->free();
    }

    /// @brief changes the color to be displayed by the Rectangle, white by default
    /// @param color color for the rectangle, defaults to (255, 255, 255, 255)
    void set_color(SDL_Color color);

    void set_size(float x, float y);


    void create_graphics_pipeline();

    void init();

    void draw();

    void free();
};