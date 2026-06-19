#pragma once

#include "filter.h"

class TimestretchFilter : public IFilter {
public:
    TimestretchFilter(double factor);
    ~TimestretchFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _factor;
};
