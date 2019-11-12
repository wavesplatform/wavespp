#ifndef __WAVESPP_ADDRESS_HPP_18906__
#define __WAVESPP_ADDRESS_HPP_18906__

#include <cstring>
#include <string>
#include "coda/error.hpp"
#include "utils.hpp"

namespace wavespp {

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
    // Length of a public key in Base58 format.
    static const size_t PUBLIC_KEY_B58_LEN = 45;
    // Length of a public key in binary format.
    static constexpr size_t PUBLIC_KEY_BIN_LEN = 32;

    public_key();
    public_key(const char* _str); // from base58
    public_key(const public_key& _pub_k);
    // `binary_public_key` is a raw (binary) representation of a public key.
    explicit public_key(const unsigned char (&binary_public_key)[PUBLIC_KEY_BIN_LEN]);

    bool is_set() const; // true if _data != {0,0,0,0...}

    bool verify_signature(const unsigned char* msg, size_t msg_sz, const unsigned char* _signature) const;
    std::string to_base58() const;

    inline bool operator <(const public_key& other) const
    {
        return memcmp(_data, other._data, sizeof(_data)) < 0;
    }

    inline bool operator ==(const public_key& other) const
    {
        return memcmp(_data, other._data, sizeof(_data)) == 0;
    }

    inline bool operator !=(const public_key& other) const
    {
        return memcmp(_data, other._data, sizeof(_data)) != 0;
    }

    unsigned char _data[PUBLIC_KEY_BIN_LEN];
};

struct address
{
    // Length of an address in Base58 format.
    static const size_t ADDRESS_B58_LEN = 36;
    // Length of an address in binary format.
    static constexpr size_t ADDRESS_BIN_LEN = 26;

    address();
    address(const char* _str); // from base58
    address(const char* str, size_t len); // from base58
    address(const public_key& _pub_k, unsigned char net);
    // `binary_address` is a raw (binary) representation of an address.
    explicit address(const unsigned char (&binary_address)[ADDRESS_BIN_LEN]);
    // Constructs address from its raw (binary) representation.
    static address FromBinary(const std::string& binary);

    // Returns the address in Base58 format
    std::string to_base58() const;
    // Returns raw (binary) representation of the address
    std::string to_binary() const;

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
struct hash<wavespp::address>
{
    size_t operator() (const wavespp::address& addr) const
    {
        return wavespp::utils::hash_bytes(addr._data, sizeof(addr._data));
    }
};

template<>
struct hash<wavespp::public_key>
{
    size_t operator() (const wavespp::public_key& pk) const
    {
        return wavespp::utils::hash_bytes(pk._data, sizeof(pk._data));
    }
};

}

#endif /* __WAVESPP_ADDRESS_HPP_18906__ */
