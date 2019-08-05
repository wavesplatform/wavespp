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
static std::string bin2hex(const unsigned char *data, size_t len)/*{{{*/
{
    std::string s(len * 2, ' ');
    for (size_t i = 0; i < len; ++i) {
        s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}/*}}}*/

static unsigned char to_nibble (char c) noexcept/*{{{*/
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + c - 'a';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    return 0xFF;
}/*}}}*/

static std::string hex2bin(const char* hex, size_t len) noexcept /*{{{*/
{
    std::string res;
    res.reserve(len / 2);
    if (len & 1) {
        return "";
    }
    for (size_t i = 0; i < len; i++) {
        if (!isxdigit(hex[i]) || !isxdigit(hex[i+1])) {
            return "";
        }
        res += to_nibble(hex[i]) * 16 + to_nibble(hex[i+1]);
    }
    return res;
}/*}}}*/

static inline std::string hex2bin(const char* hex) noexcept /*{{{*//*{{{*/
{
    return hex2bin(hex, std::char_traits<char>::length(hex));
}/*}}}*/

static int simple_test()//{{{
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
}//}}}

// https://wavesexplorer.com/testnet/tx/CRDtQ3TQ8WvkM9J8Zq7c3ep1J3n1GqvZM7bErL31gCeq
int hundred_transfers_test()
{

    waves::MassTransferTransaction::Builder builder;

    // https://wavesexplorer.com/testnet/tx/CRDtQ3TQ8WvkM9J8Zq7c3ep1J3n1GqvZM7bErL31gCeq

    builder.setVersion(1)
        .setChainId(84)
        .setSenderPublicKey(hex2bin("f0a8c3a7f0cb4e8669a4e3c85f36bb9d34fa5f0ee8906cfb2c976a5e96ad2065"))
        .setFee(5150000L)
        .setTimestamp(1551178449125L);

    builder.setAssetId(hex2bin("b12558c530fb3509a46a2eb165f15eedb6e40391de95eaf530d180f267921138"))
        .setAttachment("");

    builder.addTransferByAddress(hex2bin("01544c690cce8adea26c6b50ec1804560acd8ee7be7c0d5b193f"), 100000);
    builder.addTransferByAddress(hex2bin("0154d6834f213553f627b1a39ce956a01bafef8fc08772a6a55b"), 100000);
    builder.addTransferByAddress(hex2bin("0154c1ab79a3c25c32f8d206b0926cbb7ca2c478d2991fec5870"), 100000);
    builder.addTransferByAddress(hex2bin("01549c7413e672c46a3cd6b7a56dcb2d448cb04f26ab58c7905a"), 100000);
    builder.addTransferByAddress(hex2bin("01548fe052c69a507048a9f67b7dbb7a456a38f74bd550df9bb8"), 100000);
    builder.addTransferByAddress(hex2bin("0154ac8fb65cf848b30ee3f59635a95e57edf5a267c64bebf093"), 100000);
    builder.addTransferByAddress(hex2bin("01546d5bbc64d0c017b5e453977f5319b997f860b1be9508f8a3"), 100000);
    builder.addTransferByAddress(hex2bin("01549c9f6a066e197b529ff4a9bd7cd817d467e433f4b8755b32"), 100000);
    builder.addTransferByAddress(hex2bin("0154ee8066847e7add4b682dd547373e6845fea2e58e00cf8e63"), 100000);
    builder.addTransferByAddress(hex2bin("015468743586450a5b8f6a4b492424cac34cf6555a3952cf6a07"), 100000);
    builder.addTransferByAddress(hex2bin("0154c3aec2d80365226108a9a5800c47cc53ef1f03bf51bee798"), 100000);
    builder.addTransferByAddress(hex2bin("01543cd177aa937d87cedd0cdb847c02ec9863c3eb36d1523388"), 100000);
    builder.addTransferByAddress(hex2bin("0154706f79a600ba62dcb0b8f4b0af755bf040469b4ac0aa8f0a"), 100000);
    builder.addTransferByAddress(hex2bin("015417ed27915497c0e2d118cbf257a1b61bdfa9d295b412cfb3"), 100000);
    builder.addTransferByAddress(hex2bin("0154e7ccc12ced76f63b8a6a93d8fee47a68078f0fe71c546f95"), 100000);
    builder.addTransferByAddress(hex2bin("01545dba79687027980bebd73e603dd2e7acfb9263470170e9c5"), 100000);
    builder.addTransferByAddress(hex2bin("0154c70f091399fc2f32cbd4bede26b72020e5896fe1ad4c285b"), 100000);
    builder.addTransferByAddress(hex2bin("01543817b2d94fc7923cb2f42f0e96954b809bdadffae387305a"), 100000);
    builder.addTransferByAddress(hex2bin("01543bfc125de4bf9cdafa9da1f004e3b92166fa01d7c3c120a9"), 100000);
    builder.addTransferByAddress(hex2bin("0154acf0e43fefe28f560e6fdf0f9436895ed181c720937b529e"), 100000);
    builder.addTransferByAddress(hex2bin("0154b1b8f7383df05d2245aaa569b87ec12fb2d6d8c7b5eb73a3"), 100000);
    builder.addTransferByAddress(hex2bin("01545493d07f8a136cc8ea10e39f92ebf47c3bd60594e2c7772f"), 100000);
    builder.addTransferByAddress(hex2bin("0154c400944494cdecb4db98c025d0d007439217ea025b1e7693"), 100000);
    builder.addTransferByAddress(hex2bin("01547ac2634aef0216335b4131839f4952f03ea9881177b6a180"), 100000);
    builder.addTransferByAddress(hex2bin("01544eb16b406fd702eadede2c8390730f08c3290c45a7a80f95"), 100000);
    builder.addTransferByAddress(hex2bin("0154b792b809c8a616f076f688eba4c3bc0e054aa65d3dfbd345"), 100000);
    builder.addTransferByAddress(hex2bin("0154bd55e7ea44fe4f72278839b5ca196e0ea368c61f04c915aa"), 100000);
    builder.addTransferByAddress(hex2bin("0154cbd08947331392c726ea3cd0d7ab23dc9c0f13de8040afc6"), 100000);
    builder.addTransferByAddress(hex2bin("0154464fa4d04329a1a67c952da1613855403816a823b96db058"), 100000);
    builder.addTransferByAddress(hex2bin("0154b399979c7c9810ec1a268d1453fc6cc47906e6771906f8a6"), 100000);
    builder.addTransferByAddress(hex2bin("0154ec1680d77d66c711bc190feafe43435094b225c32c92aaa5"), 100000);
    builder.addTransferByAddress(hex2bin("01544574a168cb1892f57af1c78f9437d35702327ba900415365"), 100000);
    builder.addTransferByAddress(hex2bin("015418a5655d5d474df2617f5d64015485cb0ff4f09f89ac13c4"), 100000);
    builder.addTransferByAddress(hex2bin("01542572999659c51b967d10177b227c27b3fab63f9e4c4376d5"), 100000);
    builder.addTransferByAddress(hex2bin("015434880bef338bbd2f624cee2a9c75d16ce35f6f0144aecd7c"), 100000);
    builder.addTransferByAddress(hex2bin("01545e9336ecdceaf334235b1768f3df3a922f4e1f6a31f0e1eb"), 100000);
    builder.addTransferByAddress(hex2bin("01541551800371047e790b3451604252b9ac254a65ffb3f70326"), 100000);
    builder.addTransferByAddress(hex2bin("0154c05a6e3845b774ef9979d6ed284de8803e02cfec807dbe26"), 100000);
    builder.addTransferByAddress(hex2bin("0154fceb1769a33c1bcc1ff05ffaa710f4d8cd0bef7916674f7a"), 100000);
    builder.addTransferByAddress(hex2bin("01546da34bdeded9bef1d77c954a8e6a3cacb3ddfd79d8910d65"), 100000);
    builder.addTransferByAddress(hex2bin("01541e19509a8013646fb0b10423240615702711befddca32c99"), 100000);
    builder.addTransferByAddress(hex2bin("0154826efd68ba2bf86f2a37ec31fcac2e7e0ed0b5255b3c3475"), 100000);
    builder.addTransferByAddress(hex2bin("0154d13dc0cfa0337ff5b1c0b70c738c7063994cbbb04eb9e81c"), 100000);
    builder.addTransferByAddress(hex2bin("0154e1ec6c2d654ea7b4e22f1b71eb070b333c3b0fa73e809fda"), 100000);
    builder.addTransferByAddress(hex2bin("0154f475582b741e9251c5d32f06d3038081d24907b53ae4adde"), 100000);
    builder.addTransferByAddress(hex2bin("0154ba93b9eeba9074f08029da767d016248e66887b573b849c5"), 100000);
    builder.addTransferByAddress(hex2bin("0154ab614179e5802e52fe3fe6cbd718e5d053b12c8323788088"), 100000);
    builder.addTransferByAddress(hex2bin("01549a3a8964c1b539ecc97724e92fc42f5d5598fbb51110c547"), 100000);
    builder.addTransferByAddress(hex2bin("0154af3b40cd28e50774cfdd438f25b3b9bce12b1ba8d8f42383"), 100000);
    builder.addTransferByAddress(hex2bin("01543f001762717f1e48dc05e24c0e81c8b8acf8f5d464880e72"), 100000);
    builder.addTransferByAddress(hex2bin("015452b8b4333a53b61446eaae7ba3814d7d2cb5924bab31cae4"), 100000);
    builder.addTransferByAddress(hex2bin("01544ea5fb14071087ff8683632822ead19ecba132fd7c228e67"), 100000);
    builder.addTransferByAddress(hex2bin("0154823906e89e6c47d1d6bc0e4fb97f5aee9a6e323aaa2f2614"), 100000);
    builder.addTransferByAddress(hex2bin("0154cf4ce7dd98c1917f1fb19977a4031893126ceb9c166ea9dc"), 100000);
    builder.addTransferByAddress(hex2bin("0154a6216af507917375a865f48eba6e37ee23466f03a49592d0"), 100000);
    builder.addTransferByAddress(hex2bin("0154fdef76fcf9f6894a3f8d27ec5356165b55ef75245763012a"), 100000);
    builder.addTransferByAddress(hex2bin("0154cf571f20a70dc0ccb517bc5c1bd914998adafb4404b1eff1"), 100000);
    builder.addTransferByAddress(hex2bin("015474f8f08786a39dc3a9c8e5be21354cb937269ea182accd30"), 100000);
    builder.addTransferByAddress(hex2bin("0154e235734f077a917e17a1e997ceaad068cc197a8da11393dd"), 100000);
    builder.addTransferByAddress(hex2bin("0154fa1aab5dc402cf904c6d257ae5bbf34b6e06ed6e8537f030"), 100000);
    builder.addTransferByAddress(hex2bin("01543443de4b3c7cc3b65c78403eef20ab14a927bec4c2baf1be"), 100000);
    builder.addTransferByAddress(hex2bin("015426eee2a0440e565cb8b019f625cdbe7b065f7a128c5a7d5f"), 100000);
    builder.addTransferByAddress(hex2bin("0154738a8e45a2173b3424c5ad212d49fc5aafff1bf03d551a38"), 100000);
    builder.addTransferByAddress(hex2bin("01543c8ad2655cbc701d847387c4edf541644a5e19233ec2558e"), 100000);
    builder.addTransferByAddress(hex2bin("01545edaa412477239af002bb6a55982263a5a3d5df7d046f391"), 100000);
    builder.addTransferByAddress(hex2bin("01547816585fab5f999da24a343d478ac6039d6e907710a4e06c"), 100000);
    builder.addTransferByAddress(hex2bin("0154aa9609321ac3da9ca95cfb3da1b633b26969a0c50c237186"), 100000);
    builder.addTransferByAddress(hex2bin("01549e3dd84c03d38f8004292d71caf8b0cf0d0b6f7f3ab06b3b"), 100000);
    builder.addTransferByAddress(hex2bin("0154223569768bec1d82d02dc6de630f945d8073c3b392a80ee3"), 100000);
    builder.addTransferByAddress(hex2bin("01546f192943d1e8997bb200dc8149be0a03c6a0a94f06804313"), 100000);
    builder.addTransferByAddress(hex2bin("01540e2235cf98d786bdced711a2da688389bd4123528c3b3686"), 100000);
    builder.addTransferByAddress(hex2bin("01540ffefac54105ba94c7a00ecc1babd767d559a4e01d556458"), 100000);
    builder.addTransferByAddress(hex2bin("01542e611f0b75f6c4870c85ecdefa64f609b4a3a4b170186fa3"), 100000);
    builder.addTransferByAddress(hex2bin("0154d73addf2dfff7dd8f470b3bde30ed9b6c8b3e1f83b172393"), 100000);
    builder.addTransferByAddress(hex2bin("0154d0ae0e9d70378063d521cefea6e5f3dbe8e6139a5f1e6750"), 100000);
    builder.addTransferByAddress(hex2bin("01540d2f4f89a603f45878dc2554163c1064ded3ca50ea717c06"), 100000);
    builder.addTransferByAddress(hex2bin("01547101ef0bf406c1a04bd73a9284c9366701a4bc0177185089"), 100000);
    builder.addTransferByAddress(hex2bin("01549273cd0d47e40a51d29a43b772fb7afb5a7309477b8e91f3"), 100000);
    builder.addTransferByAddress(hex2bin("0154c20aa398eee04707bb5c8471e140cbabdd9ae02d54fe39f1"), 100000);
    builder.addTransferByAddress(hex2bin("0154f17fa653ef49cb6f0a0be0dc28ea4bd8fd11f9294ec42bc5"), 100000);
    builder.addTransferByAddress(hex2bin("0154357f6067bfa6ed05b97c0a2403acd2ab77d4f61ea2008f1f"), 100000);
    builder.addTransferByAddress(hex2bin("0154076f31366a64b1ab6cb8f4d6fa2f4bf73ebe57185c8350f5"), 100000);
    builder.addTransferByAddress(hex2bin("0154650199d37ce88df27170ace83401a7fa8c2688332d051d45"), 100000);
    builder.addTransferByAddress(hex2bin("0154f5239fa1d86ca891df103cb7bc4bc33f4e9a90f84f9aa990"), 100000);
    builder.addTransferByAddress(hex2bin("015482981bf30875534271bb3e635cba6bdbe06a1207dffc27cd"), 100000);
    builder.addTransferByAddress(hex2bin("0154e9cca04b7db92de0006983b88c1412b7f6ae0d25bd0d48c0"), 100000);
    builder.addTransferByAddress(hex2bin("0154aa44ec38886ff18270d051f1d913abc0208ffa264e01d225"), 100000);
    builder.addTransferByAddress(hex2bin("01544ca0041bd3fc8b330318fa1fd807158c88be240b74f0a2c6"), 100000);
    builder.addTransferByAddress(hex2bin("01547154c5693ea2f8c6f58a7b2fc2fb63da19726a2b8101c9e9"), 100000);
    builder.addTransferByAddress(hex2bin("0154c9f4b6cf187dc18831863422440d2ac5689a1fade51a3cbe"), 100000);
    builder.addTransferByAddress(hex2bin("0154b403ab358b40e460fb7752f1ae4d3f84167438494b7ca4ac"), 100000);
    builder.addTransferByAddress(hex2bin("015491dbb34292e09097d5e33d3f4b80ae292bcc279f09d025f6"), 100000);
    builder.addTransferByAddress(hex2bin("01549408c6c50f63720382e1c60830a4348ca176642990e1c471"), 100000);
    builder.addTransferByAddress(hex2bin("015492be63efd74ee35c4ab58b823cbc918a3a83831c46a3cfac"), 100000);
    builder.addTransferByAddress(hex2bin("015451738a0ae6119ad3d9be359d03953a37983a398d27f7cba3"), 100000);
    builder.addTransferByAddress(hex2bin("0154ddbc166b5c364cc0f67ddc89d4d0e898f6c1109ab328f91d"), 100000);
    builder.addTransferByAddress(hex2bin("01547dc5d63ac40de4a35525ad66e27a91b7cac28d8fe32e9961"), 100000);
    builder.addTransferByAddress(hex2bin("015495df57cd29d5bf48206c3ca7f198a6c8aef3759efdc21d9c"), 100000);
    builder.addTransferByAddress(hex2bin("01548cbfb085087f4eb225d6e062b0860996db6692f8e12b917e"), 100000);
    builder.addTransferByAddress(hex2bin("0154a795bc84b0233a701278b170ea69c2efdf01cd7de3e7ec56"), 100000);
    auto tx_ptr = builder.build();

    const auto id_base58 = to_base58(tx_ptr->id());
    const char* expected_id_base58 = "CRDtQ3TQ8WvkM9J8Zq7c3ep1J3n1GqvZM7bErL31gCeq";
    printf("%s: TX ID = %s\n", __func__, id_base58.c_str());
    if (id_base58 != expected_id_base58) {
        fprintf(stderr, "Mass transfer tx id %s != %s\n", id_base58.c_str(), expected_id_base58);
        return 1;
    }

    return 0;
}

int main()//{{{
{
    int res = 0;

    do {
        if ((res = simple_test()) != 0) break;
        if ((res = hundred_transfers_test()) != 0) break;
    } while (false);

    return res;
}//}}}

// vim: fdm=marker
