#ifndef DRAW_H
#define DRAW_H
#include<QPainter>
#include"mainwindow.h"

void DrawBlock(QPainter* painter);


void drawStones(QPainter* painter, std::vector<std::vector<int>>&_board);//刷新棋子布局

void drawStone(QPainter* painter,int row,int col,bool isblack);//画一个棋子，由drawStones()调用

#endif // DRAW_H
