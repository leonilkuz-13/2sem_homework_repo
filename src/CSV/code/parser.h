#pragma once

#include "struct.h"

// определения типа строки
FieldType detectType(char* str);

// сам парсинг
bool parse(Row** row, char* line);