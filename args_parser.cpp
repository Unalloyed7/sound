#include "args_parser.h"

#include <cstring>

ArgsParser::ArgsParser()
    : _inFileName(nullptr),
      _outFileName(nullptr),
      _hasInFileName(false),
      _hasOutFileName(false) {
}

bool ArgsParser::isSpecialFlag(const char* s) const {
    return std::strcmp(s, "-i") == 0 || std::strcmp(s, "-o") == 0 || std::strcmp(s, "-f") == 0;
}

Result ArgsParser::parse(int argc, char* argv[]) {
    _inFileName = nullptr;
    _outFileName = nullptr;
    _hasInFileName = false;
    _hasOutFileName = false;
    _filterDescriptors.clear();

    if (argc == 1) {
        return Result::noArgs;
    }

    int i = 1;
    while (i < argc) {
        if (std::strcmp(argv[i], "-i") == 0) {
            if (_inFileName != nullptr) {
                return Result::badArgs;
            }

            if (i + 1 >= argc || isSpecialFlag(argv[i + 1])) {
                return Result::badArgs;
            }

            _inFileName = argv[i + 1];
            _hasInFileName = true;
            i += 2;
        }
        else if (std::strcmp(argv[i], "-o") == 0) {
            if (_outFileName != nullptr) {
                return Result::badArgs;
            }

            if (i + 1 >= argc || isSpecialFlag(argv[i + 1])) {
                return Result::badArgs;
            }

            _outFileName = argv[i + 1];
            _hasOutFileName = true;
            i += 2;
        }
        else if (std::strcmp(argv[i], "-f") == 0) {
            if (i + 1 >= argc || isSpecialFlag(argv[i + 1])) {
                return Result::badArgs;
            }

            FilterDescriptor cur;
            cur.name = argv[i + 1];
            i += 2;

            while (i < argc && !isSpecialFlag(argv[i])) {
                cur.params.push_back(argv[i]);
                i++;
            }

            _filterDescriptors.push_back(cur);
        }
        else {
            return Result::badArgs;
        }
    }

    return Result::ok;
}

const char* ArgsParser::getInFileName() const {
    return _inFileName;
}

const char* ArgsParser::getOutFileName() const {
    return _outFileName;
}

bool ArgsParser::hasInFileName() const {
    return _hasInFileName;
}

bool ArgsParser::hasOutFileName() const {
    return _hasOutFileName;
}

const std::vector<FilterDescriptor>& ArgsParser::getFilterDescriptors() const {
    return _filterDescriptors;
}
