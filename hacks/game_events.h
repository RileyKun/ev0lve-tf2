//
// Created by panzerfaust on 5/30/2020.
//

#ifndef EV0LVE_TF_GAME_EVENTS_H
#define EV0LVE_TF_GAME_EVENTS_H

#include <sdk/sdk.h>

namespace hack {
    class c_game_events : public sdk::game_event_listener {
    public:
        void init();
        void release();

        void fire_game_event(sdk::game_event* event) override;
        int	get_event_debug_id() override { return 42; }

        bool registered_for_events = false;
    };

    inline c_game_events game_events{};
}

#endif //EV0LVE_TF_GAME_EVENTS_H
