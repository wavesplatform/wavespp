#ifndef __WAVESPP_TX_MASS_TRANSFER_HPP_13439__
#define __WAVESPP_TX_MASS_TRANSFER_HPP_13439__

#include "base.hpp"

namespace waves {

class Transfer
{
public:
    Transfer(const std::string& alias_, tx_amount_t amount_, bool is_alias);
    bool isAlias() const;
    const std::string& address() const;
    tx_amount_t amount() const;
private:
    bool _is_alias;
    std::string _data;
    tx_amount_t _amount;
};

class MassTransferTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& addTransferByAddress(const std::string& address, tx_amount_t amount);
        Builder& addTransferByAlias(const std::string& alias, tx_amount_t amount);
        Builder& setAssetId(const std::string& v);
        Builder& setAttachment(const std::string& v);
        TransactionPtr build();
    private:
        std::vector<Transfer> _transfers;
        std::string _asset_id;
        std::string _attachment;
    };

    MassTransferTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_MASS_TRANSFER_HPP_13439__ */
