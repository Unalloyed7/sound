#include "generator_filter.h"

#include "audio_utils.h"
#include "waveform.h"

#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>

constexpr double pi = 3.14159265358979323846;

SinGeneratorFilter::SinGeneratorFilter(double frequencyHz, double durationMs)
    : _frequencyHz(frequencyHz),
      _durationMs(durationMs) {
    if (frequencyHz < 0) {
        throw std::logic_error("sin generator frequency must be non-negative");
    }
    if (durationMs < 0) {
        throw std::logic_error("sin generator duration must be non-negative");
    }
}

FilterState SinGeneratorFilter::apply(Waveform* sound) {
    if (sound == nullptr) {
        return FilterState::Ok;
    }

    size_t n = Waveform::msToSamples(_durationMs);
    std::vector<Waveform::sample> result(n);

    for (size_t i = 0; i < n; i++) {
        double t = static_cast<double>(i) / Waveform::rate;
        double value = 32767.0 * std::sin(2.0 * pi * _frequencyHz * t);
        result[i] = clampSample(value);
    }

    sound->samples() = std::move(result);
    return FilterState::Ok;
}

AmGeneratorFilter::AmGeneratorFilter(double amplitude, double carrierHz, double modulationHz, double depth, double durationMs)
    : _amplitude(amplitude),
      _carrierHz(carrierHz),
      _modulationHz(modulationHz),
      _depth(depth),
      _durationMs(durationMs) {
    if (amplitude < 0 || amplitude > 1) {
        throw std::logic_error("am amplitude must be in [0, 1]");
    }
    if (carrierHz < 0) {
        throw std::logic_error("am carrier frequency must be non-negative");
    }
    if (modulationHz < 0) {
        throw std::logic_error("am modulation frequency must be non-negative");
    }
    if (depth < 0 || depth > 1) {
        throw std::logic_error("am depth must be in [0, 1]");
    }
    if (durationMs < 0) {
        throw std::logic_error("am duration must be non-negative");
    }
}

FilterState AmGeneratorFilter::apply(Waveform* sound) {
    if (sound == nullptr) {
        return FilterState::Ok;
    }

    size_t n = Waveform::msToSamples(_durationMs);
    std::vector<Waveform::sample> result(n);

    for (size_t i = 0; i < n; i++) {
        double t = static_cast<double>(i) / Waveform::rate;
        double envelope = 1.0 + _depth * std::sin(2.0 * pi * _modulationHz * t);
        double carrier = std::sin(2.0 * pi * _carrierHz * t);
        double value = _amplitude * 32767.0 * envelope * carrier;
        result[i] = clampSample(value);
    }

    sound->samples() = std::move(result);
    return FilterState::Ok;
}

FmGeneratorFilter::FmGeneratorFilter(double amplitude, double carrierHz, double modulationHz, double deviationHz, double durationMs)
    : _amplitude(amplitude),
      _carrierHz(carrierHz),
      _modulationHz(modulationHz),
      _deviationHz(deviationHz),
      _durationMs(durationMs) {
    if (amplitude < 0 || amplitude > 1) {
        throw std::logic_error("fm amplitude must be in [0, 1]");
    }
    if (carrierHz < 0) {
        throw std::logic_error("fm carrier frequency must be non-negative");
    }
    if (modulationHz <= 0) {
        throw std::logic_error("fm modulation frequency must be positive");
    }
    if (deviationHz < 0) {
        throw std::logic_error("fm deviation must be non-negative");
    }
    if (durationMs < 0) {
        throw std::logic_error("fm duration must be non-negative");
    }
}

FilterState FmGeneratorFilter::apply(Waveform* sound) {
    if (sound == nullptr) {
        return FilterState::Ok;
    }

    size_t n = Waveform::msToSamples(_durationMs);
    std::vector<Waveform::sample> result(n);

    for (size_t i = 0; i < n; i++) {
        double t = static_cast<double>(i) / Waveform::rate;
        double phase = 2.0 * pi * _carrierHz * t +
                       (_deviationHz / _modulationHz) * std::sin(2.0 * pi * _modulationHz * t);
        double value = _amplitude * 32767.0 * std::sin(phase);
        result[i] = clampSample(value);
    }

    sound->samples() = std::move(result);
    return FilterState::Ok;
}
