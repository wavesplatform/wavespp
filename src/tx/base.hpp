#ifndef __WAVESPP_TX_BASE_HPP_8551__
#define __WAVESPP_TX_BASE_HPP_8551__

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "waves/tx.h"

namespace wavespp {

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
    HAS_DESCRIPTION = 14,
    HAS_VERSION = 15,
    HAS_LEASE_ID = 16,
    HAS_MIN_SPONSORED_ASSET_FEE = 17,
    HAS_ALIAS = 18,
    HAS_DAPP = 19,
    HAS_FUNCTION_CALL = 20,
    HAS_ORDER_TYPE = 21,
    HAS_MATCHER_PUBLIC_KEY = 22,
    HAS_PRICE = 23,
    HAS_EXPIRATION = 24,
    HAS_MATCHER_FEE = 25,
    HAS_PROOFS = 26,
    HAS_ORDER1 = 27,
    HAS_ORDER2 = 28,
    HAS_BUY_MATCHER_FEE = 29,
    HAS_SELL_MATCHER_FEE = 30
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
        virtual ~Builder() {}
        virtual Builder& setVersion(tx_version_t version);
        virtual Builder& setSenderPublicKey(const std::string& v);
        virtual Builder& setChainId(uint8_t chain_id);
        virtual Builder& setFee(tx_fee_t fee);
        virtual Builder& setTimestamp(tx_timestamp_t timestamp);
        virtual std::shared_ptr<Transaction> build() = 0;
    protected:
        BuilderFlagsChecker _flags;
        tx_version_t _version;
        std::string _sender_public_key;
        tx_chain_id_t _chain_id;
        tx_fee_t _fee;
        tx_timestamp_t _timestamp;
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
