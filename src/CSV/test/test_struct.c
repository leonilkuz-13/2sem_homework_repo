#include "struct.h"

bool testInitField()
{
    Field* field = initField();
    if (field == NULL) {
        puts("testInitField: initField returned NULL");
        return false;
    }
    bool valid = (field->colNum == 0) && (field->field == NULL) && (field->len == 0)
        && (field->type == TypeNone);
    free(field->field);
    free(field);
    if (valid) {
        puts("testInitField passed");
    } else {
        puts("testInitField failed");
    }
    return valid;
}

bool testInitRow()
{
    size_t index = 5;
    Row* row = initRow(index);
    if (row == NULL) {
        puts("testInitRow: initRow returned NULL");
        return false;
    }
    bool valid = (row->field == NULL) && (row->fieldCnt == 0) && (row->num == index)
        && (row->error == false);
    free(row);
    if (valid) {
        puts("testInitRow passed");
    } else {
        puts("testInitRow failed");
    }
    return valid;
}

bool testClearRow()
{
    Row* row = initRow(0);
    if (!row) {
        return false;
    }

    row->fieldCnt = 1;
    row->field = (Field*)malloc(sizeof(Field));
    if (!row->field) {
        free(row);
        return false;
    }
    row->field[0].field = (char*)malloc(10);
    row->field[0].len = 0;
    row->field[0].type = TypeNone;
    row->field[0].colNum = 0;

    clearRow(&row);

    bool valid = (row == NULL);

    if (valid) {
        puts("testClearRow passed");
    } else {
        puts("testClearRow failed");
    }
    return valid;
}

bool testClearRowNull()
{
    Row* row = NULL;
    clearRow(&row);
    puts("testClearRowNull passed");
    return true;
}

bool testClearRowTwice()
{
    Row* row = initRow(0);
    if (!row) {
        return false;
    }
    clearRow(&row);
    clearRow(&row);
    puts("testClearRowTwice passed");
    return true;
}

bool testInitBoard()
{
    Board* board = initBoard();
    if (board == NULL) {
        puts("testInitBoard: initBoard returned NULL");
        return false;
    }
    bool valid = (board->rows == NULL) && (board->rowsCnt == 0) && (board->maxCol == 0);
    free(board);
    if (valid) {
        puts("testInitBoard passed");
    } else {
        puts("testInitBoard failed");
    }
    return valid;
}

bool testClearBoard()
{
    Board* board = initBoard();
    if (!board) {
        return false;
    }

    board->rowsCnt = 1;
    board->rows = (Row**)malloc(sizeof(Row*));
    if (!board->rows) {
        free(board);
        return false;
    }
    board->rows[0] = initRow(0);
    if (!board->rows[0]) {
        free((void*)board->rows);
        free(board);
        return false;
    }
    board->maxCol = 1;

    clearBoard(&board);

    bool valid = (board == NULL);
    if (valid) {
        puts("testClearBoard passed");
    } else {
        puts("testClearBoard failed");
    }
    return valid;
}

bool testClearBoardNull()
{
    Board* board = NULL;
    clearBoard(&board);
    puts("testClearBoardNull passed");
    return true;
}

bool testClearBoardTwice()
{
    Board* board = initBoard();
    if (!board) { // добавлены скобки
        return false;
    }
    clearBoard(&board);
    clearBoard(&board);
    puts("testClearBoardTwice passed");
    return true;
}

void runStructTests()
{
    bool result = true;
    result = result && testInitField();
    result = result && testInitRow();
    result = result && testClearRow();
    result = result && testClearRowNull();
    result = result && testClearRowTwice();
    result = result && testInitBoard();
    result = result && testClearBoard();
    result = result && testClearBoardNull();
    result = result && testClearBoardTwice();

    if (result) {
        puts("ALL STRUCT TESTS PASSED");
    } else {
        puts("SOME STRUCT TESTS FAILED");
    }
}

int main()
{
    runStructTests();
    return 0;
}