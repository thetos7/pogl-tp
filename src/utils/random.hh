#pragma once

#include <random>

namespace pogl
{
    inline float float_rand_range(float low = 0.0, float high = 1.0)
    {
        return low + (float)std::rand() / ((float)RAND_MAX / (high - low));
    }
} // namespace pogl
