#include "base.hpp"
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

Transaction::Transaction(const waves_tx_t& tx) :
    _tx(tx)
{}

Transaction::~Transaction()
{
    waves_tx_destroy(&_tx);
}

const std::string& Transaction::id() const
{
    if (_id.empty())
    {
        const auto& bs = bytes();
        uint8_t hash[32];
        waves_tx_hash_bytes(hash, bs.data(), bs.size());
        char id_buf [sizeof(hash)*2];
        ssize_t id_sz = base58_encode(id_buf, hash, sizeof(hash));
        _id = std::string(id_buf, id_sz);
    }
    return _id;
}

const std::vector<uint8_t>& Transaction::bytes() const
{
    if (_bytes.empty())
    {
        size_t buf_size = waves_tx_buffer_size(&_tx);
        _bytes.resize(buf_size);
        waves_tx_to_bytes(_bytes.data(), &_tx);
    }
    return _bytes;
}

}
