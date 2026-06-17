#include "wav_file.h"

#include <cstring>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <vector>

struct RiffHeader {
    char chunkId[4];
    uint32_t chunkSize;
    char format[4];
};

struct FmtHeader {
    char chunkId[4];
    uint32_t chunkSize;
    uint16_t audioFormat;
    uint16_t channels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
};

struct DataHeader {
    char chunkId[4];
    uint32_t chunkSize;
};

static bool equalId(const char* a, const char* b) {
    return std::memcmp(a, b, 4) == 0;
}

static void checkRead(std::istream& in, const char* message) {
    if (!in) {
        throw std::runtime_error(message);
    }
}

static void checkWrite(std::ostream& out, const char* message) {
    if (!out) {
        throw std::runtime_error(message);
    }
}

Waveform WavFile::read(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);

    if (!in.is_open()) {
        throw std::runtime_error("cannot open input wav file");
    }

    RiffHeader riff{};
    in.read(reinterpret_cast<char*>(&riff), sizeof(riff));
    checkRead(in, "cannot read RIFF header");

    if (!equalId(riff.chunkId, "RIFF")) {
        throw std::runtime_error("bad wav file: RIFF expected");
    }

    if (!equalId(riff.format, "WAVE")) {
        throw std::runtime_error("bad wav file: WAVE expected");
    }

    FmtHeader fmt{};
    in.read(reinterpret_cast<char*>(&fmt), sizeof(fmt));
    checkRead(in, "cannot read fmt header");

    if (!equalId(fmt.chunkId, "fmt ")) {
        throw std::runtime_error("bad wav file: fmt chunk expected");
    }

    if (fmt.chunkSize != 16) {
        throw std::runtime_error("bad wav file: unsupported fmt chunk size");
    }

    if (fmt.audioFormat != 1) {
        throw std::runtime_error("bad wav file: only PCM is supported");
    }

    if (fmt.channels != Waveform::channels) {
        throw std::runtime_error("bad wav file: only mono is supported");
    }

    if (fmt.sampleRate != Waveform::rate) {
        throw std::runtime_error("bad wav file: unsupported sample rate");
    }

    if (fmt.bitsPerSample != Waveform::bitsPerSample) {
        throw std::runtime_error("bad wav file: only 16 bit samples are supported");
    }

    DataHeader data{};
    in.read(reinterpret_cast<char*>(&data), sizeof(data));
    checkRead(in, "cannot read data header");

    if (!equalId(data.chunkId, "data")) {
        throw std::runtime_error("bad wav file: data chunk expected");
    }

    if (data.chunkSize % sizeof(Waveform::sample) != 0) {
        throw std::runtime_error("bad wav file: incorrect data chunk size");
    }

    size_t samplesCount = data.chunkSize / sizeof(Waveform::sample);

    std::vector<Waveform::sample> samples(samplesCount);

    if (samplesCount > 0) {
        in.read(reinterpret_cast<char*>(samples.data()), data.chunkSize);
        checkRead(in, "cannot read wav samples");
    }

    Waveform sound;
    sound.samples() = std::move(samples);

    return sound;
}

void WavFile::write(const std::string& filename, const Waveform& sound) {
    std::ofstream out(filename, std::ios::binary);

    if (!out.is_open()) {
        throw std::runtime_error("cannot open output wav file");
    }

    const auto& samples = sound.samples();

    if (samples.size() > std::numeric_limits<uint32_t>::max() / sizeof(Waveform::sample)) {
        throw std::runtime_error("wav file is too large");
    }

    uint32_t dataSize = static_cast<uint32_t>(samples.size() * sizeof(Waveform::sample));

    RiffHeader riff{
        {'R', 'I', 'F', 'F'},
        36 + dataSize,
        {'W', 'A', 'V', 'E'}
    };

    FmtHeader fmt{
        {'f', 'm', 't', ' '},
        16,
        1,
        static_cast<uint16_t>(Waveform::channels),
        static_cast<uint32_t>(Waveform::rate),
        static_cast<uint32_t>(Waveform::rate * Waveform::channels * Waveform::bitsPerSample / 8),
        static_cast<uint16_t>(Waveform::channels * Waveform::bitsPerSample / 8),
        static_cast<uint16_t>(Waveform::bitsPerSample)
    };

    DataHeader data{
        {'d', 'a', 't', 'a'},
        dataSize
    };

    out.write(reinterpret_cast<const char*>(&riff), sizeof(riff));
    checkWrite(out, "cannot write RIFF header");

    out.write(reinterpret_cast<const char*>(&fmt), sizeof(fmt));
    checkWrite(out, "cannot write fmt header");

    out.write(reinterpret_cast<const char*>(&data), sizeof(data));
    checkWrite(out, "cannot write data header");

    if (!samples.empty()) {
        out.write(reinterpret_cast<const char*>(samples.data()), dataSize);
        checkWrite(out, "cannot write wav samples");
    }
}
