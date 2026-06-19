#include "lowpass_filter.h"

#include "audio_utils.h"
#include "waveform.h"

#include <stdexcept>
#include <utility>
#include <vector>

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
