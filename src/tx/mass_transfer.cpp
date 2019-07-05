#include "mass_transfer.hpp"
#include "utils.hpp"

namespace waves {

Transfer::Transfer(tx_chain_id_t chain_id_, const std::string& alias_, tx_amount_t amount_) :
    _is_alias(true),
    _chain_id(chain_id_),
    _data(alias_),
    _amount(amount_)
{}

Transfer::Transfer(const std::string& address_, tx_amount_t amount_) :
    _is_alias(false),
    _data(address_),
    _amount(amount_)
{}

bool Transfer::isAlias() const
{
    return _is_alias;
}

tx_chain_id_t Transfer::chainId() const
{
    return _chain_id;
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
    _transfers.emplace_back(Transfer(address, amount));
    return *this;
}

MassTransferTransaction::Builder&
MassTransferTransaction::Builder::addTransferByAlias(tx_chain_id_t chain_id, const std::string& alias, tx_amount_t amount)
{
    _transfers.emplace_back(Transfer(chain_id, alias, amount));
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
            e->recipient.data.alias.chain_id = transfer.chainId();
            waves_tx_set_string(&e->recipient.data.alias.alias, transfer.address().c_str());
        }
        else
        {
            auto address = waves::utils::secure_hash_to_address(transfer.address().c_str(), TX_VERSION_1, transfer.chainId());
            waves_tx_set_address_bytes(&e->recipient.data.address, address.c_str());
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
