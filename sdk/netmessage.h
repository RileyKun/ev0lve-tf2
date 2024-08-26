//
// Created by panzerfaust on 6/8/2020.
//

#ifndef EV0LVE_TF_NETMESSAGE_H
#define EV0LVE_TF_NETMESSAGE_H

namespace sdk {
    class net_message_base {
    public:
        virtual                 ~net_message_base(){};
        virtual void	        set_net_channel(net_channel * netchan) = 0;
        virtual void	        set_reliable(bool state) = 0;
        virtual bool	        process() = 0;
        virtual	bool	        read_from_buffer(bf_read &buffer) = 0;
        virtual	bool	        write_to_buffer(bf_write &buffer)  = 0;
        virtual bool	        is_reliable() = 0;
        virtual int				get_type() = 0;
        virtual int				get_group() = 0;
        virtual const char		*get_name() = 0;
        virtual net_channel		*get_net_channel() = 0;
        virtual const char		*to_string() = 0;
    };

    class net_message : public net_message_base {
    public:
        inline net_message() {
            reliable = true;
            channel = NULL;
        }

        virtual ~net_message() {};
        virtual int		get_group() { return 0; }
        net_channel		*get_net_channel() { return channel; }
        virtual void	set_reliable(bool state) { reliable = state; };
        virtual bool	is_reliable() { return reliable; };
        virtual void    set_net_channel(net_channel* netchan) { channel = netchan; }
        virtual bool	process() { return false; };

    protected:
        bool				reliable;
        net_channel			*channel;
    };

    class msg_move : public net_message
    {
    public:
        msg_move() {
            reliable = false;
        }

        bool read_from_buffer( bf_read &buffer ) {
            return true;
        }

		bool write_to_buffer( bf_write &buffer ) {
            buffer.write_ulong_bit(get_type(), 6, true);
            length = data_out.cur_bit;

            buffer.write_ulong_bit(new_commands, 4, true);
            buffer.write_ulong_bit(backup_commands, 3, true);
            buffer.write_ulong_bit(length, sizeof(unsigned short) << 3, true);

            return buffer.write_bits((unsigned char*)data_out.data, length);
        }

		const char *to_string() {
            return "eat ass";
        }

        bool process() {
            return true;
        }

        int	get_group() {
            return 10;
        }

		int				get_type() {
            return 9;
        }

		const char		*get_name() {
            return "clc_Move";
        }

        void            *message_handler{};
        int				backup_commands{};
        int				new_commands{};
        int				length{};
        bf_read			data_in{};
        bf_write		data_out{};
    };
}

#endif //EV0LVE_TF_NETMESSAGE_H
