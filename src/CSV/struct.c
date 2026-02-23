#include "struct.h"

Field* initField()
{
    Field* field = malloc(sizeof(Field));
    if (field == NULL) {
        return NULL;
    }
    field->colNum = -1;
    field->field = NULL;
    field->len = 0;
    field->type = TYPE_NONE;

    return field;
}

Row* initRow(size_t index)
{
    Row* row = malloc(sizeof(Row));
    if (row == NULL) {
        return NULL;
    }

    row->field = NULL;
    row->fieldCnt = 0;
    row->num = index;
    row->error = true;
    return row;
}

void clearRow(Row* row)
{
    while (row->fieldCnt != 0) {
        size_t len = row->fieldCnt;
        for (size_t index = 0; index < len; index++) {
            if (index != row->field->colNum) {
                continue;
            } else {
                free(row->field);
                row->fieldCnt--;
                break;
            }
        }
    }
    free(row);
}
