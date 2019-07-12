#include "invoke_script.hpp"
#include "utils.hpp"

namespace waves {

Payment::Payment(const std::string& asset_id_, tx_amount_t amount_) :
    asset_id(asset_id_),
    amount(amount_)
{}

InvokeScriptTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_CHAIN_ID,
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_DAPP,
           BuilderFlags::HAS_FUNCTION_CALL,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP
    })
{}

InvokeScriptTransaction::Builder&
InvokeScriptTransaction::Builder::addPayment(const std::string& asset_id, tx_amount_t amount)
{
    _payments.emplace_back(Payment(asset_id, amount));
    return *this;
}

InvokeScriptTransaction::Builder&
InvokeScriptTransaction::Builder::setFunctionCall(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_FUNCTION_CALL);
    _function_call = v;
    return *this;
}

InvokeScriptTransaction::Builder&
InvokeScriptTransaction::Builder::setFeeAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_FEE_ASSET_ID);
    _fee_asset_id = v;
    return *this;
}

InvokeScriptTransaction::Builder&
InvokeScriptTransaction::Builder::setDappPublicKeyHash(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_DAPP);
    _is_dapp_alias = false;
    _dapp_public_key_hash = v;
    return *this;
}

InvokeScriptTransaction::Builder&
InvokeScriptTransaction::Builder::setDappAlias(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_DAPP);
    _is_dapp_alias = true;
    _dapp_alias = v;
    return *this;
}

TransactionPtr InvokeScriptTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_INVOKE_SCRIPT);
    tx->version = TX_VERSION_1;
    tx->data.invoke_script.chain_id = _chain_id;
    waves_tx_set_public_key_bytes(&tx->data.invoke_script.sender_public_key, _sender_public_key.c_str());
    tx_array_t* payments_array = &tx->data.invoke_script.payments;
    for (const auto& payment : _payments)
    {
        tx_payment_t* e = (tx_payment_t*)tx_array_new_elem(payments_array);
        e->amount = payment.amount;
        if (!payment.asset_id.empty())
        {
            waves_tx_set_asset_id_bytes(&e->asset_id, payment.asset_id.c_str());
        }
    }
    tx_load_func_call(&tx->data.invoke_script.call, (const unsigned char*)_function_call.c_str());
    tx->data.invoke_script.d_app.is_alias = _is_dapp_alias;
    if (_is_dapp_alias)
    {
        tx->data.invoke_script.d_app.data.alias.chain_id = _chain_id;
        waves_tx_set_string(&tx->data.invoke_script.d_app.data.alias.alias, _dapp_alias.c_str());
    }
    else
    {
        auto address = waves::utils::secure_hash_to_address(_dapp_public_key_hash, _chain_id);
        waves_tx_set_address_bytes(&tx->data.invoke_script.d_app.data.address, address.c_str());
    }
    if (!_fee_asset_id.empty())
    {
        waves_tx_set_asset_id_bytes(&tx->data.invoke_script.fee_asset_id, _fee_asset_id.c_str());
    }
    tx->data.invoke_script.fee = _fee;
    tx->data.invoke_script.timestamp = _timestamp;
    return std::make_shared<InvokeScriptTransaction>(tx);
}

InvokeScriptTransaction::InvokeScriptTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t InvokeScriptTransaction::fee() const
{
    return _tx->data.invoke_script.fee;
}

tx_timestamp_t InvokeScriptTransaction::timestamp() const
{
    return _tx->data.invoke_script.timestamp;
}

}
