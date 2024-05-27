#pragma once

#include <random.hpp>

#include <cassert>
#include <cstddef>
#include <random>
#include <array>

template<typename T, size_t N = 6>
using Dice = std::array<T, N>;

template<typename T, size_t N>
T throwDice(const Dice<T, N>& dice) {
    assert(N > 0);
    auto& rng = getRNG();
    std::uniform_int_distribution<size_t> dis(0, N - 1);
    return dice[dis(rng)];
}
