#pragma once

#include "filter.h"

class NormalizeFilter : public IFilter {
public:
    NormalizeFilter(double peak = 1.0);
    ~NormalizeFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _peak;
};
