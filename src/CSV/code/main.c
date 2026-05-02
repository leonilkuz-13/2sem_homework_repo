#include "graphics.h"

int main(void)
{
    Board* board = initBoard();
    if (!board) {
        return 1;
    }
    if (!makeBoard(board)) {
        clearBoard(&board);
        return 1;
    }
    printGraphics(board);
    clearBoard(&board);
    return 0;
}
