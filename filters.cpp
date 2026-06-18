#include "filters.h"

#include "audio_utils.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

constexpr double pi = 3.14159265358979323846;

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

LowpassFilter::LowpassFilter(size_t windowSize)
    : _windowSize(windowSize) {
    if (windowSize == 0 || windowSize % 2 == 0) {
        throw std::logic_error("lowpass window size must be odd and positive");
    }
}

FilterState LowpassFilter::apply(Waveform* sound) {
    if (sound == nullptr) {
        return FilterState::Error;
    }

    if (sound->empty() || _windowSize == 1) {
        return FilterState::Ok;
    }

    const auto& samples = sound->samples();
    size_t n = samples.size();
    size_t half = _windowSize / 2;

    std::vector<long long> pref(n + 1, 0);

    for (size_t i = 0; i < n; i++) {
        pref[i + 1] = pref[i] + samples[i];
    }

    std::vector<Waveform::sample> result(n);

    for (size_t i = 0; i < n; i++) {
        size_t l = 0;
        size_t r = n - 1;

        if (i > half) {
            l = i - half;
        }

        if (i + half < n) {
            r = i + half;
        }

        long long sum = pref[r + 1] - pref[l];

        size_t leftExtra = 0;
        if (i < half) {
            leftExtra = half - i;
        }

        size_t rightExtra = 0;
        if (i + half >= n) {
            rightExtra = i + half - (n - 1);
        }

        sum += static_cast<long long>(leftExtra) * samples.front();
        sum += static_cast<long long>(rightExtra) * samples.back();
        double value = static_cast<double>(sum) / _windowSize;
        result[i] = clampSample(value);
    }

    sound->samples() = std::move(result);
    return FilterState::Ok;
}

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
        double phase = 2.0 * pi * _carrierHz * t + (_deviationHz / _modulationHz) * std::sin(2.0 * pi * _modulationHz * t);
        double value = _amplitude * 32767.0 * std::sin(phase);
        result[i] = clampSample(value);
    }

    sound->samples() = std::move(result);
    return FilterState::Ok;
}
