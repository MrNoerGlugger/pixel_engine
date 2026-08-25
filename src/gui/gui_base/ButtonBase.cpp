#include "ButtonBase.hpp"


////////////////////////////////////////////////////////////
ButtonBase::ButtonBase() :
pos_mapped            (),
pos_visual            (),
selection_holder      (),
activation_holder     (),
selected              (false),
bounds                ()
{
    bounding_box.set_color(SDL_Color{255, 0, 0, 255});
}

////////////////////////////////////////////////////////////
ButtonBase::ButtonBase(SDL_Point mapped, SDL_Point visual) :
pos_mapped            (mapped),
pos_visual            (visual),
selection_holder      (),
activation_holder     (),
selected              (false),
bounds                ()
{  
    bounding_box.set_color(SDL_Color{255, 0, 0, 255});
}


////////////////////////////////////////////////////////////
void ButtonBase::callSelectionFunctions() {
    selection_holder.callAll();
}

////////////////////////////////////////////////////////////
void ButtonBase::callActivationFunctions() {
    activation_holder.callAll();
}

////////////////////////////////////////////////////////////
void ButtonBase::checkCursor() {
    // Get the bounds of the button to check for overlap
    SDL_FRect buttonBounds = get_bounds();
    bool overlap = SDL_PointInRectFloat(&input::mouse_pos, &buttonBounds);

    // Check the overlap vs the selected and potentially call the selection functions
    if (!overlap) {
        if (selected) selected = overlap;
        return;
    }
    else if (!selected) {
        selected = overlap;
        callSelectionFunctions();
    }
    if (input::check_input(CONFIRM)) {
        callActivationFunctions();
    }
}

void ButtonBase::set_position(float x, float y) {
    Drawable::set_position(x, y);
    viewport.x = position.x + pos_visual.x;
    viewport.y = position.y + pos_visual.y;
    geometry_update_needed  = true;
}

////////////////////////////////////////////////////////////
SDL_FRect ButtonBase::get_bounds()
{
    update_geometry();

    return bounds;
}


////////////////////////////////////////////////////////////
void ButtonBase::createBoundingBox() {
    if (!in_debug_mode) return;

    //print out the bounds to console
    Logger::log_debug(std::format("Button Bounds: {}:x,y:{}\t{}:w,h:{}", bounds.x, bounds.y, bounds.w, bounds.h));

    //debug bounding box testing
    bounding_box.set_position(bounds.x, bounds.y);
    bounding_box.set_size(bounds.w, bounds.h);
}


////////////////////////////////////////////////////////////
void ButtonBase::draw()
{
    if (selected && in_debug_mode) {
        bounding_box.draw();
    }
}


void ButtonBase::update_geometry()
{
    // Do nothing, if geometry has not changed
    if (!geometry_update_needed)
        return;

    bounds.x = viewport.x;
    bounds.y = viewport.y;
    
    createBoundingBox();

    // Mark geometry as updated
    geometry_update_needed = false;
}
