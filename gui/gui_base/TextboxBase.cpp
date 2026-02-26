#include "TextboxBase.hpp"

////////////////////////////////////////////////////////////
TextboxBase::TextboxBase():
textbox_id            (),
interactable          (false),
displayed             (false),
buttons               (),
selected              (false),
focus                 (false),
geometryNeedUpdate    (false),
bounds                (),
bounding_box          ()
{
    bounding_box.set_color(SDL_Color{255, 255, 0, 255});
}


////////////////////////////////////////////////////////////
TextboxBase::TextboxBase(vector<ButtonBase*> buttons, const SDL_Point& visual, string textbox_id, bool interactable):
textbox_id            (textbox_id),
interactable          (interactable),
displayed             (false),
buttons               (buttons),
selected              (false),
focus                 (false),
geometryNeedUpdate    (true),
bounds                (),
bounding_box          ()
{
    bounding_box.set_color(SDL_Color{255, 255, 0, 255});
    set_position(visual.x, visual.y);
}


void TextboxBase::set_displayed(bool displayed) {
    this->displayed = displayed;
}

void TextboxBase::set_interactable(bool interactable)  {
    this->interactable = interactable;
}


void TextboxBase::checkCursor() {
    // Check if it is currently possible to interact with the textbox
    if (!interactable || !displayed) return;

    // Get the bounds of the textbox to check for overlap
    SDL_FRect textboxBounds = get_bounds();
    selected = SDL_PointInRectFloat(&input::mouse_pos, &textboxBounds);

    //check Cursor position for buttons
    for (auto i : buttons) {
        i->checkCursor();
    }
}

void TextboxBase::add_button(ButtonBase* button)  {
    buttons.push_back(button);
    geometryNeedUpdate = true;
}


////////////////////////////////////////////////////////////
SDL_FRect TextboxBase::get_bounds()
{
    ensureGeometryUpdate();

    return bounds;
}

////////////////////////////////////////////////////////////
void TextboxBase::createBoundingBox() const {
    if (!in_debug_mode) return;

    //print out the bounds to console
    Logger::log_debug(std::format("Textbox Bounds: {}:x,y:{}\t{}:w,h:{}", bounds.x, bounds.y, bounds.w, bounds.h));

    //debug bounding box testing
    bounding_box.set_position(bounds.x, bounds.y);
    bounding_box.set_size(bounds.w, bounds.h);
}


////////////////////////////////////////////////////////////
void TextboxBase::draw()
{
    if (!displayed) return;

    ensureGeometryUpdate();

    //draw the debug bounding box
    if (selected && in_debug_mode) {
        bounding_box.draw();
    }

    //draw the individual Buttons
    for (auto it = buttons.begin(); it != buttons.end(); it++) {
        (*it)->draw();
    }
}


void TextboxBase::ensureGeometryUpdate()
{
    // Do nothing, if geometry has not changed
    if (!geometryNeedUpdate)
        return;

    bounds.x = position.x;
    bounds.y = position.y;

    //update button geometry
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->set_position(position.x, position.y);
        SDL_FRect button_bounds = buttons[i]->get_bounds();

        //check if the button has any bounds
        if (button_bounds.w == 0 || button_bounds.h == 0) {
            continue;
        }

        //update bounds based on button bounds
        float button_width = button_bounds.w + button_bounds.x - position.x;
        bounds.w = bounds.w < button_width ? button_width : bounds.w;
        float button_height = button_bounds.h + button_bounds.y - position.y;
        bounds.h = bounds.h < button_height ? button_height : bounds.h;

        //check for negative button positions to update bounds
        bounds.x = bounds.x > button_bounds.x ? button_bounds.x : bounds.x;
        bounds.y = bounds.y > button_bounds.y ? button_bounds.y : bounds.y;
    }

    createBoundingBox();

    // Mark geometry as updated
    geometryNeedUpdate = false;
}