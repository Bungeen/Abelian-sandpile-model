#include "matrix_converter.h"


char* CreateFullPath(char* path, char* file_name) {
    int32_t path_lenght = StrLen(path) - 1;
    int32_t file_name_lenght = StrLen(file_name) - 1;

    char* result = new char[path_lenght + file_name_lenght + kSubChars];
    for (int i = 0; i < path_lenght; ++i) {
        result[i] = path[i];
    }

    result[path_lenght] = '/';
    for (int i = 0; i < file_name_lenght; ++i) {
        result[i + 1 + path_lenght] = file_name[i];
    }

    result[path_lenght + file_name_lenght + 1] = '.';
    result[path_lenght + file_name_lenght + 2] = 'b';
    result[path_lenght + file_name_lenght + 3] = 'm';
    result[path_lenght + file_name_lenght + 4] = 'p';
    result[path_lenght + file_name_lenght + 5] = '\0';

    return result;
}

int GetColor(int value) {
    if (value < 4) {
        return value;
    }

    return 4;
}


void CreateBMP(matrix* plane, char* path, char* file_name) {
    std::ofstream fout;
    char* full_path = CreateFullPath(path, file_name);

    std::ifstream fin;
    fin.open(full_path);
    if (fin.is_open()){
        std::cerr << "Folder not empty. File: " << full_path << " exists\n";
        delete[] full_path;
        return;
    }
    fin.close();

    fout.open(full_path, std::ios::out | std::ios::binary);
    if (!fout.is_open()){
        std::cerr << "Invalid path\n";
        delete[] full_path;
        return;
    }

    int64_t real_resolution_widght = static_cast<int64_t>(plane->x_size) /
            2LL + static_cast<int64_t>(plane->x_size) % 2LL;
    int64_t padding_byets = (4 - real_resolution_widght % 4) % 4;
    int64_t pixels_size = (real_resolution_widght + padding_byets) *
                            static_cast<int64_t>(plane->y_size);
    int64_t file_size = pixels_size + kFileHeaderSize +
                kInformationHeaderSize + kColorTableSize;

    unsigned char file_header[kFileHeaderSize];

    // File Type
    file_header[0] = 'B';
    file_header[1] = 'M';

    // File size
    file_header[2] = file_size;
    file_header[3] = file_size >> 8;
    file_header[4] = file_size >> 16;
    file_header[5] = file_size >> 24;
    file_header[6] = file_size >> 32;
    file_header[7] = file_size >> 40;
    file_header[8] = file_size >> 48;
    file_header[9] = file_size >> 56;

    // Headers size
    file_header[10] = kFileHeaderSize + kInformationHeaderSize;
    file_header[11] = 0;
    file_header[12] = 0;
    file_header[13] = 0;

    unsigned char information_header[kInformationHeaderSize];

    // Size of information header (BITMAPINFOHEADER)
    information_header[0] = kInformationHeaderSize;
    information_header[1] = 0;
    information_header[2] = 0;
    information_header[3] = 0;

    // Width
    information_header[4] = plane->x_size;
    information_header[5] = plane->x_size >> 8;
    information_header[6] = plane->x_size >> 16;
    information_header[7] = plane->x_size >> 24;

    // Height
    information_header[8] = plane->y_size;
    information_header[9] = plane->y_size >> 8;
    information_header[10] = plane->y_size >> 16;
    information_header[11] = plane->y_size >> 24;

    // Planes (Always 1)
    information_header[12] = 1;
    information_header[13] = 0;

    // Bits per pixel
    information_header[14] = 4;
    information_header[15] = 0;

    // Compression method (0 - None)
    information_header[16] = 0;
    information_header[17] = 0;
    information_header[18] = 0;
    information_header[19] = 0;

    // Size of the raw bitmap data (Can be 0)
    information_header[20] = 0;
    information_header[21] = 0;
    information_header[22] = 0;
    information_header[23] = 0;

    // Horizontal resolution
    information_header[24] = 0;
    information_header[25] = 0;
    information_header[26] = 0;
    information_header[27] = 0;

    // Vertical resolution
    information_header[28] = 0;
    information_header[29] = 0;
    information_header[30] = 0;
    information_header[31] = 0;

    // Color palette size
    information_header[32] = 5;
    information_header[33] = 0;
    information_header[34] = 0;
    information_header[35] = 0;

    // Important colors (All important)
    information_header[36] = 0;
    information_header[37] = 0;
    information_header[38] = 0;
    information_header[39] = 0;

    unsigned char color_table[kColorTableSize];

    // White
    color_table[0] = 255;
    color_table[1] = 255;
    color_table[2] = 255;
    color_table[3] = 0;

    // Green
    color_table[4] = 0;
    color_table[5] = 255;
    color_table[6] = 0;
    color_table[7] = 0;

    // Purpur
    color_table[8] = 128;
    color_table[9] = 0;
    color_table[10] = 128;
    color_table[11] = 0;

    // Yellow
    color_table[12] = 0;
    color_table[13] = 255;
    color_table[14] = 255;
    color_table[15] = 0;

    // Black
    color_table[16] = 0;
    color_table[17] = 0;
    color_table[18] = 0;
    color_table[19] = 0;

    unsigned char* pixels_storage = new unsigned char[pixels_size];
    for (int32_t i = 0; i < plane->y_size; ++i) {
        int tmp_pixels_pair = 0b00000000;
        for (int32_t j = 0; j < plane->x_size; ++j) {
            if (j % 2 != 0 && j != 0) {
                int32_t index = i * (real_resolution_widght + padding_byets) +
                    j / 2;

                tmp_pixels_pair <<= 4;
                tmp_pixels_pair += GetColor(plane->data[j][i]);
                pixels_storage[index] = tmp_pixels_pair;
                tmp_pixels_pair = 0b00000000;

                continue;
            }

            tmp_pixels_pair += GetColor(plane->data[j][i]);
        }

        if (plane->x_size % 2 != 0) {
            tmp_pixels_pair <<= 4;
            int32_t index = i * (real_resolution_widght + padding_byets) +
                plane->x_size / 2;
            pixels_storage[index] = tmp_pixels_pair;
        }

        for (int t = 0; t < padding_byets; ++t) {
            int32_t index = i * (real_resolution_widght + padding_byets) +
                real_resolution_widght + t;
            pixels_storage[index] = 0b00000000;
        }
    }

    fout.write(reinterpret_cast<char*>(file_header), kFileHeaderSize);
    fout.write(reinterpret_cast<char*>(information_header), 
                                                kInformationHeaderSize);                                           
    fout.write(reinterpret_cast<char*>(color_table), kColorTableSize);
    fout.write(reinterpret_cast<char*>(pixels_storage), pixels_size);

    delete[] pixels_storage;
    delete[] full_path;
}
