#pragma once
#include "struct.h"
#include "parser.h"
char* read(FILE* file);
bool Makeboard(Board* board);
size_t* maxFieldWidth(Board* board);