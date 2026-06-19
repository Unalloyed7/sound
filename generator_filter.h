#pragma once

#include "filter.h"

class SinGeneratorFilter : public IFilter {
public:
    SinGeneratorFilter(double frequencyHz, double durationMs);
    ~SinGeneratorFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _frequencyHz;
    double _durationMs;
};

class AmGeneratorFilter : public IFilter {
public:
    AmGeneratorFilter(double amplitude, double carrierHz, double modulationHz, double depth, double durationMs);
    ~AmGeneratorFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _amplitude;
    double _carrierHz;
    double _modulationHz;
    double _depth;
    double _durationMs;
};

class FmGeneratorFilter : public IFilter {
public:
    FmGeneratorFilter(double amplitude, double carrierHz, double modulationHz, double deviationHz, double durationMs);
    ~FmGeneratorFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _amplitude;
    double _carrierHz;
    double _modulationHz;
    double _deviationHz;
    double _durationMs;
};
