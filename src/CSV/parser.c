#include "parser.h"

FieldType detectType(char* str)
{
    if (str == NULL || *str == '\0') {
        return TypeNone;
    }
    char* ptr;
    strtod(str, &ptr);
    if (ptr == str) {
        return TypeString;
    }

    while (isspace((unsigned char)*ptr)) {
        ptr++;
    }

    return (*ptr == '\0') ? TypeNumber : TypeString;
}

Field* makeField(char* line, size_t end, size_t start, size_t fieldInd)
{
    Field* field = initField();
    if (field == NULL) {
        return NULL;
    }
    size_t len = end - start;
    if (len == 0) {
        field->type = TypeNone;
        field->colNum = fieldInd;
        return field;
    }

    char* fieldStr = malloc(sizeof(char) * (len + 1));
    if (fieldStr == NULL) {
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

//
void parse(Row* row, char* line)
{
    size_t len = strlen(line);
    size_t fieldCnt = 0;
    bool flag = false;
    // проход len включительно не выдаст ошибку, поскольку indCntField+1 не может быть на позиции
    // line[len] (для себя)
    for (size_t indCntField = 0; indCntField <= len; indCntField++) {
        if (line[indCntField] == '"') {
            if (flag == false) {
                flag = true;
            } else {
                if (line[indCntField + 1] == '"') {
                    indCntField++;
                } else {
                    flag = false;
                }
            }
        } else if ((line[indCntField] == ',' && flag == false) || line[indCntField] == '\0') {
            fieldCnt++;
        }
    }

    // вкидываем ошибку в строку и не делаем парсинг
    if (flag == true) {
        row->error = true;
        return;
    }

    row->field = malloc(sizeof(Field) * fieldCnt);
    if (row->field == NULL) {
        return;
    }
    row->fieldCnt = fieldCnt;

    size_t fieldInd = 0;
    size_t start = 0;
    int end = -1;
    bool inside = 0;
    for (size_t index = 0; index <= len; index++) {
        if (line[index] == '"') {
            if (inside == false) {
                inside = true;
                start = index + 1;
            } else {
                if (line[index + 1] == '"') {
                    index++;
                } else {
                    inside = false;
                    end = index;
                }
            }
        } else if ((line[index] == ',' && inside == false) || line[index] == '\0') {
            size_t endSymbol;
            if (end != -1) {
                endSymbol = end;
                end = -1;
            } else {
                endSymbol = index;
            }

            Field* field = makeField(line, endSymbol, start, fieldInd);
            if (field == NULL) {
                clearRow(row);
                return;
            }
            row->field[fieldInd] = *field;
            free(field);
            fieldInd++;
            start = index + 1;
        }
    }
}