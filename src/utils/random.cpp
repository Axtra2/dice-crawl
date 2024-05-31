#include <utils/random.hpp>

std::mt19937& getRNG() {
    static std::mt19937 rng_ = std::mt19937(1);
    return rng_;
}
