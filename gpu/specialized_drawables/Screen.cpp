#include "Screen.hpp"

int Screen::add_default_drawable(Drawable* drawable) {
    int i = 0;
    for (auto drawable_pair : drawable_default_map) {
        if (i != drawable_pair.first) {
            break;
        }
        i++;
    }
    drawable_default_map.insert({i, drawable});
    return i;
}
int Screen::add_overlay_drawable(Drawable* drawable) {
    int i = 0;
    for (auto drawable_pair : drawable_overlay_map) {
        if (i != drawable_pair.first) {
            break;
        }
        i++;
    }
    drawable_overlay_map.insert({i, drawable});
    return i;
}
void Screen::remove_default_drawable(int id) {
    drawable_default_map.erase(id);
}
void Screen::remove_overlay_drawable(int id) {
    drawable_overlay_map.erase(id);
}

SDL_FRect Screen::get_bounds() {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    bool start_value_set = false;
    for (auto drawable_pair : drawable_default_map) {
        auto drawable = drawable_pair.second;
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
    for (auto drawable_pair : drawable_default_map) {
        drawable_pair.second->update_geometry();
    }
    for (auto drawable_pair : drawable_overlay_map) {
        drawable_pair.second->update_geometry();
    }
}

void Screen::init() {
    for (auto drawable_pair : drawable_default_map) {
        drawable_pair.second->init();
    }
    for (auto drawable_pair : drawable_overlay_map) {
        drawable_pair.second->init();
    }
}

void Screen::draw() {
    switch (graphics::draw_stage) {
    case DEFAULT:
    case SHADOW:
    case LIGHT:
        for (auto drawable_pair : drawable_default_map) {
            drawable_pair.second->draw();
        }
        break;
    case OVERLAY:
        for (auto drawable_pair : drawable_overlay_map) {
            drawable_pair.second->draw();
        }
        break;
    }
}

void Screen::free() {
    for (auto it = drawable_default_map.begin(); it != drawable_default_map.end(); it++) {
        (*it).second->free();
        delete((*it).second);
    }
    for (auto it = drawable_overlay_map.begin(); it != drawable_overlay_map.end(); it++) {
        (*it).second->free();
        delete((*it).second);
    }
}