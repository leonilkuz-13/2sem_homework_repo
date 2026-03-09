#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>

bool ordinaryLine(FILE* file, const size_t* widths, Board* board, char symbol)
{
    if (widths == NULL || board == NULL || (symbol != '-' && symbol != '=')) {
        return false;
    }
    (void)fputc('+', file);
    for (size_t index = 0; index < board->maxCol; index++) {
        for (size_t ind = 0; ind < widths[index]; ind++) {
            (void)fputc(symbol, file);
        }
        (void)fputc('+', file);
    }
    (void)fputc('\n', file);
    return true;
}

bool writeField(FILE* file, Field* field, size_t col)
{
    if (file == NULL) {
        return false;
    }
    (void)fputc('|', file);
    if (field == NULL) {
        for (size_t i = 0; i < col; i++) {
            (void)fputc(' ', file);
        }
        (void)fputc('|', file);
        return true;
    }
    size_t len = field->len;
    if (field->type == TypeNumber) {
        size_t padding = col - len;
        for (size_t index = 0; index < col; index++) {
            if (index < padding) {
                (void)fputc(' ', file);
            } else {
                (void)fputc(field->field[index - padding], file);
            }
        }
    } else {
        for (size_t index = 0; index < len; index++) {
            (void)fputc(field->field[index], file);
        }
        for (size_t index = len; index < col; index++) {
            (void)fputc(' ', file);
        }
    }
    (void)fputc('|', file);
    return true;
}

void graphics(Board* board)
{
    size_t* widths = maxFieldWidth(board);
    if (widths == NULL) {
        clearBoard(&board);
        return;
    }
    FILE* file = fopen("output.txt", "w");
    if (file == NULL) {
        free(widths);
        return;
    }
    if (!ordinaryLine(file, widths, board, '=')) {
        (void)fclose(file);
        free(widths);
        return;
    }
    for (size_t index = 0; index < board->rowsCnt; index++) {
        Row* row = board->rows[index];
        for (size_t col = 0; col < board->maxCol; col++) {
            Field* found = NULL;
            for (size_t ind = 0; ind < row->fieldCnt; ind++) {
                if (row->field[ind].colNum == col) {
                    found = &row->field[ind];
                    break;
                }
            }
            if (!writeField(file, found, widths[col])) {
                (void)fclose(file);
                free(widths);
                return;
            }
        }
        (void)fputc('\n', file);
    }
    if (!ordinaryLine(file, widths, board, '-')) {
        (void)fclose(file);
        free(widths);
        return;
    }
    (void)fclose(file);
    free(widths);
}