#include "mass_transfer.hpp"
#include "utils.hpp"

namespace wavespp {

Transfer::Transfer(const std::string& alias_, tx_amount_t amount_, bool is_alias) :
    _is_alias(is_alias),
    _data(alias_),
    _amount(amount_)
{}

bool Transfer::isAlias() const
{
    return _is_alias;
}

const std::string& Transfer::address() const
{
    return _data;
}

tx_amount_t Transfer::amount() const
{
    return _amount;
}

MassTransferTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
           BuilderFlags::HAS_ATTACHMENT,
    })
{
}

MassTransferTransaction::Builder&
MassTransferTransaction::Builder::addTransferByAddress(const std::string& address, tx_amount_t amount)
{
    _transfers.emplace_back(Transfer(address, amount, false));
    return *this;
}

MassTransferTransaction::Builder&
MassTransferTransaction::Builder::addTransferByAlias(const std::string& alias, tx_amount_t amount)
{
    _transfers.emplace_back(Transfer(alias, amount, true));
    return *this;
}

MassTransferTransaction::Builder&
MassTransferTransaction::Builder::setAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ASSET_ID);
    _asset_id = v;
    return *this;
}

MassTransferTransaction::Builder&
MassTransferTransaction::Builder::setAttachment(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ATTACHMENT);
    _attachment = v;
    return *this;
}

TransactionPtr MassTransferTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_MASS_TRANSFER);
    tx->version = TX_VERSION_1;
    waves_tx_set_public_key_bytes(&tx->data.mass_transfer.sender_public_key, _sender_public_key.c_str());
    if (!_asset_id.empty())
    {
        waves_tx_set_asset_id_bytes(&tx->data.mass_transfer.asset_id, _asset_id.c_str());
    }
    tx_array_t* transfers_array = &tx->data.mass_transfer.transfers;
    for (const auto& transfer : _transfers)
    {
        tx_transfer_t* e = (tx_transfer_t*)tx_array_new_elem(transfers_array);
        e->amount = transfer.amount();
        e->recipient.is_alias = transfer.isAlias();
        if (transfer.isAlias())
        {
            e->recipient.data.alias.chain_id = _chain_id;
            waves_tx_set_string(&e->recipient.data.alias.alias, transfer.address().c_str());
        }
        else
        {
            waves_tx_set_address_bytes(&e->recipient.data.address, transfer.address().c_str());
        }
    }
    tx->data.mass_transfer.fee = _fee;
    tx->data.mass_transfer.timestamp = _timestamp;
    tx_set_encoded_string_bytes(&tx->data.mass_transfer.attachment, _attachment.c_str(), _attachment.size());
    return std::make_shared<MassTransferTransaction>(tx);
}

MassTransferTransaction::MassTransferTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t MassTransferTransaction::fee() const
{
    return _tx->data.mass_transfer.fee;
}

tx_timestamp_t MassTransferTransaction::timestamp() const
{
    return _tx->data.mass_transfer.timestamp;
}

}
