#include "graphics.h"

int main(void)
{
    Board* board = initBoard();
    if (!board) {
        return 1;
    }
    if (!makeboard(board)) {
        clearBoard(&board);
        return 1;
    }
    graphics(board);
    clearBoard(&board);
    return 0;
}