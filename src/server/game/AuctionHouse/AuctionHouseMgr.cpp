////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "ObjectMgr.h"
#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DBCStores.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"
#include "AuctionHouseMgr.h"
#include "Item.h"
#include "Language.h"
#include "Log.h"
#include "Common.h"

enum eAuctionHouse
{
    AH_MINIMUM_DEPOSIT = 100
};

AuctionHouseMgr::AuctionHouseMgr()
{
}

AuctionHouseMgr::~AuctionHouseMgr()
{
    for (ItemMap::iterator itr = mAitems.begin(); itr != mAitems.end(); ++itr)
        delete itr->second;
}

AuctionHouseObject* AuctionHouseMgr::GetAuctionsMap(uint32 factionTemplateId)
{
    if (sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION))
        return &mNeutralAuctions;

    // team have linked auction houses
    FactionTemplateEntry const* u_entry = sFactionTemplateStore.LookupEntry(factionTemplateId);
    if (!u_entry)
        return &mNeutralAuctions;
    else if (u_entry->Mask & FACTION_MASK_ALLIANCE)
        return &mAllianceAuctions;
    else if (u_entry->Mask & FACTION_MASK_HORDE)
        return &mHordeAuctions;
    else
        return &mNeutralAuctions;
}

uint32 AuctionHouseMgr::GetAuctionDeposit(uint32 p_DepositTime, Item* p_Item, uint32 p_ItemCount)
{
    uint32 l_SellPrice = p_Item->GetTemplate()->SellPrice;

    if (l_SellPrice <= 0)
        return eAuctionHouse::AH_MINIMUM_DEPOSIT * sWorld->getRate(Rates::RATE_AUCTION_DEPOSIT);

    /// Magic value found in full-debug client, this is working just right
    float l_Multiplier  = 0.0041666669f;
    uint32 l_RunTime    = p_DepositTime / 60;
    uint32 l_Deposit    = p_ItemCount * uint32(l_RunTime * l_Multiplier * floor(l_SellPrice * 5.0f / 100.0f)) * sWorld->getRate(Rates::RATE_AUCTION_DEPOSIT);

    sLog->outDebug(LogFilterType::LOG_FILTER_AUCTIONHOUSE, "SellPrice:  %u", l_SellPrice);
    sLog->outDebug(LogFilterType::LOG_FILTER_AUCTIONHOUSE, "Items:      %u", p_ItemCount);
    sLog->outDebug(LogFilterType::LOG_FILTER_AUCTIONHOUSE, "Deposit:    %u", l_Deposit);

    if (l_Deposit < eAuctionHouse::AH_MINIMUM_DEPOSIT * sWorld->getRate(Rates::RATE_AUCTION_DEPOSIT))
        return eAuctionHouse::AH_MINIMUM_DEPOSIT * sWorld->getRate(Rates::RATE_AUCTION_DEPOSIT);
    else
        return l_Deposit;
}

//does not clear ram
void AuctionHouseMgr::SendAuctionWonMail(AuctionEntry* auction, SQLTransaction& trans)
{
    Item* pItem = GetAItem(auction->itemGUIDLow);
    if (!pItem)
        return;

    uint32 bidder_accId = 0;
    uint64 bidder_guid = MAKE_NEW_GUID(auction->bidder, 0, HIGHGUID_PLAYER);
    Player* bidder = ObjectAccessor::FindPlayer(bidder_guid);
    // data for gm.log
    if (sWorld->getBoolConfig(CONFIG_GM_LOG_TRADE))
    {
        uint32 bidder_security = 0;
        std::string bidder_name;
        if (bidder)
        {
            bidder_accId = bidder->GetSession()->GetAccountId();
            bidder_security = bidder->GetSession()->GetSecurity();
            bidder_name = bidder->GetName();
        }
        else
        {
            bidder_accId = sObjectMgr->GetPlayerAccountIdByGUID(bidder_guid);
            bidder_security = AccountMgr::GetSecurity(bidder_accId, g_RealmID);

            if (!AccountMgr::IsPlayerAccount(bidder_security)) // not do redundant DB requests
            {
                if (!sObjectMgr->GetPlayerNameByGUID(bidder_guid, bidder_name))
                    bidder_name = sObjectMgr->GetTrinityStringForDBCLocale(LANG_UNKNOWN);
            }
        }
        if (!AccountMgr::IsPlayerAccount(bidder_security))
        {
            std::string owner_name;
            if (!sObjectMgr->GetPlayerNameByGUID(auction->owner, owner_name))
                owner_name = sObjectMgr->GetTrinityStringForDBCLocale(LANG_UNKNOWN);

            uint32 owner_accid = sObjectMgr->GetPlayerAccountIdByGUID(auction->owner);

            sLog->outCommand(bidder_accId, "", 0, bidder_name.c_str(), owner_accid, "", 0, owner_name.c_str(),
                "GM %s (Account: %u) won item in auction: %s (Entry: %u Count: %u) and pay money: " UI64FMTD ". Original owner %s (Account: %u)",
                bidder_name.c_str(), bidder_accId, pItem->GetTemplate()->Name1->Get(sWorld->GetDefaultDbcLocale()), pItem->GetEntry(), pItem->GetCount(), auction->bid, owner_name.c_str(), owner_accid);
        }
    }

    // receiver exist
    if (bidder || bidder_accId)
    {
        // set owner to bidder (to prevent delete item with sender char deleting)
        // owner in `data` will set at mail receive and item extracting
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_OWNER);
        stmt->setUInt32(0, auction->bidder);
        stmt->setUInt32(1, pItem->GetGUIDLow());
        trans->Append(stmt);

        if (bidder)
        {
            bidder->GetSession()->SendAuctionBidderNotification(auction, bidder_guid, 0);
            // FIXME: for offline player need also
            bidder->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WON_AUCTIONS, 1);
        }

        MailDraft(auction->BuildAuctionMailSubject(AUCTION_WON), AuctionEntry::BuildAuctionMailBody(auction->owner, auction->bid, auction->buyout, 0, 0))
            .AddItem(pItem)
            .SendMailTo(trans, MailReceiver(bidder, auction->bidder), auction, MAIL_CHECK_MASK_COPIED);
    }
}

void AuctionHouseMgr::SendAuctionSalePendingMail(AuctionEntry* auction, SQLTransaction& trans)
{
    uint64 owner_guid = MAKE_NEW_GUID(auction->owner, 0, HIGHGUID_PLAYER);
    Player* owner = ObjectAccessor::FindPlayer(owner_guid);
    uint32 owner_accId = sObjectMgr->GetPlayerAccountIdByGUID(owner_guid);
    // owner exist (online or offline)
    if (owner || owner_accId)
        MailDraft(auction->BuildAuctionMailSubject(AUCTION_SALE_PENDING), AuctionEntry::BuildAuctionMailBody(auction->bidder, auction->bid, auction->buyout, auction->deposit, auction->GetAuctionCut()))
            .SendMailTo(trans, MailReceiver(owner, auction->owner), auction, MAIL_CHECK_MASK_COPIED);
}

//call this method to send mail to auction owner, when auction is successful, it does not clear ram
void AuctionHouseMgr::SendAuctionSuccessfulMail(AuctionEntry* auction, SQLTransaction& trans)
{
    uint64 owner_guid = MAKE_NEW_GUID(auction->owner, 0, HIGHGUID_PLAYER);
    Player* owner = ObjectAccessor::FindPlayer(owner_guid);
    uint32 owner_accId = sObjectMgr->GetPlayerAccountIdByGUID(owner_guid);
    // owner exist
    if (owner || owner_accId)
    {
        uint64 profit = auction->bid + auction->deposit - auction->GetAuctionCut();

        //FIXME: what do if owner offline
        if (owner)
        {
            owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GOLD_EARNED_BY_AUCTIONS, profit);
            owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_AUCTION_SOLD, auction->bid);
            //send auction owner notification, bidder must be current!
            owner->GetSession()->SendAuctionOwnerNotification(auction);
        }

        MailDraft(auction->BuildAuctionMailSubject(AUCTION_SUCCESSFUL), AuctionEntry::BuildAuctionMailBody(auction->bidder, auction->bid, auction->buyout, auction->deposit, auction->GetAuctionCut()))
            .AddMoney(profit)
            .SendMailTo(trans, MailReceiver(owner, auction->owner), auction, MAIL_CHECK_MASK_COPIED, sWorld->getIntConfig(CONFIG_MAIL_DELIVERY_DELAY));
    }
}

//does not clear ram
void AuctionHouseMgr::SendAuctionExpiredMail(AuctionEntry* auction, SQLTransaction& trans)
{
    //return an item in auction to its owner by mail
    Item* pItem = GetAItem(auction->itemGUIDLow);
    if (!pItem)
        return;

    uint64 owner_guid = MAKE_NEW_GUID(auction->owner, 0, HIGHGUID_PLAYER);
    Player* owner = ObjectAccessor::FindPlayer(owner_guid);
    uint32 owner_accId = sObjectMgr->GetPlayerAccountIdByGUID(owner_guid);
    // owner exist
    if (owner || owner_accId)
    {
        if (owner)
            owner->GetSession()->SendAuctionOwnerNotification(auction);

        MailDraft(auction->BuildAuctionMailSubject(AUCTION_EXPIRED), AuctionEntry::BuildAuctionMailBody(0, 0, auction->buyout, auction->deposit, 0))
            .AddItem(pItem)
            .SendMailTo(trans, MailReceiver(owner, auction->owner), auction, MAIL_CHECK_MASK_COPIED, 0);
    }
}

//this function sends mail to old bidder
void AuctionHouseMgr::SendAuctionOutbiddedMail(AuctionEntry* auction, uint64 newPrice, Player* newBidder, SQLTransaction& trans)
{
    uint64 oldBidder_guid = MAKE_NEW_GUID(auction->bidder, 0, HIGHGUID_PLAYER);
    Player* oldBidder = ObjectAccessor::FindPlayer(oldBidder_guid);

    uint32 oldBidder_accId = 0;
    if (!oldBidder)
        oldBidder_accId = sObjectMgr->GetPlayerAccountIdByGUID(oldBidder_guid);

    // old bidder exist
    if (oldBidder || oldBidder_accId)
    {
        if (oldBidder && newBidder)
            oldBidder->GetSession()->SendAuctionBidderNotification(auction, newBidder->GetGUID(), newPrice);

        MailDraft(auction->BuildAuctionMailSubject(AUCTION_OUTBIDDED), AuctionEntry::BuildAuctionMailBody(auction->owner, auction->bid, auction->buyout, auction->deposit, auction->GetAuctionCut()))
            .AddMoney(auction->bid)
            .SendMailTo(trans, MailReceiver(oldBidder, auction->bidder), auction, MAIL_CHECK_MASK_COPIED);
    }
}

//this function sends mail, when auction is cancelled to old bidder
void AuctionHouseMgr::SendAuctionCancelledToBidderMail(AuctionEntry* auction, SQLTransaction& trans, Item* /*item*/)
{
    uint64 bidder_guid = MAKE_NEW_GUID(auction->bidder, 0, HIGHGUID_PLAYER);
    Player* bidder = ObjectAccessor::FindPlayer(bidder_guid);

    uint32 bidder_accId = 0;
    if (!bidder)
        bidder_accId = sObjectMgr->GetPlayerAccountIdByGUID(bidder_guid);

    // bidder exist
    if (bidder || bidder_accId)
        MailDraft(auction->BuildAuctionMailSubject(AUCTION_CANCELLED_TO_BIDDER), AuctionEntry::BuildAuctionMailBody(auction->owner, auction->bid, auction->buyout, auction->deposit, 0))
            .AddMoney(auction->bid)
            .SendMailTo(trans, MailReceiver(bidder, auction->bidder), auction, MAIL_CHECK_MASK_COPIED);
}

void AuctionHouseMgr::LoadAuctionItems()
{
    uint32 oldMSTime = getMSTime();

    // need to clear in case we are reloading
    if (!mAitems.empty())
    {
        for (ItemMap::iterator itr = mAitems.begin(); itr != mAitems.end(); ++itr)
            delete itr->second;

        mAitems.clear();
    }

    // data needs to be at first place for Item::LoadFromDB
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_AUCTION_ITEMS);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 auction items. DB table `auctionhouse` or `item_instance` is empty!");

        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 item_guid    = fields[16].GetUInt32();
        uint32 itemEntry    = fields[17].GetUInt32();

        ItemTemplate const* proto = sObjectMgr->GetItemTemplate(itemEntry);
        if (!proto)
        {
            sLog->outError(LOG_FILTER_GENERAL, "AuctionHouseMgr::LoadAuctionItems: Unknown item (GUID: %u id: #%u) in auction, skipped.", item_guid, itemEntry);
            continue;
        }

        Item* item = NewItemOrBag(proto);
        if (!item->LoadFromDB(item_guid, 0, fields, itemEntry))
        {
            delete item;
            continue;
        }
        AddAItem(item);

        ++count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u auction items in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void AuctionHouseMgr::LoadAuctions()
{
    uint32 oldMSTime = getMSTime();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_AUCTIONS);
    PreparedQueryResult result = CharacterDatabase.Query(stmt);

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 auctions. DB table `auctionhouse` is empty.");

        return;
    }

    uint32 count = 0;

    SQLTransaction trans = CharacterDatabase.BeginTransaction();
    do
    {
        Field* fields = result->Fetch();

        AuctionEntry* aItem = new AuctionEntry();
        if (!aItem->LoadFromDB(fields))
        {
            aItem->DeleteFromDB(trans);
            delete aItem;
            continue;
        }

        GetAuctionsMap(aItem->factionTemplateId)->AddAuction(aItem);
        count++;
    }
    while (result->NextRow());

    CharacterDatabase.CommitTransaction(trans);

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u auctions in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

void AuctionHouseMgr::AddAItem(Item* it)
{
    ASSERT(it);
    ASSERT(mAitems.find(it->GetGUIDLow()) == mAitems.end());
    mAitems[it->GetGUIDLow()] = it;
}

bool AuctionHouseMgr::RemoveAItem(uint32 id)
{
    ItemMap::iterator i = mAitems.find(id);
    if (i == mAitems.end())
        return false;

    mAitems.erase(i);
    return true;
}

void AuctionHouseMgr::Update()
{
    mHordeAuctions.Update();
    mAllianceAuctions.Update();
    mNeutralAuctions.Update();
}

AuctionHouseEntry const* AuctionHouseMgr::GetAuctionHouseEntry(uint32 factionTemplateId)
{
    uint32 houseid = 7; // goblin auction house

    if (!sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_AUCTION))
    {
        //FIXME: found way for proper auctionhouse selection by another way
        // AuctionHouse.dbc have faction field with _player_ factions associated with auction house races.
        // but no easy way convert creature faction to player race faction for specific city
        switch (factionTemplateId)
        {
            case   12: houseid = 1; break; // human
            case   29: houseid = 6; break; // orc, and generic for horde
            case   55: houseid = 2; break; // dwarf, and generic for alliance
            case   68: houseid = 4; break; // undead
            case   80: houseid = 3; break; // n-elf
            case  104: houseid = 5; break; // trolls
            case  120: houseid = 7; break; // booty bay, neutral
            case  474: houseid = 7; break; // gadgetzan, neutral
            case  855: houseid = 7; break; // everlook, neutral
            case 1604: houseid = 6; break; // b-elfs,
            default:                       // for unknown case
            {
                FactionTemplateEntry const* u_entry = sFactionTemplateStore.LookupEntry(factionTemplateId);
                if (!u_entry)
                    houseid = 7; // goblin auction house
                else if (u_entry->Mask & FACTION_MASK_ALLIANCE)
                    houseid = 1; // human auction house
                else if (u_entry->Mask & FACTION_MASK_HORDE)
                    houseid = 6; // orc auction house
                else
                    houseid = 7; // goblin auction house
                break;
            }
        }
    }

    return sAuctionHouseStore.LookupEntry(houseid);
}

void AuctionHouseObject::AddAuction(AuctionEntry* auction)
{
    ASSERT(auction);

    AuctionsMap[auction->Id] = auction;
    sScriptMgr->OnAuctionAdd(this, auction);
}

bool AuctionHouseObject::RemoveAuction(AuctionEntry* auction, uint32 /*itemEntry*/)
{
    bool wasInMap = AuctionsMap.erase(auction->Id) ? true : false;

    sScriptMgr->OnAuctionRemove(this, auction);

    // we need to delete the entry, it is not referenced any more
    delete auction;
    return wasInMap;
}

void AuctionHouseObject::Update()
{
    time_t curTime = sWorld->GetGameTime();
    ///- Handle expired auctions

    // If storage is empty, no need to update. next == NULL in this case.
    if (AuctionsMap.empty())
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_AUCTION_BY_TIME);
    stmt->setUInt32(0, (uint32)curTime+60);
    
    PreparedQueryResult l_Result = CharacterDatabase.Query(stmt);
    if (!l_Result)
        return;

    do
    {
        // from auctionhousehandler.cpp, creates auction pointer & player pointer
        AuctionEntry* auction = GetAuction(l_Result->Fetch()->GetUInt32());

        if (!auction)
            continue;

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        ///- Either cancel the auction if there was no bidder
        if (auction->bidder == 0)
        {
            sAuctionMgr->SendAuctionExpiredMail(auction, trans);
            sScriptMgr->OnAuctionExpire(this, auction);
        }
        ///- Or perform the transaction
        else
        {
            //we should send an "item sold" message if the seller is online
            //we send the item to the winner
            //we send the money to the seller
            sAuctionMgr->SendAuctionSuccessfulMail(auction, trans);
            sAuctionMgr->SendAuctionWonMail(auction, trans);
            sScriptMgr->OnAuctionSuccessful(this, auction);
        }

        uint32 itemEntry = auction->itemEntry;

        ///- In any case clear the auction
        auction->DeleteFromDB(trans);
        CharacterDatabase.CommitTransaction(trans);

        sAuctionMgr->RemoveAItem(auction->itemGUIDLow);
        RemoveAuction(auction, itemEntry);
    }
    while (l_Result->NextRow());
}

void AuctionHouseObject::BuildListBidderItems(WorldPacket& data, Player* player, uint32& count, uint32& totalcount)
{
    for (AuctionEntryMap::const_iterator itr = AuctionsMap.begin(); itr != AuctionsMap.end(); ++itr)
    {
        AuctionEntry* Aentry = itr->second;
        if (Aentry && Aentry->bidder == player->GetGUIDLow())
        {
            if (itr->second->BuildAuctionInfo(data))
                ++count;

            ++totalcount;
        }
    }
}

void AuctionHouseObject::BuildListOwnerItems(WorldPacket& data, Player* player, uint32& count, uint32& totalcount)
{
    for (AuctionEntryMap::const_iterator itr = AuctionsMap.begin(); itr != AuctionsMap.end(); ++itr)
    {
        AuctionEntry* Aentry = itr->second;
        if (Aentry && Aentry->owner == player->GetGUIDLow())
        {
            if (Aentry->BuildAuctionInfo(data))
                ++count;

            ++totalcount;
        }
    }
}

void AuctionHouseObject::BuildListAuctionItems(WorldPacket& data, Player* player,
    std::wstring const& wsearchedname, uint32 listfrom, uint8 levelmin, uint8 levelmax, uint8 usable,
    uint32 inventoryType, uint32 itemClass, uint32 itemSubClass, uint32 quality,
    uint32& count, uint32& totalcount)
{
    int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
    int locdbc_idx = player->GetSession()->GetSessionDbcLocale();

    for (AuctionEntryMap::const_iterator itr = AuctionsMap.begin(); itr != AuctionsMap.end(); ++itr)
    {
        AuctionEntry* Aentry = itr->second;
        Item* item = sAuctionMgr->GetAItem(Aentry->itemGUIDLow);
        if (!item)
            continue;

        ItemTemplate const* proto = item->GetTemplate();

        if (itemClass != 0xffffffff && proto->Class != itemClass)
            continue;

        if (itemSubClass != 0xffffffff && proto->SubClass != itemSubClass)
            continue;

        if (inventoryType != 0xffffffff && proto->InventoryType != inventoryType)
            continue;

        if (quality != 0xffffffff && proto->Quality != quality)
            continue;

        if (levelmin != 0x00 && (proto->RequiredLevel < levelmin || (levelmax != 0x00 && proto->RequiredLevel > levelmax)))
            continue;

        if (usable != 0x00 && player->CanUseItem(item) != EQUIP_ERR_OK)
            continue;

        // Allow search by suffix (ie: of the Monkey) or partial name (ie: Monkey)
        // No need to do any of this if no search term was entered
        if (!wsearchedname.empty())
        {
            std::string name = proto->Name1->Get(loc_idx);
            if (name.empty())
                continue;

            // DO NOT use GetItemEnchantMod(proto->RandomProperty) as it may return a result
            //  that matches the search but it may not equal item->GetItemRandomPropertyId()
            //  used in BuildAuctionInfo() which then causes wrong items to be listed
            int32 propRefID = item->GetItemRandomPropertyId();

            if (propRefID)
            {
                // Append the suffix to the name (ie: of the Monkey) if one exists
                // These are found in ItemRandomProperties.dbc, not ItemRandomSuffix.dbc
                //  even though the DBC names seem misleading
                const ItemRandomPropertiesEntry* itemRandProp = sItemRandomPropertiesStore.LookupEntry(propRefID);

                if (itemRandProp)
                {
                    char* temp = itemRandProp->nameSuffix;
                    //char* temp = itemRandProp->nameSuffix;

                    // dbc local name
                    if (temp)
                    {
                        // Append the suffix (ie: of the Monkey) to the name using localization
                        // or default enUS if localization is invalid
                        name += ' ';
                        name += temp[locdbc_idx >= 0 ? locdbc_idx : LOCALE_enUS];
                    }
                }
            }

            // Perform the search (with or without suffix)
            if (!Utf8FitTo(name, wsearchedname))
                continue;
        }

        // Add the item if no search term or if entered search term was found
        if (count < 50 && totalcount >= listfrom)
        {
            ++count;
            Aentry->BuildAuctionInfo(data);
        }
        ++totalcount;
    }
}

//this function inserts to WorldPacket auction's data
bool AuctionEntry::BuildAuctionInfo(WorldPacket& p_Data) const
{
    Item* l_Item = sAuctionMgr->GetAItem(itemGUIDLow);
    if (!l_Item)
    {
        sLog->outError(LOG_FILTER_GENERAL, "AuctionEntry::BuildAuctionInfo: Auction %u has a non-existent item: %u", Id, itemGUIDLow);
        return false;
    }

    Item::BuildDynamicItemDatas(p_Data, l_Item);

    p_Data << uint32(l_Item->GetCount());
    p_Data << uint32(l_Item->GetSpellCharges());
    p_Data << uint32(PROP_ENCHANTMENT_SLOT_0);
    p_Data << uint32(0);    // Flags
    p_Data << uint32(Id);
    p_Data.appendPackGUID(MAKE_NEW_GUID(owner, 0, HIGHGUID_PLAYER));
    p_Data << uint64(startbid);
    p_Data << uint64(bid ? GetAuctionOutBid() : 0);
    p_Data << uint64(buyout);
    p_Data << uint32((expire_time - time(NULL)) * IN_MILLISECONDS);
    p_Data << uint8(0); // Delete reason

    for (uint8 l_Iter = 0; l_Iter < PROP_ENCHANTMENT_SLOT_0; ++l_Iter)
    {
        p_Data << uint32(l_Item->GetEnchantmentId(EnchantmentSlot(l_Iter)));
        p_Data << uint32(l_Item->GetEnchantmentDuration(EnchantmentSlot(l_Iter)));
        p_Data << uint32(l_Item->GetEnchantmentCharges(EnchantmentSlot(l_Iter)));
        p_Data << uint8(l_Iter);
    }

    p_Data.WriteBit(true);  // !CensorServerSideInfo
    p_Data.WriteBit(false); // !CensorBidInfo
    p_Data.FlushBits();
    p_Data.appendPackGUID(MAKE_NEW_GUID(bidder, 0, HIGHGUID_PLAYER));
    p_Data << uint64(bid);

    return true;
}

uint64 AuctionEntry::GetAuctionCut() const
{
    int64 cut = int64(CalculatePct(bid, auctionHouseEntry->ConsignmentRate) * sWorld->getRate(RATE_AUCTION_CUT));
    return std::max(cut, (int64)0);
}

/// the sum of outbid is (1% from current bid)*5, if bid is very small, it is 1c
uint64 AuctionEntry::GetAuctionOutBid() const
{
    uint64 outbid = CalculatePct(bid, 5);
    return outbid ? outbid : 1;
}

void AuctionEntry::DeleteFromDB(SQLTransaction& trans) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_AUCTION);
    stmt->setUInt32(0, Id);
    trans->Append(stmt);
}

void AuctionEntry::SaveToDB(SQLTransaction& trans) const
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_AUCTION);
    stmt->setUInt32(0, Id);
    stmt->setUInt32(1, auctioneer);
    stmt->setUInt32(2, itemGUIDLow);
    stmt->setUInt32(3, owner);
    stmt->setUInt64(4, buyout);
    stmt->setUInt32(5, expire_time);
    stmt->setUInt32(6, bidder);
    stmt->setUInt64(7, bid);
    stmt->setUInt64(8, startbid);
    stmt->setUInt64(9, deposit);
    trans->Append(stmt);
}

bool AuctionEntry::LoadFromDB(Field* fields)
{
    uint8 index = 0;

    Id          = fields[index++].GetUInt32();
    auctioneer  = fields[index++].GetUInt32();
    itemGUIDLow = fields[index++].GetUInt32();
    itemEntry   = fields[index++].GetUInt32();
    itemCount   = fields[index++].GetUInt32();
    owner       = fields[index++].GetUInt32();
    buyout      = fields[index++].GetUInt64();
    expire_time = fields[index++].GetUInt32();
    bidder      = fields[index++].GetUInt32();
    bid         = fields[index++].GetUInt64();
    startbid    = fields[index++].GetUInt64();
    deposit     = fields[index++].GetUInt64();

    CreatureData const* auctioneerData = sObjectMgr->GetCreatureData(auctioneer);
    if (!auctioneerData)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auction %u has not a existing auctioneer (GUID : %u)", Id, auctioneer);
        return false;
    }

    CreatureTemplate const* auctioneerInfo = sObjectMgr->GetCreatureTemplate(auctioneerData->id);
    if (!auctioneerInfo)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auction %u has not a existing auctioneer (GUID : %u Entry: %u)", Id, auctioneer, auctioneerData->id);
        return false;
    }

    factionTemplateId = auctioneerInfo->faction;
    auctionHouseEntry = AuctionHouseMgr::GetAuctionHouseEntry(factionTemplateId);
    if (!auctionHouseEntry)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auction %u has auctioneer (GUID : %u Entry: %u) with wrong faction %u", Id, auctioneer, auctioneerData->id, factionTemplateId);
        return false;
    }

    // check if sold item exists for guid
    // and itemEntry in fact (GetAItem will fail if problematic in result check in AuctionHouseMgr::LoadAuctionItems)
    if (!sAuctionMgr->GetAItem(itemGUIDLow))
    {
        sLog->outError(LOG_FILTER_GENERAL, "Auction %u has not a existing item : %u", Id, itemGUIDLow);
        return false;
    }
    return true;
}

void AuctionHouseMgr::DeleteExpiredAuctionsAtStartup()
{
    // Deletes expired auctions. Should be called at server start before loading auctions.

    // DO NOT USE after auctions are already loaded since this deletes from the DB
    //  and assumes the auctions HAVE NOT been loaded into a list or AuctionEntryMap yet

    uint32 oldMSTime = getMSTime();
    uint32 expirecount = 0;
    time_t curTime = sWorld->GetGameTime();

    // Query the DB to see if there are any expired auctions
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_EXPIRED_AUCTIONS);
    stmt->setUInt32(0, (uint32)curTime+60);
    PreparedQueryResult expAuctions = CharacterDatabase.Query(stmt);

    if (!expAuctions)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> No expired auctions to delete");
        return;
    }

    do
    {
        Field* fields = expAuctions->Fetch();

        AuctionEntry* auction = new AuctionEntry();

        // Can't use LoadFromDB() because it assumes the auction map is loaded
        if (!auction->LoadFromFieldList(fields))
        {
            // For some reason the record in the DB is broken (possibly corrupt
            //  faction info). Delete the object and move on.
            delete auction;
            continue;
        }

        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        if (auction->bidder == 0)
        {
            // Cancel the auction, there was no bidder
            sAuctionMgr->SendAuctionExpiredMail(auction, trans);
        }
        else
        {
            // Send the item to the winner and money to seller
            sAuctionMgr->SendAuctionSuccessfulMail(auction, trans);
            sAuctionMgr->SendAuctionWonMail(auction, trans);
        }

        // Call the appropriate AuctionHouseObject script
        //  ** Do we need to do this while core is still loading? **
        sScriptMgr->OnAuctionExpire(GetAuctionsMap(auction->factionTemplateId), auction);

        // Delete the auction from the DB
        auction->DeleteFromDB(trans);
        CharacterDatabase.CommitTransaction(trans);

        // Release memory
        delete auction;
        ++expirecount;

    }
    while (expAuctions->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Deleted %u expired auctions in %u ms", expirecount, GetMSTimeDiffToNow(oldMSTime));
}

bool AuctionEntry::LoadFromFieldList(Field* fields)
{
    // Loads an AuctionEntry item from a field list. Unlike "LoadFromDB()", this one
    //  does not require the AuctionEntryMap to have been loaded with items. It simply
    //  acts as a wrapper to fill out an AuctionEntry struct from a field list

    uint8 index = 0;

    Id          = fields[index++].GetUInt32();
    auctioneer  = fields[index++].GetUInt32();
    itemGUIDLow = fields[index++].GetUInt32();
    itemEntry   = fields[index++].GetUInt32();
    itemCount   = fields[index++].GetUInt32();
    owner       = fields[index++].GetUInt32();
    buyout      = fields[index++].GetUInt32();
    expire_time = fields[index++].GetUInt32();
    bidder      = fields[index++].GetUInt32();
    bid         = fields[index++].GetUInt32();
    startbid    = fields[index++].GetUInt32();
    deposit     = fields[index++].GetUInt32();

    CreatureData const* auctioneerData = sObjectMgr->GetCreatureData(auctioneer);
    if (!auctioneerData)
    {
        sLog->outError(LOG_FILTER_GENERAL, "AuctionEntry::LoadFromFieldList() - Auction %u has not a existing auctioneer (GUID : %u)", Id, auctioneer);
        return false;
    }

    CreatureTemplate const* auctioneerInfo = sObjectMgr->GetCreatureTemplate(auctioneerData->id);
    if (!auctioneerInfo)
    {
        sLog->outError(LOG_FILTER_GENERAL, "AuctionEntry::LoadFromFieldList() - Auction %u has not a existing auctioneer (GUID : %u Entry: %u)", Id, auctioneer, auctioneerData->id);
        return false;
    }

    factionTemplateId = auctioneerInfo->faction;
    auctionHouseEntry = AuctionHouseMgr::GetAuctionHouseEntry(factionTemplateId);

    if (!auctionHouseEntry)
    {
        sLog->outError(LOG_FILTER_GENERAL, "AuctionEntry::LoadFromFieldList() - Auction %u has auctioneer (GUID : %u Entry: %u) with wrong faction %u", Id, auctioneer, auctioneerData->id, factionTemplateId);
        return false;
    }

    return true;
}

std::string AuctionEntry::BuildAuctionMailSubject(MailAuctionAnswers response) const
{
    std::ostringstream strm;
    strm << itemEntry << ":0:" << response << ':' << Id << ':' << itemCount;
    strm << ":0" << ":0" << ":0" << ":0" << ":0" << ":0" << ":0";
    return strm.str();
}

std::string AuctionEntry::BuildAuctionMailBody(uint32 p_LowGUID, uint64 p_BID, uint64 p_Buyout, uint64 p_Deposit, uint64 p_Cut)
{
    std::ostringstream l_Body;

    l_Body << "Player-" << g_RealmID << '-' << std::hex << p_LowGUID;
    l_Body << std::dec << ':' << p_BID << ':' << p_Buyout;
    l_Body << ':' << p_Deposit << ':' << p_Cut << ":0";

    return l_Body.str();
}
#endif