#include "ButtonText.hpp"

////////////////////////////////////////////////////////////
ButtonText::ButtonText() :
text                  (),
text_selected         (),
ButtonText::ButtonBase()
{

}


////////////////////////////////////////////////////////////
ButtonText::ButtonText(const string& text, const SDL_Point& mapped, const SDL_Point& visual, const string& textbox) :
text                  (Text(text)),
text_selected         (Text(">" + text)),
ButtonText::ButtonBase(mapped, visual, textbox)
{
    geometry_update_needed = true;
}


////////////////////////////////////////////////////////////
void ButtonText::draw()
{
    update_geometry();

    ButtonBase::draw();

    if (selected) {
        text_selected.draw();
    }
    else {
        text.draw();
    }
}


void ButtonText::update_geometry()
{
    // Do nothing, if geometry has not changed
    if (!geometry_update_needed)
        return;

    text.set_position(viewport.x, viewport.y);
    text_selected.set_position(viewport.x, viewport.y);

    text.init();
    text_selected.init();

    //update bounds
    SDL_FRect text_bounds = text_selected.get_bounds();
    bounds.x += text_bounds.x;
    bounds.y += text_bounds.y;
    bounds.w += text_bounds.w;
    bounds.h += text_bounds.h;

    // update ButtonBase geometry
    ButtonBase::update_geometry();
}