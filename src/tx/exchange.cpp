#include "exchange.hpp"

namespace wavespp {

ExchangeTransaction::Builder::Builder() :
    Transaction::Builder({
        BuilderFlags::HAS_VERSION,
        BuilderFlags::HAS_ORDER1,
        BuilderFlags::HAS_ORDER2,
        BuilderFlags::HAS_PRICE,
        BuilderFlags::HAS_AMOUNT,
        BuilderFlags::HAS_BUY_MATCHER_FEE,
        BuilderFlags::HAS_SELL_MATCHER_FEE,
        BuilderFlags::HAS_FEE,
        BuilderFlags::HAS_TIMESTAMP
    })
{}

ExchangeTransaction::Builder& ExchangeTransaction::Builder::setAmount(tx_amount_t v)
{
    _flags.set(BuilderFlags::HAS_AMOUNT);
    _amount = v;
    return *this;
}

ExchangeTransaction::Builder& ExchangeTransaction::Builder::setPrice(tx_amount_t v)
{
    _flags.set(BuilderFlags::HAS_PRICE);
    _price = v;
    return *this;
}

ExchangeTransaction::Builder& ExchangeTransaction::Builder::setOrder1(const OrderBuilder& order)
{
    _flags.set(BuilderFlags::HAS_ORDER1);
    _order1 = order;
    return *this;
}

ExchangeTransaction::Builder& ExchangeTransaction::Builder::setOrder2(const OrderBuilder& order)
{
    _flags.set(BuilderFlags::HAS_ORDER2);
    _order2 = order;
    return *this;
}

ExchangeTransaction::Builder& ExchangeTransaction::Builder::setBuyMatcherFee(tx_amount_t v)
{
    _flags.set(BuilderFlags::HAS_BUY_MATCHER_FEE);
    _buy_matcher_fee = v;
    return *this;
}

ExchangeTransaction::Builder& ExchangeTransaction::Builder::setSellMatcherFee(tx_amount_t v)
{
    _flags.set(BuilderFlags::HAS_SELL_MATCHER_FEE);
    _sell_matcher_fee = v;
    return *this;
}

TransactionPtr ExchangeTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_EXCHANGE);
    tx->version = _version;
    _order1.build(&tx->data.exchange.order1);
    _order2.build(&tx->data.exchange.order2);
    tx->data.exchange.price = _price;
    tx->data.exchange.amount = _amount;
    tx->data.exchange.buy_matcher_fee = _buy_matcher_fee;
    tx->data.exchange.sell_matcher_fee = _sell_matcher_fee;
    tx->data.exchange.fee = _fee;
    tx->data.exchange.timestamp = _timestamp;
    return std::make_shared<ExchangeTransaction>(tx);
}

ExchangeTransaction::ExchangeTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t ExchangeTransaction::fee() const
{
    return _tx->data.exchange.fee;
}

tx_timestamp_t ExchangeTransaction::timestamp() const
{
    return _tx->data.exchange.timestamp;
}

}
