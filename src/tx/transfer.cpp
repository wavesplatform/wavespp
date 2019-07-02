#include "transfer.hpp"

namespace waves {

TransferTransaction::Builder::Builder() :
    _flags({
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_AMOUNT,
           BuilderFlags::HAS_RECIPIENT,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
           BuilderFlags::HAS_ATTACHMENT
    })
{
    _tx = *waves_tx_new(TRANSACTION_TYPE_TRANSFER);
}

TransferTransaction::Builder&
TransferTransaction::Builder::setPublicKey(const std::string& v)
{
    waves_tx_set_public_key_bytes(&_tx.data.transfer.sender_public_key, v.c_str());
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAssetId(const std::string& v)
{
    waves_tx_set_asset_id_bytes(&_tx.data.transfer.asset_id, v.c_str());
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setFeeAssetId(const std::string& v)
{
    waves_tx_set_asset_id_bytes(&_tx.data.transfer.fee_asset_id, v.c_str());
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAmount(tx_amount_t amount_)
{
    _tx.data.transfer.amount = amount_;
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAddress(const std::string& v)
{
    _tx.data.transfer.recipient.is_alias = false;
    waves_tx_set_address_bytes(&_tx.data.transfer.recipient.data.address, v.c_str());
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAlias(const std::string& v, uint8_t chain_id)
{
    _tx.data.transfer.recipient.is_alias = true;
    _tx.data.transfer.recipient.data.alias.chain_id = chain_id;
    waves_tx_set_string(&_tx.data.transfer.recipient.data.alias.alias, v.c_str());
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setFee(tx_fee_t fee_)
{
    _tx.data.transfer.fee = fee_;
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setTimestamp(tx_timestamp_t timestamp_)
{
    _tx.data.transfer.timestamp = timestamp_;
    return *this;
}

TransferTransaction::Builder&
TransferTransaction::Builder::setAttachment(const std::string& v)
{
    tx_set_encoded_string_bytes(&_tx.data.transfer.attachment, v.c_str(), v.size());
    return *this;
}

TransactionPtr TransferTransaction::Builder::build()
{
    _flags.check_and_throw();
    return std::make_shared<TransferTransaction>(_tx);
}

TransferTransaction::TransferTransaction(const waves_tx_t& tx) :
    Transaction(tx)
{}

tx_fee_t TransferTransaction::fee() const
{
    return _tx.data.transfer.fee;
}

tx_timestamp_t TransferTransaction::timestamp() const
{
    return _tx.data.transfer.timestamp;
}

}
