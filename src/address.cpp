#include "address.hpp"
#include "waves/crypto.h"
#include "waves/b58.h"
#include <coda/error.hpp>
#include <stdio.h>

namespace waves {

base58_decode_exception::base58_decode_exception(const std::string& str, size_t pos) :
    _orig_b58(str),
    _error_pos(pos)
{
    if (_error_pos < 10)
    {
        snprintf(_what_buf, 128, "Base58 parse error at %u: %.*s", (unsigned)_error_pos, (unsigned)_error_pos, _orig_b58.c_str());
    } else
    {
        snprintf(_what_buf, 128, "Base58 parse error at %u: %.3s...%.3s", (unsigned)_error_pos, _orig_b58.c_str(), _orig_b58.c_str() + _error_pos - 3);
    }
}

const std::string& base58_decode_exception::get_orig_b58() const throw()
{
    return _orig_b58;
}

size_t base58_decode_exception::get_error_pos() const throw()
{
    return _error_pos;
}

const char* base58_decode_exception::what () const throw()
{
    return _what_buf;
}

public_key::public_key()
{
    memset(_data, 0, sizeof(_data));
}

public_key::public_key(const char* _str)
{
    size_t b58len = strlen(_str);
    if (b58len > public_key::PUBLIC_KEY_B58_LEN)
    {
        throw coda_error("Public key base58 string length is %lu, "
                         "should be no more than %lu", b58len, public_key::PUBLIC_KEY_B58_LEN);
    }
    memset(_data, 0, sizeof(_data));
    ssize_t ret = base58_decode(_data, _str);
    if (ret < 0)
    {
        throw base58_decode_exception(_str, -ret+1);
    }
}

public_key::public_key(const public_key& _pub_k)
{
    memcpy(_data, _pub_k._data, sizeof(_data));
}

bool public_key::is_set() const
{
    size_t data_len = sizeof(_data);
    while (data_len--)
    {
       if (_data[data_len-1]) return true;
    }
    return false;
}

bool public_key::verify_signature(unsigned char* msg, size_t msg_sz, unsigned char* _signature) const
{
    return waves_verify_message(_data, msg, msg_sz, _signature);
}

std::string public_key::to_base58() const
{
    char out_buf[64];
    base58_encode(out_buf, _data, sizeof(_data));
    return std::string(out_buf);
}

address::address()
{
    memset(_data, 0, sizeof(_data));
}

address::address(const char* _str)
{
    size_t b58len = strlen(_str);
    if (b58len > address::ADDRESS_B58_LEN)
    {
        throw coda_error("Address base58 string length is %lu, "
                         "should be no more than %lu", b58len, address::ADDRESS_B58_LEN);
    }
    memset(_data, 0, sizeof(_data));
    ssize_t ret = base58_decode(_data, _str);
    if (ret < 0)
    {
        throw base58_decode_exception(_str, -ret+1);
    }
}

address::address(const public_key& _pub_k, unsigned char net)
{
    waves_public_key_to_address(_pub_k._data, net, _data);
}

bool address::satisfy(const public_key& _pub_k, unsigned char net)
{
    return *this == address(_pub_k, net);
}

std::string address::to_base58() const
{
    char out_buf[address::ADDRESS_B58_LEN+1];
    base58_encode(out_buf, _data, sizeof(_data));
    return std::string(out_buf);
}

signature::signature()
{
    memset(_data, 0, sizeof(_data));
}

signature::signature(const char* _str)
{
    size_t b58len = strlen(_str);
    if (b58len > signature::SIGNATURE_B58_LEN)
    {
        throw coda_error("Signature base58 string length is %lu, "
                         "should be no more than %lu", b58len, signature::SIGNATURE_B58_LEN);
    }
    memset(_data, 0, sizeof(_data));
    ssize_t ret = base58_decode(_data, _str);
    if (ret < 0)
    {
        throw base58_decode_exception(_str, -ret+1);
    }
}

signature::signature(const signature& _sig)
{
    memcpy(_data, _sig._data, sizeof(_data));
}

}
