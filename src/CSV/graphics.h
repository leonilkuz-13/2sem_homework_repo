#pragma once
#include "tools.h"
bool ordinaryLine(FILE* file, size_t* widths, Board* board, char symbol);
bool writeField(FILE* file, Field* field, size_t col);
void graphics(Board* board);