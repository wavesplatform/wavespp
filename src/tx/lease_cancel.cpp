#include "lease_cancel.hpp"

namespace wavespp {

LeaseCancelTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_VERSION,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_LEASE_ID,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
    })
{
}

LeaseCancelTransaction::Builder&
LeaseCancelTransaction::Builder::setLeaseId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_LEASE_ID);
    _lease_id = v;
    return *this;
}

LeaseCancelTransaction::LeaseCancelTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

TransactionPtr LeaseCancelTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_CANCEL_LEASE);
    tx->version = _version;
    waves_tx_set_public_key_bytes(&tx->data.lease_cancel.sender_public_key, _sender_public_key.c_str());
    tx->data.lease_cancel.chain_id = _chain_id;
    tx->data.lease_cancel.fee = _fee;
    tx->data.lease_cancel.timestamp = _timestamp;
    waves_tx_set_lease_id_bytes(&tx->data.lease_cancel.lease_id, _lease_id.c_str());
    return std::make_shared<LeaseCancelTransaction>(tx);
}

tx_fee_t LeaseCancelTransaction::fee() const
{
    return _tx->data.lease_cancel.fee;
}

tx_timestamp_t LeaseCancelTransaction::timestamp() const
{
    return _tx->data.lease_cancel.timestamp;
}

}



