#pragma once

#include "cmd_line_args_2_pipeline_converter.h"

class Application {
public:
    Application();
    void configure();
    int start(int argc, char* argv[]);

private:
    CmdLineArgs2PipelineConverter _converter;
};
