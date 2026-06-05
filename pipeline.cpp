#include "pipeline.h"

FilterState Pipeline::apply(Waveform* sound) {
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
