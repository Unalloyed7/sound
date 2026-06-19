#include "timestretch_filter.h"

#include "audio_utils.h"
#include "waveform.h"

#include <cmath>
#include <stdexcept>
#include <vector>

TimestretchFilter::TimestretchFilter(double factor)
    : _factor(factor) {
    if (factor <= 0) {
        throw std::logic_error("timestretch factor must be positive");
    }
}

FilterState TimestretchFilter::apply(Waveform* sound) {
    if (sound == nullptr) {
        return FilterState::Error;
    }

    if (sound->empty()) {
        return FilterState::Ok;
    }

    std::vector<Waveform::sample> oldSamples = sound->samples();
    size_t oldSize = oldSamples.size();
    size_t newSize = static_cast<size_t>(std::round(oldSize * _factor));

    if (newSize == 0) {
        sound->clear();
        return FilterState::Ok;
    }

    std::vector<Waveform::sample> newSamples(newSize);

    for (size_t i = 0; i < newSize; i++) {
        double pos = static_cast<double>(i) / _factor;
        size_t l = static_cast<size_t>(std::floor(pos));
        if (l >= oldSize) {
            l = oldSize - 1;
        }
        double frac = pos - l;
        double value = oldSamples[l];
        if (l + 1 < oldSize) {
            value = oldSamples[l] * (1.0 - frac) + oldSamples[l + 1] * frac;
        }
        newSamples[i] = clampSample(value);
    }

    sound->samples() = std::move(newSamples);
    return FilterState::Ok;
}
