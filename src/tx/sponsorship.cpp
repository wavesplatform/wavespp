#include "sponsorship.hpp"

namespace waves {

SponsorshipTransaction::Builder::Builder() :
    Transaction::Builder({
           BuilderFlags::HAS_PUBLIC_KEY,
           BuilderFlags::HAS_MIN_SPONSORED_ASSET_FEE,
           BuilderFlags::HAS_FEE,
           BuilderFlags::HAS_TIMESTAMP,
    })
{
}

SponsorshipTransaction::Builder& SponsorshipTransaction::Builder::setAssetId(const std::string& v)
{
    _flags.set(BuilderFlags::HAS_ASSET_ID);
    _asset_id = v;
    return *this;
}

SponsorshipTransaction::Builder& SponsorshipTransaction::Builder::setMinSponsoredAssetFee(tx_fee_t v)
{
    _flags.set(BuilderFlags::HAS_MIN_SPONSORED_ASSET_FEE);
    _min_sponsored_asset_fee = v;
    return *this;
}

TransactionPtr SponsorshipTransaction::Builder::build()
{
    _flags.check_and_throw();
    auto tx = waves_tx_new(TRANSACTION_TYPE_SPONSORSHIP);
    tx->version = TX_VERSION_1;
    waves_tx_set_public_key_bytes(&tx->data.sponsorship.sender_public_key, _sender_public_key.c_str());
    if (!_asset_id.empty())
    {
        waves_tx_set_asset_id_bytes(&tx->data.sponsorship.asset_id, _asset_id.c_str());
    }
    tx->data.sponsorship.min_sponsored_asset_fee = _min_sponsored_asset_fee;
    tx->data.sponsorship.fee = _fee;
    tx->data.sponsorship.timestamp = _timestamp;
    return std::make_shared<SponsorshipTransaction>(tx);
}

SponsorshipTransaction::SponsorshipTransaction(waves_tx_t* tx) :
    Transaction(tx)
{}

tx_fee_t SponsorshipTransaction::fee() const
{
    return _tx->data.sponsorship.fee;
}

tx_timestamp_t SponsorshipTransaction::timestamp() const
{
    return _tx->data.sponsorship.timestamp;
}

}
