#pragma once

#include "filter.h"

#include <cstddef>
#include <string>

class SilenceFilter : public IFilter {
public:
    SilenceFilter(const std::string& unit, double start, double end);
    ~SilenceFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    size_t _start;
    size_t _end;
};
