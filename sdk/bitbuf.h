//
// Created by panzerfaust on 6/8/2020.
//

#ifndef EV0LVE_TF_BITBUF_H
#define EV0LVE_TF_BITBUF_H

namespace sdk {
    namespace bitbuf {
        inline int bit_for_bitnum(int bitNum)
        {
            static int bitsForBitnum[] =
                    {
                            ( 1 << 0 ),
                            ( 1 << 1 ),
                            ( 1 << 2 ),
                            ( 1 << 3 ),
                            ( 1 << 4 ),
                            ( 1 << 5 ),
                            ( 1 << 6 ),
                            ( 1 << 7 ),
                            ( 1 << 8 ),
                            ( 1 << 9 ),
                            ( 1 << 10 ),
                            ( 1 << 11 ),
                            ( 1 << 12 ),
                            ( 1 << 13 ),
                            ( 1 << 14 ),
                            ( 1 << 15 ),
                            ( 1 << 16 ),
                            ( 1 << 17 ),
                            ( 1 << 18 ),
                            ( 1 << 19 ),
                            ( 1 << 20 ),
                            ( 1 << 21 ),
                            ( 1 << 22 ),
                            ( 1 << 23 ),
                            ( 1 << 24 ),
                            ( 1 << 25 ),
                            ( 1 << 26 ),
                            ( 1 << 27 ),
                            ( 1 << 28 ),
                            ( 1 << 29 ),
                            ( 1 << 30 ),
                            ( 1 << 31 ),
                    };

            return bitsForBitnum[bitNum & 31];
        }

        inline unsigned long bit_write_masks[32][33]{};
        inline void init_masks() {
            static auto _ = false;
            if (_) return;
            _ = true;

            for (unsigned int startbit=0; startbit < 32; startbit++) {
                for (unsigned int nBitsLeft=0; nBitsLeft < 33; nBitsLeft++) {
                    auto endbit = startbit + nBitsLeft;
                    bit_write_masks[startbit][nBitsLeft] = bit_for_bitnum(startbit) - 1;

                    if(endbit < 32)
                        bit_write_masks[startbit][nBitsLeft] |= ~(bit_for_bitnum(endbit) - 1);
                }
            }
        }
    }

    class bf_write {
    public:
        inline bf_write() {
            data = nullptr;
            data_bytes = 0;
            data_bits = -1;
            cur_bit = 0;
            overflow = false;
            assert_on_overflow = true;
            debug_name = nullptr;
        }

        inline void start_writing(void *pData, int nBytes, int iStartBit = 0, int nBits = -1) {
            nBytes &= ~3;
            data = (unsigned long*)pData;
            data_bytes = nBytes;
            data_bits = (nBits == -1) ? (nBytes << 3) : nBits;
            cur_bit = iStartBit;
            overflow = false;
        }

        inline void write_ulong_bit(uint32_t cur_data, int numbits, bool bCheckRange) {
            if (data_bits - cur_bit < numbits ) {
                cur_bit = data_bits;
                overflow = true;
                return;
            }

            auto cur_bit_masked = cur_bit & 31;
            auto cur_byte = cur_bit >> 5;
            cur_bit += numbits;

            auto out_buf = &data[cur_byte];
            cur_data = (cur_data << cur_bit_masked) | (cur_data >> (32 - cur_bit_masked));

            auto temp = 1 << (numbits-1);
            auto mask1 = (temp*2-1) << cur_bit_masked;
            auto mask2 = (temp-1) >> (31 - cur_bit_masked);

            auto i = mask2 & 1;
            auto dword_1 = little_dword(out_buf[0]);
            auto dword_2 = little_dword(out_buf[i]);

            dword_1 ^= (mask1 & (cur_data ^ dword_1 ) );
            dword_2 ^= (mask2 & (cur_data ^ dword_2 ) );
            out_buf[i] = little_dword(dword_2);
            out_buf[0] = little_dword(dword_1);
        }

        inline bool write_bits(const void* pInData, int bits) {
            bitbuf::init_masks();

            auto out_buffer = (unsigned char*)pInData;
            auto bits_left = bits;

            if (cur_bit + bits > data_bits) {
                overflow = true;
                return false;
            }

            while (((unsigned long)out_buffer & 3) != 0 && bits_left >= 8) {
                write_ulong_bit(*out_buffer, 8, false);
                ++out_buffer;
                bits_left -= 8;
            }

            if (bits_left >= 32 && (cur_bit & 7) == 0 ) {
                int num_bytes = bits_left >> 3;
                int num_bits = num_bytes << 3;

                memcpy((char*)data + (cur_bit >> 3), out_buffer, num_bytes );
                out_buffer += num_bytes;
                bits_left -= num_bits;
                cur_bit += num_bits;
            }
            
            if (bits_left >= 32)
            {
                auto i_bits_right = (cur_bit & 31);
                auto i_bits_left = 32 - i_bits_right;
                auto bit_mask_left = bitbuf::bit_write_masks[i_bits_right][32];
                auto bit_mask_right = bitbuf::bit_write_masks[0][i_bits_right];

                auto p_data = &data[cur_bit >> 5];
                while (bits_left >= 32)
                {
                    auto cur_data = *(unsigned long*)out_buffer;
                    out_buffer += sizeof(unsigned long);

                    *p_data &= bit_mask_left;
                    *p_data |= cur_data << i_bits_right;

                    p_data++;

                    if (i_bits_left < 32 )
                    {
                        cur_data >>= i_bits_left;
                        *p_data &= bit_mask_right;
                        *p_data |= cur_data;
                    }

                    bits_left -= 32;
                    cur_bit += 32;
                }
            }

            while (bits_left >= 8 ) {
                write_ulong_bit(*out_buffer, 8, false );
                ++out_buffer;
                bits_left -= 8;
            }

            if (bits_left)
                write_ulong_bit(*out_buffer, bits_left, false );

            return !overflow;
        }

        unsigned long*  data;
        int				data_bytes;
        int				data_bits;
        int				cur_bit;
        bool			overflow;
        bool			assert_on_overflow;
        const char		*debug_name;
    };

    class bf_read {
    public:
        inline bf_read() {
            data = nullptr;
            data_bytes = 0;
            data_bits = -1;
            cur_bit = 0;
            overflow = false;
            assert_on_overflow = true;
            debug_name = nullptr;
        }

        unsigned long*  data;
        int				data_bytes;
        int				data_bits;
        int				cur_bit;
        bool			overflow;
        bool			assert_on_overflow;
        const char		*debug_name;
    };
}

#endif //EV0LVE_TF_BITBUF_H
