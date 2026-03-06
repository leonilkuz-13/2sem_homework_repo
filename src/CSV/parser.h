#pragma once
#include "struct.h"
#include <ctype.h>
#include <string.h>

FieldType detectType(char* str);
Field* makeField(char* line, size_t end, size_t start, size_t indexCol);
bool parse(Row* row, char* line);