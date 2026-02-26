#pragma once
#include "ButtonBase.hpp"

extern const bool in_debug_mode;

class TextboxBase : public Drawable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default Constructor
    ///
    /// Creates empty Textbox
    ///
    ////////////////////////////////////////////////////////////
    TextboxBase();

    ////////////////////////////////////////////////////////////
    /// \brief Construct from a Button vector, a Position, a string & a bool
    ///
    /// Creates a Textbox with a boolean(can the textbox be
    /// interacted with), a Button vector(as the
    /// integrated Buttons), a visual Position and a textbox id string
    ///
    ////////////////////////////////////////////////////////////
    TextboxBase(vector<ButtonBase*> buttons, const SDL_Point& visual, string textbox_id, bool interactable);

    /// Sets whether the Textbox should be displayed or not
    void set_displayed(bool displayed);

    /// Sets whether the textbox is interactable or not
    void set_interactable(bool interactable);

    /// @brief Check if the cursor overlaps with any buttons
    ///
    /// @param cursorPos current Position of the cursor
    ///
    /// Checks if the cursor overlaps with any button
    virtual void checkCursor();

    /// @brief add a Button to the textbox after construction
    /// @param button the button to be added as a pointer
    void add_button(ButtonBase* button);

    /// @brief Get the global bounding rectangle of the entity
    ///
    /// The returned rectangle is in global coordinates, which means
    /// that it takes into account the transformations (translation,
    /// rotation, scale, ...) that are applied to the entity.
    /// In other words, this function returns the bounds of the
    /// text in the global 2D world's coordinate system.
    ///
    /// @return Global bounding rectangle of the entity
    virtual SDL_FRect get_bounds();

    /// @brief Draw the textbox to a window through the gpu
    virtual void draw(); 
 
protected:
    /// @brief create a textbox bounding box for debug purposes
    void createBoundingBox() const;

    /// @brief Make sure the text's geometry is updated
    ///
    /// All the attributes related to rendering are cached, such
    /// that the geometry is only updated when necessary.
    void ensureGeometryUpdate();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    string                  textbox_id;         ///< id of the Textbox (if already set)
    bool                    interactable;       ///< Can the Textbox be interacted with?
    bool                    displayed;          ///< Is the Textbox currently displayed on screen?
    vector<ButtonBase*>     buttons;            ///< Buttons included in the Textbox
    mutable bool            selected;           ///< Is this textbox currently selected?
    mutable bool            focus;              ///< Is the Textbox in focus?
    mutable bool            geometryNeedUpdate; ///< Does the geometry need to be recomputed?
    mutable SDL_FRect       bounds;             ///< Bounding rectangle of the textbox (in local coordinates)
    mutable Rectangle       bounding_box;       ///< Bounding Box as a Drawable for bugfixing
};