#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>

#include "../src/address.hpp"
#include "../src/utils.hpp"

int main(int argc, char const* argv[])
{

    const std::string base58_address = "3MtBqEtkF8cYkNJv85QUS4wteNj48ZMAnt9";
    const std::string bin_address = waves::utils::from_base58(base58_address);

    if (waves::utils::to_base58(bin_address) != base58_address) {
        fprintf(stderr, "waves::utils::to_base58/from_base58 results do not match\n");
        return 1;
    }
    if (bin_address.size() != waves::address::ADDRESS_BIN_LEN) {
        fprintf(stderr, "bin_address.size() = %ld != ADDRESS_BIN_LEN\n", bin_address.size());
        return 1;
    }

    //const unsigned char bin_uchar_address[waves::address::ADDRESS_BIN_LEN] = bin_address.c_str();
    unsigned char bin_uchar_address[waves::address::ADDRESS_BIN_LEN] = {0};
    std::copy(bin_address.begin(), bin_address.end(), bin_uchar_address);
    waves::address address(bin_uchar_address);

    if (address.to_base58() != base58_address) {
        fprintf(stderr, "waves::address::to_base58() != %s\n", base58_address.c_str());
        return 1;
    }

    return 0;
}
