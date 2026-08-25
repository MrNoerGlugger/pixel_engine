#include "InputHandling.hpp"

InputHandling::InputHandling() {
    GamepadHandling::update_gamepads();
}


void InputHandling::check_buttons() {
    for (auto it = input::input_map.begin(); it != input::input_map.end(); it++) {
        switch (it->second.data.type)
        {
        case SCANCODE:
            input::update_input_state(it->second, input::keyboard_state[it->second.data.scancode]);
            break;

        case KEYCODE:
            input::update_input_state(it->second, input::keyboard_state[SDL_GetScancodeFromKey(it->second.data.keycode, &input::mod_state)]);
            break;

        case MOUSE_BUTTON:
            input::update_input_state(it->second, input::mouse_state & SDL_BUTTON_MASK(it->second.data.mouse_button));
            break;
        
        default:
            break;
        }
    }
}

void InputHandling::update_mouse_state() {
    input::mouse_pos_old = input::mouse_pos;
    input::mouse_state = SDL_GetMouseState(&input::mouse_pos.x, &input::mouse_pos.y);
    input::mouse_pos.x /= graphics::zoom[0]; input::mouse_pos.y /= graphics::zoom[1];
    if (input::mouse_pos_old.x != input::mouse_pos.x || input::mouse_pos_old.y != input::mouse_pos.y) {
        if (!input::mouse_cursor) {
            SDL_ShowCursor();
        }
        input::mouse_cursor = true;
    }
}

void InputHandling::process_input() {
    update_mouse_state();
    input::keyboard_state = SDL_GetKeyboardState(NULL);
    input::mod_state = SDL_GetModState(); 
    check_buttons();
    GamepadHandling::check_gamepads();
    if (input::check_input(MOVE_UP) || input::check_input(MOVE_DOWN) || input::check_input(MOVE_LEFT) || input::check_input(MOVE_RIGHT)) {
        if (input::mouse_cursor) {
            SDL_HideCursor();
        }
        input::mouse_cursor = false;
    }
}