#ifndef __WAVESPP_TX_ALIAS_HPP_10053__
#define __WAVESPP_TX_ALIAS_HPP_10053__

#include "base.hpp"

namespace wavespp {

class AliasTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setAlias(const std::string& v);
        TransactionPtr build();
    private:
        std::string _alias;
    };

    AliasTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_ALIAS_HPP_10053__ */
