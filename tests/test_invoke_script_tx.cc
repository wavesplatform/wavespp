#include <cstdio>
#include <cstring>
#include <memory>
#include <sstream>

#include <jsoncpp/json/json.h>

#include "../src/utils.hpp"
#include "../src/tx/invoke_script.hpp"

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
    for (size_t i = 0; i < len; i += 2) {
        if (!isxdigit(hex[i]) || !isxdigit(hex[i+1])) {
            return "";
        }
        res += to_nibble(hex[i]) * 16 + to_nibble(hex[i+1]);
    }
    return res;
}/*}}}*/

static inline std::string hex2bin(const char* hex) noexcept /*{{{*/
{
    return hex2bin(hex, strlen(hex));
}/*}}}*/

static inline std::string hex2bin(const std::string s) noexcept /*{{{*/
{
    return hex2bin(s.c_str(), s.size());
}//}}}

static int test_func_call_serialization()//{{{
{
    // https://wavesexplorer.com/testnet/tx/6e1tjxaZGRR7RoYGvqvgANvBDCB9mfEcZZj6zyV5tcZg
    const char* expected_tx_id = "6e1tjxaZGRR7RoYGvqvgANvBDCB9mfEcZZj6zyV5tcZg";
    const char* expected_func_name = "withdraw";
    const char* expected_arg0 = "31KuAvrq2PsiQZL1dqZKFZyNYAfgpLchK99XiGmZXUg3";
    const char* expected_arg1 = "base64:W6LAllyAaOmtHi8ixFXBUvmzact63Xowv5F7kKpCZUCA0OcSzpzNyGiwKCxgJi6KToe83USFV4KAYT33z9iLRpuCy/GL5qIT7H3HqIraa7R/naxV+vjStXFd6akwFcYglpChcTUXYB2TU/SspH5ONlTG6FZmDBQxGwCHznrvDeVkkWfdcfLkyJ5tSrYzP8VlRDBcwuun4oX6b1fotNXJrrFNG7Ht/E14boxfkZ8E2BlWCttKOgE8ZsdrVqOx8uTb4KNAFZJQ2z/3op4zNHu22mcseJMFEA3N80ryDeo/e072iqoWTCmSi68LNE+KaR/sFV4uEPkl0Gp9tQwyWXSweg==";

    waves::InvokeScriptTransaction::Builder builder;
    builder
        .setVersion(1)
        .setChainId(84)
        .setSenderPublicKey(hex2bin("3657e0319014c536d0f8219b6eea31e189c283889d5d9ca299d8e75a73bbd111"))
        .setFee(500000)
        .setTimestamp(1563347126887);
    builder
        .setFunctionCall(hex2bin("01090100000008776974686472617700000002020000002c33314b754176727132507369515a4c3164715a4b465a794e59416667704c63684b39395869476d5a5855673301000001005ba2c0965c8068e9ad1e2f22c455c152f9b369cb7add7a30bf917b90aa42654080d0e712ce9ccdc868b0282c60262e8a4e87bcdd4485578280613df7cfd88b469b82cbf18be6a213ec7dc7a88ada6bb47f9dac55faf8d2b5715de9a93015c6209690a1713517601d9353f4aca47e4e3654c6e856660c14311b0087ce7aef0de5649167dd71f2e4c89e6d4ab6333fc56544305cc2eba7e285fa6f57e8b4d5c9aeb14d1bb1edfc4d786e8c5f919f04d819560adb4a3a013c66c76b56a3b1f2e4dbe0a340159250db3ff7a29e33347bb6da672c789305100dcdf34af20dea3f7b4ef68aaa164c29928baf0b344f8a691fec155e2e10f925d06a7db50c325974b07a"))
        .setDappPublicKeyHash(hex2bin("104710bd7c2a27f9b0c64f7ccc712fde2bfc0aae"));

    const auto tx = builder.build();

    auto&& tx_id = to_base58(tx->id());
    auto&& bytes_vec = tx->bytes();
    const auto bytes = reinterpret_cast<const unsigned char*>(bytes_vec.data());
    const auto bytes_len = bytes_vec.size();
    auto&& tx_bytes_base58 = to_base58(bytes, bytes_len);

    const auto invoke_script_tx_ptr = reinterpret_cast<waves::InvokeScriptTransaction*>(tx.get());
    const auto fc = invoke_script_tx_ptr->function_call();
    const auto func_name = fc.func_name();
    const auto args_json = fc.args_json();

    Json::Value root;
    Json::CharReaderBuilder reader;
    reader["collectComments"] = false;
    JSONCPP_STRING errs;
    std::istringstream stream(args_json);

    if (!parseFromStream(reader, stream, &root, &errs)) {
        fprintf(stderr, "Failed to parse function arguments\n");
        return 1;
    }

    printf("Invoke Script TX bytes\n"
            "(base58): %s\n"
            "(hex): %s\nfunction name: %s\n"
            "function args: %s\n",
            tx_bytes_base58.c_str(),
            bin2hex(bytes, bytes_len).c_str(),
            func_name.c_str(),
            args_json.c_str()
    );

    if (tx_id != expected_tx_id) {
        fprintf(stderr, "Invoke Script TX ID does not match expected value: %s != %s\n",
            tx_id.c_str(), expected_tx_id);
        return 1;
    }

    if (func_name != expected_func_name) {
        fprintf(stderr, "Wrong function name: %s != %s\n", func_name.c_str(), expected_func_name);
        return 1;
    }

    if (!root.isArray()) {
        fprintf(stderr, "Argument list is expected to be an array\n");
        return 1;
    }
    const Json::Value nullValue;
    const auto arg0 = root.get(static_cast<unsigned>(0), nullValue).asString();
    const auto arg1 = root.get(static_cast<unsigned int>(1), Json::nullValue).asString();

    if (arg0 != expected_arg0) {
        fprintf(stderr, "Wrong arg0 value: %s != %s\n", arg0.c_str(), expected_arg0);
        return 1;
    }
    if (arg1 != expected_arg1) {
        fprintf(stderr, "Wrong arg1 value: %s != %s\n", arg1.c_str(), expected_arg1);
        return 1;
    }

    return 0;
}//}}}

static int test_default_func_call_serialization()//{{{
{
    // https://wavesexplorer.com/testnet/tx/4n78dua6HRcrPqvX56qz8TVqcmBqvnKLMDndzE7w63DU
    const char* expected_tx_id = "4n78dua6HRcrPqvX56qz8TVqcmBqvnKLMDndzE7w63DU";
#if 0
Tx bytes in hex:
1001543d155a4972d5f000e0d9f904d8fee756ec3b835ba21133f301c7d8489e4a1e4f02540009746573742d64617070000000000000000007a120000000016c0022286e

10 // tx type
01 // version
54 // chain id
3d155a4972d5f000e0d9f904d8fee756ec3b835ba21133f301c7d8489e4a1e4f // sender public key
02 // alias flag
54 // chain id
0009 // alias length
746573742d64617070 // alias
00 // default
0000 // payments number
000000000007a120 // fee
00 // fee asset id
0000016c0022286e // timestamp
#endif

    waves::InvokeScriptTransaction::Builder builder;
    builder
        .setVersion(1)
        .setChainId(84)
        .setSenderPublicKey(hex2bin("3d155a4972d5f000e0d9f904d8fee756ec3b835ba21133f301c7d8489e4a1e4f"))
        .setFee(500000) // 7a120
        .setTimestamp(1563370334318) // 16c0022286e
    ;

    auto&& tx = builder
        .setFunctionCall(""/*0x0*/)
        .setDappAlias(hex2bin("746573742d64617070")) // test-dapp
        .build();

    auto&& tx_id = to_base58(tx->id());
    auto&& bytes_vec = tx->bytes();
    const auto bytes = reinterpret_cast<const unsigned char*>(bytes_vec.data());
    const auto bytes_len = bytes_vec.size();
    auto&& tx_bytes_base58 = to_base58(bytes, bytes_len);

    printf("Invoke Script TX bytes\n"
            "(base58): %s\n"
            "(hex): %s\n",
            tx_bytes_base58.c_str(),
            bin2hex(bytes, bytes_len).c_str()
          );

    if (tx_id != expected_tx_id) {
        fprintf(stderr, "Invoke Script TX ID does not match expected value: %s != %s\n",
                tx_id.c_str(), expected_tx_id);
        return 1;
    }

    return 0;
}//}}}

int main()
{
    int res = 0;

    do {
        if ((res = test_func_call_serialization()) != 0) break;
        if ((res = test_default_func_call_serialization()) != 0) break;
    } while (false);

    return res;
}
