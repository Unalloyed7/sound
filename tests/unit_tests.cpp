#include <catch2/catch_test_macros.hpp>

#include "../args_parser.h"
#include "../cmd_line_args_2_pipeline_converter.h"
#include "../filter_producers.h"
#include "../filters.h"
#include "../pipeline.h"
#include "../wav_file.h"
#include "../waveform.h"

#include <cstdio>
#include <string>
#include <vector>

TEST_CASE("ArgsParser") {
    const char* argv[] = {
        "sound_processor",
        "-i", "input.wav",
        "-o", "output.wav",
        "-f", "ampl", "0.8",
        "-f", "silence", "sec", "0.2", "0.4"
    };

    ArgsParser parser;
    Result result = parser.parse(13, const_cast<char**>(argv));

    REQUIRE(result == Result::ok);
    REQUIRE(parser.hasInFileName());
    REQUIRE(parser.hasOutFileName());
    REQUIRE(std::string(parser.getInFileName()) == "input.wav");
    REQUIRE(std::string(parser.getOutFileName()) == "output.wav");

    const auto& filters = parser.getFilterDescriptors();
    REQUIRE(filters.size() == 2);
    REQUIRE(filters[0].name == "ampl");
    REQUIRE(filters[0].params == std::vector<std::string>{"0.8"});
    REQUIRE(filters[1].name == "silence");
    REQUIRE(filters[1].params == std::vector<std::string>{"sec", "0.2", "0.4"});
}

TEST_CASE("ArgsParser no args and bad args") {
    ArgsParser parser;

    const char* noArgs[] = {"sound_processor"};
    REQUIRE(parser.parse(1, const_cast<char**>(noArgs)) == Result::noArgs);

    const char* badArgs[] = {"sound_processor", "-i", "-o", "output.wav"};
    REQUIRE(parser.parse(4, const_cast<char**>(badArgs)) == Result::badArgs);
}

TEST_CASE("Waveform") {
    Waveform sound;
    REQUIRE(sound.empty());

    sound.samples() = {1, 2, 3};
    REQUIRE(sound.size() == 3);
    REQUIRE(sound.getSample(1) == 2);

    sound.setSample(1, 10);
    REQUIRE(sound.getSample(1) == 10);
    REQUIRE(Waveform::secToSamples(1.0) == 44100);
    REQUIRE(Waveform::msToSamples(500.0) == 22050);
}

TEST_CASE("Amplitude and normalize") {
    Waveform sound;
    sound.samples() = {1000, -2000, 30000};

    AmplFilter ampl(2.0);
    REQUIRE(ampl.apply(sound) == FilterState::Ok);
    REQUIRE(sound.samples()[0] == 2000);
    REQUIRE(sound.samples()[1] == -4000);
    REQUIRE(sound.samples()[2] == 32767);

    NormalizeFilter normalize(1.0);
    REQUIRE(normalize.apply(sound) == FilterState::Ok);
    REQUIRE(sound.samples()[2] == 32767);
}

TEST_CASE("Transform") {
    Waveform stretched;
    stretched.samples() = {0, 100};

    TimestretchFilter timestretch(2.0);
    REQUIRE(timestretch.apply(stretched) == FilterState::Ok);
    REQUIRE(stretched.samples() == std::vector<Waveform::sample>{0, 50, 100, 100});

    Waveform lowpassed;
    lowpassed.samples() = {0, 30, 60};

    LowpassFilter lowpass(3);
    REQUIRE(lowpass.apply(lowpassed) == FilterState::Ok);
    REQUIRE(lowpassed.samples() == std::vector<Waveform::sample>{10, 30, 50});
}

TEST_CASE("Generator") {
    Waveform sound;
    sound.samples() = {1, 2, 3};

    SinGeneratorFilter generator(440.0, 1000.0);
    REQUIRE(generator.apply(sound) == FilterState::Ok);
    REQUIRE(sound.samples().size() == 44100);
    REQUIRE(sound.samples()[0] == 0);
}

TEST_CASE("pipeline") {
    CmdLineArgs2PipelineConverter converter;
    converter.addFilterProducer("ampl", createAmplFilter);

    FilterDescriptor fd;
    fd.name = "ampl";
    fd.params = {"0.5"};

    Pipeline pipeline = converter.createPipeline({fd});
    REQUIRE(pipeline.getFilterNumbers() == 1);

    Waveform sound;
    sound.samples() = {1000, -1000};
    REQUIRE(pipeline.apply(&sound) == FilterState::Ok);
    REQUIRE(sound.samples() == std::vector<Waveform::sample>{500, -500});
}
