#include "G_Item.h"

#include "Greed/Item/Structures/G_ItemInfo.h"

uint8 AG_Item::GetIDDatabase() const
{
    return mID;
}

void AG_Item::SetIDDatabase(const uint8& _id)
{
    mID = _id;
}
