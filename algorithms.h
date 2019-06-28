#include <array>
#include <algorithm>
#include <cstdlib>

#pragma once

#ifdef BUFFER_SIZE
constexpr int AUDIO_BUFFER_SIZE = BUFFER_SIZE;
#endif

using AudioBuffer = std::array<float, AUDIO_BUFFER_SIZE>;

int clip_detect_std_alg(const AudioBuffer& buffer);

int clip_detection_custom_minmax(const AudioBuffer& buffer);

int clip_detect_early_return(const AudioBuffer& buffer);

int clip_detect_optimised(const AudioBuffer& buffer);

void clamp_alg(AudioBuffer& buffer);

void clamp_alg_direct_access(AudioBuffer& buffer);

void custom_clamp(AudioBuffer& buffer);

void alg_fill(AudioBuffer& buffer, float value);

void memset_fill(AudioBuffer& buffer, int value);

void custom_fill(AudioBuffer& buffer, float value);

void alg_copy(const AudioBuffer& source, AudioBuffer& dest);

void memcpy_copy(const AudioBuffer& source, AudioBuffer& dest);

void custom_copy(const AudioBuffer& source, AudioBuffer& dest);