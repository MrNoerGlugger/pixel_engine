#pragma once

#include <Drawable.hpp>
#include <TextboxBase.hpp>

class Screen : public Drawable {
    map<int, Drawable*> drawable_default_map;
    map<int, Drawable*> drawable_overlay_map;
    map<int, TextboxBase*> textboxes;
public:
    int add_drawable(Drawable* drawable, bool overlay = false, bool textbox = false);

    void remove_drawable(int id, bool overlay = false);

    void check_textboxes();

    virtual SDL_FRect get_bounds();

    virtual void update_geometry();

    virtual void init();

    virtual void draw();

    virtual void free();
};