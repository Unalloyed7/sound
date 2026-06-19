#include "normalize_filter.h"

#include "audio_utils.h"
#include "waveform.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

NormalizeFilter::NormalizeFilter(double peak)
    : _peak(peak) {
    if (peak < 0 || peak > 1) {
        throw std::logic_error("normalize peak must be in [0, 1]");
    }
}

FilterState NormalizeFilter::apply(Waveform* sound) {
    if (sound == nullptr) {
        return FilterState::Error;
    }

    if (sound->empty()) {
        return FilterState::Ok;
    }

    int currentPeak = 0;
    for (auto x : sound->samples()) {
        int value = std::abs(static_cast<int>(x));
        currentPeak = std::max(currentPeak, value);
    }

    if (currentPeak == 0) {
        return FilterState::Ok;
    }

    double scale = _peak * 32767.0 / currentPeak;

    for (auto& x : sound->samples()) {
        x = clampSample(static_cast<double>(x) * scale);
    }

    return FilterState::Ok;
}
