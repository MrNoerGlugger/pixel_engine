#include "Drawable.hpp"

void Drawable::set_position(float x, float y) {
    position.x = x;
    position.y = y;
    viewport.x = position.x;
    viewport.y = position.y;
}
void Drawable::move(float x, float y) {
    position.x += x;
    position.y += y;
    viewport.x = position.x;
    viewport.y = position.y;
}
void Drawable::set_scale(float x, float y) {
    scale.x = x;
    scale.y = y;
}
void Drawable::set_scale(float scale) {
    set_scale(scale, scale);
}
SDL_FRect Drawable::get_bounds() {
    return {viewport.x, viewport.y, viewport.w * scale.x, viewport.h * scale.y};
}

void Drawable::create_graphics_pipeline() {

}

void Drawable::update_geometry() {

}

void Drawable::init() {

}

void Drawable::draw() {

}

void Drawable::free() {
    // Logger::log_debug("Drawable freed");
}