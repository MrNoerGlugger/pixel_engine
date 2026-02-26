#pragma once

#include <cmath>

#include "../../../gpu/generic_drawables/TextureSheet.hpp"
#include "../../gui_base/TextboxBase.hpp"

class TextboxFramed : public TextboxBase 
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default Constructor
    ///
    /// Creates empty Textbox
    ///
    ////////////////////////////////////////////////////////////
    TextboxFramed();

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a Button vector, a Position, a string & a bool
    ///
    /// Creates a Textbox with a boolean(can the textbox be
    /// interacted with), a Button vector(as the
    /// integrated Buttons), a visual Position, a textbox id string
    /// and no size for automated size calculation
    ///
    ////////////////////////////////////////////////////////////
    TextboxFramed(vector<ButtonBase*> buttons, const SDL_Point& pos_visual, string textbox_id, bool interactable);

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a Vector2i, a Button vector, a Position, a string & a bool
    ///
    /// Creates a Textbox with a boolean(can the textbox be
    /// interacted with), a Button vector(as the
    /// integrated Buttons), a visual Position, a textbox id string
    /// and size Vector
    ///
    ////////////////////////////////////////////////////////////
    TextboxFramed(const SDL_Point& size, vector<ButtonBase*> buttons, const SDL_Point& pos_visual, string textbox_id, bool interactable);

    ////////////////////////////////////////////////////////////
    /// \brief Draw the button to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw();


private:
    ////////////////////////////////////////////////////////////
    /// \brief Draw the button to a render target
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    void createTextboxFrame(const SDL_Point& size) const;
    ////////////////////////////////////////////////////////////
    /// \brief Make sure the button's geometry is updated
    ///
    /// All the attributes related to rendering are cached, such
    /// that the geometry is only updated when necessary.
    ///
    ////////////////////////////////////////////////////////////
    virtual void ensureGeometryUpdate();
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    SDL_Point                    size;          ///< size of the textbox(leave empty for automation)
	mutable Rectangle            background;    ///< background color as a shape
	mutable vector<TextureSheet> frame_parts;   ///< vector of Textures of the frame around the textbox
};