#include "InputBase.hpp"

SDL_FPoint input::mouse_pos = {0.0f, 0.0f};
SDL_FPoint input::mouse_pos_old = {0.0f, 0.0f};
bool input::mouse_cursor = false;

SDL_MouseButtonFlags input::mouse_state = 0;
const bool* input::keyboard_state;
SDL_Keymod input::mod_state;
vector<SDL_Gamepad*> input::gamepads;

multimap<Action, Input> input::input_map;

void input::update_input_state(Input& input, bool state) {
    if (!state) {
        input.repeated_once = false;
        if (input.state == EVENT_NONE) {
            return;
        }
        if (input.state == EVENT_END) {
            input.state = EVENT_NONE;
            return;
        }
        input.state = EVENT_END;
        return;
    }
    if (input.state == EVENT_NONE || input.state == EVENT_END) {
        input.state = EVENT_START;
        input.start_time = SDL_GetTicks();
        return;
    }
    if (input.state == EVENT_START || input.state == EVENT_REPEAT) {
        input.state = EVENT_HOLD;
        return;
    }
    if (input.repeat_delay == -1) {
        return;
    }
    if (input.repeat_delay_start == -1 || input.repeated_once) {
        if (SDL_GetTicks() - input.start_time > input.repeat_delay) {
            input.state = EVENT_REPEAT;
            input.start_time = SDL_GetTicks();
        }
        return;
    }
    if (SDL_GetTicks() - input.start_time > input.repeat_delay_start) {
        input.state = EVENT_REPEAT;
        input.start_time = SDL_GetTicks();
        input.repeated_once = true;
    }
}

void input::register_input(Action action, InputData data, int repeat_delay, int repeat_delay_start, ButtonState activation_state) {
    Input input_temp;
    input_temp.data = data;
    input_temp.activation_state = activation_state;
    input_temp.repeat_delay = repeat_delay;
    input_temp.repeat_delay_start = repeat_delay_start;
    input::input_map.insert({action, input_temp});
}

bool input::check_input(Action action) {
    if (input::input_map.count(action) == 0) {
        return false;
    }
    for (auto it = input::input_map.find(action); it != input::input_map.end(); it++) {
        if (it->first != action) {
            break;
        }
        if (it->second.state == it->second.activation_state || it->second.state == EVENT_REPEAT) {
            return true;
        }
    }
    return false;
}