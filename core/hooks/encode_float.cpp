//
// Created by panzerfaust on 6/25/2020.
//
#include <core/hooks.h>

void __cdecl hooks::encode_float(sdk::send_prop* prop, float val, sdk::bf_write* out, int object_id) {
    auto flags = prop->flags;
    if (flags & sdk::sprop_coord)
        return o_encode_float(prop, val, out, object_id);
    if (flags & (sdk::sprop_coord_mp | sdk::sprop_coord_mp_lowprecision | sdk::sprop_coord_mp_integral))
        return o_encode_float(prop, val, out, object_id);
    if (flags & sdk::sprop_normal)
        return o_encode_float(prop, val, out, object_id);
    if (flags & sdk::sprop_noscale)
        return o_encode_float(prop, val, out, object_id);

    if (val < prop->low_value)
        return o_encode_float(prop, prop->low_value, out, object_id);
    if (val > prop->high_value)
        return o_encode_float(prop, prop->high_value, out, object_id);

    return o_encode_float(prop, val, out, object_id);
}