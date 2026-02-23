#pragma once
#include "struct.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

FieldType detectType(char* str);
Field* makeField(char* line, size_t ind, size_t lenField, size_t indexCol);
void parse(Row* row, char* line);