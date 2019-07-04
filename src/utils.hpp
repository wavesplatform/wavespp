#ifndef __WAVESPP_UTILS_HPP_15740__
#define __WAVESPP_UTILS_HPP_15740__

#include <string>

namespace waves {
namespace utils {

std::string to_base58(const std::string& v);
std::string to_base64(const std::string& v);

std::string from_base58(const std::string& v);
std::string from_base64(const std::string& v);

}}

#endif /* __WAVESPP_UTILS_HPP_15740__ */
