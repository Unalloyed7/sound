#pragma once

#include "filter_descriptor.h"

#include <vector>

enum class Result {
    ok,
    noArgs,
    badArgs,
};

class ArgsParser {
public:
    ArgsParser();
    Result parse(int argc, char* argv[]);

    const char* getInFileName() const;
    const char* getOutFileName() const;

    bool hasInFileName() const;
    bool hasOutFileName() const;

    const std::vector<FilterDescriptor>& getFilterDescriptors() const;

private:
    char* _inFileName;
    char* _outFileName;
    bool _hasInFileName;
    bool _hasOutFileName;
    std::vector<FilterDescriptor> _filterDescriptors;
    bool isSpecialFlag(const char* s) const;
};
