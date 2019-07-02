#ifndef __WAVESPP_ADDRESS_HPP_18906__
#define __WAVESPP_ADDRESS_HPP_18906__

#include <cstring>
#include <string>
#include "coda/error.hpp"

namespace waves {

struct base58_decode_exception : std::exception
{
    base58_decode_exception(const std::string& str, size_t pos);
    const std::string& get_orig_b58() const throw();
    size_t get_error_pos() const throw();
    const char* what () const throw();

    std::string _orig_b58;
    size_t _error_pos;
    char _what_buf[128];
};

struct public_key
{
    static const size_t PUBLIC_KEY_B58_LEN = 45;

    unsigned char _data[32];

    public_key();
    public_key(const char* _str); // from base58
    public_key(const public_key& _pub_k);

    bool is_set() const; // true if _data != {0,0,0,0...}

    bool verify_signature(unsigned char* msg, size_t msg_sz, unsigned char *_signature) const;
    std::string to_base58() const;
};

struct address
{
        static const size_t ADDRESS_B58_LEN = 36;

        address();
        address(const char* _str); // from base58
        address(const public_key& _pub_k, unsigned char net);

        std::string to_base58() const;

        /**
         * Returns user group ID.
         *
         * User group ID determines which shard serves the users associated with the group ID.
         *
         * By convention, we build an uint16_t from the 2-nd and the 3-rd bytes of the address.
         * @link https://wsn.usrsrc.ru/projects/back/wiki/User_Groups
         */
        inline uint16_t group_id() const
        {
            return _data[3] | (_data[2] << 8);
        }

        bool satisfy(const public_key& _pub_k, unsigned char net);

        inline bool operator <(const address& other) const
        {
            return memcmp(_data, other._data, sizeof(_data)) < 0;
        }

        inline bool operator ==(const address& other) const
        {
            return memcmp(_data, other._data, sizeof(_data)) == 0;
        }

        inline bool operator !=(const address& other) const
        {
            return memcmp(_data, other._data, sizeof(_data)) != 0;
        }

        unsigned char _data[26];
};

struct signature
{
    static const size_t SIGNATURE_B58_LEN = 89;

    signature();
    signature(const char* _str); // from base58
    signature(const signature& _sig);

    unsigned char _data[64];
};

}

namespace std {

template<>
struct hash<waves::address>
{
    size_t operator() (const waves::address& addr) const
    {
        std::hash<unsigned char> hasher;
        size_t result = 0;
        for(size_t i = 0; i < sizeof(addr._data); ++i)
        {
            result = result * 31 + hasher(addr._data[i]);
        }
        return result;
    }
};

}

#endif /* __WAVESPP_ADDRESS_HPP_18906__ */
