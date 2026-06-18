#include "filter_producers.h"

#include "filters.h"

#include <stdexcept>
#include <string>

static double parseDouble(const std::string& s, const std::string& message) {
    try {
        size_t pos = 0;
        double value = std::stod(s, &pos);

        if (pos != s.size()) {
            throw std::logic_error(message);
        }

        return value;
    }
    catch (...) {
        throw std::logic_error(message);
    }
}

static long long parseInt(const std::string& s, const std::string& message) {
    try {
        size_t pos = 0;
        long long value = std::stoll(s, &pos);
        if (pos != s.size()) {
            throw std::logic_error(message);
        }

        return value;
    }
    catch (...) {
        throw std::logic_error(message);
    }
}

IFilter* createAmplFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw std::logic_error("ampl filter requires 1 parameter");
    }

    double factor = parseDouble(fd.params[0], "bad ampl factor");

    return new AmplFilter(factor);
}

IFilter* createNormalizeFilter(const FilterDescriptor& fd) {
    if (fd.params.size() > 1) {
        throw std::logic_error("normalize filter requires 0 or 1 parameter");
    }

    double peak = 1.0;

    if (fd.params.size() == 1) {
        peak = parseDouble(fd.params[0], "bad normalize peak");
    }

    return new NormalizeFilter(peak);
}

IFilter* createSilenceFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 3) {
        throw std::logic_error("silence filter requires 3 parameters");
    }

    std::string unit = fd.params[0];
    double start = parseDouble(fd.params[1], "bad silence start");
    double end = parseDouble(fd.params[2], "bad silence end");

    return new SilenceFilter(unit, start, end);
}

IFilter* createTimestretchFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw std::logic_error("timestretch filter requires 1 parameter");
    }

    double factor = parseDouble(fd.params[0], "bad timestretch factor");

    return new TimestretchFilter(factor);
}

IFilter* createLowpassFilter(const FilterDescriptor& fd) {
    if (fd.params.size() != 1) {
        throw std::logic_error("lowpass filter requires 1 parameter");
    }

    long long windowSize = parseInt(fd.params[0], "bad lowpass window size");

    if (windowSize <= 0) {
        throw std::logic_error("lowpass window size must be positive");
    }

    return new LowpassFilter(static_cast<size_t>(windowSize));
}

IFilter* createGeneratorFilter(const FilterDescriptor& fd) {
    if (fd.params.empty()) {
        throw std::logic_error("generator requires signal type");
    }

    const std::string& type = fd.params[0];

    if (type == "sin") {
        if (fd.params.size() != 3) {
            throw std::logic_error("generator sin requires 2 parameters");
        }

        double frequencyHz = parseDouble(fd.params[1], "bad sin frequency");
        double durationMs = parseDouble(fd.params[2], "bad sin duration");

        return new SinGeneratorFilter(frequencyHz, durationMs);
    }

    if (type == "am") {
        if (fd.params.size() != 6) {
            throw std::logic_error("generator am requires 5 parameters");
        }

        double amplitude = parseDouble(fd.params[1], "bad am amplitude");
        double carrierHz = parseDouble(fd.params[2], "bad am carrier frequency");
        double modulationHz = parseDouble(fd.params[3], "bad am modulation frequency");
        double depth = parseDouble(fd.params[4], "bad am depth");
        double durationMs = parseDouble(fd.params[5], "bad am duration");

        return new AmGeneratorFilter(amplitude, carrierHz, modulationHz, depth, durationMs);
    }

    if (type == "fm") {
        if (fd.params.size() != 6) {
            throw std::logic_error("generator fm requires 5 parameters");
        }

        double amplitude = parseDouble(fd.params[1], "bad fm amplitude");
        double carrierHz = parseDouble(fd.params[2], "bad fm carrier frequency");
        double modulationHz = parseDouble(fd.params[3], "bad fm modulation frequency");
        double deviationHz = parseDouble(fd.params[4], "bad fm deviation");
        double durationMs = parseDouble(fd.params[5], "bad fm duration");

        return new FmGeneratorFilter(amplitude, carrierHz, modulationHz, deviationHz, durationMs);
    }

    throw std::logic_error("unknown generator type");
}
