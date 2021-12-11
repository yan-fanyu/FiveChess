#include "draw.h"
#include"mainwindow.h"
#include<set>

void DrawBlock(QPainter *painter)
{
    //竖线
    for(int i = 1; i <= COL; i++)
    {
        painter->drawLine(i*DITH, DITH, i*DITH, ROW*DITH);
    }
    //横线
    for(int i = 1; i<= ROW; i++)
    {
        painter->drawLine(DITH, i*DITH, COL*DITH, i*DITH);
    }
}



void drawStones(QPainter* painter, std::vector<std::vector<int>>&_board)
{
    for(int i=0; i < ROW; i++)
    {
        for(int j=0; j < COL; j++)
        {
            if(_board[i][j] > 0)
            {
                drawStone(painter,i,j, _board[i][j] == 1);
            }
        }
    }
}

void drawStone(QPainter* painter,int row, int col,bool isblack)
{

    if(isblack)
    {
        painter->setBrush(QBrush(Qt::black));
    }
    else
    {
        painter->setBrush(QBrush(Qt::white));
    }
    double x=col*DITH + DITH/2;
    double y=row*DITH + DITH/2;
    painter->drawEllipse(x, y, DITH - 2, DITH - 2);

}
