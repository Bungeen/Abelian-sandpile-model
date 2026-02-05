#pragma once

#include <cstdint>

const int16_t kMaxInt16 = 32767;
const int32_t kMaxInt32 = 2147483647;
const uint64_t kMaxUInt64 = 18446744073709551615;

struct ToUInt64Result {
    bool is_correct = true;
    uint64_t value = 0;
};

int StrCmp(const char* s1, const char* s2);
int StrNCmp(const char* s1, const char* s2, int n);
int StrLen(char* str);

char* FromInt(uint64_t value);

int32_t ToInt32(char* str);
ToUInt64Result ToUInt64(char* str);