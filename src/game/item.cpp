#include <item.hpp>
#include <dice.hpp>

#include <functional>

const std::unordered_map<ItemID, ItemInfo>& getItemsDict() {
    using enum ItemInfo::WearInfo::WearType;
    static const std::unordered_map<ItemID, ItemInfo> dict_ = {
        { ItemInfo::EMPTY, { .w = 1.0 } },
        { 1, {
            .w = 0.01,
            .name = "sword",
            .wearInfo = ItemInfo::WearInfo{
                .wearType = HAND,
                .attackDice = Dice<std::function<int32_t(int32_t)>>{
                    [](int32_t base){ return base * base; },
                    [](int32_t base){ return base + 5; },
                    [](int32_t base){ return base * 2; },
                    [](int32_t base){ return base; },
                    [](int32_t     ){ return 5; },
                    [](int32_t     ){ return 0; }
                } } } },
        { 2, {
            .w = 0.01,
            .name = "helmet",
            .wearInfo = ItemInfo::WearInfo{
                .wearType = HEAD,
                .defenseDice = Dice<std::function<int32_t(int32_t)>>{
                    [](int32_t){ return 0; },
                    [](int32_t){ return 1; },
                    [](int32_t){ return 2; },
                    [](int32_t){ return 3; },
                    [](int32_t){ return 4; },
                    [](int32_t){ return 5; }
                } } } }
    };
    return dict_;
}
