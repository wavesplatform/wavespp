#ifndef __WAVESPP_TX_BURN_HPP_30870__
#define __WAVESPP_TX_BURN_HPP_30870__

#include "base.hpp"

namespace waves {

class BurnTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setAssetId(const std::string& v);
        Builder& setQuantity(tx_quantity_t quantity);
        TransactionPtr build();
    private:
        tx_version_t _version;
        std::string _sender_public_key;
        std::string _asset_id;
        tx_chain_id_t _chain_id;
        tx_quantity_t _quantity;
        tx_fee_t _fee;
        tx_timestamp_t _timestamp;
    };

    BurnTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_BURN_HPP_30870__ */
