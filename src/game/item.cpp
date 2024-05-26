#include <item.hpp>

const std::unordered_map<ItemID, ItemInfo>& getItemsDict() {
    using enum ItemInfo::WearInfo::WearType;
    static const std::unordered_map<ItemID, ItemInfo> dict_ = {
        { ItemInfo::EMPTY, { .w = 1.0 } },
        { 1, {
            .w = 0.01,
            .name = "sword",
            .wearInfo = ItemInfo::WearInfo{
                .wearType = HAND,
                .attackModifier = [](int32_t a){return a + 1;} } } },
        { 2, {
            .w = 0.01,
            .name = "helmet",
            .wearInfo = ItemInfo::WearInfo{
                .wearType = HEAD,
                .maxHealthModifier = [](int32_t h){return h + 3;} } } },
    };
    return dict_;
}
