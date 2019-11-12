#include <cstring>
#include <iostream>

#include "../src/utils.hpp"

int main(int argc, char const* argv[])
{
    const char* expected_base58 = "oT2YHgTvpkeAHLLtXTHdzAxq9iY4kG953J";
    const char* expected_raw = "some string one two three";

    const auto res_base58 = wavespp::utils::to_base58(
            reinterpret_cast<const unsigned char*>(expected_raw),
            std::strlen(expected_raw));
    if (res_base58.compare(expected_base58) != 0) {
        fprintf(stderr, "to_base58() returned incorrect result: %s\n", res_base58.c_str());
        return 1;
    }

    const auto res_raw = wavespp::utils::from_base58(res_base58);
    if (std::strncmp(res_raw.c_str(), expected_raw, res_raw.size())) {
        fprintf(stderr, "from_base58() returned value different from expected %s\n",
                expected_raw);
    }

    return 0;
}
