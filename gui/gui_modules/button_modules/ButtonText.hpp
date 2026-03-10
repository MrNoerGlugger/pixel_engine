#pragma once
#include <ButtonBase.hpp>
#include <Text.hpp>

class ButtonText : public ButtonBase 
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default Constructor
    ///
    /// Creates empty Text Button
    ///
    ////////////////////////////////////////////////////////////
    ButtonText();
    
    ////////////////////////////////////////////////////////////
    /// \brief Construct from two Positions & two strings
    ///
    /// Creates a Button with a mapped Position(for textbox
    /// controls) a visual Position (for on-screen positioning)
    /// and a string(as a textbox id)
    ///
    ////////////////////////////////////////////////////////////
    ButtonText(string text, SDL_Point mapped, SDL_Point visual);

    ////////////////////////////////////////////////////////////
    /// \brief Draw the button to a render target
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw();

private:
    ////////////////////////////////////////////////////////////
    /// \brief Make sure the button's geometry is updated
    ///
    /// All the attributes related to rendering are cached, such
    /// that the geometry is only updated when necessary.
    ///
    ////////////////////////////////////////////////////////////
    virtual void update_geometry();
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    mutable Text text;           ///< text displayed by the Button
    mutable Text text_selected;  ///< text displayed by the Button when selected
};