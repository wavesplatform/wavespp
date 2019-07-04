#include "utils.hpp"
#include <waves/b58.h>
#include <waves/b64.h>

namespace waves {
namespace utils {

std::string to_base58(const std::string& v)
{
    char buf[v.size() * 2 + 1];
    base58_encode(buf, (const unsigned char*)v.c_str(), v.size());
    return std::string(buf);
}

std::string to_base64(const std::string& v)
{
    char buf[v.size() * 2 + 1];
    base64_encode(buf, (const unsigned char*)v.c_str(), v.size());
    return std::string(buf);
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

}}
