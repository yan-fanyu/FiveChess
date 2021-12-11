#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>

#include "board.h"
#include <QPainter>
using namespace std;


#define ROW 15
#define COL 15
#define DITH 40

#include<QMouseEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QByteArray data;

    void paintEvent(QPaintEvent *) override;
    virtual void mousePressEvent(QMouseEvent *event) override;


    bool click(int row_stone, int col_stone);

    void GetPushPos(Board* board, double, double);

    bool IsInRange(int x, int y, Board* board);

    bool IsWon(bool is_black, int row_stone, int col_stone);

    void won(bool isblack);

    void Back(Board*);

    void SetThisStep(int x, int y);

    bool IsAvailable(int x, int y);

    void SetBoard(int x, int y, bool black_white);

    void PrintGameData();

private slots:
    void on_setListen_clicked();

    void on_sendMsg_clicked();

    void on_back_clicked();

    void on_gamedata_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer* m_s;
    QTcpSocket* m_tcp;
    QLabel* m_status;
};
#endif // MAINWINDOW_H
