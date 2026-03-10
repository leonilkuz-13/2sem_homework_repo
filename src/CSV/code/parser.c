#include "parser.h"

FieldType detectType(char* str)
{
    if (str == NULL || *str == '\0') {
        return TypeNone;
    }
    char* ptr;
    (void)strtod(str, &ptr);
    if (ptr == str) {
        return TypeString;
    }
    while (isspace((unsigned char)*ptr)) {
        ptr++;
    }
    return (*ptr == '\0') ? TypeNumber : TypeString;
}

static size_t countFields(const char* line, size_t len, bool* unbalanced)
{
    if (len == 0) {
        *unbalanced = false;
        return 0;
    }

    size_t count = 1;
    bool inQuotes = false;
    *unbalanced = false;
    for (size_t pos = 0; pos < len; pos++) {
        if (inQuotes) {
            if (line[pos] == '"') {
                if (pos + 1 < len && line[pos + 1] == '"') {
                    pos++;
                } else {
                    inQuotes = false;
                }
            }
        } else {
            if (line[pos] == '"') {
                inQuotes = true;
            } else if (line[pos] == ',') {
                count++;
            }
        }
    }

    *unbalanced = inQuotes;
    return count;
}

static bool appendBuffer(char** current, size_t* len, char symbol, size_t* capacity)
{
    if (*len + 1 >= *capacity) {
        *capacity = (*capacity == 0) ? 16 : *capacity * 2;
        char* newCurrent = realloc(*current, *capacity);
        if (newCurrent == NULL) {
            free(*current);
            *current = NULL;
            *len = 0;
            *capacity = 0;
            return false;
        }
        *current = newCurrent;
    }
    (*current)[*len] = symbol;
    (*len)++;
    return true;
}

static bool cleanBuffer(char** current, size_t* len, Row* row, size_t* capacity)
{
    if (*current) {
        free(*current);
        *current = NULL;
    }
    *len = 0;
    *capacity = 0;
    row->error = true;
    return false;
}

static bool commitField(size_t* idx, char** current, size_t* len, Row* row, size_t* capacity)
{
    Field* field = &row->field[*idx];

    if (*current != NULL && *len > 0) {
        if (*len + 1 > *capacity) {
            *capacity = (*capacity == 0) ? 16 : *capacity * 2;
            char* ptr = realloc(*current, *capacity);
            if (ptr == NULL) {
                return cleanBuffer(current, len, row, capacity);
            }
            *current = ptr;
        }
        (*current)[*len] = '\0';
        field->field = *current;
        field->len = *len;
    } else {
        if (*current) {
            free(*current);
        }
        field->field = strdup("");
        if (field->field == NULL) {
            return cleanBuffer(current, len, row, capacity);
        }
        field->len = 0;
    }
    field->type = detectType(field->field);
    field->colNum = (*idx)++;
    *capacity = 0;
    *len = 0;
    *current = NULL;
    return true;
}

static bool handleQuotedChar(char symbol, const char* line, size_t len, size_t* pos, char** current,
                             size_t* lenCurrent, Row* row, size_t* capacityCurrent,
                             bool* insideQuoted)
{
    if (symbol == '"') {
        if (*pos + 1 < len && line[*pos + 1] == '"') {
            if (!appendBuffer(current, lenCurrent, '"', capacityCurrent)) {
                return cleanBuffer(current, lenCurrent, row, capacityCurrent);
            }
            (*pos)++;
        } else {
            *insideQuoted = false;
            if (*pos + 1 < len && line[*pos + 1] != ',' && line[*pos + 1] != '\0') {
                return cleanBuffer(current, lenCurrent, row, capacityCurrent);
            }
        }
    } else if (symbol == '\0') {
        return cleanBuffer(current, lenCurrent, row, capacityCurrent);
    } else {
        if (!appendBuffer(current, lenCurrent, symbol, capacityCurrent)) {
            return cleanBuffer(current, lenCurrent, row, capacityCurrent);
        }
    }
    return true;
}

static bool handleUnquotedChar(char symbol, Row* row, size_t* idx, char** current,
                               size_t* lenCurrent, bool* insideQuoted, size_t* capacityCurrent)
{
    if (symbol == '"') {
        if (*lenCurrent > 0) {
            return cleanBuffer(current, lenCurrent, row, capacityCurrent);
        }
        *insideQuoted = true;
    } else if (symbol == ',' || symbol == '\0') {
        if (!commitField(idx, current, lenCurrent, row, capacityCurrent)) {
            return false;
        }
    } else {
        if (!appendBuffer(current, lenCurrent, symbol, capacityCurrent)) {
            return cleanBuffer(current, lenCurrent, row, capacityCurrent);
        }
    }
    return true;
}

static bool parseFields(Row* row, const char* line, size_t len)
{
    char* current = NULL;
    size_t lenCurrent = 0;
    size_t capacityCurrent = 0;
    size_t idx = 0;
    bool insideQuoted = false;

    for (size_t pos = 0; pos <= len; pos++) {
        char symbol = (char)((pos < len) ? line[pos] : '\0');

        if (insideQuoted) {
            if (!handleQuotedChar(symbol, line, len, &pos, &current, &lenCurrent, row,
                                  &capacityCurrent, &insideQuoted)) {
                return false;
            }
        } else {
            if (!handleUnquotedChar(symbol, row, &idx, &current, &lenCurrent, &insideQuoted,
                                    &capacityCurrent)) {
                return false;
            }
        }
    }

    if (insideQuoted) {
        return cleanBuffer(&current, &lenCurrent, row, &capacityCurrent);
    }

    return true;
}

bool parse(Row** row, char* line)
{
    if (row == NULL) {
        return false;
    }
    Row* ptr = *row;

    size_t len = strlen(line);
    if (len == 0) {
        ptr->fieldCnt = 0;
        ptr->field = NULL;
        return true;
    }

    bool unbalanced;
    size_t fieldCnt = countFields(line, len, &unbalanced);
    if (unbalanced) {
        return false;
    }

    ptr->field = (Field*)malloc(fieldCnt * sizeof(Field));
    if (ptr->field == NULL) {
        return false;
    }
    ptr->fieldCnt = 0;
    bool success = parseFields(ptr, line, len);
    if (!success) {
        clearRow(row);
        return false;
    }
    ptr->fieldCnt = fieldCnt;
    return true;
}