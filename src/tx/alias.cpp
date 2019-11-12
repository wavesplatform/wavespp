#include "alias.hpp"

namespace wavespp {

AliasTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_VERSION,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_ALIAS,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP
    })
{}

AliasTransaction::Builder&
AliasTransaction::Builder::setAlias(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ALIAS);
    _alias = v;
    return *this;
}

TransactionPtr AliasTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_ALIAS);
    tx->version = _version;
    waves_tx_set_public_key_bytes(&tx->data.alias.sender_public_key, _sender_public_key.c_str());
    tx->data.alias.alias.chain_id = _chain_id;
    waves_tx_set_string(&tx->data.alias.alias.alias, _alias.c_str());
    tx->data.alias.fee = _fee;
    tx->data.alias.timestamp = _timestamp;
    return std::make_shared<AliasTransaction>(tx);
}

AliasTransaction::AliasTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t AliasTransaction::fee() const
{
    return _tx->data.alias.fee;
}

tx_timestamp_t AliasTransaction::timestamp() const
{
    return _tx->data.alias.timestamp;
}

}
