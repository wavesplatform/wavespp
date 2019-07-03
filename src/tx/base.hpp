#ifndef __WAVESPP_TX_BASE_HPP_8551__
#define __WAVESPP_TX_BASE_HPP_8551__

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "waves/tx.h"

namespace waves {

enum class BuilderFlags
{
    HAS_PUBLIC_KEY = 0,
    HAS_ASSET_ID = 1,
    HAS_FEE_ASSET_ID = 2,
    HAS_AMOUNT = 3,
    HAS_RECIPIENT = 4,
    HAS_FEE = 5,
    HAS_TIMESTAMP = 6,
    HAS_ATTACHMENT = 7,
    HAS_CHAIN_ID = 8,
    HAS_QUANTITY = 9,
    HAS_DECIMALS = 10,
    HAS_REISSUABLE = 11,
    HAS_SCRIPT = 12,
    HAS_NAME = 13,
    HAS_DESCRIPTION = 14
};

class BuilderFlagsChecker
{
public:
    BuilderFlagsChecker(std::initializer_list<BuilderFlags> flags_);
    void set(BuilderFlags flag);
    bool get(BuilderFlags flag);
    bool check();
    void check_and_throw();
private:
    uint64_t _flags;
};

class Transaction
{
public:
    class Builder
    {
    public:
        Builder(std::initializer_list<BuilderFlags> flags_);
        ~Builder();
        virtual std::shared_ptr<Transaction> build() = 0;
    protected:
        BuilderFlagsChecker _flags;
    };
    Transaction(waves_tx_t *tx);
    virtual ~Transaction();
    const std::string& id() const;
    const std::vector<uint8_t>& bytes() const;

    virtual tx_fee_t fee() const = 0;
    virtual tx_timestamp_t timestamp() const = 0;
protected:
    mutable std::string _id;
    mutable std::vector<uint8_t> _bytes;
    waves_tx_t* _tx;
};

typedef std::shared_ptr<Transaction> TransactionPtr;

}

#endif /* __WAVESPP_TX_BASE_HPP_8551__ */
