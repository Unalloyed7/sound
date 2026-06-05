#pragma once

#include "filter.h"
#include "waveform.h"

#include <cstddef>
#include <string>

class AmplFilter : public IFilter {
public:
    explicit AmplFilter(double factor);
    FilterState apply(Waveform& sound) override;

private:
    double _factor;
};

class NormalizeFilter : public IFilter {
public:
    explicit NormalizeFilter(double peak = 1.0);
    FilterState apply(Waveform& sound) override;

private:
    double _peak;
};

class SilenceFilter : public IFilter {
public:
    SilenceFilter(const std::string& unit, double start, double end);
    FilterState apply(Waveform& sound) override;

private:
    size_t _start;
    size_t _end;
};

class TimestretchFilter : public IFilter {
public:
    explicit TimestretchFilter(double factor);
    FilterState apply(Waveform& sound) override;

private:
    double _factor;
};

class LowpassFilter : public IFilter {
public:
    explicit LowpassFilter(size_t windowSize);
    FilterState apply(Waveform& sound) override;

private:
    size_t _windowSize;
};

class SinGeneratorFilter : public IFilter {
public:
    SinGeneratorFilter(double frequencyHz, double durationMs);
    FilterState apply(Waveform& sound) override;

private:
    double _frequencyHz;
    double _durationMs;
};

class AmGeneratorFilter : public IFilter {
public:
    AmGeneratorFilter(double amplitude, double carrierHz, double modulationHz, double depth, double durationMs);
    FilterState apply(Waveform& sound) override;

private:
    double _amplitude;
    double _carrierHz;
    double _modulationHz;
    double _depth;
    double _durationMs;
};

class FmGeneratorFilter : public IFilter {
public:
    FmGeneratorFilter(double amplitude, double carrierHz, double modulationHz, double deviationHz, double durationMs);
    FilterState apply(Waveform& sound) override;

private:
    double _amplitude;
    double _carrierHz;
    double _modulationHz;
    double _deviationHz;
    double _durationMs;
};
