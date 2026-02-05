#include "resize_matrix.h"


uint64_t* InitializateNewColumn(int32_t size) {
    uint64_t* result_column = new uint64_t[size];
    for (int32_t i = 0; i < size; ++i) {
        result_column[i] = 0;
    }

    return result_column;
}

matrix* InitializateMatrix(int32_t x_size, int32_t y_size) {
    matrix* result = new matrix;

    result->x_size = x_size;
    result->y_size = y_size;

    uint64_t** tmp_data = new uint64_t*[result->x_size];
    for (int32_t i = 0; i < x_size; ++i) {
        tmp_data[i] = InitializateNewColumn(result->y_size);
    }

    result->data = tmp_data;

    return result;
}

void SetShift(matrix* current_matrix, int32_t x, int32_t y) {
    if (x < 0) {
        current_matrix->x_index_shift = std::abs(x);
    } else {
        current_matrix->x_index_shift = -std::abs(x);
    }

    if (y < 0) {
        current_matrix->y_index_shift = std::abs(y);
    } else {
        current_matrix->y_index_shift = -std::abs(y);
    }
}

void ClearShift(matrix* current_matrix) {
    current_matrix->x_index_shift = 0;
    current_matrix->y_index_shift = 0;
}

void Insert(matrix* current_matrix, int32_t x, int32_t y, uint64_t value) {
    int32_t index_x = x + current_matrix->x_index_shift;
    int32_t index_y = y + current_matrix->y_index_shift;
    current_matrix->data[index_x][index_y] = value;
}

void Add(matrix* current_matrix, int64_t x, int64_t y, uint64_t value) {
    if (x + current_matrix->x_index_shift == -1 ||
        x + current_matrix->x_index_shift == current_matrix->x_size) {
        ResizeMatrix(current_matrix,
            static_cast<int64_t>(current_matrix->x_size) +
            static_cast<int64_t>(current_matrix->x_size % 2), 0);
    }

    if (y + current_matrix->y_index_shift == -1 ||
        y + current_matrix->y_index_shift == current_matrix->y_size) {
        ResizeMatrix(current_matrix, 0,
            static_cast<int64_t>(current_matrix->y_size) +
            static_cast<int64_t>(current_matrix->y_size % 2));   
    }

    if (y + static_cast<int64_t>(current_matrix->y_index_shift) < 0 ||
        y + static_cast<int64_t>(current_matrix->y_index_shift) >=
            current_matrix->y_size ||
        x + static_cast<int64_t>(current_matrix->x_index_shift) < 0 ||
        x + static_cast<int64_t>(current_matrix->x_index_shift) >
            current_matrix->x_size) {
        return;
    }

    int32_t index_x = x + current_matrix->x_index_shift;
    int32_t index_y = y + current_matrix->y_index_shift;

    current_matrix->data[index_x][index_y] += value;
}

void ResizeMatrix(matrix* current_matrix, int64_t add_x_size, 
                                            int64_t add_y_size) {
    if (kMaxInt32 - current_matrix->x_size < add_x_size) {
        add_x_size = kMaxInt32 - current_matrix->x_size;
        add_x_size -= add_x_size % 2;
    }

    if (kMaxInt32 - current_matrix->y_size < add_y_size) {
        add_y_size = kMaxInt32 - current_matrix->y_size;
        add_y_size -= add_y_size % 2;
    }

    uint64_t** tmp_data = new uint64_t*[current_matrix->x_size + add_x_size];

    int32_t new_y_size = current_matrix->y_size + add_y_size;
    int32_t new_x_size = add_x_size + current_matrix->x_size;

    for (int32_t i = 0; i < add_x_size / 2; ++i) {
        tmp_data[i] = InitializateNewColumn(new_y_size);
    }

    int32_t middle_end = add_x_size / 2 + current_matrix->x_size;

    for (int32_t i = add_x_size / 2; i < middle_end; ++i) {
        if (add_y_size == 0) {
            tmp_data[i] = current_matrix->data[i - add_x_size / 2];
            continue;
        }

        tmp_data[i] = InitializateNewColumn(new_y_size);
        int32_t middle_end_y = add_y_size / 2 + current_matrix->y_size;
        for (int32_t j = add_y_size / 2; j < middle_end_y; ++j) {
            int32_t new_index_x = i - add_x_size / 2;
            int32_t new_index_y = j - add_y_size / 2;
            tmp_data[i][j] = current_matrix->data[new_index_x][new_index_y];
        }
    }

    for (int32_t i = middle_end; i < new_x_size; ++i) {
        tmp_data[i] = InitializateNewColumn(new_y_size);
    }

    if (add_y_size != 0) {
        for (int32_t i = 0; i < current_matrix->x_size; ++i) {
            delete[] current_matrix->data[i];
        }
    }

    delete[] current_matrix->data;

    current_matrix->data = tmp_data;
    current_matrix->x_index_shift += add_x_size / 2;
    current_matrix->y_index_shift += add_y_size / 2;
    current_matrix->x_size += add_x_size;
    current_matrix->y_size += add_y_size;
}

void Delete(matrix* current_matrix) {
    for (int32_t i = 0; i < current_matrix->x_size; ++i) {
        delete[] current_matrix->data[i];
    }

    delete[] current_matrix->data;
}
