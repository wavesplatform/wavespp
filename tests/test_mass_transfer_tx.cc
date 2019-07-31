#include <cstring>
#include <string>

#include "../src/utils.hpp"
#include "../src/address.hpp"
#include "../src/tx/mass_transfer.hpp"

using waves::utils::from_base58;
using waves::utils::to_base58;

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

// Converts binary string to hex representation
static std::string bin2hex(const unsigned char *data, size_t len)
{
    std::string s(len * 2, ' ');
    for (size_t i = 0; i < len; ++i) {
        s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}

int main()
{
#if 0
Mass Transfer TX ID = 64pWj5LfhKWghuaNHKvhV4W1uS3YGR1st8sdHNFDWYRe
Mass Transfer TX Body Bytes
(Base58): iHnbwbvFoEHfNQhW3TA6s2xQFJNHhGWNebYq9GLW3xhNmLnJfEBTeDvmNGnVB5wvna7wPiM4Cr28Ha8wnu4ZGh7jWbjTfnPVzEkLwpiRw5vz1f7Tq5Nf2RjxARYKtUrtwxedpRjqx4kYRvoe155qxJfW5ySamhptE5M29nzLoNdFMrYKJkAx786tkSaBHTf73pDcVgrJQD6DqLZyEEuobLsyY5ToryEN7vrkX6awEWxebXMfDr4gtKxjSSkJzBX5berEzBR2459HuMKgzsADbQ1yx9
(Hex): 0b01cff48bcc69c8d78e60a009c1ece9ee48045c765b42def115e6eb516a39f30b740000020154216a1659b3ab3c481a81550b2fa6d2d4242dbdb232b8deca00000000000186a001540535851541d341f55df872dad30ed5e66b7827952293da8c000000000001e24000000169296e82ad00000000004e9530000f54657374206174746163686d656e7401000100409ac7fbe97bfc7732bab9d2dd7d941eaa84941933728bb1a2552f2a5ee702737bbbbbebe4f8fc475dc68572a6bacc17b8fe38e6a2fa5721508a5b84fa69ec898e
#endif

    const char* expected_tx_id { "64pWj5LfhKWghuaNHKvhV4W1uS3YGR1st8sdHNFDWYRe" };

    //const std::string seed { "nasty sound ketchup inflict lecture fetch long tornado elbow scan inside shuffle" };
    const std::string public_key_str { "Ezmfw3GgJerTZFgSdzEnXydu1LJ52LsAFZXUF5c63UrF" };
    const std::string public_key_bin = from_base58(public_key_str);

    printf("public_key_bin = %s\n", bin2hex(reinterpret_cast<const unsigned char*>(public_key_bin.c_str()), public_key_bin.size()).c_str());

    const uint8_t chain_id { 'T' };
    //const auto nonce { 0 };
    const uint64_t fee { 5150000L }; // 0x 4e9530
    const std::string asset_id {""};
    // 0x 54657374206174746163686d656e74
    const std::string attachment_str { "Test attachment" };
    const uint64_t timestamp = 1551178302125L; // 0x 169296e82ad

    // 0154216a1659b3ab3c481a81550b2fa6d2d4242dbdb232b8deca
    const std::string recipient1 { "3MrxmAKscY6yupdg46vG22SfEpiWCRKCXoF" };
    // 01540535851541d341f55df872dad30ed5e66b7827952293da8c
    const std::string recipient2 { "3MpPdGWDEZpXauK7AuMSuz1jjzRPRW5sNwm" };
    const uint64_t amount1 = 100000L;
    const uint64_t amount2 = 123456L;

#if 0
    const std::string recipient1_bin = from_base58(recipient1);
    const std::string recipient2_bin = from_base58(recipient2);
    printf("recipient1 = %s\n", bin2hex(reinterpret_cast<const unsigned char*>(recipient1_bin.c_str()), recipient1_bin.size()).c_str());
    printf("recipient2 = %s\n", bin2hex(reinterpret_cast<const unsigned char*>(recipient2_bin.c_str()), recipient2_bin.size()).c_str());
#endif

    waves::MassTransferTransaction::Builder builder;
    builder
        .setAssetId(asset_id)
        .setFee(fee)
        .setChainId(chain_id)
        .setVersion(1)
        .setTimestamp(timestamp)
        .setSenderPublicKey(public_key_bin);

    builder
        .addTransferByAddress(from_base58(recipient1), amount1)
        .addTransferByAddress(from_base58(recipient2), amount2)
        .setAttachment(attachment_str);

    auto&& tx = builder.build();
    auto&& tx_id = to_base58(tx->id());
    auto&& bytes_vec = tx->bytes();
    const auto bytes = reinterpret_cast<const unsigned char*>(&bytes_vec[0]);
    const auto bytes_len = bytes_vec.size();
    auto&& tx_bytes_base58 = to_base58(bytes, bytes_len);

    printf("Mass Transfer TX bytes\n(base58): %s\n(hex): %s\n",
        tx_bytes_base58.c_str(),
        bin2hex(bytes, bytes_len).c_str()
    );

    if (tx_id != expected_tx_id) {
        fprintf(stderr, "Mass Transfer TX ID does not match expected value: %s != %s\n",
            tx_id.c_str(), expected_tx_id);
        return 1;
    }

    return 0;
}
