#pragma once

#include "filter.h"
#include "waveform.h"

#include <cstddef>
#include <string>

class AmplFilter : public IFilter {
public:
    AmplFilter(double factor);
    ~AmplFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _factor;
};

class NormalizeFilter : public IFilter {
public:
    NormalizeFilter(double peak = 1.0);
    ~NormalizeFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _peak;
};

class SilenceFilter : public IFilter {
public:
    SilenceFilter(const std::string& unit, double start, double end);
    ~SilenceFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    size_t _start;
    size_t _end;
};

class TimestretchFilter : public IFilter {
public:
    TimestretchFilter(double factor);
    ~TimestretchFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    double _factor;
};

class LowpassFilter : public IFilter {
public:
    LowpassFilter(size_t windowSize);
    ~LowpassFilter() override = default;
    FilterState apply(Waveform* sound) override;

protected:
    size_t _windowSize;
};

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
