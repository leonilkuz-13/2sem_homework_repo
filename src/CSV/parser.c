#include "parser.h"

static size_t countFields(const char* line, size_t len, bool* unbalanced)
{
    size_t count = 0;
    bool inQuotes = false;
    for (size_t pos = 0; pos <= len; ++pos) {
        if (line[pos] == '"') {
            if (!inQuotes) {
                inQuotes = true;
            } else {
                if (pos + 1 <= len && line[pos + 1] == '"') {
                    ++pos;
                } else {
                    inQuotes = false;
                }
            }
        } else if ((line[pos] == ',' && !inQuotes) || line[pos] == '\0') {
            ++count;
        }
    }
    *unbalanced = inQuotes;
    return count;
}

static bool handleQuote(const char* line, size_t pos, size_t len, bool* insideQuotes, size_t* start,
                        size_t* end, bool* hasEnd)
{
    if (!*insideQuotes) {
        *insideQuotes = true;
        *start = pos + 1;
        return false;
    }
    if (pos + 1 <= len && line[pos + 1] == '"') {
        return true;
    }
    *insideQuotes = false;
    *end = pos;
    *hasEnd = true;
    return false;
}

static bool parseFields(Row* row, const char* line, size_t fieldCnt, size_t len) // NOLINT
{
    (void)fieldCnt; // параметр не используется, но сохранён для совместимости

    size_t fieldInd = 0;
    size_t start = 0;
    size_t end = 0;
    bool hasEnd = false;
    bool insideQuotes = false;

    for (size_t pos = 0; pos <= len; ++pos) {
        if (line[pos] == '"') {
            if (handleQuote(line, pos, len, &insideQuotes, &start, &end, &hasEnd)) {
                ++pos;
            }
        } else if ((line[pos] == ',' && !insideQuotes) || line[pos] == '\0') {
            size_t endSymbol = hasEnd ? end : pos;

            Field* field = makeField((char*)line, start, endSymbol, fieldInd);
            if (field == NULL) {
                for (size_t idx = 0; idx < fieldInd; ++idx) {
                    free(row->field[idx].field);
                }
                free(row->field);
                return false;
            }
            row->field[fieldInd] = *field;
            free(field);
            ++fieldInd;
            start = pos + 1;
            hasEnd = false;
        }
    }
    return true;
}

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
        ++ptr;
    }
    return (*ptr == '\0') ? TypeNumber : TypeString;
}

Field* makeField(char* line, size_t start, size_t end, size_t fieldInd)
{
    Field* field = initField();
    if (!field) {
        return NULL;
    }
    size_t len = end - start;
    if (len == 0) {
        field->type = TypeNone;
        field->colNum = fieldInd;
        return field;
    }
    char* fieldStr = malloc(len + 1);
    if (!fieldStr) {
        free(field);
        return NULL;
    }
    memcpy(fieldStr, line + start, len);
    fieldStr[len] = '\0';
    field->field = fieldStr;
    field->colNum = fieldInd;
    field->len = len;
    field->type = detectType(fieldStr);
    return field;
}

bool parse(Row* row, char* line)
{
    size_t len = strlen(line);
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
    return parseFields(row, line, fieldCnt, len);
}