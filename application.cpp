#include "application.h"

#include "args_parser.h"
#include "filter_producers.h"
#include "pipeline.h"
#include "wav_file.h"
#include "waveform.h"

#include <iostream>
#include <stdexcept>

Application::Application() {
}

void Application::configure() {
    _converter.addFilterProducer("ampl", createAmplFilter);
    _converter.addFilterProducer("normalize", createNormalizeFilter);
    _converter.addFilterProducer("silence", createSilenceFilter);
    _converter.addFilterProducer("timestretch", createTimestretchFilter);
    _converter.addFilterProducer("lowpass", createLowpassFilter);
    _converter.addFilterProducer("generator", createGeneratorFilter);
}

int Application::start(int argc, char* argv[]) {
    ArgsParser parser;

    Result result = parser.parse(argc, argv);

    if (result == Result::noArgs) {
        return 0;
    }

    if (result == Result::badArgs) {
        std::cerr << "Bad command line arguments\n";
        return 1;
    }

    Pipeline pipeline = _converter.createPipeline(parser.getFilterDescriptors());

    Waveform sound;

    if (parser.hasInFileName()) {
        sound = WavFile::read(parser.getInFileName());
    }

    FilterState state = pipeline.apply(&sound);

    if (state != FilterState::Ok) {
        std::cerr << "Error while applying filters\n";
        return 2;
    }

    if (parser.hasOutFileName()) {
        WavFile::write(parser.getOutFileName(), sound);
    }

    return 0;
}
