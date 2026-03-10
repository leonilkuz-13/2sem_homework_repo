#pragma once
#include "tools.h"
bool ordinaryLine(FILE* file, const size_t* widths, Board* board, char symbol);
bool writeField(FILE* file, Field* field, size_t width);
void graphics(Board* board);
