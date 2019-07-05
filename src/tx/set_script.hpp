#ifndef __WAVESPP_TX_SET_SCRIPT_HPP_5822__
#define __WAVESPP_TX_SET_SCRIPT_HPP_5822__

#include "base.hpp"

namespace waves {

class SetScriptTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setScript(const std::string& v);
        TransactionPtr build();
    private:
        std::string _script;
    };

    SetScriptTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_SET_SCRIPT_HPP_5822__ */
