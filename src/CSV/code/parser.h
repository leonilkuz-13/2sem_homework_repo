#pragma once
#include "struct.h"
#include <ctype.h>
#include <string.h>

FieldType detectType(char* str);
bool parse(Row* row, char* line);