#ifndef __WAVESPP_TX_REISSUE_HPP_16079__
#define __WAVESPP_TX_REISSUE_HPP_16079__

#include "base.hpp"

namespace wavespp {

class ReissueTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setAssetId(const std::string& v);
        Builder& setQuantity(tx_quantity_t v);
        Builder& setReissuable(bool v);
        TransactionPtr build();
    private:
        std::string _asset_id;
        tx_quantity_t _quantity;
        tx_reissuable_t _reissuable;
    };

    ReissueTransaction(waves_tx_t* tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_REISSUE_HPP_16079__ */
