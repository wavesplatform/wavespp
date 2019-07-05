#ifndef __WAVESPP_TX_TRANSFER_HPP_10190__
#define __WAVESPP_TX_TRANSFER_HPP_10190__

#include "base.hpp"

namespace waves {

class TransferTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();        
        Builder& setAssetId(const std::string& v);
        Builder& setFeeAssetId(const std::string& v);
        Builder& setAmount(tx_amount_t amount);
        Builder& setRecipientPublicKeyHash(const std::string& v);
        Builder& setAlias(const std::string& v);
        Builder& setAttachment(const std::string& v);
        TransactionPtr build();
    private:
        tx_version_t _version;

        tx_chain_id_t _chain_id;
        std::string _asset_id;
        std::string _fee_asset_id;
        tx_amount_t _amount;
        bool _is_alias;
        std::string _recipient_public_key_hash;
        std::string _alias;
        tx_fee_t _fee;
        tx_timestamp_t _timestamp;
        std::string _attachment;
    };

    TransferTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_TRANSFER_HPP_10190__ */
