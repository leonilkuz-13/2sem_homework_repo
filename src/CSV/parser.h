#pragma once
#include "struct.h"
#include <ctype.h>
#include <string.h>

static size_t countFields(const char* line, size_t len, bool* unbalanced);
static bool handleQuote(const char* line, size_t pos, size_t len, bool* insideQuotes, size_t* start,
                        size_t* end, bool* hasEnd);
static bool parseFields(Row* row, const char* line, size_t fieldCnt, size_t len);
FieldType detectType(char* str);
Field* makeField(char* line, size_t start, size_t end, size_t fieldInd);
bool parse(Row* row, char* line);