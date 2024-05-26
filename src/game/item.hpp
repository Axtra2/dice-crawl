#pragma once

#include <functional>
#include <optional>
#include <cstdint>
#include <string>

using ItemID = int32_t;

struct ItemInfo {
    static constexpr ItemID EMPTY = 0;

    struct WearInfo {
        enum class WearType {
            HAND,
            HEAD,
            TORSO,
            LEGS,
            FEET
        };
        WearType wearType;
        std::optional<std::function<int32_t(int32_t)>> maxHealthModifier = std::nullopt;
        std::optional<std::function<int32_t(int32_t)>> attackModifier = std::nullopt;
    };

    double w = 0.0;
    std::string name;
    std::optional<WearInfo> wearInfo = std::nullopt;
};

const std::unordered_map<ItemID, ItemInfo>& getItemsDict();
