#include "tools.h"

char* read(FILE* file)
{
    size_t capacity = 100;
    size_t len = 0;
    char* buffer = malloc(capacity);
    if (buffer == NULL) {
        return NULL;
    }

    int var = 0;
    while ((var = fgetc(file)) != EOF) {
        if (var == '\r') {
            continue;
        }
        if (var == '\n') {
            break;
        }

        if (len + 2 >= capacity) {
            capacity *= 2;
            char* newBuffer = realloc(buffer, capacity);
            if (newBuffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = newBuffer;
        }

        buffer[len++] = (char)var;

        if (var == '\n') {
            break;
        }
    }

    if (len == 0 && var == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[len] = '\0';
    return buffer;
}

bool makeboard(Board* board)
{
    printf("makeboard: opening file input.csv\n");
    FILE* file = fopen("input.csv", "r");
    if (file == NULL) {
        printf("makeboard: cannot open file\n");
        return false;
    }
    printf("makeboard: file opened successfully\n");

    char* line;
    size_t maxCol = 0;
    size_t numStr = 0;
    Row** rows = NULL;

    while ((line = read(file)) != NULL) {
        printf("makeboard: read line: %s\n", line);

        Row* row = initRow(numStr);
        if (row == NULL) {
            printf("makeboard: initRow failed for line %zu\n", numStr);
            free(line);
            for (size_t index = 0; index < numStr; index++) {
                clearRow(&rows[index]);
            }
            free((void*)rows);
            fclose(file);
            return false;
        }
        printf("makeboard: calling parse for line %zu\n", numStr);
        if (!parse(&row, line)) {
            printf("makeboard: parse FAILED for line: %s\n", line);
            free(line);
            for (size_t index = 0; index < numStr; index++) {
                clearRow(&rows[index]);
            }
            free((void*)rows);
            fclose(file);
            return false;
        }
        printf("makeboard: parse OK, fieldCnt = %zu\n", row->fieldCnt);
        maxCol = (row->fieldCnt > maxCol) ? row->fieldCnt : maxCol;

        Row** newRows = (Row**)realloc((void*)rows, (numStr + 1) * sizeof(Row*));
        if (newRows == NULL) {
            printf("makeboard: realloc failed for line %zu\n", numStr);
            free(line);
            for (size_t index = 0; index < numStr; index++) {
                clearRow(&rows[index]);
            }
            free((void*)rows);
            fclose(file);
            return false;
        }
        rows = newRows;
        rows[numStr++] = row;
        free(line);
    }

    printf("makeboard: finished reading, total rows = %zu, max columns = %zu\n", numStr, maxCol);
    board->rows = rows;
    board->rowsCnt = numStr;
    board->maxCol = maxCol;

    fclose(file);
    return true;
}

size_t* maxFieldWidth(Board* board)
{
    if (board->maxCol == 0 || board == NULL) {
        return NULL;
    }
    size_t* widths = calloc(board->maxCol, sizeof(size_t));
    if (widths == NULL) {
        return NULL;
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
            if (found != NULL) {
                if (found->len > widths[col]) {
                    widths[col] = found->len;
                }
            }
        }
    }
    return widths;
}