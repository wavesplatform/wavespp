#include "order.hpp"

namespace waves {

OrderBuilder::OrderBuilder() :
    _flags({
           BuilderFlags::HAS_VERSION,
           BuilderFlags::HAS_ORDER_TYPE,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_MATCHER_PUBLIC_KEY,
           BuilderFlags::HAS_PRICE,
           BuilderFlags::HAS_AMOUNT,
           BuilderFlags::HAS_TIMESTAMP,
           BuilderFlags::HAS_EXPIRATION,
           BuilderFlags::HAS_MATCHER_FEE,
           BuilderFlags::HAS_PROOFS
    })
{}

OrderBuilder& OrderBuilder::setVersion(uint8_t v)
{
    _flags.set(BuilderFlags::HAS_VERSION);
    _verison = v;
    return *this;
}

OrderBuilder& OrderBuilder::setOrderType(OrderType order_type)
{
    _flags.set(BuilderFlags::HAS_ORDER_TYPE);
    _order_type = order_type;
    return *this;
}

OrderBuilder& OrderBuilder::setSenderPublicKey(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_PUBLIC_KEY);
    _sender_public_key = v;
    return *this;
}

OrderBuilder& OrderBuilder::setMatcherPublicKey(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_MATCHER_PUBLIC_KEY);
    _matcher_public_key = v;
    return *this;
}

OrderBuilder& OrderBuilder::setAmountAssetId(const std::string& v)
{
    _amount_asset = v;
    return *this;
}

OrderBuilder& OrderBuilder::setPriceAssetId(const std::string& v)
{
    _price_asset = v;
    return *this;
}

OrderBuilder& OrderBuilder::setAmount(tx_amount_t v)
{
    _flags.set(BuilderFlags::HAS_AMOUNT);
    _amount = v;
    return *this;
}

OrderBuilder& OrderBuilder::setPrice(tx_amount_t v)
{
    _flags.set(BuilderFlags::HAS_PRICE);
    _price = v;
    return *this;
}

OrderBuilder& OrderBuilder::setTimestamp(tx_timestamp_t v)
{
    _flags.set(BuilderFlags::HAS_TIMESTAMP);
    _timestamp = v;
    return *this;
}

OrderBuilder& OrderBuilder::setExpiration(tx_timestamp_t v)
{
    _flags.set(BuilderFlags::HAS_EXPIRATION);
    _expiration = v;
    return *this;
}

OrderBuilder& OrderBuilder::setMatcherFee(tx_fee_t v)
{
    _flags.set(BuilderFlags::HAS_MATCHER_FEE);
    _matcher_fee = v;
    return *this;
}

OrderBuilder& OrderBuilder::setProofs(const std::vector<std::string>& proofs)
{
    _flags.set(BuilderFlags::HAS_PROOFS);
    _proofs = proofs;
    return *this;
}

void OrderBuilder::build(tx_order_t* order)
{
    _flags.check_and_throw();
    order->version = _verison;
    waves_tx_set_public_key_bytes(&order->sender_public_key, _sender_public_key.c_str());
    waves_tx_set_public_key_bytes(&order->matcher_public_key, _matcher_public_key.c_str());
    if (!_amount_asset.empty())
    {
        waves_tx_set_asset_id_bytes(&order->asset_pair.amount_asset, _amount_asset.c_str());
    }
    if (!_price_asset.empty())
    {
        waves_tx_set_asset_id_bytes(&order->asset_pair.price_asset, _price_asset.c_str());
    }
    order->order_type = static_cast<uint8_t>(_order_type);
    order->price = _price;
    order->amount = _amount;
    order->timestamp = _timestamp;
    order->expiration = _expiration;
    order->matcher_fee = _matcher_fee;
    if (_verison == 1)
    {
        const auto& sig = _proofs.at(0);
        tx_set_encoded_string_bytes(&order->signature, sig.c_str(), sig.size());
    }
    else if (_verison == 2)
    {
        tx_array_t* proofs_array = &order->proofs;
        for (const auto proof: _proofs)
        {
            tx_encoded_string_t* e = (tx_encoded_string_t*)tx_array_new_elem(proofs_array);
            tx_set_encoded_string_bytes(e, proof.c_str(), proof.size());
        }
    }
}

}
