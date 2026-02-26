#include "Screen.hpp"

void Screen::add_default_drawable(Drawable* drawable) {
    drawable_default_map.push_back(drawable);
}
void Screen::add_overlay_drawable(Drawable* drawable) {
    drawable_overlay_map.push_back(drawable);
}

SDL_FRect Screen::get_bounds() {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    bool start_value_set = false;
    for (auto drawable : drawable_default_map) {
        if (!start_value_set || x_min > drawable->get_bounds().x) {
            x_min = drawable->get_bounds().x;
        }
        if (!start_value_set || x_max < drawable->get_bounds().x + drawable->get_bounds().w) {
            x_max = drawable->get_bounds().x + drawable->get_bounds().w;
        }
        if (!start_value_set || y_min > drawable->get_bounds().y) {
            y_min = drawable->get_bounds().y;
        }
        if (!start_value_set || y_max < drawable->get_bounds().y + drawable->get_bounds().h) {
            y_max = drawable->get_bounds().y + drawable->get_bounds().h;
        }
        start_value_set = true;
    }
    return {x_min, y_min, x_max - x_min, y_max - y_min};
}

void Screen::update_geometry() {
    for (auto drawable : drawable_default_map) {
        drawable->update_geometry();
    }
    for (auto drawable : drawable_overlay_map) {
        drawable->update_geometry();
    }
}

void Screen::init() {
    for (auto drawable : drawable_default_map) {
        drawable->init();
    }
    for (auto drawable : drawable_overlay_map) {
        drawable->init();
    }
}

void Screen::draw() {
    switch (graphics::draw_stage) {
    case DEFAULT:
    case SHADOW:
    case LIGHT:
        for (auto drawable : drawable_default_map) {
            drawable->draw();
        }
        break;
    case OVERLAY:
        for (auto drawable : drawable_overlay_map) {
            drawable->draw();
        }
        break;
    }
}

void Screen::free() {
    for (auto it = drawable_default_map.begin(); it != drawable_default_map.end(); it++) {
        (*it)->free();
        delete(*it);
    }
    for (auto it = drawable_overlay_map.begin(); it != drawable_overlay_map.end(); it++) {
        (*it)->free();
        delete(*it);
    }
}