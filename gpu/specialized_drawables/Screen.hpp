#pragma once

#include <Drawable.hpp>

class Screen : public Drawable {
    vector<Drawable*> drawable_default_map;
    vector<Drawable*> drawable_overlay_map;
public:
    void add_default_drawable(Drawable* drawable);

    void add_overlay_drawable(Drawable* drawable);

    virtual SDL_FRect get_bounds();


    virtual void update_geometry();

    virtual void init();

    virtual void draw();

    virtual void free();
};