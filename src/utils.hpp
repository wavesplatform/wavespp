#ifndef __WAVESPP_UTILS_HPP_15740__
#define __WAVESPP_UTILS_HPP_15740__

#include <string>

namespace waves {
namespace utils {

std::string to_base58(const unsigned char* v, size_t len);
std::string to_base58(const std::string& v);
std::string to_base64(const unsigned char* v, size_t len);
std::string to_base64(const std::string& v);

std::string from_base58(const std::string& v);
std::string from_base64(const std::string& v);

std::string secure_hash_to_address(const std::string& hash, uint8_t chain_id);

}}

#endif /* __WAVESPP_UTILS_HPP_15740__ */
