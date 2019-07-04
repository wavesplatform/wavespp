#include <functional>

#include "base.hpp"
#include "burn.hpp"
#include "transfer.hpp"

#include <waves/b58.h>
#include <coda/error.hpp>

namespace waves {

BuilderFlagsChecker::BuilderFlagsChecker(std::initializer_list<BuilderFlags> flags_)
{
    _flags = 0;
    for (auto flag : flags_)
    {
        _flags |= (uint64_t(1) << static_cast<int>(flag));
    }
}

void BuilderFlagsChecker::set(BuilderFlags flag)
{
    _flags &= ~(uint64_t(1) << static_cast<int>(flag));
}

bool BuilderFlagsChecker::get(BuilderFlags flag)
{
    return (_flags >> static_cast<int>(flag)) & 1;
}

bool BuilderFlagsChecker::check()
{
    return _flags == 0;
}

void BuilderFlagsChecker::check_and_throw()
{
    if (!check())
    {
        throw coda_error("Can't build transaction");
    }
}

Transaction::Builder::Builder(std::initializer_list<BuilderFlags> flags_) :
    _flags(flags_)
{}

Transaction::Transaction(waves_tx_t* tx) :
    _tx(tx)
{}

Transaction::~Transaction()
{
    waves_tx_destroy(_tx);
}

const std::string& Transaction::id() const
{
    if (_id.empty())
    {
        const auto& bs = bytes();
        _id.resize(32);
        waves_tx_get_id((uint8_t*)_id.c_str(), bs.data(), bs.size());
    }
    return _id;
}

const std::vector<uint8_t>& Transaction::bytes() const
{
    if (_bytes.empty())
    {
        size_t buf_size = waves_tx_buffer_size(_tx);
        _bytes.resize(buf_size);
        waves_tx_to_bytes(_bytes.data(), _tx);
    }
    return _bytes;
}

}
