#ifndef __WAVESPP_TX_INVOKE_SCRIPT_HPP_19595__
#define __WAVESPP_TX_INVOKE_SCRIPT_HPP_19595__

#include <string>
#include <vector>

#include "base.hpp"

namespace wavespp {

struct Payment
{
    Payment(const std::string& asset_id_, tx_amount_t amount_);
    std::string asset_id;
    tx_amount_t amount;
};

class FunctionCall//{{{
{
public:
    FunctionCall() = delete;
    explicit FunctionCall(const std::string& func_name, const std::string& args_json);
    virtual ~FunctionCall() = default;

    std::string func_name() const noexcept { return func_name_; }
    std::string args_json() const noexcept { return args_json_; }
private:
    std::string func_name_;
    std::string args_json_;
};//}}}

class InvokeScriptTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& addPayment(const std::string& asset_id, tx_amount_t amount);
        Builder& setFunctionCall(const std::string& v);
        Builder& setFeeAssetId(const std::string& v);
        Builder& setDappPublicKeyHash(const std::string& v);
        Builder& setDappAlias(const std::string& v);
        TransactionPtr build();
    private:
        std::vector<Payment> _payments;
        std::string _function_call;
        std::string _fee_asset_id;
        bool _is_dapp_alias;
        std::string _dapp_public_key_hash;
        std::string _dapp_alias;
    };

    InvokeScriptTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
    FunctionCall function_call() const;
};

}

#endif /* __WAVESPP_TX_INVOKE_SCRIPT_HPP_19595__ */
