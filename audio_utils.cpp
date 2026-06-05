#include "audio_utils.h"

#include <cmath>

Waveform::sample clampSample(double value) {
    value = std::round(value);
    if (value > 32767) {
        return 32767;
    }

    if (value < -32768) {
        return -32768;
    }

    return static_cast<Waveform::sample>(value);
}
