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
        Builder& setSenderPublicKey(const std::string& v);
        Builder& setAssetId(const std::string& v);
        Builder& setChainId(tx_chain_id_t chain_id);
        Builder& setQuantity(tx_quantity_t quantity);
        Builder& setFee(tx_fee_t fee);
        Builder& setTimestamp(tx_timestamp_t timestamp);
        TransactionPtr build();
    private:
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
