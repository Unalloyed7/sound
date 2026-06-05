#pragma once

class Waveform;

enum class FilterState {
    Ok,
    Error
};

class IFilter {
public:
    virtual ~IFilter() = default;

    virtual FilterState apply(Waveform& sound) = 0;
};
