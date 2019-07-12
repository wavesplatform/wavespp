#include "transfer.hpp"
#include "utils.hpp"

namespace waves {

TransferTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_VERSION,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_AMOUNT,
           BuilderFlags::HAS_RECIPIENT,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
           BuilderFlags::HAS_ATTACHMENT,
           BuilderFlags::HAS_CHAIN_ID
    })
{
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ASSET_ID);
    _asset_id = v;
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setFeeAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_FEE_ASSET_ID);
    _fee_asset_id = v;
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAmount(tx_amount_t amount)
{
    _flags.set(BuilderFlags::HAS_AMOUNT);
    _amount = amount;
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setRecipientPublicKeyHash(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_RECIPIENT);
    _is_alias = false;
    _recipient_public_key_hash = v;
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAlias(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_RECIPIENT);
    _is_alias = true;
    _alias = v;
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAttachment(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ATTACHMENT);
    _attachment = v;
    return *this;
}

TransactionPtr TransferTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_TRANSFER);
    tx->version = _version;
    waves_tx_set_public_key_bytes(&tx->data.transfer.sender_public_key, _sender_public_key.c_str());
    if (!_asset_id.empty())
    {
        waves_tx_set_asset_id_bytes(&tx->data.transfer.asset_id, _asset_id.c_str());
    }
    if (!_fee_asset_id.empty())
    {
        waves_tx_set_asset_id_bytes(&tx->data.transfer.fee_asset_id, _fee_asset_id.c_str());
    }
    tx->data.transfer.amount = _amount;
    tx->data.transfer.recipient.is_alias = _is_alias;
    if (_is_alias)
    {
        tx->data.transfer.recipient.data.alias.chain_id = _chain_id;
        waves_tx_set_string(&tx->data.transfer.recipient.data.alias.alias, _alias.c_str());
    }
    else
    {
        auto address = waves::utils::secure_hash_to_address(_recipient_public_key_hash, _chain_id);
        waves_tx_set_address_bytes(&tx->data.transfer.recipient.data.address, address.c_str());
    }
    tx->data.transfer.fee = _fee;
    tx->data.transfer.timestamp = _timestamp;
    tx_set_encoded_string_bytes(&tx->data.transfer.attachment, _attachment.c_str(), _attachment.size());
    return std::make_shared<TransferTransaction>(tx);
}

TransferTransaction::TransferTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t TransferTransaction::fee() const
{
    return _tx->data.transfer.fee;
}

tx_timestamp_t TransferTransaction::timestamp() const
{
    return _tx->data.transfer.timestamp;
}

}
