#include "burn.hpp"

namespace waves {

BurnTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_VERSION,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_QUANTITY,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP
    })
{}

BurnTransaction::Builder&
BurnTransaction::Builder::setAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ASSET_ID);
    _asset_id = v;
    return *this;
}

BurnTransaction::Builder&
BurnTransaction::Builder::setQuantity(tx_quantity_t quantity)
{
    _flags.set(BuilderFlags::HAS_QUANTITY);
    _quantity = quantity;
    return *this;
}

BurnTransaction::BurnTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

TransactionPtr BurnTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_BURN);
    tx->version = _version;
    waves_tx_set_public_key_bytes(&tx->data.burn.sender_public_key, _sender_public_key.c_str());
    if (!_asset_id.empty())
    {
        waves_tx_set_asset_id_bytes(&tx->data.burn.asset_id, _asset_id.c_str());
    }
    tx->data.burn.chain_id = _chain_id;
    tx->data.burn.quantity = _quantity;
    tx->data.burn.fee = _fee;
    tx->data.burn.timestamp = _timestamp;
    return std::make_shared<BurnTransaction>(tx);
}

tx_fee_t BurnTransaction::fee() const
{
    return _tx->data.burn.fee;
}

tx_timestamp_t BurnTransaction::timestamp() const
{
    return _tx->data.burn.timestamp;
}

}
