#pragma once
#include "struct.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

FieldType detectType(char* str);
Field* makeField(char* line, size_t end, size_t start, size_t indexCol);
void parse(Row* row, char* line);