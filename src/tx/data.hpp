#ifndef __WAVESPP_TX_DATA_HPP_17588__
#define __WAVESPP_TX_DATA_HPP_17588__

#include "base.hpp"
#include <vector>

namespace wavespp {

struct DataValue
{
    DataValue(bool v);
    DataValue(int64_t v);
    DataValue(const std::string& v, bool binary);

    uint8_t type;
    bool boolean = false;
    int64_t integer = 0;
    std::string str;
};

class DataTransaction : public Transaction
{
public:
    class Builder : public Transaction::Builder
    {
    public:
        Builder();
        Builder& addBoolEntry(const std::string& key, bool value);
        Builder& addLongEntry(const std::string& key, int64_t value);
        Builder& addStringEntry(const std::string& key, const std::string& value);
        Builder& addBinaryEntry(const std::string& key, const std::string& value);
        TransactionPtr build();
    private:
        std::vector<std::pair<std::string, DataValue>> _data;
    };

    DataTransaction(waves_tx_t *tx);

    tx_fee_t fee() const;
    tx_timestamp_t timestamp() const;
};

}

#endif /* __WAVESPP_TX_DATA_HPP_17588__ */
