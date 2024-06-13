#pragma once

#include <type_traits>
#include <concepts>
#include <cassert>

template<std::integral I>
bool addInRange(
    I& a,
    std::convertible_to<I> auto b,
    std::convertible_to<I> auto from,
    std::convertible_to<I> auto to
) {
    a += b; // TODO: handle overflow
    if (a < from) {
        a = from;
        return false;
    }
    if (a > to) {
        a = to;
        return false;
    }
    return true;
}

template<std::integral I>
bool incInRange(
    I& v,
    std::convertible_to<I> auto fromInclusive,
    std::convertible_to<I> auto toExclusive
) {
    assert(static_cast<I>(toExclusive) > static_cast<I>(fromInclusive));
    ++v; // TODO: handle overflow
    if (v < static_cast<I>(fromInclusive)) {
        v = fromInclusive;
        return false;
    }
    if (v >= static_cast<I>(toExclusive)) {
        v = static_cast<I>(toExclusive) - 1;
        return false;
    }
    return true;
}

template<std::integral I>
bool decInRange(
    I& v,
    std::convertible_to<I> auto fromInclusive,
    std::convertible_to<I> auto toExclusive
) {
    assert(static_cast<I>(toExclusive) > static_cast<I>(fromInclusive));
    --v; // TODO: handle overflow
    if (v < static_cast<I>(fromInclusive)) {
        v = fromInclusive;
        return false;
    }
    if (v >= static_cast<I>(toExclusive)) {
        v = static_cast<I>(toExclusive) - 1;
        return false;
    }
    return true;
}
