#include "waveform.h"

#include <cmath>

size_t Waveform::size() const {
    return _samples.size();
}

bool Waveform::empty() const {
    return _samples.empty();
}

void Waveform::clear() {
    _samples.clear();
}

void Waveform::resize(size_t n) {
    _samples.resize(n);
}

Waveform::sample Waveform::getSample(size_t i) const {
    return _samples[i];
}

void Waveform::setSample(size_t i, sample value) {
    _samples[i] = value;
}

std::vector<Waveform::sample>& Waveform::samples() {
    return _samples;
}

const std::vector<Waveform::sample>& Waveform::samples() const {
    return _samples;
}

int32_t Waveform::getSampleRate() const {
    return rate;
}

size_t Waveform::secToSamples(double sec) {
    return static_cast<size_t>(std::round(sec * rate));
}

size_t Waveform::msToSamples(double ms) {
    return static_cast<size_t>(std::round(ms * rate / 1000.0));
}

double Waveform::durationSec() const {
    return static_cast<double>(_samples.size()) / rate;
}

double Waveform::durationMs() const {
    return durationSec() * 1000.0;
}
