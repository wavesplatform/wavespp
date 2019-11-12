#include "utils.hpp"
#include <waves/b58.h>
#include <waves/b64.h>
#include <waves/crypto.h>

namespace wavespp {
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

size_t hash_bytes(const unsigned char* data, size_t len)
{
    std::hash<unsigned char> hasher;
    size_t result = 0;
    for(size_t i = 0; i < len; ++i)
    {
        result = result * 31 + hasher(data[i]);
    }
    return result;
}

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

// Converts binary string to hex representation
std::string bin2hex(const unsigned char *data, size_t len)
{
    std::string s(len * 2, ' ');
    for (size_t i = 0; i < len; ++i) {
        s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}

static unsigned char to_nibble (char c) noexcept
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    return 0xFF;
}

std::string hex2bin(const char* hex, size_t len) noexcept
{
    std::string res;
    res.reserve(len / 2);
    if (len & 1) {
        return "";
    }
    for (size_t i = 0; i < len; i += 2) {
        if (!isxdigit(hex[i]) || !isxdigit(hex[i+1])) {
            return "";
        }
        res += to_nibble(hex[i]) * 16 + to_nibble(hex[i+1]);
    }
    return res;
}


}}
