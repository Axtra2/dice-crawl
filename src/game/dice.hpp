#pragma once

#include <utils/random.hpp>

#include <cassert>
#include <cstddef>
#include <random>
#include <string>
#include <vector>

template<typename T>
using Dice = std::vector<std::pair<T, std::string>>;

template<typename T>
T throwDice(const Dice<T>& dice) {
    assert(dice.size() > 0);
    auto& rng = getRNG();
    std::uniform_int_distribution<size_t> dis(0, dice.size() - 1);
    return dice[dis(rng)].first;
}
