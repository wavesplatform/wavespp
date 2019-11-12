#ifndef __WAVESPP_TX_ORDER_HPP_6945__
#define __WAVESPP_TX_ORDER_HPP_6945__

#include "base.hpp"

namespace wavespp {

enum class OrderType
{
    BUY = 0,
    SELL = 1
};

class OrderBuilder
{
public:
    OrderBuilder();
    OrderBuilder& setVersion(uint8_t v);
    OrderBuilder& setOrderType(OrderType order_type);
    OrderBuilder& setSenderPublicKey(const std::string& v);
    OrderBuilder& setMatcherPublicKey(const std::string& v);
    OrderBuilder& setAmountAssetId(const std::string& v);
    OrderBuilder& setPriceAssetId(const std::string& v);
    OrderBuilder& setAmount(tx_amount_t v);
    OrderBuilder& setPrice(tx_amount_t v);
    OrderBuilder& setTimestamp(tx_timestamp_t v);
    OrderBuilder& setExpiration(tx_timestamp_t v);
    OrderBuilder& setMatcherFee(tx_fee_t v);
    OrderBuilder& setProofs(const std::vector<std::string>& proofs);
    void build(tx_order_t* order);
private:
    BuilderFlagsChecker _flags;
    uint8_t _verison;
    OrderType _order_type;
    std::string _sender_public_key;
    std::string _matcher_public_key;
    std::string _amount_asset;
    std::string _price_asset;
    tx_amount_t _price;
    tx_amount_t _amount;
    tx_timestamp_t _timestamp;
    tx_timestamp_t _expiration;
    tx_fee_t _matcher_fee;
    std::vector<std::string> _proofs;
};

}

#endif /* __WAVESPP_TX_ORDER_HPP_6945__ */
