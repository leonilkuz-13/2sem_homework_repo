#include <tools.h>

char* read(FILE* file)
{
    size_t capacity = 100;
    size_t len = 0;
    char* Buff = malloc(capacity);
    if (Buff == NULL) {
        return NULL;
    }

    int var = 0;
    while ((var = fgetc(file)) != EOF) {
        if (len + 1 >= capacity) {
            capacity *= 2;
            char* newBuff = realloc(Buff, capacity);
            if (newBuff == NULL) {
                free(Buff);
                return NULL;
            }
            Buff = newBuff;
        }

        Buff[len++] = (char)var;

        if (var == '\n') {
            break;
        }
    }

    if (len == 0 && var == EOF) { // пустую строчку же можно?
        free(Buff);
        return NULL;
    }

    Buff[len] = '\0';
    return Buff;
}

bool makeboard(Board* board)
{
    FILE* file = fopen("input.csv", "r");
    if (file == NULL) {
        return false;
    }
    char* line;
    size_t maxCol = 0; // максимальное количество полей в структуре
    size_t numStr = 0; // для количества строк и номер строки
    Row** rows = NULL; // буду собирать строку сначала, а потом запихну ее в board
    while ((line = read(file)) != NULL) {
        Row* row = initRow(numStr);
        if (row == NULL) {
            free(line);
            for (size_t index = 0; index < numStr; index++) {
                clearRow(&rows[index]);
            }
            free(rows);
            fclose(file);
            return false;
        }
        if (!parse(row, line)) {
            free(line);
            for (size_t index = 0; index < numStr; index++) {
                clearRow(&rows[index]);
            }
            free(rows);
            fclose(file);
            return false;
        }
        maxCol = (row->fieldCnt > maxCol) ? row->fieldCnt : maxCol;
        Row** newRows = realloc(rows, (numStr + 1) * sizeof(Row*)); // копирую с помощью realloc
        if (newRows == NULL) {
            free(line);
            for (size_t index = 0; index < numStr; index++) {
                clearRow(&rows[index]);
            }
            free(rows);
            fclose(file);
            return false;
        }
        rows = newRows;
        rows[numStr++] = row; // индексация с нуля
        free(line);
    }

    board->rows = rows;
    board->rowsCnt = numStr;
    board->maxCol = maxCol;

    fclose(file);
    return true;
}

size_t* maxFieldWidth(Board* board)
{
    size_t* widths = calloc(board->maxCol, sizeof(size_t));
    if (widths == NULL)
        return NULL;

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