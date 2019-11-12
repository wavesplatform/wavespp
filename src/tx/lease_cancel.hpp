#ifndef __WAVESPP_TX_LEASE_CANCEL_HPP_8781__
#define __WAVESPP_TX_LEASE_CANCEL_HPP_8781__

#include "base.hpp"

namespace wavespp {

class LeaseCancelTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setLeaseId(const std::string& v);
        TransactionPtr build();
    private:
        std::string _lease_id;
    };

    LeaseCancelTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_LEASE_CANCEL_HPP_8781__ */
