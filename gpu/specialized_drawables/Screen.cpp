#include "Screen.hpp"

int Screen::add_drawable(Drawable* drawable, bool overlay, bool textbox) {
    int i = 0;
    map<int, Drawable*>* current_map = overlay ? &drawable_overlay_map : &drawable_default_map;
    for (auto drawable_pair : *current_map) {
        if (i != drawable_pair.first) {
            break;
        }
        i++;
    }
    current_map->insert({i, drawable});
    if (textbox) {
        if (dynamic_cast<TextboxBase*>(drawable) == nullptr) {
            Logger::log_warning("wrong usage of add_drawable: tried to add a non-Textbox to the Textbox map!");
            return i;
        }
        textboxes.insert({i * (overlay ? 1 : -1), (TextboxBase*)drawable});
    }
    return i;
}

void Screen::remove_drawable(int id, bool overlay, bool textbox) {
    map<int, Drawable*>* current_map = overlay ? &drawable_overlay_map : &drawable_default_map;
    current_map->erase(id);
    if (textbox) {
        textboxes.erase(id * (overlay ? 1 : -1));
    }
}

void Screen::check_textboxes() {
    for (auto textbox : textboxes) {
        textbox.second->checkCursor();
    }
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