#include "set_asset_script.hpp"

namespace wavespp {

SetAssetScriptTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_ASSET_ID,
           BuilderFlags::HAS_SCRIPT,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
    })
{
}

SetAssetScriptTransaction::Builder&
SetAssetScriptTransaction::Builder::setScript(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_SCRIPT);
    _script = v;
    return *this;
}

SetAssetScriptTransaction::Builder&
SetAssetScriptTransaction::Builder::setAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ASSET_ID);
    _asset_id = v;
    return *this;
}

TransactionPtr SetAssetScriptTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_SET_ASSET_SCRIPT);
    tx->version = TX_VERSION_1;
    tx->data.set_asset_script.chain_id = _chain_id;
    waves_tx_set_asset_id_bytes(&tx->data.set_asset_script.asset_id, _asset_id.c_str());
    waves_tx_set_public_key_bytes(&tx->data.set_asset_script.sender_public_key, _sender_public_key.c_str());
    tx_set_encoded_string_bytes(&tx->data.set_asset_script.script, _script.c_str(), _script.size());
    tx->data.set_asset_script.fee = _fee;
    tx->data.set_asset_script.timestamp = _timestamp;
    return std::make_shared<SetAssetScriptTransaction>(tx);
}

SetAssetScriptTransaction::SetAssetScriptTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t SetAssetScriptTransaction::fee() const
{
    return _tx->data.set_asset_script.fee;
}

tx_timestamp_t SetAssetScriptTransaction::timestamp() const
{
    return _tx->data.set_asset_script.timestamp;
}

}
