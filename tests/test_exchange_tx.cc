#include <cstring>

#include "../src/utils.hpp"
#include "../src/tx/exchange.hpp"

using waves::utils::from_base58;
using waves::utils::to_base58;

int main()
{
    // https://wavesexplorer.com/testnet/tx/DjnFyQ78s4baaTS4hbA6n5S9SWsJKqtrivFo2mqRjHCD
    // curl -X GET --insecure --header 'Accept: application/json' 'http://localhost:6869/transactions/info/DjnFyQ78s4baaTS4hbA6n5S9SWsJKqtrivFo2mqRjHCD'
    const char* expected_tx_id = "DjnFyQ78s4baaTS4hbA6n5S9SWsJKqtrivFo2mqRjHCD";

    waves::ExchangeTransaction::Builder builder;
    waves::OrderBuilder order_builder1;
    waves::OrderBuilder order_builder2;

    std::vector<std::string> proofs1 = {
        from_base58("55qCwCYwpuYfoy5nqZhzc8Ww3bbqEvha3TQG2zL6dmQ46FA8ZbhmK8T2hJ5FZUatFYt42geFrtY5oM3Qgg7Vn6qi")
    };
    std::vector<std::string> proofs2 = {
        from_base58("gyH4ieAHMtbU3Mgcw2FpEQnKPeLMezt5j4x8dRdYfN98PfJoDqQyzDZxVjJ4Gs7WrqAhSjMUnkRz9uqSBfPFNdW")
    };

    order_builder1
        .setVersion(1)
        .setOrderType(static_cast<waves::OrderType>(0))
        .setSenderPublicKey(from_base58("5M618KzeuoxCrQBUZLLnv6EL2PqcJVbREES8mWLFCaHM"))
        .setMatcherPublicKey(from_base58("4oP8SPd7LiUo8xsokSTiyZjwg4rojdyXqWEq7NTwWsSU"))
        .setAmountAssetId("")
        .setPriceAssetId(from_base58("FExqRNegdpai5pw2vdEKPtsrUaWqQhpgLiVLopnqUMhZ"))
        .setAmount(1)
        .setPrice(100000000)
        .setTimestamp(1504019301490)
        .setExpiration(1504105701490)
        .setMatcherFee(488640)
        .setProofs(proofs1);

    order_builder2
        .setVersion(1)
        .setOrderType(static_cast<waves::OrderType>(1))
        .setSenderPublicKey(from_base58("5M618KzeuoxCrQBUZLLnv6EL2PqcJVbREES8mWLFCaHM"))
        .setMatcherPublicKey(from_base58("4oP8SPd7LiUo8xsokSTiyZjwg4rojdyXqWEq7NTwWsSU"))
        .setAmountAssetId("")
        .setPriceAssetId("FExqRNegdpai5pw2vdEKPtsrUaWqQhpgLiVLopnqUMhZ")
        .setAmount(1)
        .setPrice(100000000)
        .setTimestamp(1504019301490)
        .setExpiration(1506611301490)
        .setMatcherFee(445092)
        .setProofs(proofs2);

    // Common fields
    builder
        .setVersion(1)
        .setChainId(84)
        .setSenderPublicKey(from_base58("4oP8SPd7LiUo8xsokSTiyZjwg4rojdyXqWEq7NTwWsSU"))
        .setFee(567582)
        .setTimestamp(1504019301492);

    builder
        .setAmount(1)
        .setPrice(100000000)
        .setOrder1(order_builder1)
        .setOrder2(order_builder2)
        .setBuyMatcherFee(300000)
        .setSellMatcherFee(300000);

    const auto tx = builder.build();
    auto&& tx_id = to_base58(tx->id());

    if (tx_id != expected_tx_id) {
        fprintf(stderr, "Exchange TX ID does not match expected value: %s != %s\n",
                tx_id.c_str(), expected_tx_id);
        return 1;
    }

    return 0;
}
