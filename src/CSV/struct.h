#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum { TypeNumber, TypeString, TypeNone } FieldType;

typedef struct {
    char* field; // ссылка на string
    size_t len; // длина поля для итерации (мб будет полезно для выравнивания)
    FieldType type; // тип
    size_t colNum;  // номер столбца
} Field;

typedef struct {
    Field* field;    // ссылка на поле
    size_t num;      // номер строки
    size_t fieldCnt; // количество полей -- необходимо для парсинга
    bool error; // флаг на ошибку, хранит 0 по умолчанию, если нет ошибки в строке, и 1, если есть
} Row;

typedef struct {
    Row** rows;     //  ссылка на строки
    size_t rowsCnt; // итерация по строкам
    size_t maxCol; // максимальное колиечество полей, в совокопнуости с type для выравнивания
} Board;

Field* initField();
Row* initRow(size_t index);
void clearRow(Row** row);
void clearBoard(Board** board);
