#include <iostream>
#include <fstream>

#include "str_functions/str_functions.h"
#include "parsing_argv/parsing_argv.h"
#include "resize_matrix/resize_matrix.h"
#include "tsv_parser/tsv_parser.h"
#include "matrix_converter/matrix_converter.h"

bool DoOneIteration(matrix* plane, matrix* plane_iteration) {
    bool is_changed = false;

    int32_t tmp_x_size = plane->x_size;
    int32_t tmp_y_size = plane->y_size;

    for (int32_t i = 0; i < tmp_y_size; ++i) {
        for (int32_t j = 0; j < tmp_x_size; ++j) {
            int32_t original_shifted_x = j + plane->x_index_shift;
            int32_t original_shifted_y = i + plane->y_index_shift;

            int32_t shifted_x = j + plane_iteration->x_index_shift;
            int32_t shifted_y = i + plane_iteration->y_index_shift;

            if (plane->data[original_shifted_x][original_shifted_y] >= 4) {
                is_changed = true;
                plane_iteration->data[shifted_x][shifted_y] +=
                    plane->data[original_shifted_x][original_shifted_y] - 4;

                Add(plane_iteration, static_cast<int64_t>(j) - 1LL, i, 1ULL);
                Add(plane_iteration, static_cast<int64_t>(j) + 1LL, i, 1ULL);
                Add(plane_iteration, j, static_cast<int64_t>(i) - 1LL, 1ULL);
                Add(plane_iteration, j, static_cast<int64_t>(i) + 1LL, 1ULL);

                continue;
            }

            plane_iteration->data[shifted_x][shifted_y] +=
                plane->data[original_shifted_x][original_shifted_y];
        }
    }
    return is_changed;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cerr << "Missing any arguments" << std::endl;
        return -1;
    }

    ParsedArguments* parsed_argv = ParserProcess(argc, argv);

    std::ifstream fin;
    fin.open(parsed_argv->input_file);
    if (!fin.is_open()) {
        std::cerr << "Can't open this file: " <<
            parsed_argv->input_file << std::endl;
        return -1;
    }

    char* tmp = new char[100];
    int32_t y_minimum = 1 << 29;
    int32_t y_maxmimum = -(1 << 29);
    int32_t x_minimum = 1 << 29;
    int32_t x_maxmimum = -(1 << 29);

    while (fin.getline(tmp, 100)) {
        ParsedValue* parsed = SplitString(tmp);
        if (!parsed->is_success) {
            std::cerr << "Bad data in input file: " <<
                parsed_argv->input_file << std::endl;
            return -1;
        }

        if (parsed->x < x_minimum) {
            x_minimum = parsed->x;
        }
        if (parsed->y < y_minimum) {
            y_minimum = parsed->y;
        }
        if (parsed->x > x_maxmimum) {
            x_maxmimum = parsed->x;
        }
        if (parsed->y > y_maxmimum) {
            y_maxmimum = parsed->y;
        }
        delete parsed;
    }

    int32_t size_x = x_maxmimum - x_minimum + 1;
    int32_t size_y = y_maxmimum - y_minimum + 1;

    matrix* plane = InitializateMatrix(size_x, size_y);
    matrix* plane_iteration = InitializateMatrix(size_x, size_y);

    SetShift(plane, x_minimum, y_minimum);

    fin.close();
    fin.open(parsed_argv->input_file);

    while (fin.getline(tmp, 100)) {
        ParsedValue* parsed = SplitString(tmp);
        Insert(plane, parsed->x, parsed->y, parsed->value);

        delete parsed;
    }

    ClearShift(plane);

    delete[] tmp;

    bool do_loop = true;
    uint64_t iteration_count = 0;
    while (do_loop) {
        if (parsed_argv->frequency && 
            iteration_count % parsed_argv->frequency == 0) {
            char* file_name = FromInt(iteration_count);
            CreateBMP(plane, parsed_argv->output_dir, file_name);
            delete[] file_name;
        }

        bool is_changed = DoOneIteration(plane, plane_iteration);

        ClearShift(plane);
        ClearShift(plane_iteration);
        Delete(plane);

        plane = plane_iteration;
        plane_iteration = InitializateMatrix(plane->x_size, plane->y_size);
        iteration_count++;

        if (!is_changed || iteration_count == parsed_argv->max_iterations) {
            do_loop = false;
            if (parsed_argv->frequency == 0) {
                char* file_name = FromInt(iteration_count);
                CreateBMP(plane, parsed_argv->output_dir, file_name);
                delete[] file_name;
            }
            break;
        }
    }

    Delete(plane);
    Delete(plane_iteration);

    delete parsed_argv;

    delete plane;
    delete plane_iteration;
}
