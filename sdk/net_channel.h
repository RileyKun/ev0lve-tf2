//
// Created by panzerfaust on 5/27/2020.
//

#ifndef EV0LVE_TF_NET_CHANNEL_H
#define EV0LVE_TF_NET_CHANNEL_H

namespace sdk {
    class net_message_base;

    enum flow : int {
        flow_outgoing,
        flow_incoming,
        flow_count
    };

    class net_channel_info {
    public:
        enum {
            generic = 0,
            local_player,
            other_players,
            entities,
            sounds,
            events,
            user_messages,
            ent_messages,
            voice,
            string_table,
            move,
            string_cmd,
            sign_on,
            total,
        };

        virtual const char  *get_name() = 0;
        virtual const char  *get_address() = 0;
        virtual float		get_time() = 0;
        virtual float		get_time_connected() = 0;
        virtual int			get_buffer_size() = 0;
        virtual int			get_data_rate() = 0;
        virtual bool		is_loopback() = 0;
        virtual bool		is_timing_out() = 0;
        virtual bool		is_playback() = 0;
        virtual float		get_latency(int flow) = 0;
        virtual float		get_avg_latency(int flow) = 0;
        virtual float		get_avg_loss(int flow) = 0;
        virtual float		get_avg_choke(int flow) = 0;
        virtual float		get_avg_data(int flow) = 0;
        virtual float		get_avg_packets(int flow) = 0;
        virtual int			get_total_data(int flow) = 0;
        virtual int			get_sequence_number(int flow) = 0;
        virtual bool		is_valid_packet(int flow, int frame_number) = 0;
        virtual float		get_packet_time(int flow, int frame_number) = 0;
        virtual int			get_packet_Bytes(int flow, int frame_number, int group) = 0;
        virtual bool		get_stream_progress(int, int*, int*) = 0;
        virtual float		get_time_since_received() = 0;
        virtual	float		get_command_interpolation(int flow, int frame_number) = 0;
        virtual void		get_packet_response_latency(int, int, int*, int*) = 0;
        virtual void		get_remove_framerate(float*, float*) = 0;
        virtual float		get_timeout_seconds() = 0;
    };

    class net_channel : public net_channel_info {
    public:
        virtual	            ~net_channel() = default;
        virtual void	    set_data_rate(float rate) = 0;
        virtual bool	    register_message(void*) = 0;
        virtual bool	    start_streaming(unsigned int) = 0;
        virtual void	    reset_streaming() = 0;
        virtual void	    set_timeout(float) = 0;
        virtual void	    set_demo_recorder(void*) = 0;
        virtual void	    set_challenge_nr(unsigned int) = 0;
        virtual void	    reset() = 0;
        virtual void	    clear() = 0;
        virtual void	    shutdown(const char *) = 0;
        virtual void	    process_playback() = 0;
        virtual bool	    process_stream() = 0;
        virtual void	    process_packet(void*, bool) = 0;
        virtual bool	    send_net_msg(net_message_base& msg, bool bForceReliable, bool bVoice) = 0;
        virtual bool	    send_data(void*, bool) = 0;
        virtual bool	    send_file(const char *, unsigned int) = 0;
        virtual void	    deny_file(const char *, unsigned int) = 0;
        virtual void	    unused(const char *, unsigned int) = 0;
        virtual void	    set_choked() = 0;
        virtual int		    send_datagram(void* data) = 0;
        virtual bool	    transmit(bool only_reliable) = 0;
        virtual void        *get_remote_address() = 0;
        virtual void        *get_msg_handler() = 0;
        virtual int			get_drop_number() = 0;
        virtual int			get_socket() = 0;
        virtual uint32_t    get_challenge_nr() = 0;
        virtual void	    get_sequence_data(int&, int&, int&) = 0;
        virtual void	    set_sequence_data(int, int, int) = 0;
        virtual void	    update_message_stats(int, int) = 0;
        virtual bool	    can_packet() = 0;
        virtual bool	    is_overflowed() = 0;
        virtual bool	    is_timed_out()  = 0;
        virtual bool	    has_pending_reliable_data() = 0;
        virtual void	    set_file_transmission_mode(bool) = 0;
        virtual void	    set_compression_mode(bool) = 0;
        virtual uint32_t    request_file(const char *) = 0;
        virtual float	    get_time_since_received() = 0;
        virtual void	    set_max_buffer_size(bool, int, bool) = 0;
        virtual bool	    is_null() = 0;
        virtual int		    get_num_bits_written(bool) = 0;
        virtual void	    set_interpolation(float) = 0;
        virtual void	    set_remote_framerate(float, float) = 0;
        virtual void	    set_max_payload_size(int) = 0;
        virtual int		    get_max_payload_size() = 0;
        virtual int		    get_protocol_version() = 0;
    };
}

#endif //EV0LVE_TF_NET_CHANNEL_H
