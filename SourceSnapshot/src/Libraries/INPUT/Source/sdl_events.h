/*

Copyright (C) 2020 Shockolate Project

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

#ifndef SDL_EVENT_H
#define SDL_EVENT_H

// Ingest SDL events into internal Event system
void pump_events(void);
void set_mouse_chaos(short dx, short dy);

#ifdef __ANDROID__
#include <stdbool.h>

typedef void (*rp5np_text_input_handler)(const char *utf8_text, void *userdata);

typedef enum rp5np_controller_action {
    RP5NP_CONTROLLER_ACTION_CROUCH_TOGGLE = 1,
    RP5NP_CONTROLLER_ACTION_SWAP_WEAPON_MODE,
    RP5NP_CONTROLLER_ACTION_QUICK_CONSUMABLE,
    RP5NP_CONTROLLER_ACTION_QUICK_EXPLOSIVE,
    RP5NP_CONTROLLER_ACTION_TARGET_ID,
    RP5NP_CONTROLLER_ACTION_CYCLE_WEAPON_PREV,
    RP5NP_CONTROLLER_ACTION_CYCLE_WEAPON_NEXT,
    RP5NP_CONTROLLER_ACTION_LOOK_MODE,
    RP5NP_CONTROLLER_ACTION_CURSOR_MODE
} rp5np_controller_action;

typedef void (*rp5np_controller_action_handler)(rp5np_controller_action action, void *userdata);

void rp5np_controller_set_setup_context(bool setup_context);
void rp5np_set_text_input_handler(rp5np_text_input_handler handler, void *userdata);
void rp5np_set_controller_action_handler(rp5np_controller_action_handler handler, void *userdata);
#endif

#endif
