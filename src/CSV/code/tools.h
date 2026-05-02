#pragma once

#include "struct.h"

// Чтение файла
char* read(FILE* file);

// заполняет доску (иницилизирует + заполняет построчно считывая и парся ее)
bool makeBoard(Board* board);

// вычисляет максимальную длину текста для каждой колонки. Возвращает массив ширин для ровного
// форматирования таблицы.
size_t* maxFieldWidth(Board* board);