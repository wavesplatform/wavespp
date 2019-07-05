#include "set_script.hpp"

namespace waves {

SetScriptTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_SCRIPT,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
    })
{
}

SetScriptTransaction::Builder&
SetScriptTransaction::Builder::setScript(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_SCRIPT);
    _script = v;
    return *this;
}

TransactionPtr SetScriptTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_SET_SCRIPT);
    tx->version = TX_VERSION_1;
    tx->data.set_script.chain_id = _chain_id;
    waves_tx_set_public_key_bytes(&tx->data.set_script.sender_public_key, _sender_public_key.c_str());
    tx_set_encoded_string_bytes(&tx->data.set_script.script, _script.c_str(), _script.size());
    tx->data.set_script.fee = _fee;
    tx->data.set_script.timestamp = _timestamp;
    return std::make_shared<SetScriptTransaction>(tx);
}

SetScriptTransaction::SetScriptTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t SetScriptTransaction::fee() const
{
    return _tx->data.set_script.fee;
}

tx_timestamp_t SetScriptTransaction::timestamp() const
{
    return _tx->data.set_script.timestamp;
}

}
