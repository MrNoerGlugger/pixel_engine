#include "GamepadHandling.hpp"

void GamepadHandling::check_gamepad(SDL_Gamepad* gamepad) {
    for (auto it = input::input_map.begin(); it != input::input_map.end(); it++) {
        switch (it->second.data.type)
        {
        case GAMEPAD_BUTTON:
            input::update_input_state(it->second, SDL_GetGamepadButton(gamepad, it->second.data.gamepad_button));
            break;

        case GAMEPAD_AXIS:
            input::update_input_state(it->second, SDL_GetGamepadAxis(gamepad, it->second.data.axis.axis) * it->second.data.axis.direction > 4000);
            break;

        default:
            break;
        }
    }
}

void GamepadHandling::check_gamepads() {
    for (auto gamepad : input::gamepads) {
        check_gamepad(gamepad);
    }
}

void GamepadHandling::update_gamepads() {
    int gamepad_count;
    SDL_JoystickID* joysticks = SDL_GetGamepads(&gamepad_count);

    if (gamepad_count == 0) {
        return;
    }

    for (auto gamepad : input::gamepads) {
        SDL_CloseGamepad(gamepad);
    }
    input::gamepads.clear();
    for (int i = 0; i < gamepad_count; i++) {
        if (!SDL_IsGamepad(joysticks[i])) {
            continue;
        }
        SDL_Gamepad* gamepad = SDL_OpenGamepad(joysticks[i]);
        if (gamepad == NULL) {
            continue;
        }
        input::gamepads.push_back(gamepad);
    }
    SDL_free(joysticks);
}