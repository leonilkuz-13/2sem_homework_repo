#pragma once

#include "tools.h"

// печать горизонтальной линии
bool isOrdinaryLine(FILE* file, const size_t* widths, Board* board, char symbol);

// dыводит содержимое одной ячейки в файл с учетом выравнивания
bool writeField(FILE* file, Field* field, size_t width);

// отрисовывает доску
void printGraphics(Board* board);
