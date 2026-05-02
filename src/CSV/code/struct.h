#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum { TypeNumber, TypeString, TypeNone } FieldType;

typedef struct {
    // ссылка на string
    char* field;

    // длина поля для итерации (мб будет полезно для выравнивания)
    size_t len;

    // тип
    FieldType type;

    // номер столбца
    size_t colNum;
} Field;

typedef struct {
    // ссылка на поле
    Field* field;

    // номер строки
    size_t num;

    // количество полей -- необходимо для парсинга
    size_t fieldCnt;

    // флаг на ошибку, хранит 0 по умолчанию, если нет ошибки в строке, и 1, если есть
    bool error;
} Row;

typedef struct {
    //  ссылка на строки
    Row** rows;

    // итерация по строкам
    size_t rowsCnt;

    // максимальное колиечество полей, в совокопнуости с type для выравнивания
    size_t maxCol;
} Board;

Field* initField();
Row* initRow(size_t index);
void clearRow(Row** row);
void clearBoard(Board** board);
Board* initBoard();
