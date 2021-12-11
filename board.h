#ifndef BOARD_H
#define BOARD_H


#include <vector>


class Board
{

public:
    Board();
    //棋盘尺寸
    int row;
    int col;
    int d;

    int push_x;
    int push_y;


    //有人胜出
    bool b_win;
    std::vector<std::vector<int>> _board;//0代表空格，1代表黑棋，2代表白棋
    //bool hasWon(bool is_black,int row_stone,int col_stone);


    int last_x;
    int last_y;

    void SetBack();

    void SetThisStep(int x, int y);


    void SetBoard(int x, int y, bool black_white);


};

#endif // BOARD_H
