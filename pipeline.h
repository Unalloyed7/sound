#pragma once

#include "filter.h"

#include <cstddef>
#include <vector>

class Pipeline {
public:
    ~Pipeline() {
        for (IFilter* filter : _filters) {
            delete filter;
        }
    }

    FilterState apply(Waveform* sound);
    IFilter* addFilter(IFilter* filter);
    size_t getFilterNumbers() const;
    IFilter* operator[](size_t i) const;

protected:
    std::vector<IFilter*> _filters;
};
