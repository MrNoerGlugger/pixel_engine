#pragma once

#include <iostream>
#include <vector>
#include <map>

#include <SDL3/SDL.h>

using std::vector;
using std::pair;
using std::map;
using std::multimap;

typedef enum InputType {
    SCANCODE,
    KEYCODE,
    MOUSE_BUTTON,
    GAMEPAD_BUTTON,
    GAMEPAD_AXIS
} InputType;

typedef enum Direction{
    UP_OR_LEFT = -1,
    DOWN_OR_RIGHT = 1
} Direction;

struct GamepadAxisInput {
    SDL_GamepadAxis axis;
    Direction direction;
};

struct InputData {
    InputType type;
    union {
        SDL_Scancode scancode;
        SDL_Keycode keycode;
        unsigned char mouse_button;
        SDL_GamepadButton gamepad_button;
        GamepadAxisInput axis;
    };
};

typedef enum ButtonState {
    EVENT_START,
    EVENT_HOLD,
    EVENT_REPEAT,
    EVENT_END,
    EVENT_NONE
} ButtonState;

struct Input {
    InputData data;
    ButtonState state;
    ButtonState activation_state = EVENT_NONE;
    int repeat_delay = -1;
    int repeat_delay_start = -1;
    bool repeated_once = false;
    unsigned long int start_time;
};


typedef enum Action {
    CLOSE,
    HOLD_TO_CLOSE,
    CONFIRM,
    ATTACK,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SKIP_TURN,
    SPEED_UP,
    SHOW_GRID,
    RETRY
} Action;

namespace input {
    extern SDL_FPoint mouse_pos;
    extern SDL_FPoint mouse_pos_old;
    extern bool mouse_cursor;

    extern SDL_MouseButtonFlags mouse_state;
    extern const bool* keyboard_state;
    extern SDL_Keymod mod_state;
    extern vector<SDL_Gamepad*> gamepads;

    extern multimap<Action, Input> input_map;

    extern void update_input_state(Input& input, bool state);
    extern void register_input(Action action, InputData data, int repeat_delay = -1, int repeat_delay_start = -1, ButtonState activation_state = EVENT_START);
    extern bool check_input(Action action);
}
