#include <cstring>

#include "../src/utils.hpp"
#include "../src/tx/exchange.hpp"

using wavespp::utils::from_base58;
using wavespp::utils::to_base58;

int main()
{
    // https://wavesexplorer.com/testnet/tx/DjnFyQ78s4baaTS4hbA6n5S9SWsJKqtrivFo2mqRjHCD
    // curl -X GET --header 'Accept: application/json' 'https://testnet1.wavesnodes.com/transactions/info/DjnFyQ78s4baaTS4hbA6n5S9SWsJKqtrivFo2mqRjHCD'
    const char* expected_tx_id = "DjnFyQ78s4baaTS4hbA6n5S9SWsJKqtrivFo2mqRjHCD";

    wavespp::ExchangeTransaction::Builder builder;
    wavespp::OrderBuilder order_builder1;
    wavespp::OrderBuilder order_builder2;

    const std::vector<std::string> proofs1 = {
        from_base58("55qCwCYwpuYfoy5nqZhzc8Ww3bbqEvha3TQG2zL6dmQ46FA8ZbhmK8T2hJ5FZUatFYt42geFrtY5oM3Qgg7Vn6qi")
    };
    const std::vector<std::string> proofs2 = {
        from_base58("gyH4ieAHMtbU3Mgcw2FpEQnKPeLMezt5j4x8dRdYfN98PfJoDqQyzDZxVjJ4Gs7WrqAhSjMUnkRz9uqSBfPFNdW")
    };
    const auto price_asset_id = from_base58("FExqRNegdpai5pw2vdEKPtsrUaWqQhpgLiVLopnqUMhZ");
    const auto amount_asset_id = "";
    const uint64_t amount = 1L;
    const uint64_t price = 100000000L;
    const uint64_t order_timestamp = 1504019301490L;
    const auto sender_public_key = from_base58("5M618KzeuoxCrQBUZLLnv6EL2PqcJVbREES8mWLFCaHM");
    const auto matcher_public_key = from_base58("4oP8SPd7LiUo8xsokSTiyZjwg4rojdyXqWEq7NTwWsSU");
    const uint8_t chain_id = 'T';
    const uint64_t buy_matcher_fee = 300000L;
    const uint64_t sell_matcher_fee = 300000L;
    const uint64_t buy_order_expiration = 1504105701490L;
    const uint64_t sell_order_expiration = 1506611301490L;
    const uint64_t buy_order_fee = 488640L;
    const uint64_t sell_order_fee = 445092L;
    const auto order_version = 1;
    const auto tx_version = 1;

    order_builder1
        .setVersion(order_version)
        .setOrderType(wavespp::OrderType::BUY)
        .setSenderPublicKey(sender_public_key)
        .setMatcherPublicKey(matcher_public_key)
        .setAmountAssetId(amount_asset_id)
        .setPriceAssetId(price_asset_id)
        .setAmount(amount)
        .setPrice(price)
        .setTimestamp(order_timestamp)
        .setExpiration(buy_order_expiration)
        .setMatcherFee(buy_order_fee)
        .setProofs(proofs1);

    order_builder2
        .setVersion(order_version)
        .setOrderType(wavespp::OrderType::SELL)
        .setSenderPublicKey(sender_public_key)
        .setMatcherPublicKey(matcher_public_key)
        .setAmountAssetId(amount_asset_id)
        .setPriceAssetId(price_asset_id)
        .setAmount(amount)
        .setPrice(price)
        .setTimestamp(order_timestamp)
        .setExpiration(sell_order_expiration)
        .setMatcherFee(sell_order_fee)
        .setProofs(proofs2);

    // Common TX fields
    builder
        .setVersion(tx_version)
        .setChainId(chain_id)
        .setSenderPublicKey(sender_public_key)
        .setFee(567582L)
        .setTimestamp(1504019301492L);

    builder
        .setAmount(amount)
        .setPrice(price)
        .setOrder1(order_builder1)
        .setOrder2(order_builder2)
        .setBuyMatcherFee(buy_matcher_fee)
        .setSellMatcherFee(sell_matcher_fee);

    auto&& tx = builder.build();
    auto&& tx_id = to_base58(tx->id());
    auto&& bytes_vec = tx->bytes();
    auto&& tx_bytes = to_base58(&bytes_vec[0], bytes_vec.size());

    printf("Exchange TX bytes (base58): %s\n", tx_bytes.c_str());

    if (tx_id != expected_tx_id) {
        fprintf(stderr, "Exchange TX ID does not match expected value: %s != %s\n",
                tx_id.c_str(), expected_tx_id);
        return 1;
    }

    return 0;
}
