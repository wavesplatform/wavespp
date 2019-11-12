#ifndef __WAVESPP_TX_SPONSORSHIP_HPP_11353__
#define __WAVESPP_TX_SPONSORSHIP_HPP_11353__

#include "base.hpp"

namespace wavespp {

class SponsorshipTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setAssetId(const std::string& v);
        Builder& setMinSponsoredAssetFee(tx_fee_t v);
        TransactionPtr build();
    private:
        std::string _asset_id;
        tx_fee_t _min_sponsored_asset_fee;
    };

    SponsorshipTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_SPONSORSHIP_HPP_11353__ */
