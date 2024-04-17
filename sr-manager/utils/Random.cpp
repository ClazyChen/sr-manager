#include "Random.hpp"

namespace sr {
    std::mt19937 Random::m_generator{ std::random_device{}() };
    std::uniform_int_distribution<std::size_t> Random::m_distribution;
}