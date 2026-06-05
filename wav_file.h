#pragma once

#include "waveform.h"

#include <string>

class WavFile {
public:
    static Waveform read(const std::string& filename);
    static void write(const std::string& filename, const Waveform& sound);
};
