#ifndef __WAVESPP_TX_EXCHANGE_HPP_5214__
#define __WAVESPP_TX_EXCHANGE_HPP_5214__

#include "base.hpp"
#include "order.hpp"

namespace waves {

class ExchangeTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& setAmount(tx_amount_t v);
        Builder& setPrice(tx_amount_t v);
        Builder& setOrder1(const OrderBuilder& order);
        Builder& setOrder2(const OrderBuilder& order);
        Builder& setBuyMatcherFee(tx_amount_t v);
        Builder& setSellMatcherFee(tx_amount_t v);
        TransactionPtr build();
    private:
        tx_amount_t _amount;
        tx_amount_t _price;
        OrderBuilder _order1;
        OrderBuilder _order2;
        tx_fee_t _buy_matcher_fee;
        tx_fee_t _sell_matcher_fee;
    };

    ExchangeTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_EXCHANGE_HPP_5214__ */
