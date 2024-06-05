#pragma once

#include <game/dice.hpp>

#include <string_view>
#include <functional>
#include <optional>
#include <cstdint>
#include <string>

class ItemInfo {
public:
    struct WearInfo {
        enum class WearType {
            HAND = 0,
            HEAD,
            TORSO,
            LEGS,
            FEET,
            N_WEAR_TYPES
        };
        WearType wearType;
        std::optional<Dice<std::function<int32_t(int32_t)>>> attackDice = std::nullopt;
        std::optional<Dice<std::function<int32_t(int32_t)>>> defenseDice = std::nullopt;
    };

    bool isWearable() const;
    bool hasAttackDice() const;
    bool hasDefenseDice() const;

    double getW() const;
    const std::string& getName() const;
    const Dice<std::function<int32_t(int32_t)>>& getAttackDice() const;
    const Dice<std::function<int32_t(int32_t)>>& getDefenseDice() const;

    WearInfo::WearType getWearType() const;

    ItemInfo& setW(double w);
    ItemInfo& setName(const std::string_view name);
    ItemInfo& setWearType(WearInfo::WearType wearType);
    ItemInfo& addAttackDiceSide(
        std::function<int32_t(int32_t)> f,
        const std::string_view desc
    );
    ItemInfo& addDefenseDiceSide(
        std::function<int32_t(int32_t)> f,
        const std::string_view desc
    );

private:
    std::optional<WearInfo> wearInfo_ = std::nullopt;
    std::string name_ = "";
    bool isWall_ = false;
    double w_ = 0.0;
};

const std::unordered_map<int32_t, ItemInfo>& getItemsDict();
