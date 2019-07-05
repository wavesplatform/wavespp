#include "data.hpp"

namespace waves {

DataValue::DataValue(bool v)
{
    type = TX_DATA_TYPE_BOOLEAN;
    boolean = v;
}

DataValue::DataValue(uint64_t v)
{
    type = TX_DATA_TYPE_INTEGER;
    integer = v;
}

DataValue::DataValue(const std::string& v, bool binary)
{
    type = binary ? TX_DATA_TYPE_BINARY : TX_DATA_TYPE_STRING;
    str = v;
}

DataTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP
    })
{}

DataTransaction::DataTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

DataTransaction::Builder&
DataTransaction::Builder::addBoolEntry(const std::string& key, bool value)
{
    _data.emplace_back(std::make_pair(key, DataValue(value)));
    return *this;
}

DataTransaction::Builder&
DataTransaction::Builder::addLongEntry(const std::string& key, uint64_t value)
{
    _data.emplace_back(std::make_pair(key, DataValue(value)));
    return *this;
}

DataTransaction::Builder&
DataTransaction::Builder::addStringEntry(const std::string& key, const std::string& value)
{
    _data.emplace_back(std::make_pair(key, DataValue(value, false)));
    return *this;
}

DataTransaction::Builder&
DataTransaction::Builder::addBinaryEntry(const std::string& key, const std::string& value)
{
    _data.emplace_back(std::make_pair(key, DataValue(value, true)));
    return *this;
}

TransactionPtr DataTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_DATA);
    tx->version = TX_VERSION_1;
    waves_tx_set_public_key_bytes(&tx->data.data.sender_public_key, _sender_public_key.c_str());
    for (const auto& p : _data)
    {
        const auto& key = p.first;
        const auto& data_value = p.second;
        switch(data_value.type)
        {
            case TX_DATA_TYPE_BOOLEAN:
                waves_tx_data_add_entry_boolean(tx, key.c_str(), data_value.boolean);
                break;
            case TX_DATA_TYPE_INTEGER:
                waves_tx_data_add_entry_integer(tx, key.c_str(), data_value.integer);
                break;
            case TX_DATA_TYPE_STRING:
                waves_tx_data_add_entry_string(tx, key.c_str(), data_value.str.c_str());
                break;
            case TX_DATA_TYPE_BINARY:
                waves_tx_data_add_entry_binary(tx, key.c_str(), data_value.str.c_str(), data_value.str.size());
                break;
            default:;
        }
    }
    tx->data.data.fee = _fee;
    tx->data.data.timestamp = _timestamp;
    return std::make_shared<DataTransaction>(tx);
}

tx_fee_t DataTransaction::fee() const
{
    return _tx->data.data.fee;
}

tx_timestamp_t DataTransaction::timestamp() const
{
    return _tx->data.data.timestamp;
}


}
