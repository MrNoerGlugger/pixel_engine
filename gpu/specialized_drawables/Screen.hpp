#pragma once

#include <Drawable.hpp>

class Screen : public Drawable {
    map<int, Drawable*> drawable_default_map;
    map<int, Drawable*> drawable_overlay_map;
public:
    int add_default_drawable(Drawable* drawable);

    int add_overlay_drawable(Drawable* drawable);

    void remove_default_drawable(int id);

    void remove_overlay_drawable(int id);

    virtual SDL_FRect get_bounds();


    virtual void update_geometry();

    virtual void init();

    virtual void draw();

    virtual void free();
};