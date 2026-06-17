#include "pipeline.h"

#include "waveform.h"

#include <utility>

Pipeline::Pipeline(Pipeline&& other) noexcept
    : _filters(std::move(other._filters)) {
    other._filters.clear();
}

Pipeline& Pipeline::operator=(Pipeline&& other) noexcept {
    if (this != &other) {
        clearFilters();
        _filters = std::move(other._filters);
        other._filters.clear();
    }

    return *this;
}

Pipeline::~Pipeline() {
    clearFilters();
}

FilterState Pipeline::apply(Waveform* sound) {
    Waveform emptySound;
    if (sound == nullptr) {
        sound = &emptySound;
    }

    for (IFilter* filter : _filters) {
        if (filter == nullptr) {
            return FilterState::Error;
        }
        FilterState state = filter->apply(*sound);
        if (state != FilterState::Ok) {
            return state;
        }
    }
    return FilterState::Ok;
}

IFilter* Pipeline::addFilter(IFilter* filter) {
    _filters.push_back(filter);
    return filter;
}

size_t Pipeline::getFilterNumbers() const {
    return _filters.size();
}

IFilter* Pipeline::operator[](size_t i) const {
    return _filters[i];
}

void Pipeline::clearFilters() {
    for (IFilter* filter : _filters) {
        delete filter;
    }
    _filters.clear();
}
