#pragma once

#include <cstdint>

#include "str_functions/str_functions.h"

const char kInputFileArg[] = "--input=";
const char kInputFileArgShort[] = "-i";
const char kOutputDirArg[] = "--output=";
const char kOutputDirArgShort[] = "-o";
const char kMaxIterationsArg[] = "--max-iter=";
const char kMaxIterationsArgShort[] = "-m";
const char kFrequencyArg[] = "--freq=";
const char kFrequencyArgShort[] = "-f";
const char kHelpArg[] = "--help";
const char kHelpArgShort[] = "-h";

struct ParsedArguments{
    char* output_dir = nullptr;
    char* input_file = nullptr;
    uint64_t max_iterations = 0;
    uint64_t frequency = 0;

    bool help_input = false;

    bool invalid_argument = false;
};

ParsedArguments* ParserProcess(int argc, char** argv);
