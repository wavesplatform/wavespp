#ifndef __WAVESPP_TX_ISSUE_HPP_3403__
#define __WAVESPP_TX_ISSUE_HPP_3403__

#include "base.hpp"

namespace waves {

class IssueTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setName(const std::string& v);
        Builder& setDescription(const std::string& v);
        Builder& setQuantity(tx_quantity_t quantity);
        Builder& setDecimals(tx_decimals_t decimals);
        Builder& setReissuable(bool reissuable);
        Builder& setScript(const std::string& v);
        TransactionPtr build();
    private:
        std::string _name;
        std::string _description;
        tx_quantity_t _quantity;
        tx_decimals_t _decimals;
        tx_reissuable_t _reissuable;
        std::string _script;
    };

    IssueTransaction(waves_tx_t* tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_ISSUE_HPP_3403__ */
