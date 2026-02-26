#pragma once

#include <Text.hpp>
#include <TextureSheet.hpp>

class CustomText : public Drawable {
    vector<Drawable*> glyphs;
    string text_string;
    int text_size = 16;
    SDL_Color text_color = {0, 0, 0, 0};
    bool single_glyphs = true;

public:
    CustomText();
    CustomText(string text_string);
    virtual ~CustomText() {
        this->free();
    }

    void set_text_string(string text_string);
    void set_single_glyphs(bool single_glyphs);

    virtual void update_geometry();


    virtual void init();

    virtual void draw();

    virtual void free();
};