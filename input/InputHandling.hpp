#pragma once

#include <iostream>

#include "InputBase.hpp"
#include "GamepadHandling.hpp"
#include "../functional_base/GPUAccessables.hpp"

#include <SDL3/SDL.h>

class InputHandling {

    void check_buttons();

    void update_mouse_state();

public:
    InputHandling();

    void process_input();
};