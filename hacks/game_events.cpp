//
// Created by panzerfaust on 5/30/2020.
//

#include <misc/xor.h>
#include <misc/logger.h>
#include "game_events.h"

void hack::c_game_events::init() {
    registered_for_events = true;
}

void hack::c_game_events::fire_game_event(sdk::game_event *event) {

}

void hack::c_game_events::release() {
    registered_for_events = false;
}

