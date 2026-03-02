#pragma once

#include "../../gpu/generic_drawables/Rectangle.hpp"
#include "../../functional_base/FunctionHolder.hpp"
#include "../../input/InputBase.hpp"

extern const bool in_debug_mode;

class ButtonBase : public Drawable
{
public:
    /// @brief Default Constructor
    ButtonBase();

    
    /// @brief Construct from two Positions & a string
    /// @param mapped mapped Position(for textbox controls)
    /// @param visual visual Position (for on-screen positioning)
    /// @param textbox textbox id
    ButtonBase(const SDL_Point& mapped, const SDL_Point& visual, const string& textbox);


    /// @brief Store functions to call on the selection event
    /// @param func function call
    /// @param ...args arguments to use for the function call
    template<typename Func, typename... Args>
    void storeSelectionFunction(Func&& func, Args&&... args) {
        selection_holder.store(func, args...);
    }

    /// @brief Store functions to call on the selection event
    /// @param func function call as a lambda expression
    template<typename Func>
    void storeSelectionLambda(Func&& func) {
        selection_holder.storeLambda(func);
    }


    /// @brief Store functions to call on the activation event
    /// @param func function call
    /// @param ...args arguments to use for the function call
    template<typename Func, typename... Args>
    void storeActivationFunction(Func&& func, Args&&... args) {
        activation_holder.store(func, args...);
    }

    /// @brief Store functions to call on the activation event
    /// @param func function call as a lambda expression
    template<typename Func>
    void storeActivationLambda(Func&& func) {
        activation_holder.storeLambda(func);
    }

    /// @brief Check if the cursor overlaps with this button
    virtual void checkCursor();

    /// @brief get the bounds of the entity
    /// @return returns the bounds of the entity
    virtual SDL_FRect get_bounds();

    
    /// @brief Draw the button to a window through the gpu
    virtual void draw();

protected:
    /// @brief create a button bounding box for debug purposes
    void createBoundingBox() const;


    /// @brief Make sure the button's geometry is updated
    ///
    /// All the attributes related to rendering are cached, such
    /// that the geometry is only updated when necessary.
    virtual void update_geometry();


    /// @brief Call all functions for the selection event
    void callSelectionFunctions() const;


public:
    /// @brief Call all functions for the activation event
    void callActivationFunctions() const;

    
    ////////////////////////////////////////////////////////////
    /// Member data
    ////////////////////////////////////////////////////////////
public:
	SDL_Point               pos_mapped;             ///< Position of the mapped button
protected:
    string                  textbox_id;             ///< id of the associated Textbox (if already set)
    FunctionHolder          selection_holder;       ///< Functions needed to be called when selected
    FunctionHolder          activation_holder;      ///< Functions needed to be called when activated
    mutable bool            selected;               ///< Is this button currently selected?
    mutable SDL_FRect       bounds;                 ///< Bounding rectangle of the button (in local coordinates)
    mutable Rectangle       bounding_box;           ///< Bounding Box as a Drawable for bugfixing
};