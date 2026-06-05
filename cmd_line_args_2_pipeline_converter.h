#pragma once

#include "args_parser.h"
#include "pipeline.h"
#include "filter_producers.h"

#include <map>
#include <string>
#include <vector>

class CmdLineArgs2PipelineConverter {
public:
    CmdLineArgs2PipelineConverter();

    void addFilterProducer(const std::string& filterName, FilterProducer producer);
    FilterProducer getFilterProducer(const std::string& filterName) const;

    Pipeline createPipeline(const std::vector<FilterDescriptor>& descriptors) const;

private:
    std::map<std::string, FilterProducer> _producers;
};
