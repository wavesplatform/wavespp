#include "issue.hpp"

namespace waves {

IssueTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_VERSION,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_NAME,
           BuilderFlags::HAS_DESCRIPTION,
           BuilderFlags::HAS_DECIMALS,
           BuilderFlags::HAS_REISSUABLE,
           BuilderFlags::HAS_QUANTITY,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
           BuilderFlags::HAS_SCRIPT
    })
{}

IssueTransaction::Builder&
IssueTransaction::Builder::setName(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_NAME);
    _name = v;
    return *this;
}

IssueTransaction::Builder&
IssueTransaction::Builder::setDescription(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_DESCRIPTION);
    _description = v;
    return *this;
}

IssueTransaction::Builder&
IssueTransaction::Builder::setQuantity(tx_quantity_t quantity)
{
    _flags.set(BuilderFlags::HAS_QUANTITY);
    _quantity = quantity;
    return *this;
}

IssueTransaction::Builder&
IssueTransaction::Builder::setDecimals(tx_decimals_t decimals)
{
    _flags.set(BuilderFlags::HAS_DECIMALS);
    _decimals = decimals;
    return *this;
}

IssueTransaction::Builder&
IssueTransaction::Builder::setReissuable(bool reissuable)
{
    _flags.set(BuilderFlags::HAS_REISSUABLE);
    _reissuable = reissuable;
    return *this;
}

IssueTransaction::Builder&
IssueTransaction::Builder::setScript(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_SCRIPT);
    _script = v;
    return *this;
}

TransactionPtr IssueTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_ISSUE);
    tx->version = _version;
    tx->data.issue.chain_id = _chain_id;
    waves_tx_set_public_key_bytes(&tx->data.issue.sender_public_key, _sender_public_key.c_str());
    waves_tx_set_string(&tx->data.issue.name, _name.c_str());
    waves_tx_set_string(&tx->data.issue.description, _description.c_str());
    tx->data.issue.quantity = _quantity;
    tx->data.issue.decimals = _decimals;
    tx->data.issue.reissuable = _reissuable;
    tx->data.issue.fee = _fee;
    tx->data.issue.timestamp = _timestamp;
    tx_set_encoded_string_bytes(&tx->data.issue.script, _script.c_str(), _script.size());
    return std::make_shared<IssueTransaction>(tx);
}

IssueTransaction::IssueTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t IssueTransaction::fee() const
{
    return _tx->data.issue.fee;
}

tx_timestamp_t IssueTransaction::timestamp() const
{
    return _tx->data.issue.timestamp;
}

}
