#pragma once

#include <cmath>

#include <TextureSheet.hpp>
#include <TextboxBase.hpp>

class TextboxFramed : public TextboxBase 
{
private:
    /// Member data
    mutable string*              corners;       ///< image_path of the textbox corners
    mutable string*              edges;         ///< image_path of the textbox edges
    SDL_Point                    texture_size;  ///< the texture_size of a single frame part
    SDL_Point                    size;          ///< size of the textbox(leave empty for automation)
	mutable Rectangle            background;    ///< background color as a shape
	mutable vector<TextureSheet> frame_parts;   ///< vector of Textures of the frame around the textbox
public:
    /// @brief Default Constructor
    ///
    /// Creates empty Textbox
    TextboxFramed();

    /// @brief Construct from a Button vector, a Position, a string & a bool
    ///
    /// Creates a Textbox with a boolean(can the textbox be
    /// interacted with), a Button vector(as the
    /// integrated Buttons), a visual Position, a textbox id string
    /// and no size for automated size calculation
    TextboxFramed(vector<ButtonBase*> buttons, const SDL_Point& pos_visual, string textbox_id, bool interactable);

    /// @brief Construct from a Vector2i, a Button vector, a Position, a string & a bool
    ///
    /// Creates a Textbox with a boolean(can the textbox be
    /// interacted with), a Button vector(as the
    /// integrated Buttons), a visual Position, a textbox id string
    /// and size Vector
    TextboxFramed(const SDL_Point& size, vector<ButtonBase*> buttons, const SDL_Point& pos_visual, string textbox_id, bool interactable);

    /// @brief sets the image_paths to the corners & edges of the textbox
    /// @param corners the corners of the textbox frame
    /// @param edges the edges of the textbox frame
    virtual void set_image_paths(std::string* corners, std::string* edges);

    /// @brief Draw the button to a render target
    /// @param target Render target to draw to
    /// @param states Current render states
    virtual void draw();


private:
    /// @brief Draw the button to a render target
    /// @param target Render target to draw to
    /// @param states Current render states
    void createTextboxFrame(const SDL_Point& size) const;
    /// @brief Make sure the button's geometry is updated
    ///
    /// All the attributes related to rendering are cached, such
    /// that the geometry is only updated when necessary.
    virtual void ensureGeometryUpdate();
};