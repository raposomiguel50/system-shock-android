/*

Copyright (C) 2015-2018 Night Dive Studios, LLC.
Copyright (C) 2019 Shockolate Project

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

//
// DG 2018: (eventually) SDL versions of the functions previously in kbMac.c, mouse.c and kbcook.c
//

#include "lg.h"
#include "kb.h"
#include "mouse.h"
#include "sdl_events.h"
#include <stdlib.h>
#include <SDL.h>
#include <OpenGL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;

bool fullscreenActive = false;

static void toggleFullScreen() {
    fullscreenActive = !fullscreenActive;
    SDL_SetWindowFullscreen(window, fullscreenActive ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

    if (!(SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED))
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

// current state of the keys, based on the SystemShock/Mac Keycodes (sshockKeyStates[keyCode] has the state for that
// key) set at the beginning of each frame in pump_events()
uchar sshockKeyStates[256];

enum { kNumKBevents = 128, kNumMouseEvents = 128 };

// queue keyboard events, created in pump_events(), consumed by kb_next()
static kbs_event kbEvents[kNumKBevents];
static int nextKBevent = 0; // where next to insert (also, if 0 there are no events)

static void addKBevent(const kbs_event *ev) {
    if (nextKBevent < kNumKBevents) {
        kbEvents[nextKBevent] = *ev;
        ++nextKBevent;
    } else {
        // printf("WTF, the kbEvents queue is full?!");
        // drop the oldest event
        memmove(&kbEvents[0], &kbEvents[1], sizeof(kbs_event) * (kNumKBevents - 1));
        kbEvents[kNumKBevents - 1] = *ev;
    }
}

// same for mouse events, also created in pump_events(), consumed by mouse_next()
static ss_mouse_event mouseEvents[kNumMouseEvents];
static int nextMouseEvent = 0;

// latest mouse state as input for MousePollProc() in mouse.c
ss_mouse_event latestMouseEvent;

static void addMouseEvent(const ss_mouse_event *ev) {
    latestMouseEvent = *ev;

    if (nextMouseEvent < kNumMouseEvents) {
        mouseEvents[nextMouseEvent] = latestMouseEvent;
        ++nextMouseEvent;
    } else {
        // printf("WTF, the mouseEvents queue is full?!");
        // drop the oldest event
        memmove(&mouseEvents[0], &mouseEvents[1], sizeof(ss_mouse_event) * (kNumMouseEvents - 1));
        mouseEvents[kNumMouseEvents - 1] = latestMouseEvent;
    }
}

static uchar sdlKeyCodeToSSHOCKkeyCode(SDL_Keycode kc) {
    // apparently System Shock uses the same keycodes as Mac
    // which are luckily documented, see
    // see http://snipplr.com/view/42797/
    // and https://stackoverflow.com/a/16125341
    // see also GameSrc/movekeys.c for a very short list

    // printf("sdlKeyCodeToSSHOCKkeyCode: %x\n", kc);

    switch (kc) {
    case SDLK_a:
        return 0x00; //  kVK_ANSI_A = 0x00,
    case SDLK_s:
        return 0x01; //  kVK_ANSI_S = 0x01,
    case SDLK_d:
        return 0x02; //  kVK_ANSI_D = 0x02,
    case SDLK_f:
        return 0x03; //  kVK_ANSI_F = 0x03,
    case SDLK_h:
        return 0x04; //  kVK_ANSI_H = 0x04,
    case SDLK_g:
        return 0x05; //  kVK_ANSI_G = 0x05,
    case SDLK_z:
        return 0x06; //  kVK_ANSI_Z = 0x06,
    case SDLK_x:
        return 0x07; //  kVK_ANSI_X = 0x07,
    case SDLK_c:
        return 0x08; //  kVK_ANSI_C = 0x08,
    case SDLK_v:
        return 0x09; //  kVK_ANSI_V = 0x09,
    case SDLK_b:
        return 0x0B; //  kVK_ANSI_B = 0x0B,
    case SDLK_q:
        return 0x0C; //  kVK_ANSI_Q = 0x0C,
    case SDLK_w:
        return 0x0D; //  kVK_ANSI_W = 0x0D,
    case SDLK_e:
        return 0x0E; //  kVK_ANSI_E = 0x0E,
    case SDLK_r:
        return 0x0F; //  kVK_ANSI_R = 0x0F,
    case SDLK_y:
        return 0x10; //  kVK_ANSI_Y = 0x10,
    case SDLK_t:
        return 0x11; //  kVK_ANSI_T = 0x11,
    case SDLK_1:
        return 0x12; //  kVK_ANSI_1 = 0x12,
    case SDLK_2:
        return 0x13; //  kVK_ANSI_2 = 0x13,
    case SDLK_3:
        return 0x14; //  kVK_ANSI_3 = 0x14,
    case SDLK_4:
        return 0x15; //  kVK_ANSI_4 = 0x15,
    case SDLK_6:
        return 0x16; //  kVK_ANSI_6 = 0x16,
    case SDLK_5:
        return 0x17; //  kVK_ANSI_5 = 0x17,
    case SDLK_EQUALS:
        return 0x18; //  kVK_ANSI_Equal = 0x18,
    case SDLK_9:
        return 0x19; //  kVK_ANSI_9 = 0x19,
    case SDLK_7:
        return 0x1A; //  kVK_ANSI_7 = 0x1A,
    case SDLK_MINUS:
        return 0x1B; //  kVK_ANSI_Minus = 0x1B,
    case SDLK_8:
        return 0x1C; //  kVK_ANSI_8 = 0x1C,
    case SDLK_0:
        return 0x1D; //  kVK_ANSI_0 = 0x1D,
    case SDLK_RIGHTBRACKET:
        return 0x1E; //  kVK_ANSI_RightBracket = 0x1E,
    case SDLK_o:
        return 0x1F; //  kVK_ANSI_O = 0x1F,
    case SDLK_u:
        return 0x20; //  kVK_ANSI_U = 0x20,
    case SDLK_LEFTBRACKET:
        return 0x21; //  kVK_ANSI_LeftBracket = 0x21,
    case SDLK_i:
        return 0x22; //  kVK_ANSI_I = 0x22,
    case SDLK_p:
        return 0x23; //  kVK_ANSI_P = 0x23,
    case SDLK_l:
        return 0x25; //  kVK_ANSI_L = 0x25,
    case SDLK_j:
        return 0x26; //  kVK_ANSI_J = 0x26,
    case SDLK_QUOTE:
        return 0x27; //  kVK_ANSI_Quote = 0x27, // TODO: or QUOTEDBL ?
    case SDLK_k:
        return 0x28; //  kVK_ANSI_K = 0x28,
    case SDLK_SEMICOLON:
        return 0x29; //  kVK_ANSI_Semicolon = 0x29,
    case SDLK_BACKSLASH:
        return 0x2A; //  kVK_ANSI_Backslash = 0x2A,
    case SDLK_COMMA:
        return 0x2B; //  kVK_ANSI_Comma = 0x2B,
    case SDLK_SLASH:
        return 0x2C; //  kVK_ANSI_Slash = 0x2C,
    case SDLK_n:
        return 0x2D; //  kVK_ANSI_N = 0x2D,
    case SDLK_m:
        return 0x2E; //  kVK_ANSI_M = 0x2E,
    case SDLK_PERIOD:
        return 0x2F; //  kVK_ANSI_Period = 0x2F,
    case SDLK_BACKQUOTE:
        return 0x32; //  kVK_ANSI_Grave = 0x32, // TODO: really?
    case SDLK_KP_DECIMAL:
        return 0x41; //  kVK_ANSI_KeypadDecimal   = 0x41,
    case SDLK_KP_MULTIPLY:
        return 0x43; //  kVK_ANSI_KeypadMultiply = 0x43,
    case SDLK_KP_PLUS:
        return 0x45; //  kVK_ANSI_KeypadPlus = 0x45,
    case SDLK_KP_CLEAR:
        return 0x47; //  kVK_ANSI_KeypadClear = 0x47,
    case SDLK_KP_DIVIDE:
        return 0x4B; //  kVK_ANSI_KeypadDivide = 0x4B,
    case SDLK_KP_ENTER:
        return 0x4C; //  kVK_ANSI_KeypadEnter   = 0x4C, aka _ENTER2_
    case SDLK_KP_MINUS:
        return 0x4E; //  kVK_ANSI_KeypadMinus   = 0x4E,
    case SDLK_KP_EQUALS:
        return 0x51; //  kVK_ANSI_KeypadEquals = 0x51,
    case SDLK_KP_0:
        return 0x52; //  kVK_ANSI_Keypad0 = 0x52,
    case SDLK_KP_1:
        return 0x53; //  kVK_ANSI_Keypad1 = 0x53, aka _END2_
    case SDLK_KP_2:
        return 0x54; //  kVK_ANSI_Keypad2 = 0x54, aka _DOWN2_
    case SDLK_KP_3:
        return 0x55; //  kVK_ANSI_Keypad3 = 0x55, aka _PGDN2_
    case SDLK_KP_4:
        return 0x56; //  kVK_ANSI_Keypad4 = 0x56, aka _LEFT2_
    case SDLK_KP_5:
        return 0x57; //  kVK_ANSI_Keypad5 = 0x57, aka _PAD5_
    case SDLK_KP_6:
        return 0x58; //  kVK_ANSI_Keypad6 = 0x58, aka _RIGHT2_
    case SDLK_KP_7:
        return 0x59; //  kVK_ANSI_Keypad7 = 0x59, aka _HOME2_
    case SDLK_KP_8:
        return 0x5B; //  kVK_ANSI_Keypad8 = 0x5B, aka _UP2_
    case SDLK_KP_9:
        return 0x5C; //  kVK_ANSI_Keypad9 = 0x5C, aka _PGUP2_

    // keycodes for keys that are independent of keyboard layout
    case SDLK_RETURN:
        return 0x24; //  kVK_Return  = 0x24,
    case SDLK_TAB:
        return 0x30; //  kVK_Tab     = 0x30,
    case SDLK_SPACE:
        return 0x31; //  kVK_Space   = 0x31,
    case SDLK_DELETE:
        return 0x33; //  kVK_Delete  = 0x33,
    case SDLK_BACKSPACE:
        return 0x33; //  kVK_Delete  = 0x33,
    case SDLK_ESCAPE:
        return 0x35; //  kVK_Escape  = 0x35,

        //    returning these is unnecessary and can cause keypresses to be missed
        //    (esp keys with modifiers)
        // case SDLK_LGUI : // fall-through
        // case SDLK_RGUI : return 0x37; //  kVK_Command = 0x37, // FIXME: I think command is the windows/meta key?
        // case SDLK_LSHIFT : return 0x38; //  kVK_Shift   = 0x38,
        // case SDLK_CAPSLOCK : return 0x39; //  kVK_CapsLock= 0x39,
        // case SDLK_LALT : return 0x3A; //  kVK_Option  = 0x3A, Option == Aalt
        // case SDLK_LCTRL : return 0x3B; //  kVK_Control = 0x3B,
        // case SDLK_RSHIFT : return 0x3C; //  kVK_RightShift  = 0x3C,
        // case SDLK_RALT : return 0x3D; //  kVK_RightOption = 0x3D,
        // case SDLK_RCTRL : return 0x3E; //  kVK_RightControl = 0x3E,

    // case SDLK_ : return 0x3F; //  kVK_Function = 0x3F, // TODO: what's this?
    case SDLK_F17:
        return 0x40; //  kVK_F17 = 0x40,
    case SDLK_VOLUMEUP:
        return 0x48; //  kVK_VolumeUp = 0x48,
    case SDLK_VOLUMEDOWN:
        return 0x49; //  kVK_VolumeDown = 0x49,
    case SDLK_MUTE:
        return 0x4A; //  kVK_Mute = 0x4A,
    case SDLK_F18:
        return 0x4F; //  kVK_F18 = 0x4F,
    case SDLK_F19:
        return 0x50; //  kVK_F19 = 0x50,
    case SDLK_F20:
        return 0x5A; //  kVK_F20 = 0x5A,
    case SDLK_F5:
        return 0x60; //  kVK_F5  = 0x60,
    case SDLK_F6:
        return 0x61; //  kVK_F6  = 0x61,
    case SDLK_F7:
        return 0x62; //  kVK_F7  = 0x62,
    case SDLK_F3:
        return 0x63; //  kVK_F3  = 0x63,
    case SDLK_F8:
        return 0x64; //  kVK_F8  = 0x64,
    case SDLK_F9:
        return 0x65; //  kVK_F9  = 0x65,
    case SDLK_F11:
        return 0x67; //  kVK_F11 = 0x67,
    case SDLK_F13:
        return 0x69; //  kVK_F13 = 0x69,
    case SDLK_F16:
        return 0x6A; //  kVK_F16 = 0x6A,
    case SDLK_F14:
        return 0x6B; //  kVK_F14 = 0x6B,
    case SDLK_F10:
        return 0x6D; //  kVK_F10 = 0x6D,
    case SDLK_F12:
        return 0x6F; //  kVK_F12 = 0x6F,
    case SDLK_F15:
        return 0x71; //  kVK_F15 = 0x71,
    case SDLK_HELP:
        return 0x72; //  kVK_Help = 0x72,
    case SDLK_HOME:
        return 0x73; //  kVK_Home = 0x73,
    case SDLK_PAGEUP:
        return 0x74; //  kVK_PageUp = 0x74,
    // case SDLK_ : return 0x75; //  kVK_ForwardDelete = 0x75, // TODO: what's this?
    case SDLK_F4:
        return 0x76; //  kVK_F4 = 0x76,
    case SDLK_END:
        return 0x77; //  kVK_End = 0x77,
    case SDLK_F2:
        return 0x78; //  kVK_F2 = 0x78,
    case SDLK_PAGEDOWN:
        return 0x79; //  kVK_PageDown = 0x79,
    case SDLK_F1:
        return 0x7A; //  kVK_F1 = 0x7A,
    case SDLK_LEFT:
        return 0x7B; //  kVK_LeftArrow  = 0x7B, aka _LEFT_
    case SDLK_RIGHT:
        return 0x7C; //  kVK_RightArrow = 0x7C, aka _RIGHT
    case SDLK_DOWN:
        return 0x7D; //  kVK_DownArrow  = 0x7D, aka _DOWN_
    case SDLK_UP:
        return 0x7E; //  kVK_UpArrow    = 0x7E, aka _UP_
    default:
        return KBC_NONE;
    }
}

static SDL_GameController *rp5np_controller = NULL;
static SDL_JoystickID rp5np_controller_instance = -1;
static int rp5np_last_scan_count = -1;

static Sint16 rp5np_left_x = 0;
static Sint16 rp5np_left_y = 0;
static bool rp5np_lx_left = false;
static bool rp5np_lx_right = false;
static bool rp5np_ly_up = false;
static bool rp5np_ly_down = false;
static bool rp5np_setup_context = false;
static bool rp5np_shift_held = false;

#ifdef __ANDROID__
static rp5np_text_input_handler rp5np_text_handler = NULL;
static void *rp5np_text_handler_userdata = NULL;
static rp5np_controller_action_handler rp5np_action_handler = NULL;
static void *rp5np_action_handler_userdata = NULL;

void rp5np_set_text_input_handler(rp5np_text_input_handler handler, void *userdata) {
    const bool changed = (rp5np_text_handler != handler) || (rp5np_text_handler_userdata != userdata);
    rp5np_text_handler = handler;
    rp5np_text_handler_userdata = userdata;

    if (changed)
        SDL_Log("RP5NP_TEXT_INPUT_HANDLER active=%d", handler != NULL ? 1 : 0);
}

void rp5np_set_controller_action_handler(rp5np_controller_action_handler handler, void *userdata) {
    const bool changed = (rp5np_action_handler != handler) || (rp5np_action_handler_userdata != userdata);
    rp5np_action_handler = handler;
    rp5np_action_handler_userdata = userdata;

    if (changed)
        SDL_Log("RP5NP_CONTROLLER_ACTION_HANDLER active=%d", handler != NULL ? 1 : 0);
}

static void rp5np_dispatch_action(rp5np_controller_action action) {
    if (rp5np_action_handler != NULL)
        rp5np_action_handler(action, rp5np_action_handler_userdata);
}
#endif

static Sint16 rp5np_right_x = 0;
static Sint16 rp5np_right_y = 0;
static bool rp5np_right_stick_cursor_mode = false;
static bool rp5np_right_stick_active = false;
static bool rp5np_trigger_left_down = false;
static bool rp5np_trigger_right_down = false;
static uchar rp5np_mouse_buttons = 0;

int MouseX;
int MouseY;
int MouseChaosX;
int MouseChaosY;

static void rp5np_emit_key(SDL_Keycode kc, bool down) {
    uchar c = sdlKeyCodeToSSHOCKkeyCode(kc);
    if (c == KBC_NONE)
        return;

    kbs_event keyEvent = {0};
    keyEvent.code = c;
    keyEvent.state = down ? KBS_DOWN : KBS_UP;
    keyEvent.modifiers = rp5np_shift_held ? KB_MOD_SHIFT : 0;

    if (kc == SDLK_RETURN) keyEvent.ascii = 13;
    else if (kc == SDLK_ESCAPE) keyEvent.ascii = 27;
    else if (kc == SDLK_TAB) keyEvent.ascii = 9;
    else if (kc == SDLK_SPACE) keyEvent.ascii = 32;
    else if (kc >= 32 && kc <= 126) keyEvent.ascii = (uchar)kc;

    addKBevent(&keyEvent);
    sshockKeyStates[c] = down ? (keyEvent.modifiers | KB_MOD_PRESSED) : 0;
}

static void rp5np_set_shift(bool down) {
    if (rp5np_shift_held == down)
        return;

    rp5np_shift_held = down;
    for (int i = 0; i < 256; ++i) {
        if (sshockKeyStates[i] != 0) {
            if (down)
                sshockKeyStates[i] |= KB_MOD_SHIFT;
            else
                sshockKeyStates[i] &= ~KB_MOD_SHIFT;
        }
    }
    SDL_Log("RP5NP_CONTROLLER_RUN_MODIFIER down=%d", down ? 1 : 0);
}

static void rp5np_axis_key(bool *state, bool active, SDL_Keycode key) {
    if (*state == active)
        return;
    *state = active;
    rp5np_emit_key(key, active);
}

static void rp5np_release_left_stick(bool setup_context) {
    rp5np_axis_key(&rp5np_lx_left, false, setup_context ? SDLK_LEFT : SDLK_a);
    rp5np_axis_key(&rp5np_lx_right, false, setup_context ? SDLK_RIGHT : SDLK_d);
    rp5np_axis_key(&rp5np_ly_up, false, setup_context ? SDLK_UP : SDLK_w);
    rp5np_axis_key(&rp5np_ly_down, false, setup_context ? SDLK_DOWN : SDLK_s);
}

void rp5np_controller_set_setup_context(bool setup_context) {
    if (rp5np_setup_context == setup_context)
        return;

    rp5np_release_left_stick(rp5np_setup_context);
    rp5np_setup_context = setup_context;
    rp5np_right_stick_active = false;

    if (setup_context) {
        rp5np_set_shift(false);
        rp5np_right_stick_cursor_mode = true;
#ifdef __ANDROID__
        rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_CURSOR_MODE);
#endif
    } else {
        /* RP5 handheld default: right stick is camera look. View/Select can
           explicitly switch it to the legacy UI cursor when required. */
        rp5np_right_stick_cursor_mode = false;
    }

    SDL_Log("RP5NP_CONTROLLER_CONTEXT mode=%s right_stick=%s",
            setup_context ? "SETUP" : "GAME",
            (setup_context || rp5np_right_stick_cursor_mode) ? "CURSOR" : "LOOK");
}

static void rp5np_apply_left_stick(void) {
    const bool setup_context = rp5np_setup_context;

    rp5np_axis_key(&rp5np_lx_left, rp5np_left_x < -16000, setup_context ? SDLK_LEFT : SDLK_a);
    rp5np_axis_key(&rp5np_lx_right, rp5np_left_x > 16000, setup_context ? SDLK_RIGHT : SDLK_d);
    rp5np_axis_key(&rp5np_ly_up, rp5np_left_y < -16000, setup_context ? SDLK_UP : SDLK_w);
    rp5np_axis_key(&rp5np_ly_down, rp5np_left_y > 16000, setup_context ? SDLK_DOWN : SDLK_s);
}

static bool rp5np_require_button(SDL_GameController *controller, SDL_GameControllerButton button, const char *name) {
    const bool present = SDL_GameControllerHasButton(controller, button) == SDL_TRUE;
    SDL_GameControllerButtonBind bind = SDL_GameControllerGetBindForButton(controller, button);
    SDL_Log("RP5NP_CONTROLLER_BIND_BUTTON name=%s present=%d bindType=%d",
            name, present ? 1 : 0, (int)bind.bindType);
    return present;
}

static bool rp5np_require_axis(SDL_GameController *controller, SDL_GameControllerAxis axis, const char *name) {
    const bool present = SDL_GameControllerHasAxis(controller, axis) == SDL_TRUE;
    SDL_GameControllerButtonBind bind = SDL_GameControllerGetBindForAxis(controller, axis);
    SDL_Log("RP5NP_CONTROLLER_BIND_AXIS name=%s present=%d bindType=%d",
            name, present ? 1 : 0, (int)bind.bindType);
    return present;
}

static void rp5np_validate_controller(SDL_GameController *controller) {
    bool ok = true;

    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_A, "A");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_B, "B");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_X, "X");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_Y, "Y");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_BACK, "BACK_SELECT");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_START, "START");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_LEFTSTICK, "L3");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_RIGHTSTICK, "R3");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER, "L1");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, "R1");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_DPAD_UP, "DPAD_UP");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN, "DPAD_DOWN");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT, "DPAD_LEFT");
    ok &= rp5np_require_button(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, "DPAD_RIGHT");

    ok &= rp5np_require_axis(controller, SDL_CONTROLLER_AXIS_LEFTX, "LEFT_X");
    ok &= rp5np_require_axis(controller, SDL_CONTROLLER_AXIS_LEFTY, "LEFT_Y");
    ok &= rp5np_require_axis(controller, SDL_CONTROLLER_AXIS_RIGHTX, "RIGHT_X");
    ok &= rp5np_require_axis(controller, SDL_CONTROLLER_AXIS_RIGHTY, "RIGHT_Y");
    ok &= rp5np_require_axis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT, "L2");
    ok &= rp5np_require_axis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT, "R2");

    SDL_Log(ok ? "RP5NP_CONTROLLER_CAPS_OK profile=RP5_REMAKE_V1"
               : "RP5NP_CONTROLLER_CAPS_FAIL profile=RP5_REMAKE_V1");
}

static void rp5np_open_controller(int device_index) {
    if (rp5np_controller != NULL || !SDL_IsGameController(device_index))
        return;

    char *mapping = SDL_GameControllerMappingForDeviceIndex(device_index);
    if (mapping != NULL) {
        SDL_Log("RP5NP_CONTROLLER_MAPPING index=%d mapping=%s", device_index, mapping);
        SDL_free(mapping);
    }

    rp5np_controller = SDL_GameControllerOpen(device_index);
    if (rp5np_controller == NULL) {
        SDL_Log("RP5NP_CONTROLLER_OPEN_FAIL index=%d error=%s", device_index, SDL_GetError());
        return;
    }

    SDL_Joystick *joy = SDL_GameControllerGetJoystick(rp5np_controller);
    rp5np_controller_instance = SDL_JoystickInstanceID(joy);
    SDL_Log("RP5NP_CONTROLLER_OPEN name=%s instance=%d vendor=%u product=%u",
            SDL_GameControllerName(rp5np_controller),
            (int)rp5np_controller_instance,
            (unsigned)SDL_GameControllerGetVendor(rp5np_controller),
            (unsigned)SDL_GameControllerGetProduct(rp5np_controller));
    rp5np_validate_controller(rp5np_controller);
#ifdef __ANDROID__
    SDL_Log("RP5NP_CONTROLLER_PROFILE=RP5_REMAKE_V1");
#endif
}

static void rp5np_open_first_controller(void) {
    if (rp5np_controller != NULL)
        return;
    if ((SDL_WasInit(SDL_INIT_GAMECONTROLLER) & SDL_INIT_GAMECONTROLLER) == 0)
        return;

    const int count = SDL_NumJoysticks();
    if (count != rp5np_last_scan_count) {
        SDL_Log("RP5NP_CONTROLLER_SCAN count=%d", count);
        rp5np_last_scan_count = count;
    }

    for (int i = 0; i < count; ++i) {
        if (SDL_IsGameController(i)) {
            rp5np_open_controller(i);
            if (rp5np_controller != NULL)
                return;
        }
    }
}

static void rp5np_refresh_logical_size(void) {
    if (renderer == NULL)
        return;

    int lw = 0, lh = 0;
    int ow = 0, oh = 0;
    SDL_RenderGetLogicalSize(renderer, &lw, &lh);
    SDL_GetRendererOutputSize(renderer, &ow, &oh);

    if (lw > 0 && lh > 0) {
        SDL_RenderSetViewport(renderer, NULL);
        SDL_RenderSetLogicalSize(renderer, lw, lh);
        SDL_Log("RP5NP_LOGICAL_SIZE_REAPPLIED logical=%dx%d output=%dx%d", lw, lh, ow, oh);
    }
}

static void rp5np_emit_mouse_button(bool left_button, bool down) {
    const uchar bit = (uchar)(1u << (left_button ? MOUSE_LBUTTON : MOUSE_RBUTTON));
    const bool was_down = (rp5np_mouse_buttons & bit) != 0;
    if (was_down == down)
        return;

    if (down)
        rp5np_mouse_buttons |= bit;
    else
        rp5np_mouse_buttons &= (uchar)~bit;

    ss_mouse_event mouseEvent = {0};
    mouseEvent.type = left_button ? (down ? MOUSE_LDOWN : MOUSE_LUP)
                                  : (down ? MOUSE_RDOWN : MOUSE_RUP);
    mouseEvent.x = (short)MouseX;
    mouseEvent.y = (short)MouseY;
    mouseEvent.buttons = rp5np_mouse_buttons;
    mouseEvent.timestamp = mouse_get_time();
    mouseEvent.modifiers = rp5np_shift_held ? 1 : 0;
    addMouseEvent(&mouseEvent);
    SDL_Log("RP5NP_CONTROLLER_MOUSE_BUTTON button=%s down=%d",
            left_button ? "LEFT" : "RIGHT", down ? 1 : 0);
}

#ifdef __ANDROID__
static void rp5np_emit_interact_double_click(void) {
    /* Original System Shock uses a left double-click for "use/interact".
       Emit the native input sequence so the existing UI double-click state
       machine performs the action at the current cursor/crosshair position. */
    rp5np_emit_mouse_button(true, true);
    rp5np_emit_mouse_button(true, false);
    rp5np_emit_mouse_button(true, true);
    rp5np_emit_mouse_button(true, false);
    SDL_Log("RP5NP_REMAKE_ACTION=INTERACT_DOUBLECLICK");
}
#endif

static void rp5np_update_trigger(bool *state, Sint16 value, bool left_button) {
    const bool active = *state ? (value > 7000) : (value > 14000);
    if (active == *state)
        return;
    *state = active;
    rp5np_emit_mouse_button(left_button, active);
}

static void rp5np_release_controller_inputs(void) {
    rp5np_release_left_stick(rp5np_setup_context);
    rp5np_left_x = 0;
    rp5np_left_y = 0;
    rp5np_right_x = 0;
    rp5np_right_y = 0;
    rp5np_right_stick_active = false;

    if (rp5np_trigger_left_down)
        rp5np_emit_mouse_button(true, false);
    if (rp5np_trigger_right_down)
        rp5np_emit_mouse_button(false, false);
    rp5np_trigger_left_down = false;
    rp5np_trigger_right_down = false;

    rp5np_set_shift(false);
}

void SetMouseXY(int mx, int my);

static int rp5np_stick_step(Sint16 value) {
    const int deadzone = 7000;
    const int max_step = 14;
    int magnitude = value < 0 ? -(int)value : (int)value;
    if (magnitude <= deadzone)
        return 0;
    int step = 1 + ((magnitude - deadzone) * (max_step - 1)) / (32767 - deadzone);
    return value < 0 ? -step : step;
}

static void rp5np_poll_right_stick(void) {
    if (rp5np_controller == NULL)
        return;

    /* Android controller motion is also polled each pump. This makes the
       analog path independent of event coalescing while retaining SDL's
       controller mapping/dead-zone semantics above this layer. */
    rp5np_right_x = SDL_GameControllerGetAxis(rp5np_controller, SDL_CONTROLLER_AXIS_RIGHTX);
    rp5np_right_y = SDL_GameControllerGetAxis(rp5np_controller, SDL_CONTROLLER_AXIS_RIGHTY);
}

static void rp5np_apply_right_stick(void) {
    if (rp5np_controller == NULL || renderer == NULL)
        return;

    const int dx = rp5np_stick_step(rp5np_right_x);
    const int dy = rp5np_stick_step(rp5np_right_y);
    const bool active = (dx != 0 || dy != 0);

    if (!active) {
        if (rp5np_right_stick_active) {
            SDL_Log("RP5NP_CONTROLLER_RIGHT_STICK active=0");
            rp5np_right_stick_active = false;
        }
        return;
    }

    const bool became_active = !rp5np_right_stick_active;
    if (became_active) {
        SDL_Log("RP5NP_CONTROLLER_RIGHT_STICK active=1 route=%s raw=%d,%d step=%d,%d",
                (!rp5np_setup_context && !rp5np_right_stick_cursor_mode) ? "LOOK" : "CURSOR",
                (int)rp5np_right_x, (int)rp5np_right_y, dx, dy);
        rp5np_right_stick_active = true;
    }

    if (!rp5np_setup_context && !rp5np_right_stick_cursor_mode) {
#ifdef __ANDROID__
        /* High-level GameSrc owns the actual freelook state. Dispatch once per
           stick engagement; View/Select resets the engagement when modes change. */
        if (became_active)
            rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_LOOK_MODE);
#endif
        MouseChaosX += dx;
        MouseChaosY += dy;
        return;
    }

    int lw = 0, lh = 0;
    SDL_RenderGetLogicalSize(renderer, &lw, &lh);
    if (lw <= 0 || lh <= 0)
        return;

    int nx = MouseX + dx;
    int ny = MouseY + dy;
    if (nx < 0) nx = 0;
    if (ny < 0) ny = 0;
    if (nx >= lw) nx = lw - 1;
    if (ny >= lh) ny = lh - 1;

    if (nx == MouseX && ny == MouseY)
        return;

    /* Use the same state transition as genuine SDL mouse motion instead of
       assigning MouseX/MouseY directly. This keeps cursor visibility/focus
       behavior identical for touchpad and right-stick UI cursor motion. */
    SetMouseXY(nx, ny);

    ss_mouse_event mouseEvent = {0};
    mouseEvent.type = MOUSE_MOTION;
    mouseEvent.x = (short)MouseX;
    mouseEvent.y = (short)MouseY;
    mouseEvent.buttons = rp5np_mouse_buttons;
    mouseEvent.timestamp = mouse_get_time();
    mouseEvent.modifiers = rp5np_shift_held ? 1 : 0;
    addMouseEvent(&mouseEvent);
}

extern bool MouseCaptured;

void SetMouseXY(int mx, int my) {
    int physical_width, physical_height;
    SDL_GetWindowSize(window, &physical_width, &physical_height);

    int w, h;
    SDL_RenderGetLogicalSize(renderer, &w, &h);

    float scale_x = (float)physical_width / w;
    float scale_y = (float)physical_height / h;

    int x, y;
    if (scale_x >= scale_y) {
        x = (physical_width - w * scale_x) / 2;
        y = 0;
    } else {
        x = 0;
        y = (physical_height - h * scale_y) / 2;
    }

    bool inside = (mx >= x && mx < x + w && my >= y && my < y + h);
    bool focus = (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS); //checking mouse focus isn't what we want here

    if (!inside && focus) {
        if (mx < x)
            mx = x;
        if (mx > x + w - 1)
            mx = x + w - 1;
        if (my < y)
            my = y;
        if (my > y + h - 1)
            my = y + h - 1;
    }

    if (focus) {
        MouseX = mx;
        MouseY = my;
    }

    SDL_ShowCursor((!focus || (!inside && !MouseCaptured)) ? SDL_ENABLE : SDL_DISABLE);
}

void get_mouselook_vel(int *vx, int *vy);

extern bool TriggerRelMouseMode;

static SDL_bool saved_rel_mouse = FALSE;

// same codes as returned by sdlKeyCodeToSSHOCKkeyCode()
uchar Ascii2Code[95] = {
    0x31, // space
    0x12, // !
    0x27, // "
    0x14, // #
    0x15, // $
    0x17, // %
    0x1A, // &
    0x27, // '
    0x19, // (
    0x1D, // )
    0x1C, // *
    0x18, // +
    0x2B, // ,
    0x1B, // -
    0x2F, // .
    0x2C, // /
    0x1D, // 0
    0x12, // 1
    0x13, // 2
    0x14, // 3
    0x15, // 4
    0x17, // 5
    0x16, // 6
    0x1A, // 7
    0x1C, // 8
    0x19, // 9
    0x29, // :
    0x29, // ;
    0x2B, // <
    0x18, // =
    0x2F, // >
    0x2C, // ?
    0x13, // @
    0x00, // A
    0x0B, // B
    0x08, // C
    0x02, // D
    0x0E, // E
    0x03, // F
    0x05, // G
    0x04, // H
    0x22, // I
    0x26, // J
    0x28, // K
    0x25, // L
    0x2E, // M
    0x2D, // N
    0x1F, // O
    0x23, // P
    0x0C, // Q
    0x0F, // R
    0x01, // S
    0x11, // T
    0x20, // U
    0x09, // V
    0x0D, // W
    0x07, // X
    0x10, // Y
    0x06, // Z
    0x21, // [
    0x2A, // backslash
    0x1E, // ]
    0x16, // ^
    0x1B, // _
    0x32, // `
    0x00, // a
    0x0B, // b
    0x08, // c
    0x02, // d
    0x0E, // e
    0x03, // f
    0x05, // g
    0x04, // h
    0x22, // i
    0x26, // j
    0x28, // k
    0x25, // l
    0x2E, // m
    0x2D, // n
    0x1F, // o
    0x23, // p
    0x0C, // q
    0x0F, // r
    0x01, // s
    0x11, // t
    0x20, // u
    0x09, // v
    0x0D, // w
    0x07, // x
    0x10, // y
    0x06, // z
    0x21, // {
    0x2A, // |
    0x1E, // }
    0x32  // ~
};

void pump_events(void) {
    SDL_Event ev;

#ifdef __ANDROID__
    rp5np_open_first_controller();
#endif

    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT:
            // a bit hacky at this place, but this would allow exiting the game via the window's [x] button
            exit(0); // TODO: I guess there is a better way.
            break;

        // TODO: really also handle key up here? the mac code apparently didn't, but where else do
        //       kbs_events with .state == KBS_UP come from?
        case SDL_KEYUP:
        case SDL_KEYDOWN: {
            uchar c = sdlKeyCodeToSSHOCKkeyCode(ev.key.keysym.sym);
            if (c != KBC_NONE) {
                kbs_event keyEvent = {0};

                keyEvent.code = c;
                keyEvent.ascii = 0;
                keyEvent.modifiers = 0;

                // https://wiki.libsdl.org/SDLKeycodeLookup
                // Keycodes for keys with printable characters are represented by the
                // character byte in parentheses. Keycodes without character representations
                // are determined by their scancode bitwise OR-ed with 1<<30 (0x40000000).

                if (ev.key.keysym.sym >= 0x08 && ev.key.keysym.sym <= 127)
                    keyEvent.ascii = ev.key.keysym.sym;
                else {
                    // use these invented "ascii" codes for hotkey system
                    // see MacSrc/Prefs.c
                    switch (ev.key.keysym.sym) {
                    case SDLK_F1:
                        keyEvent.ascii = 128 + 0;
                        break;
                    case SDLK_F2:
                        keyEvent.ascii = 128 + 1;
                        break;
                    case SDLK_F3:
                        keyEvent.ascii = 128 + 2;
                        break;
                    case SDLK_F4:
                        keyEvent.ascii = 128 + 3;
                        break;
                    case SDLK_F5:
                        keyEvent.ascii = 128 + 4;
                        break;
                    case SDLK_F6:
                        keyEvent.ascii = 128 + 5;
                        break;
                    case SDLK_F7:
                        keyEvent.ascii = 128 + 6;
                        break;
                    case SDLK_F8:
                        keyEvent.ascii = 128 + 7;
                        break;
                    case SDLK_F9:
                        keyEvent.ascii = 128 + 8;
                        break;
                    case SDLK_F10:
                        keyEvent.ascii = 128 + 9;
                        break;
                    case SDLK_F11:
                        keyEvent.ascii = 128 + 10;
                        break;
                    case SDLK_F12:
                        keyEvent.ascii = 128 + 11;
                        break;
                    case SDLK_KP_DIVIDE:
                        keyEvent.ascii = 128 + 12;
                        break;
                    case SDLK_KP_MULTIPLY:
                        keyEvent.ascii = 128 + 13;
                        break;
                    case SDLK_KP_MINUS:
                        keyEvent.ascii = 128 + 14;
                        break;
                    case SDLK_KP_PLUS:
                        keyEvent.ascii = 128 + 15;
                        break;
                    case SDLK_KP_ENTER:
                        keyEvent.ascii = 128 + 16;
                        break;
                    case SDLK_KP_DECIMAL:
                        keyEvent.ascii = 128 + 17;
                        break;
                    case SDLK_KP_0:
                        keyEvent.ascii = 128 + 18;
                        break;
                    }
                }

                Uint16 mod = ev.key.keysym.mod;

                if (mod & KMOD_SHIFT)
                    keyEvent.modifiers |= KB_MOD_SHIFT;
                if (mod & KMOD_CTRL)
                    keyEvent.modifiers |= KB_MOD_CTRL;
                if (mod & KMOD_ALT)
                    keyEvent.modifiers |= KB_MOD_ALT;

                if (ev.key.state == SDL_PRESSED) {
                    if (ev.key.keysym.sym == SDLK_RETURN && mod & KMOD_ALT) {
                        toggleFullScreen();
                        break;
                    }

                    // handle non-printable or ctrl'd or alt'd keys here
                    // other cases are handled by text input event below
                    if (ev.key.keysym.sym < 32 || ev.key.keysym.sym > 126 || (mod & KMOD_CTRL) || (mod & KMOD_ALT)) {
                        keyEvent.state = KBS_DOWN;
                        addKBevent(&keyEvent);

                        sshockKeyStates[c] = keyEvent.modifiers | KB_MOD_PRESSED;
                    }
                } else {
                    // key up following text input event case below is handled here

                    keyEvent.state = KBS_UP;
                    addKBevent(&keyEvent);

                    sshockKeyStates[c] = 0;
                }
            }

            // hack to allow pressing shift after move key
            // sets all current shock states in array to shifted or non-shifted
            if (ev.key.keysym.sym == SDLK_LSHIFT || ev.key.keysym.sym == SDLK_RSHIFT) {
                for (int i = 0; i < 256; i++)
                    if (sshockKeyStates[i]) {
                        if (ev.key.state == SDL_PRESSED)
                            sshockKeyStates[i] |= KB_MOD_SHIFT;
                        else
                            sshockKeyStates[i] &= ~KB_MOD_SHIFT;
                    }
            }
        } break;

        case SDL_TEXTINPUT: {
#ifdef __ANDROID__
            if (rp5np_text_handler != NULL) {
                const uint32_t direct_len = (uint32_t)strlen(ev.text.text);
                SDL_Log("RP5NP_TEXT_INPUT_DISPATCH bytes=%u", (unsigned int)direct_len);
                rp5np_text_handler(ev.text.text, rp5np_text_handler_userdata);
                break;
            }
#endif

            uint32_t len = strlen(ev.text.text);

            // for every utf8 char in null-terminated string
            for (uint32_t i = 0; i < len; i++) {
                int ch = ev.text.text[i];

                // ignore if non-printable key
                if (!isprint(ch))
                    continue;

                kbs_event keyEvent = {0};

                keyEvent.modifiers = 0;

                // if uppercase, lower it and set shift modifier
                if (isupper(ch)) {
                    ch = tolower(ch);
                    keyEvent.modifiers |= KB_MOD_SHIFT;
                }

                // get code for this printable ascii key
                int c = Ascii2Code[ch - 32];

                keyEvent.code = c;
                keyEvent.ascii = ch;

                // This is a key down event. Physical keyboards later generate SDL_KEYUP.
                // Android IME commits can arrive as text-only events, so synthesize the
                // matching release while the on-screen keyboard is visible to avoid
                // leaving typed player-name keys stuck into gameplay.
                keyEvent.state = KBS_DOWN;
                addKBevent(&keyEvent);

#ifdef __ANDROID__
                if (window != NULL && SDL_IsScreenKeyboardShown(window) == SDL_TRUE) {
                    kbs_event keyUpEvent = keyEvent;
                    keyUpEvent.state = KBS_UP;
                    addKBevent(&keyUpEvent);
                    sshockKeyStates[c] = 0;
                } else
#endif
                {
                    sshockKeyStates[c] = keyEvent.modifiers | KB_MOD_PRESSED;
                }
            }
        } break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            bool down = (ev.button.state == SDL_PRESSED);
            ss_mouse_event mouseEvent = {0};
            mouseEvent.type = 0;

            // TODO: the old mac code used to emulate right mouse clicks if space, enter, or return
            //       was pressed at the same time - do the same? (=> could check sshockKeyStates[])

            mouseEvent.buttons = 0;

            switch (ev.button.button) {
            case SDL_BUTTON_LEFT:
                mouseEvent.type = down ? MOUSE_LDOWN : MOUSE_LUP;
                mouseEvent.buttons |= down ? (1 << MOUSE_LBUTTON) : 0;
                break;

            case SDL_BUTTON_RIGHT:
                mouseEvent.type = down ? MOUSE_RDOWN : MOUSE_RUP;
                mouseEvent.buttons |= down ? (1 << MOUSE_RBUTTON) : 0;
                break;

                // case SDL_BUTTON_MIDDLE: // TODO: is this MOUSE_CDOWN/UP ?
                // break;
            }

            if (mouseEvent.type != 0) {
                bool shifted = ((SDL_GetModState() & KMOD_SHIFT) != 0);

                mouseEvent.x = MouseX;
                mouseEvent.y = MouseY;
                mouseEvent.timestamp = mouse_get_time();
                mouseEvent.modifiers = (shifted ? 1 : 0);
                addMouseEvent(&mouseEvent);
            }
        } break;

        case SDL_MOUSEMOTION: {
            // call this first; it sets MouseX and MouseY
            if (SDL_GetRelativeMouseMode() == SDL_TRUE)
                SetMouseXY(MouseX + ev.motion.xrel, MouseY + ev.motion.yrel);
            else
                SetMouseXY(ev.motion.x, ev.motion.y);

            ss_mouse_event mouseEvent = {0};
            mouseEvent.type = MOUSE_MOTION;
            mouseEvent.x = MouseX;
            mouseEvent.y = MouseY;
            mouseEvent.buttons = 0;
            if (ev.motion.state & SDL_BUTTON_LMASK)
                mouseEvent.buttons |= (1 << MOUSE_LBUTTON);
            if (ev.motion.state & SDL_BUTTON_RMASK)
                mouseEvent.buttons |= (1 << MOUSE_RBUTTON);
            mouseEvent.timestamp = mouse_get_time();
            addMouseEvent(&mouseEvent);

            if (TriggerRelMouseMode) {
                TriggerRelMouseMode = FALSE;

                SDL_SetRelativeMouseMode(SDL_TRUE);
                // throw away this first relative mouse reading
                int mvelx, mvely;
                get_mouselook_vel(&mvelx, &mvely);
            }
        } break;

        case SDL_MOUSEWHEEL:
            if (ev.wheel.y != 0) {
                ss_mouse_event mouseEvent = {0};
                mouseEvent.type = ev.wheel.y < 0 ? MOUSE_WHEELDN : MOUSE_WHEELUP;
                mouseEvent.x = MouseX;
                mouseEvent.y = MouseY;
                mouseEvent.buttons = 0;
                mouseEvent.timestamp = mouse_get_time();
                addMouseEvent(&mouseEvent);
            }
            break;

        case SDL_CONTROLLERDEVICEADDED:
            rp5np_open_controller(ev.cdevice.which);
            break;

        case SDL_CONTROLLERDEVICEREMOVED:
            if (rp5np_controller != NULL && ev.cdevice.which == rp5np_controller_instance) {
                SDL_Log("RP5NP_CONTROLLER_REMOVED instance=%d", (int)rp5np_controller_instance);
                rp5np_release_controller_inputs();
                SDL_GameControllerClose(rp5np_controller);
                rp5np_controller = NULL;
                rp5np_controller_instance = -1;
                rp5np_open_first_controller();
            }
            break;

        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP: {
            if (rp5np_controller == NULL || ev.cbutton.which != rp5np_controller_instance)
                break;

            const bool down = ev.cbutton.state == SDL_PRESSED;
            const bool setup_context = rp5np_setup_context;
            SDL_Keycode key = SDLK_UNKNOWN;

            switch (ev.cbutton.button) {
            case SDL_CONTROLLER_BUTTON_A:
                /* Remake: A=Jump. In setup UI, A remains the confirm button. */
                key = setup_context ? SDLK_RETURN : SDLK_SPACE;
                break;

            case SDL_CONTROLLER_BUTTON_B:
                /* Remake: B=Crouch. In setup UI, B remains Back. */
                if (setup_context)
                    key = SDLK_ESCAPE;
#ifdef __ANDROID__
                else if (down)
                    rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_CROUCH_TOGGLE);
#endif
                break;

            case SDL_CONTROLLER_BUTTON_X:
#ifdef __ANDROID__
                /* Remake: X=Interact. Original System Shock uses double-click. */
                if (!setup_context && down)
                    rp5np_emit_interact_double_click();
#endif
                break;

            case SDL_CONTROLLER_BUTTON_Y:
#ifdef __ANDROID__
                /* Remake: Y=Swap Weapon Mode. Closest original action is
                   switching the active weapon's available ammo/mode. */
                if (!setup_context && down)
                    rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_SWAP_WEAPON_MODE);
#endif
                break;

            case SDL_CONTROLLER_BUTTON_BACK:
#ifdef __ANDROID__
                /* View/Select toggles the RP5 right stick between camera look
                   (default in realspace) and the legacy UI/MFD cursor. */
                if (!setup_context && down) {
                    rp5np_right_stick_cursor_mode = !rp5np_right_stick_cursor_mode;
                    rp5np_right_stick_active = false;
                    rp5np_dispatch_action(rp5np_right_stick_cursor_mode
                        ? RP5NP_CONTROLLER_ACTION_CURSOR_MODE
                        : RP5NP_CONTROLLER_ACTION_LOOK_MODE);
                    SDL_Log("RP5NP_CONTROLLER_LOOK_CURSOR_MODE=%s",
                            rp5np_right_stick_cursor_mode ? "CURSOR" : "LOOK");
                }
#endif
                break;

            case SDL_CONTROLLER_BUTTON_START:
                key = SDLK_ESCAPE;
                break;

            case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                /* Remake: L3=Sprint. Preserve hold-to-sprint via Shift. */
                if (!setup_context)
                    rp5np_set_shift(down);
                break;

            case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
#ifdef __ANDROID__
                /* Remake: R3=Target ID Hardware. */
                if (!setup_context && down)
                    rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_TARGET_ID);
#endif
                break;

            case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
#ifdef __ANDROID__
                /* Remake: LB=Quick Use Consumable. */
                if (!setup_context && down)
                    rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_QUICK_CONSUMABLE);
#endif
                break;

            case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
#ifdef __ANDROID__
                /* Remake: RB=Quick Use Explosive. */
                if (!setup_context && down)
                    rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_QUICK_EXPLOSIVE);
#endif
                break;

            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                if (setup_context)
                    key = SDLK_LEFT;
#ifdef __ANDROID__
                else if (down)
                    rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_CYCLE_WEAPON_PREV);
#endif
                break;

            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                if (setup_context)
                    key = SDLK_RIGHT;
#ifdef __ANDROID__
                else if (down)
                    rp5np_dispatch_action(RP5NP_CONTROLLER_ACTION_CYCLE_WEAPON_NEXT);
#endif
                break;

            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                if (setup_context) key = SDLK_UP;
                break;

            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                if (setup_context) key = SDLK_DOWN;
                break;

            default:
                break;
            }

            if (key != SDLK_UNKNOWN) {
                rp5np_emit_key(key, down);
                SDL_Log("RP5NP_CONTROLLER_BUTTON button=%d down=%d key=%d",
                        ev.cbutton.button, down ? 1 : 0, (int)key);
            }
        } break;

        case SDL_CONTROLLERAXISMOTION:
            if (rp5np_controller == NULL || ev.caxis.which != rp5np_controller_instance)
                break;

            switch (ev.caxis.axis) {
            case SDL_CONTROLLER_AXIS_LEFTX:
                rp5np_left_x = ev.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_LEFTY:
                rp5np_left_y = ev.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTX:
                rp5np_right_x = ev.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_RIGHTY:
                rp5np_right_y = ev.caxis.value;
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                /* Remake LT is ADS, which the 1994 engine does not have. Keep LT as the legacy left-mouse select/drag control for MFD/UI precision. */
                rp5np_update_trigger(&rp5np_trigger_left_down, ev.caxis.value, true);
                break;
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                /* Remake RT = Fire/Attack; right mouse is System Shock's native fire path. */
                rp5np_update_trigger(&rp5np_trigger_right_down, ev.caxis.value, false);
                break;
            default:
                break;
            }
            break;
        case SDL_WINDOWEVENT:
            switch (ev.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                if (can_use_opengl())
                    opengl_resize(ev.window.data1, ev.window.data2);
#ifdef __ANDROID__
                rp5np_refresh_logical_size();
#endif
                break;

            case SDL_WINDOWEVENT_MOVED:
            case SDL_WINDOWEVENT_RESIZED:
                break;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
#ifdef __ANDROID__
                rp5np_refresh_logical_size();
#endif
                SDL_SetRelativeMouseMode(saved_rel_mouse);
                if (saved_rel_mouse == SDL_TRUE) {
                    // throw away this first relative mouse reading
                    int mvelx, mvely;
                    get_mouselook_vel(&mvelx, &mvely);
                }
                SDL_ShowCursor(SDL_DISABLE);
                break;

            case SDL_WINDOWEVENT_FOCUS_LOST:
                saved_rel_mouse = SDL_GetRelativeMouseMode();
                SDL_SetRelativeMouseMode(SDL_FALSE);
                SDL_ShowCursor(SDL_ENABLE);
                break;
            }
            break;
        }
    }

#ifdef __ANDROID__
    rp5np_poll_right_stick();
    rp5np_apply_left_stick();
    rp5np_apply_right_stick();
#endif
}

//===============================================================
//
// This section is adapted from:
// kbMac.c - All the keyboard handling routines that are specific to the Macintosh.
//
//===============================================================

//------------------
//  Globals
//------------------
int pKbdStatusFlags;

//---------------------------------------------------------------
//  Startup and keyboard handlers and initialize globals.   Shutdown follows.
//---------------------------------------------------------------
int kb_startup(void *v) {
    pKbdStatusFlags = 0;

    memset(sshockKeyStates, 0, sizeof(sshockKeyStates));
    nextKBevent = 0;
#ifdef __ANDROID__
    SDL_Log("RP5NP_CONTROLLER_SCAN_DEFERRED_UNTIL_SDL_INIT");
#endif

    return (0);
}

int kb_shutdown(void) { return (0); }

//---------------------------------------------------------------
//  Get and set the global flags.
//---------------------------------------------------------------
int kb_get_flags() { return (pKbdStatusFlags); }

void kb_set_flags(int flags) { pKbdStatusFlags = flags; }

//---------------------------------------------------------------
//  Get the next available key from the event queue.
//---------------------------------------------------------------
kbs_event kb_next(void) {
    kbs_event retEvent = kb_look_next();
    // kb_look_next() doesn't remove events from the queue, this function does,
    // right here (but only if there actually was an event in the queue, of course):
    if (nextKBevent > 0) {
        --nextKBevent;
        memmove(&kbEvents[0], &kbEvents[1], sizeof(kbs_event) * (kNumKBevents - 1));
    }
    return retEvent;

#if 0
	bool gotKey = FALSE;
	EventRecord	theEvent;
	while(!gotKey)
	{
		gotKey = GetOSEvent(keyDownMask | autoKeyMask, &theEvent);		// Get a key
		if (gotKey)
		{
			retEvent.code = (uchar)(theEvent.message >> 8); // keyCodeMask == 0x0000FF00
			retEvent.state = KBS_DOWN;
			retEvent.ascii = (uchar)(theEvent.message & charCodeMask);
			retEvent.modifiers = (uchar)(theEvent.modifiers >> 8);
		}
		else if ((flags & KBF_BLOCK) == 0)					// If there was no key and we're
			return (retEvent);										// not blocking, then return.
	}
	return (retEvent);
#endif
}

//---------------------------------------------------------------
//  See if there is a key waiting in the queue.
//---------------------------------------------------------------
kbs_event kb_look_next(void) {
    kbs_event retEvent = {0xFF, 0x00};

    int flags = kb_get_flags();
    if (flags & KBF_BLOCK) {
        while (nextKBevent == 0) {
            pump_events();
        }
    }

    if (nextKBevent > 0) {
        retEvent = kbEvents[0];
    }
    return retEvent;

#if 0
	bool				gotKey = FALSE;
	EventRecord	theEvent;
	while(!gotKey)
	{
		gotKey = OSEventAvail(keyDownMask | autoKeyMask, &theEvent);		// Get a key
		if (gotKey)
		{
			retEvent.code = (uchar)(theEvent.message >> 8);
			retEvent.state = KBS_DOWN;
			retEvent.ascii = (uchar)(theEvent.message & charCodeMask);
			retEvent.modifiers = (uchar)(theEvent.modifiers >> 8);
		}
		else if (flags & KBF_BLOCK == 0)					// If there was no key and we're
			return (retEvent);										// not blocking, then return.
	}
	return (retEvent);
#endif
}

//---------------------------------------------------------------
//  Flush keyboard events from the event queue.
//---------------------------------------------------------------
void kb_flush(void) {
    // http://mirror.informatimago.com/next/developer.apple.com/documentation/Carbon/Reference/Event_Manager/event_mgr_ref/function_group_5.html#//apple_ref/c/func/FlushEvents
    // FlushEvents(keyDownMask | autoKeyMask, 0);

    SDL_FlushEvents(SDL_KEYDOWN, SDL_KEYUP); // Note: that's a range!

    nextKBevent = 0; // this flushes the keyboard events already buffered - TODO is that desirable?
}

//---------------------------------------------------------------
//  Return the state of the indicated key (scan code).
//---------------------------------------------------------------

uchar kb_state(uchar code) {
    // see
    // http://mirror.informatimago.com/next/developer.apple.com/documentation/Carbon/Reference/Event_Manager/event_mgr_ref/function_group_4.html#//apple_ref/c/func/GetKeys
    // GetKeys((UInt32 *) pKbdGetKeys);
    // return ((pKbdGetKeys[code>>3] >> (code & 7)) & 1);

    return sshockKeyStates[code] != 0;
}

//---------------------------
//
// MOUSE STUFF
//
//---------------------------

// ---------------------------------------------------------
// mouse_next gets the event in the front event queue,
// and removes the event from the queue.
// res = ptr to event to be filled.
//	---------------------------------------------------------
//  For Mac version: Get event from the normal Mac event queue for mouse events.
//  The events looked for depend on the 'mouseMask' setting.

uchar btn_left = FALSE;
uchar btn_right = FALSE;
errtype mouse_next(ss_mouse_event *res) {
    if (nextMouseEvent <= 0)
        return ERR_DUNDERFLOW;

    *res = mouseEvents[0];

    --nextMouseEvent;
    memmove(&mouseEvents[0], &mouseEvents[1], sizeof(ss_mouse_event) * (kNumMouseEvents - 1));

    return OK;
}

errtype mouse_flush(void) {
    // FlushEvents(mouseDown | mouseUp, 0);
    //   Spew(DSRC_MOUSE_Flush,("Entering mouse_flush()\n"));
    // mouseQueueIn = mouseQueueOut = 0;
    nextMouseEvent = 0;
    // TODO: anything else?
    return OK;
}

errtype mouse_get_xy(short *x, short *y) {
    *x = MouseX;
    *y = MouseY;

    return OK;
}

void middleize_mouse(void) {
    int w, h;
    SDL_RenderGetLogicalSize(renderer, &w, &h);

    MouseX = latestMouseEvent.x = w / 2;
    MouseY = latestMouseEvent.y = h / 2;
}

void get_mouselook_vel(int *vx, int *vy) {
    if (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE)
        *vx = *vy = 0;
    else {
        SDL_GetRelativeMouseState(vx, vy);

        *vx += MouseChaosX;
        MouseChaosX = 0;
        *vy += MouseChaosY;
        MouseChaosY = 0;
    }
}

errtype mouse_put_xy(short x, short y) {
    MouseX = x;
    MouseY = y;

    return OK;
}

void set_mouse_chaos(short dx, short dy) {
    MouseChaosX = dx;
    MouseChaosY = dy;
}

void sdl_mouse_init(void) { nextMouseEvent = 0; }
