#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class Waveform {
public:
    using sample = int16_t;

    static const int32_t rate = 44100;
    static const int32_t channels = 1;
    static const int32_t bitsPerSample = 16;

    size_t size() const;
    bool empty() const;

    void clear();
    void resize(size_t n);

    sample getSample(size_t i) const;
    void setSample(size_t i, sample value);

    std::vector<sample>& samples();
    const std::vector<sample>& samples() const;

    int32_t getSampleRate() const;

    static size_t secToSamples(double sec);
    static size_t msToSamples(double ms);

    double durationSec() const;
    double durationMs() const;

private:
    std::vector<sample> _samples;
};
