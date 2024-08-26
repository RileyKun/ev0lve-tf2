//
// Created by panzerfaust on 5/27/2020.
//

#ifndef EV0LVE_TF_CLIENT_STATE_H
#define EV0LVE_TF_CLIENT_STATE_H

namespace sdk {
    class clock_drift_mgr {
    public:
        float   clock_offsets[16];
        int     current_offset;
        int     server_tick;
        int     client_tick;
    };

    class c_client_state {
    public:
        void *v_table{}; //0x0000
        int32_t socket{}; //0x0004
        net_channel *channel{}; //0x0008
        char pad_000C[12]{}; //0x000C
        int32_t retry_number{}; //0x0018
        char retry_address[260]{}; //0x001C
        char *retry_source_tag{}; //0x0120
        int32_t retry_challenge{}; //0x0124
        int32_t sign_on_state{}; //0x0128
        char pad_012C[4]{}; //0x012C
        double next_cmd_time{}; //0x0130
        int32_t server_count{}; //0x0138
        char pad_013C[4]{}; //0x013C
        uint64_t server_steam_id{}; //0x0140
        int32_t current_sequence{}; //0x0148
        clock_drift_mgr clock_offsets{}; //0x014C
        int32_t delta_tick{}; //0x0198
        bool is_paused{}; //0x019C
        float paused_expire_time{}; //0x019D
        uint32_t view_entity{}; //0x01A1
        int32_t player_slot{}; //0x01A5
        char N000005DB[3]{}; //0x01A9
        char level_file_name[128]{}; //0x01AC
        char level_base_name[128]{}; //0x022C
        int32_t max_clients{}; //0x02AC
        char entity_base_lines[2][4]{}; //0x02B0
        void *server_classes{}; //0x02B8
        int32_t server_class_count{}; //0x02BC
        int32_t server_class_bits{}; //0x02C0
        char encryption_key[2048]{}; //0x02C4
        char pad_0AC4[16448]{}; //0x0AC4
        float last_server_tick_time{}; //0x4B08
        bool is_in_simulation{}; //0x4B0C
        char pad_4B0D[3]{}; //0x4B0D
        int32_t old_tick_count{}; //0x4B10
        float tick_remainder{}; //0x4B14
        float frame_time{}; //0x4B18
        int32_t last_outgoing_command{}; //0x4B1C
        int32_t choked_commands{}; //0x4B20
        int32_t last_command_ack{}; //0x4B24
        int32_t command_ack{}; //0x4B28
        int32_t sound_sequence{}; //0x4B2C
        bool is_hl_tv{}; //0x4B30
        char pad_4B31[1]{}; //0x4B31
        char N00001453[16]{}; //0x4B32
        char N00001454[32]{}; //0x4B42
        char N00001455[24]{}; //0x4B62
        bool are_area_bits_valid{}; //0x4B7A
        char pad_4B7B[1]{}; //0x4B7B
        ang view_angles; //0x4B7C
    };

    inline c_client_state* client_state = nullptr;
}

#endif //EV0LVE_TF_CLIENT_STATE_H
