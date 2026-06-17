#pragma once

#include "filter.h"

#include <cstddef>
#include <vector>

class Pipeline {
public:
    Pipeline() = default;
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline(Pipeline&& other) noexcept;
    Pipeline& operator=(Pipeline&& other) noexcept;
    ~Pipeline();

    FilterState apply(Waveform* sound);
    IFilter* addFilter(IFilter* filter);
    size_t getFilterNumbers() const;
    IFilter* operator[](size_t i) const;

protected:
    std::vector<IFilter*> _filters;

private:
    void clearFilters();
};
