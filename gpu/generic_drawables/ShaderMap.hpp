#pragma once

#include <Texture.hpp>

class ShaderMap : public Texture {
public:
    virtual ~ShaderMap() {
        this->free();
    }

    virtual void create_graphics_pipeline();

    virtual void init();

    virtual void draw();

    virtual void free();
};