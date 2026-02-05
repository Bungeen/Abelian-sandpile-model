#pragma once

#include <fstream>
#include <iostream>

#include "resize_matrix\resize_matrix.h"
#include "str_functions\str_functions.h"

const int kFileHeaderSize = 14;
const int kInformationHeaderSize = 40;
const int kColorTableSize = 20;
const int kSubChars = 6;

void CreateBMP(matrix* plane, char* path, char* file_name);
