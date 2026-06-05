#include "cmd_line_args_2_pipeline_converter.h"

#include <stdexcept>

CmdLineArgs2PipelineConverter::CmdLineArgs2PipelineConverter() {
}

void CmdLineArgs2PipelineConverter::addFilterProducer(const std::string& filterName, FilterProducer producer) {
    if (producer == nullptr) {
        throw std::logic_error("null filter producer");
    }
    _producers[filterName] = producer;
}

FilterProducer CmdLineArgs2PipelineConverter::getFilterProducer(const std::string& filterName) const {
    auto it = _producers.find(filterName);
    if (it == _producers.end()) {
        return nullptr;
    }
    return it->second;
}

Pipeline CmdLineArgs2PipelineConverter::createPipeline(const std::vector<FilterDescriptor>& descriptors) const {
    Pipeline pipeline;
    for (const auto& fd : descriptors) {
        FilterProducer producer = getFilterProducer(fd.name);
        if (producer == nullptr) {
            throw std::logic_error("unknown filter: " + fd.name);
        }
        IFilter* filter = producer(fd);
        if (filter == nullptr) {
            throw std::logic_error("filter producer returned null");
        }
        pipeline.addFilter(filter);
    }
    return pipeline;
}
