#include "board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>


Board::Board()
{
    row=15;
    col=15;
    d=40;

    push_x = 0;
    push_y = 0;


    b_win=false;
    _board=std::vector<std::vector<int>>(row, std::vector<int>(col,0));
    last_x = 0;
    last_y = 0;
}

void Board::SetBack()
{
    _board[last_x][last_y] = 0;
}

void Board::SetThisStep(int x, int y)
{
    push_x = x;
    push_y = y;
}

void Board::SetBoard(int x, int y, bool black_white)
{
    this->_board[x][y] = black_white?1:2;

}
