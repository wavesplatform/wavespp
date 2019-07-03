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

}}
