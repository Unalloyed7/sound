#pragma once

#include "filter.h"

class AmplFilter : public IFilter {
public:
    AmplFilter(double factor);
    ~AmplFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _factor;
};
