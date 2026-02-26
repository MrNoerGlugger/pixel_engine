#pragma once

#include "Drawable.hpp"

class BasicRhombus : public Drawable {

public:
    BasicRhombus();
    virtual ~BasicRhombus() {
        this->free();
    }

    void init();

    void draw();

    void free();
};