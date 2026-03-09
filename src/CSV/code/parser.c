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
        if (line[pos] == '"') {
            if (pos + 1 < len && line[pos + 1] == '"') {
                pos++;
            } else {
                inQuotes = !inQuotes;
                pos++;
            }
        } else if (line[pos] == ',' && !inQuotes) {
            count++;
        }
    }

    *unbalanced = inQuotes;

    // случай (d,d,d,) я хотел считать в 4 поля. В окончательной версии решил убрать, ориентируясь
    // на популярные библиотеки. (скинутый стандарт вообще ничего про это не говорит)
    /*if (line[len - 1] == ',' && !inQuotes) {
    //    count++;
    }*/

    return count;
}

static bool appendBuffer(char** current, size_t* len, size_t* capacity, char symbol)
{
    if (*len + 1 >= capacity) {
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

static void commitField(Row* row, size_t* idx, char** current, size_t* len, size_t* capacity)
{
    Field* field = &row->field[*idx];

    if (*current != NULL && *len > 0) {
        (*current)[*len] = '\0';
        field->field = *current;
        field->len = *len;
    } else {
        field->field = strdup("");
        field->len = 0;
        *current = NULL;
    }
    field->type = detectType(field->field);
    field->colNum = (*idx)++;
    *capacity = 0;
    *len = 0;
    *current = NULL;
}

static bool cleanBuffer(char** current, Row* row)
{
    if (current) {
        free(*current);
    }
    row->error = true;
    return !row->error;
}

static bool parseFields(Row* row, const char* line, size_t len)
{
    char* current = NULL;
    size_t lenCurrent = 0;
    size_t capacityCurrent = 0;
    size_t idx = 0;
    bool insideQuoted = false;
    bool success = true;

    for (size_t pos = 0; pos <= len; pos++) {
        char symbol = (pos < len) ? line[pos] : '\0';
        if (insideQuoted) {
            if (symbol == '"') {
                if (pos + 1 < len && line[pos + 1] == '"') {
                    if (!appendBuffer(&current, &lenCurrent, &capacityCurrent, '"')) {
                        success = false;
                        return cleanBuffer(&current, row);
                    }
                    pos++;
                    continue;
                } else {
                    insideQuoted = false;
                    commitField(row, &idx, &current, &lenCurrent, &capacityCurrent);
                    continue;
                }
            } else {
                if (!appendBuffer(&current, &lenCurrent, &capacityCurrent, symbol)) {
                    success = false;
                    return cleanBuffer(&current, row);
                }
            }
        } else {
            if (symbol == '"') {
                insideQuoted = true;
                continue;
            }

            if (symbol == ',' || symbol == '\0') {
                commitField(row, &idx, &current, &lenCurrent, &capacityCurrent);

                if (symbol == '\0') {
                    break;
                }
                continue;
            }
            if (!appendBuffer(&current, &lenCurrent, &capacityCurrent, symbol)) {
                success = false;
                cleanBuffer(&current, row);
            }
        }
    }

    if (success && (lenCurrent > 0 || idx == 0)) {
        commitField(row, &idx, &current, &lenCurrent, &capacityCurrent);
    }

    if (insideQuoted) {
        success = false;
        row->error = true;
    }

    row->fieldCnt = idx;
    return success;
}

bool parse(Row* row, char* line)
{
    size_t len = strlen(line);
    if (len == 0) {
        row->fieldCnt = 0;
        row->field = NULL;
        return true;
    }
    bool unbalanced;
    size_t fieldCnt = countFields(line, len, &unbalanced);
    if (unbalanced) {
        row->error = true;
        return false;
    }
    if (fieldCnt == 0) {
        row->field = NULL;
        row->fieldCnt = 0;
        return true;
    }
    row->field = malloc(sizeof(Field) * fieldCnt);
    if (!row->field) {
        return false;
    }
    row->fieldCnt = fieldCnt;
    return parseFields(row, line, len);
}