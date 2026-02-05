#include "tsv_parser.h"


ParsedValue* SplitString(char* str) {
    ParsedValue* result = new ParsedValue;
    char* start = str;
    while (*str != '\t') {
        str++;
    }

    *str = '\0';
    result->x = ToInt32(start);
    str++;
    start = str;
    while (*str != '\t') {
        str++;
    }

    *str = '\0';
    result->y = ToInt32(start);
    str++;

    ToUInt64Result converted_value = ToUInt64(str);
    if (!converted_value.is_correct) {
        result->is_success = false;
        return result;
    }

    result->value = converted_value.value;

    if (result->x > kMaxInt16 || result->x < -kMaxInt16
        || result->y > kMaxInt16 || result->y < -kMaxInt16) {
        result->is_success = false;
    }
    
    return result;
}
