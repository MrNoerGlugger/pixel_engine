#pragma once

#include <LightSource.hpp>
#include <TextureSheet.hpp>

class ShaderTexture : public TextureSheet {
    TextureSheet texture_default;
    vector<LightSource*> sources;
    bool light_sources_active = true;
public:
    virtual ~ShaderTexture() {
        this->free();
    }

    void set_image_path(std::string* image_path, bool shader_texture = false);
    void set_image_path_shader(std::string* image_path, bool shader_texture = true);

    /// @brief set the light sources to be active or inactive depending on circumstance
    void set_light_sources_active(bool light_sources_active);

    /// @brief sets the part of the sheet;  override of the base function
    /// @param part the part id, can be looked up in the Textures.hpp in init
    virtual void set_part(int part);

    /// @brief sets the mirror mode for the texturesheet;   override of the base function
    /// @param mirroring the selected mirror mode (0 = none, 1 = horizontal, 2 = vertical, 3 = both)
    virtual void set_mirroring(float mirroring);

    virtual void set_rotation(float rotation);

    virtual void set_position(float x, float y);
    virtual void move(float x, float y);

    virtual void init();

    virtual void draw();

    virtual void free();
};