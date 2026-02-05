#include "str_functions.h"


int StrLen(char* str) {
    int size = 0;
    while (*str != '\0') {
        str++;
        size++;
    }

    return size + 1;
}

int StrNCmp(const char* s1, const char* s2, int n) {
    while (*s1 && (n > 0)) {
        if (*s1 != *s2) {
            return -1;
        }
        n--;
        s1++;
        s2++;
    }

    return 0;
}

int StrCmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    return (*s1 > *s2) - (*s2 > *s1);
}

char* FromInt(uint64_t value) {
    char tmp_result[21];
    tmp_result[20] = '\0';
    int i = 19;
    if (!value) {
        tmp_result[i] = '0';
        --i;
    }

    while (value) {
        tmp_result[i] = '0' + value % 10;
        value /= 10;
        --i;
    }

    ++i;

    char* result = new char[21 - i];
    for (int j = i; j < 21; ++j) {
        result[j - i] = tmp_result[j];
    }

    return result;
}

int32_t ToInt32(char* str) {
    int32_t result = 0;
    int str_len = StrLen(str) - 1;
    bool is_negative = false;
    for (int i = 0; i < str_len; ++i) {
        if (str[i] - '0' >= 0 && str[i] - '0' <= 9) {
            if (kMaxInt32 / 10 < result ||
                kMaxInt32 - result * 10 < str[i] - '0') {
                return kMaxInt32;
            }

            result *= 10;
            result += str[i] - '0';

            continue;
        }

        if (i == 0 && str[i] == '-') {
            is_negative = true;
            continue;
        }

        return kMaxInt32;
    }

    if (is_negative) {
        result *= -1;
    }

    return result;
}

ToUInt64Result ToUInt64(char* str) {
    ToUInt64Result result;
    int str_len = StrLen(str) - 1;
    for (int i = 0; i < str_len; ++i) {
        if (str[i] - '0' >= 0 && str[i] - '0' <= 9) {
            if (kMaxUInt64 / 10ULL < result.value ||
                kMaxUInt64 - result.value * 10ULL <
                    str[i] - '0') {
                result.is_correct = false;
                return result;
            }

            result.value *= 10ULL;
            result.value += str[i] - '0';

            continue;
        }

        result.is_correct = false;

        return result;
    }

    return result;
}
