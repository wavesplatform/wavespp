#include "reissue.hpp"

namespace waves {

ReissueTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_VERSION,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_ASSET_ID,
           BuilderFlags::HAS_REISSUABLE,
           BuilderFlags::HAS_QUANTITY,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
    })
{}

ReissueTransaction::Builder& ReissueTransaction::Builder::setAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ASSET_ID);
    _asset_id = v;
    return *this;
}

ReissueTransaction::Builder& ReissueTransaction::Builder::setQuantity(tx_quantity_t v)
{
    _flags.set(BuilderFlags::HAS_QUANTITY);
    _quantity = v;
    return *this;
}

ReissueTransaction::Builder& ReissueTransaction::Builder::setReissuable(bool v)
{
    _flags.set(BuilderFlags::HAS_REISSUABLE);
    _reissuable = v;
    return *this;
}

ReissueTransaction::ReissueTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

TransactionPtr ReissueTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_REISSUE);
    tx->version = _version;
    tx->data.reissue.chain_id = _chain_id;
    waves_tx_set_public_key_bytes(&tx->data.reissue.sender_public_key, _sender_public_key.c_str());
    waves_tx_set_asset_id_bytes(&tx->data.reissue.asset_id, _asset_id.c_str());
    tx->data.reissue.quantity = _quantity;
    tx->data.reissue.reissuable = _reissuable;
    tx->data.reissue.fee = _fee;
    tx->data.reissue.timestamp = _timestamp;
    return std::make_shared<ReissueTransaction>(tx);
}


tx_fee_t ReissueTransaction::fee() const
{
    return _tx->data.reissue.fee;
}

tx_timestamp_t ReissueTransaction::timestamp() const
{
    return _tx->data.reissue.timestamp;
}

}
