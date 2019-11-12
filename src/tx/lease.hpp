#ifndef __WAVESPP_TX_LEASE_HPP_17173__
#define __WAVESPP_TX_LEASE_HPP_17173__

#include "base.hpp"

namespace wavespp {

class LeaseTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setLeaseAssetId(const std::string& v);
        Builder& setRecipientPublicKeyHash(const std::string& v);
        Builder& setAlias(const std::string& v);
        Builder& setAmount(tx_amount_t amount);
        TransactionPtr build();
    private:
        std::string _lease_asset_id;
        bool _is_alias;
        std::string _recipient_public_key_hash;
        std::string _alias;
        tx_amount_t _amount;
    };

    LeaseTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_LEASE_HPP_17173__ */
