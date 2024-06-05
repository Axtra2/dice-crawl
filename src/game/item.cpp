#include <game/item.hpp>
#include <game/dice.hpp>

#include <functional>
#include <cassert>
#include <utility>

const std::unordered_map<int32_t, ItemInfo>& getItemsDict() {
    using enum ItemInfo::WearInfo::WearType;
    static const std::unordered_map<int32_t, ItemInfo> dict_ = {
        { 1,
          ItemInfo{}.setW(0.01)
                    .setName("Sword")
                    .setWearType(HAND)
                    .addAttackDiceSide([](int32_t base){ return base * base; }, "^2")
                    .addAttackDiceSide([](int32_t base){ return base * 2; }, "*2")
                    .addAttackDiceSide([](int32_t base){ return base + 5; }, "+5")
                    .addAttackDiceSide([](int32_t base){ return base; }, "=")
                    .addAttackDiceSide([](int32_t     ){ return 5; }, "=5")
                    .addAttackDiceSide([](int32_t     ){ return 0; }, "=0")
        },
        { 2,
          ItemInfo{}.setW(0.01)
                    .setName("Helmet")
                    .setWearType(HEAD)
                    .addDefenseDiceSide([](int32_t base){ return base + 3; }, "+3")
        }
    };
    return dict_;
}

bool ItemInfo::isWearable() const {
    return wearInfo_.has_value();
}

bool ItemInfo::hasAttackDice() const {
    return wearInfo_ && wearInfo_.value().attackDice.has_value();
}

bool ItemInfo::hasDefenseDice() const {
    return wearInfo_ && wearInfo_.value().defenseDice.has_value();
}

double ItemInfo::getW() const {
    return w_;
}

const std::string& ItemInfo::getName() const {
    return name_;
}

const Dice<std::function<int32_t(int32_t)>>& ItemInfo::getAttackDice() const {
    assert(wearInfo_.has_value() && wearInfo_.value().attackDice.has_value());
    return wearInfo_.value().attackDice.value();
}

const Dice<std::function<int32_t(int32_t)>>& ItemInfo::getDefenseDice() const {
    assert(wearInfo_.has_value() && wearInfo_.value().defenseDice.has_value());
    return wearInfo_.value().defenseDice.value();
}

ItemInfo::WearInfo::WearType ItemInfo::getWearType() const {
    assert(wearInfo_.has_value());
    return wearInfo_.value().wearType;
}

ItemInfo& ItemInfo::setW(double w) {
    w_ = w;
    return *this;
}

ItemInfo& ItemInfo::setName(std::string_view name) {
    name_ = name;
    return *this;
}

ItemInfo& ItemInfo::setWearType(WearInfo::WearType wearType) {
    if (!wearInfo_) {
        wearInfo_ = WearInfo();
    }
    wearInfo_.value().wearType = wearType;
    return *this;
}

ItemInfo& ItemInfo::addAttackDiceSide(
    std::function<int32_t(int32_t)> f,
    const std::string_view desc
) {
    assert(wearInfo_.has_value());
    if (!wearInfo_.value().attackDice) {
        wearInfo_.value().attackDice = Dice<decltype(f)>();
    }
    wearInfo_.value().attackDice.value().emplace_back(std::move(f), desc);
    return *this;
}

ItemInfo& ItemInfo::addDefenseDiceSide(
    std::function<int32_t(int32_t)> f,
    const std::string_view desc
) {
    assert(wearInfo_.has_value());
    if (!wearInfo_.value().defenseDice) {
        wearInfo_.value().defenseDice = Dice<decltype(f)>();
    }
    wearInfo_.value().defenseDice.value().emplace_back(std::move(f), desc);
    return *this;
}
