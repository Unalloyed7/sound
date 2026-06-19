#include "silence_filter.h"

#include "waveform.h"

#include <algorithm>
#include <stdexcept>

SilenceFilter::SilenceFilter(const std::string& unit, double start, double end) {
    if (start < 0) {
        throw std::logic_error("silence start must be non-negative");
    }
    if (end < start) {
        throw std::logic_error("silence end must be greater than or equal to start");
    }
    if (unit == "sec") {
        _start = Waveform::secToSamples(start);
        _end = Waveform::secToSamples(end);
    } else if (unit == "ms") {
        _start = Waveform::msToSamples(start);
        _end = Waveform::msToSamples(end);
    } else {
        throw std::logic_error("silence unit must be sec or ms");
    }
}

FilterState SilenceFilter::apply(Waveform* sound) {
    if (sound == nullptr) {
        return FilterState::Error;
    }

    if (_end <= _start) {
        return FilterState::Ok;
    }

    size_t count = _end - _start;
    size_t pos = std::min(_start, sound->size());

    sound->samples().insert(sound->samples().begin() + pos, count, 0);

    return FilterState::Ok;
}
