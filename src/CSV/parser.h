#pragma once
#include "struct.h"
#include <ctype.h>
#include <string.h>

FieldType detectType(char* str);
Field* makeField(char* line, size_t start, size_t end, size_t fieldInd);
static size_t countFields(const char* line, size_t len, bool* unbalanced);
static bool parseFields(Row* row, const char* line, size_t len, size_t fieldCnt);
bool parse(Row* row, char* line);