#ifndef __WAVESPP_TX_SET_ASSET_SCRIPT_HPP_5822__
#define __WAVESPP_TX_SET_ASSET_SCRIPT_HPP_5822__

#include "base.hpp"

namespace wavespp {

class SetAssetScriptTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setAssetId(const std::string& v);
        Builder& setScript(const std::string& v);
        TransactionPtr build();
    private:
        std::string _asset_id;
        std::string _script;
    };

    SetAssetScriptTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_SET_ASSET_SCRIPT_HPP_5822__ */
