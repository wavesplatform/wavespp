#ifndef __WAVESPP_UTILS_HPP_15740__
#define __WAVESPP_UTILS_HPP_15740__

#include <string>

namespace wavespp {
namespace utils {

std::string to_base58(const unsigned char* v, size_t len);
std::string to_base58(const std::string& v);
std::string to_base64(const unsigned char* v, size_t len);
std::string to_base64(const std::string& v);

std::string from_base58(const std::string& v);
std::string from_base64(const std::string& v);

std::string secure_hash_to_address(const std::string& hash, uint8_t chain_id);

size_t hash_bytes(const unsigned char *data, size_t len);

std::string bin2hex(const unsigned char *data, size_t len);

std::string hex2bin(const char* hex, size_t len) noexcept;

inline std::string hex2bin(const char* hex) noexcept
{
    return hex2bin(hex, std::char_traits<char>::length(hex));
}

inline std::string hex2bin(const std::string s) noexcept
{
    return hex2bin(s.c_str(), s.size());
}

}}

#endif /* __WAVESPP_UTILS_HPP_15740__ */
