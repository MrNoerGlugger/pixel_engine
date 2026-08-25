#pragma once

#include "InputBase.hpp"

class GamepadHandling {

    static void check_gamepad(SDL_Gamepad* gamepad);

public:
    static void check_gamepads();

    static void update_gamepads();
};