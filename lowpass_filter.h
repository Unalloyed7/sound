#pragma once

#include "filter.h"

#include <cstddef>

class LowpassFilter : public IFilter {
public:
    LowpassFilter(size_t windowSize);
    ~LowpassFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    size_t _windowSize;
};
