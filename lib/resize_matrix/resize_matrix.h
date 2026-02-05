#pragma once

#include <cstdint>
#include <iostream>

#include "str_functions\str_functions.h"

struct matrix {
    uint64_t** data;  // Array of columns
    int32_t x_size = 0;
    int32_t y_size = 0;
    int32_t x_index_shift = 0;
    int32_t y_index_shift = 0;
};

matrix* InitializateMatrix(int32_t x_size, int32_t y_size);

void SetShift(matrix* current_matrix, int32_t x, int32_t y);

void ClearShift(matrix* current_matrix);

void Insert(matrix* current_matrix, int32_t x, int32_t y, uint64_t value);

void Add(matrix* current_matrix, int64_t x, int64_t y, uint64_t value);

void ResizeMatrix(matrix* current_matrix, int64_t add_x_size, int64_t add_y_size);

void Delete(matrix* current_matrix);
