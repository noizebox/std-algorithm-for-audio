#include <algorithm>
#include <cstdlib>
#include <limits>
#include <cstring>

#include "algorithms.h"

inline float float_abs(float val)
{
    return val >= 0? val : -val;
}

inline float float_clamp(float x, float min, float max)
{
    if (x > max)
    {
        x = max;
    }
    if (x < min)
    {
        x = min;
    }
    return x;
}

int clip_detect_std_alg(const AudioBuffer& buffer)
{
    auto [min_el, max_el] = std::minmax_element(buffer.begin(), buffer.end());
    if (*min_el < -1.0f || *max_el > 1.0f)
    {
        return 1;
    }
    return 0;
}

int clip_detection_custom_minmax(const AudioBuffer& buffer)
{
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    for (const auto& sample : buffer)
    {
        min = sample < min? sample : min;
        max = sample > max? sample : max;
    }
    if (min < -1.0f || max > 1.0f)
    {
        return 1;
    }
    return 0;
}

int clip_detect_early_return(const AudioBuffer& buffer)
{
    for (auto& sample : buffer)
    {
        if (sample > 1.0f || sample < -1.0f)
        {
            return 1;
        }
    }
    return 0;
}

int clip_detect_optimised(const AudioBuffer& buffer)
{
    int clipcount = 0;
    for (auto& sample : buffer)
    {
        clipcount += (std::abs(sample) > 1.0f);
        // You can also use the custom abs(), they are more or less the same,
        // performance wise on all systems I've tried.
        //clipcount += (float_abs(sample) > 1.0f);
    }
    return clipcount > 1;
}

void clamp_alg(AudioBuffer& buffer)
{
    for (auto& sample : buffer)
    {
        sample = std::clamp(sample, -1.0f, 1.0f);
    }
}

void clamp_alg_direct_access(AudioBuffer& buffer)
{
    for (unsigned int i = 0; i < buffer.size(); ++i)
    {
        buffer[i] = std::clamp(buffer[i], -1.0f, 1.0f);
    }
}

void custom_clamp(AudioBuffer& buffer)
{
    for (auto& sample : buffer)
    {
        sample = float_clamp(sample, -1.0f, 1.0f);
    }
}

void alg_fill(AudioBuffer& buffer, float value)
{
    std::fill(buffer.begin(), buffer.end(), value);
}

void memset_fill(AudioBuffer& buffer, int value)
{
    memset(buffer.data(), value, buffer.size() * sizeof(float));
}

void custom_fill(AudioBuffer& buffer, float value)
{
    for (auto& a : buffer)
    {
        a = value;
    }
}

void alg_copy(const AudioBuffer& source, AudioBuffer& dest)
{
    std::copy(source.begin(), source.end(), dest.begin());
}

void memcpy_copy(const AudioBuffer& source, AudioBuffer& dest)
{
    memcpy(dest.data(), source.data(), source.size() * sizeof(float));
}

void custom_copy(const AudioBuffer& source, AudioBuffer& dest)
{
    for (unsigned int i = 0; i < source.size(); ++i)
    {
        dest[i] = source[i];
    }
}