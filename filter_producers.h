#pragma once

#include "filter.h"
#include "filter_descriptor.h"

using FilterProducer = IFilter* (*)(const FilterDescriptor& fd);

IFilter* createAmplFilter(const FilterDescriptor& fd);
IFilter* createNormalizeFilter(const FilterDescriptor& fd);
IFilter* createSilenceFilter(const FilterDescriptor& fd);
IFilter* createTimestretchFilter(const FilterDescriptor& fd);
IFilter* createLowpassFilter(const FilterDescriptor& fd);
IFilter* createGeneratorFilter(const FilterDescriptor& fd);
