#include "lease.hpp"
#include "utils.hpp"

namespace waves {

LeaseTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_VERSION,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_RECIPIENT,
           BuilderFlags::HAS_AMOUNT,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
    })
{
}

LeaseTransaction::Builder&
LeaseTransaction::Builder::setLeaseAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ASSET_ID);
    _lease_asset_id = v;
    return *this;
}

LeaseTransaction::Builder&
LeaseTransaction::Builder::setRecipientPublicKeyHash(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_RECIPIENT);
    _is_alias = false;
    _recipient_public_key_hash = v;
    return *this;
}

LeaseTransaction::Builder&
LeaseTransaction::Builder::setAlias(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_RECIPIENT);
    _is_alias = true;
    _alias = v;
    return *this;
}

LeaseTransaction::Builder&
LeaseTransaction::Builder::setAmount(tx_amount_t amount)
{
    _flags.set(BuilderFlags::HAS_AMOUNT);
    _amount = amount;
    return *this;
}

TransactionPtr LeaseTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_LEASE);
    tx->version = _version;
    waves_tx_set_public_key_bytes(&tx->data.lease.sender_public_key, _sender_public_key.c_str());
    if (!_lease_asset_id.empty())
    {
        waves_tx_set_asset_id_bytes(&tx->data.lease.lease_asset_id, _lease_asset_id.c_str());
    }
    tx->data.lease.amount = _amount;
    tx->data.lease.recipient.is_alias = _is_alias;
    if (_is_alias)
    {
        tx->data.lease.recipient.data.alias.chain_id = _chain_id;
        waves_tx_set_string(&tx->data.lease.recipient.data.alias.alias, _alias.c_str());
    }
    else
    {
        auto address = waves::utils::secure_hash_to_address(_recipient_public_key_hash, _version, _chain_id);
        waves_tx_set_address_bytes(&tx->data.lease.recipient.data.address, address.c_str());
    }
    tx->data.lease.fee = _fee;
    tx->data.lease.timestamp = _timestamp;
    return std::make_shared<LeaseTransaction>(tx);
}

LeaseTransaction::LeaseTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t LeaseTransaction::fee() const
{
    return _tx->data.lease.fee;
}

tx_timestamp_t LeaseTransaction::timestamp() const
{
    return _tx->data.lease.timestamp;
}

}
