#include "parsing_argv.h"

ParsedArguments* ParserProcess(int argc, char** argv) {
    char* current_arg;
    ParsedArguments* parser_result = new ParsedArguments;

    for (int i = 1; i < argc; ++i) {
        current_arg = argv[i];
        if (StrCmp(current_arg, kOutputDirArgShort) == 0) {
            parser_result->output_dir = argv[++i];
        } else if (StrNCmp(current_arg, kOutputDirArg, 9) == 0) {
            parser_result->output_dir = current_arg + 9;
        } else if (StrCmp(current_arg, kInputFileArgShort) == 0) {
            parser_result->input_file = argv[++i];
        } else if (StrNCmp(current_arg, kInputFileArg, 8) == 0) {
            parser_result->input_file = current_arg + 8;
        } else if (StrCmp(current_arg, kMaxIterationsArgShort) == 0) {
            ToUInt64Result result = ToUInt64(argv[++i]);
            if (result.is_correct) {
                parser_result->max_iterations = result.value;
            }
        } else if (StrNCmp(current_arg, kMaxIterationsArg, 11) == 0) {
            ToUInt64Result result = ToUInt64(current_arg + 11);
            if (result.is_correct) {
                parser_result->max_iterations = result.value;
            }
        } else if (StrCmp(current_arg, kFrequencyArgShort) == 0) {
            ToUInt64Result result = ToUInt64(argv[++i]);
            if (result.is_correct) {
                parser_result->frequency = result.value;
            }
        } else if (StrNCmp(current_arg, kFrequencyArg, 7) == 0) {
            ToUInt64Result result = ToUInt64(current_arg + 7);
            if (result.is_correct) {
                parser_result->frequency = result.value;
            }
        }
    }
    return parser_result;
}
