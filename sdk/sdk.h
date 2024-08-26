//
// Created by panzerfaust on 5/25/2020.
//

#ifndef EV0LVE_TF_SDK_H
#define EV0LVE_TF_SDK_H

#include <ev0lve.h>
#include "maths.h"

#include "client_class.h"
#include "netvars.h"

#include "dwords.h"
#include "bitbuf.h"
#include "matrix.h"
#include "glow_manager.h"
#include "utl.h"
#include "vector.h"
#include "user_cmd.h"
#include "handle.h"
#include "datamap.h"
#include "collideable.h"
#include "model.h"
#include "entity.h"
#include "net_channel.h"
#include "color.h"
#include "cvar.h"
#include "game_event.h"
#include "keyvalues.h"
#include "studio.h"
#include "view_setup.h"
#include "netmessage.h"

#include "global_vars.h"
#include "client.h"
#include "surface.h"
#include "engine.h"
#include "entity_list.h"
#include "game_input.h"
#include "client_state.h"
#include "client_mode.h"
#include "debug_overlay.h"
#include "localize.h"
#include "materialsystem.h"
#include "modelrender.h"
#include "renderview.h"
#include "studio_render_context.h"
#include "engine_trace.h"
#include "modelinfo.h"
#include "game_movement.h"
#include "game_prediction.h"
#include "thirdperson_manager.h"

namespace sdk {
    inline int time_to_ticks(float t) { return (int)(0.5f + t / global_vars->tick_interval); }
    inline float ticks_to_time(int t) { return global_vars->tick_interval * (float)t; }
}

#endif //EV0LVE_TF_SDK_H
