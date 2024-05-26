#include <random.hpp>

std::mt19937& getRNG() {
    static std::mt19937 rng_ = std::mt19937(std::random_device{}());
    return rng_;
}
