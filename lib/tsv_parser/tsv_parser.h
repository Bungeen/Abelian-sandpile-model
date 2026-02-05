#pragma once

#include <cstdint>

#include "str_functions\str_functions.h"


struct ParsedValue {
    bool is_success = true;
    int32_t x;
    int32_t y;
    uint64_t value;
};

ParsedValue* SplitString(char* str);