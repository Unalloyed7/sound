#include "ampl_filter.h"

#include "audio_utils.h"
#include "waveform.h"

#include <stdexcept>

AmplFilter::AmplFilter(double factor)
    : _factor(factor) {
    if (factor < 0) {
        throw std::logic_error("ampl factor must be non-negative");
    }
}

FilterState AmplFilter::apply(Waveform* sound) {
    if (sound == nullptr) {
        return FilterState::Error;
    }

    for (auto& x : sound->samples()) {
        x = clampSample(static_cast<double>(x) * _factor);
    }
    return FilterState::Ok;
}
