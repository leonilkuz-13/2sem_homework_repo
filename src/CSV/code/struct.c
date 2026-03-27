#include "struct.h"

Field* initField()
{
    Field* field = malloc(sizeof(Field));
    if (field == NULL) {
        return NULL;
    }
    field->colNum = 0;
    field->field = NULL;
    field->len = 0;
    field->type = TypeNone;

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
    row->error = false;
    return row;
}

void clearRow(Row** row)
{
    if (row == NULL || *row == NULL) {
        return;
    }

    for (size_t index = 0; index < (*row)->fieldCnt; index++) {
        Field* currentField = &(*row)->field[index];
        free(currentField->field);
    }
    free((*row)->field);
    free(*row);
    *row = NULL;
}

Board* initBoard()
{
    Board* board = malloc(sizeof(Board));
    if (board == NULL) {
        return NULL;
    }

    board->maxCol = 0;
    board->rowsCnt = 0;
    board->rows = NULL;

    return board;
}

void clearBoard(Board** board)
{
    if (board == NULL || *board == NULL) {
        return;
    }
    for (size_t index = 0; index < (*board)->rowsCnt; index++) {
        clearRow(&(*board)->rows[index]);
    }
    free((void*)(*board)->rows);
    free(*board);
    *board = NULL;
}
