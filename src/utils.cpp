#include "utils.hpp"
#include <waves/b58.h>
#include <waves/b64.h>
#include <waves/crypto.h>

namespace waves {
namespace utils {

std::string to_base58(const unsigned char* v, size_t len)
{
    char buf[len * 2 + 1];
    base58_encode(buf, v, len);
    return std::string(buf);
}

std::string to_base64(const unsigned char* v, size_t len)
{
    char buf[len * 2 + 1];
    base64_encode(buf, v, len);
    return std::string(buf);
}

std::string to_base58(const std::string& v)
{
    return to_base58(reinterpret_cast<const unsigned char*>(v.c_str()), v.size());
}

std::string to_base64(const std::string& v)
{
    return to_base64(reinterpret_cast<const unsigned char*>(v.c_str()), v.size());
}

std::string from_base58(const std::string& v)
{
    unsigned char buf[v.size()];
    ssize_t sz = base58_decode(buf, v.c_str());
    return std::string((char*)buf, sz);
}

std::string from_base64(const std::string& v)
{
    unsigned char buf[v.size()];
    ssize_t sz = base64_decode(buf, v.c_str());
    return std::string((char*)buf, sz);
}

std::string secure_hash_to_address(const std::string& hash, uint8_t chain_id)
{
    std::string address;
    address.resize(26);
    uint8_t hash2[32];
    uint8_t buf[22];
    buf[0] = 1;
    buf[1] = chain_id;
    memcpy(&buf[2], hash.c_str(), 20);
    waves_secure_hash(buf, 22, hash2);
    auto address_data = (char*)address.c_str();
    address_data[0] = 1;
    address_data[1] = chain_id;
    memcpy(&address_data[2], hash.c_str(), 20);
    memcpy(&address_data[22], hash2, 4);
    return address;
}

}}
