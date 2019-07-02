#ifndef __WAVESPP_TX_TRANSFER_HPP_10190__
#define __WAVESPP_TX_TRANSFER_HPP_10190__

#include "base.hpp"

namespace waves {

class TransferTransaction : public Transaction
{
public:
    class Builder
    {
    public:
        Builder();
        Builder& setPublicKey(const std::string& v);
        Builder& setAssetId(const std::string& v);
        Builder& setFeeAssetId(const std::string& v);
        Builder& setAmount(tx_amount_t amount_);
        Builder& setAddress(const std::string& v);
        Builder& setAlias(const std::string& v, uint8_t chain_id);
        Builder& setFee(tx_fee_t fee_);
        Builder& setTimestamp(tx_timestamp_t timestamp_);
        Builder& setAttachment(const std::string& v);
        TransactionPtr build();
    private:
        waves_tx_t _tx;
        BuilderFlagsChecker _flags;
    };

    TransferTransaction(const waves_tx_t& tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_TRANSFER_HPP_10190__ */
