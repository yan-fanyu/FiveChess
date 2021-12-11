#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include<QFileDialog>
#include"draw.h"
#include "player.h"
#include<QDir>
#include<set>

static bool END = false;

static Board*  board;
static Player* black;


#define BLACK true
#define WHITE false

static vector<pair<pair<int, int>, int>> gameData;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("黑方");

    ui->back->setEnabled(false);

    ui->gamedata->setEnabled(false);

    // 创建监听的服务器对象
    m_s = new QTcpServer(this);

    connect(m_s, &QTcpServer::newConnection, this, [=](){
        m_tcp = m_s->nextPendingConnection();

        m_status->setPixmap(QPixmap(":/connect.png").scaled(20, 20));
        // 检测是否可以接收数据
        connect(m_tcp, &QTcpSocket::readyRead, this, [=]()
        {
             QString data = m_tcp->readAll();

             if(data.size()==1)
             {
                 // 如果对方同意
                 if(data == "1")
                 {
                     black->OK = true;
                     board->SetBack();
                     // 移除最后一个
                     gameData.pop_back();
                     update();
                 }
                 else
                 {
                     // 不同意
                 }
             }

             //
             else if(data.size()==2)
             {
                  Back(board);
             }



             else if(data.size() >= 4)
             {
                 ui->back->setEnabled(false);
                 QString sx = (QString)data[0] + (QString)data[1];
                 int x = sx.toInt();
                 QString sy = (QString)data[2] + (QString)data[3];
                 int y = sy.toInt();
                 board->last_x = x;
                 board->last_y = y;
                 ::board->SetBoard(x, y, false);
                 // 白方
                 gameData.push_back(make_pair(make_pair(x, y), 0));
                 update();
                 IsWon(false, x, y);
                 if(data.size() == 5)
                 {
                      END = true;
                      ui->gamedata->setEnabled(true);
                      PrintGameData();
                      ui->back->setEnabled(false);
                      QMessageBox::information(this,"lose","You Lose");
                      black->OK = false;
                      return;
                 }
                 black->OK = true;
             }

        });
    });

    // 状态栏
    m_status = new QLabel;
    m_status->setPixmap(QPixmap(":/disconnect.png").scaled(20, 20));
    ui->statusbar->addWidget(new QLabel("连接状态: "));
    ui->statusbar->addWidget(m_status);

    /*888888888888888888888888888888888888888888*/
    board = new Board();
    black = new Player();
    this->setFixedSize(board->d*(board->col+6), board->d*(board->row+2));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setListen_clicked()
{
    unsigned short port = 8899;
    m_s->listen(QHostAddress::Any, port);
    ui->setListen->setDisabled(true);
}

void MainWindow::on_sendMsg_clicked()
{
    QString sx;
    QString sy;
    if(board->push_x < 10)
        sx = "0" + QString::number(board->push_x);
    else
        sx = QString::number(board->push_x);
    if(board->push_y < 10)
        sy = "0" + QString::number(board->push_y);
    else
        sy = QString::number(board->push_y);
    QString msg = sx + sy;

    if(END)
    {
        msg += "1";
    }

    m_tcp->write(msg.toUtf8());
}



void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(END)
    {
        QMessageBox::information(this,"提示","游戏已结束!");
        return;
    }




    double x_click;
    double y_click;



    y_click=event->x();
    x_click=event->y();



    if(IsInRange(x_click, y_click, board) && black->OK == false)
    {
        QMessageBox::information(this,"错误","还没到你!");
    }

    if(IsInRange(x_click, y_click, board) && black->OK == true)
    {
        GetPushPos(board, x_click, y_click);
        if(!IsAvailable(board->push_x, board->push_y))
        {
            QMessageBox::information(this,"错误","被占用了!");
        }
        else
        {
            gameData.push_back(make_pair(make_pair(board->push_x, board->push_y), 1));
            click(board->push_x, board->push_y);
            ui->back->setEnabled(true);
            board->last_x = board->push_x;
            board->last_y = board->push_y;
            on_sendMsg_clicked();
            if(END)
            {
                ui->back->setEnabled(false);
                emit won(true);
                ui->gamedata->setEnabled(true);
                PrintGameData();
            }
            black->OK = false;
        }
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setPen(QPen(3));
    DrawBlock(painter);
    drawStones(painter, board->_board);
    painter->end();
}

bool MainWindow::click(int x, int y)
{
    //如果该位置是空
    if(!board->b_win)
    {
        board->SetBoard(x, y, true);
        //board->_board[x][y]=2;
        update();

        if(IsWon(true, x, y))
        {
            END = true;
            ui->back->setEnabled(false);

        }
        return true;
    }
    return false;



}

void MainWindow::GetPushPos(Board *board, double click_x, double click_y)
{
    click_x += (double)DITH / 2;
    click_y += (double)DITH / 2;
    board->push_x = int(click_x / DITH) - 1;
    board->push_y = int(click_y / DITH) - 1;
}

bool MainWindow::IsInRange(int x, int y, Board* board)
{
    if(x > COL*DITH + double(DITH/2) || y > ROW*DITH + double(DITH/2) || x < double(DITH/2) || y < double(DITH/2))
    {
        return false;
    }
    return true;
}



void MainWindow::won(bool isblack)
{
    board->b_win=true;
    QMessageBox::information(this,"won","You Win!");
}

void MainWindow::Back(Board* board)
{
    QMessageBox msg(this);  //对话框设置父组件
    msg.setWindowTitle("悔棋");   //对话框标题
    msg.setText("对方悔棋，同意吗");    //对话框提示文本
    msg.setIcon(QMessageBox::Information);//设置图标类型
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox:: Cancel);//对话框上包含的按钮

    if(msg.exec() == QMessageBox::Ok)//模态调用
    {
       //on_sendMsg_clicked();
        black->OK = false;
        QString msg;
        msg = "1";
        board->SetBack();
        m_tcp->write(msg.toUtf8());
    }

    else if(msg.exec() == QMessageBox::Cancel)//模态调用
    {
        QString msg;
        msg = "0";
        m_tcp->write(msg.toUtf8());
    }
}

bool MainWindow::IsAvailable(int x, int y)
{
    return board->_board[x][y] == 0;
}

void MainWindow::PrintGameData()
{
    for(auto it: gameData)
    {
        qDebug("(%d, %d, %d)", it.first.first, it.first.second, it.second);
    }
}



void MainWindow::on_back_clicked()
{
    ui->back->setEnabled(false);
    QString msg = "22";
    m_tcp->write(msg.toUtf8());
}


bool MainWindow::IsWon(bool is_black, int x0, int y0)
{
    bool shangxia;
    bool zuoyou;
    bool youxie;
    bool zuoxie;
    int flag = is_black?1:2;
    //down

    // 竖直方向
    for(int xi = x0 -4; xi <= x0; xi++)
    {
        shangxia = true;
        if(xi < 0 || xi >= ROW)
        {
            shangxia = false;
            continue;
        }
        for(int x = xi; x <= xi+4; x++)
        {
            if(x < 0 || x >= ROW)
            {
                shangxia = false;
                break;
            }
            if(board->_board[x][y0] != flag)
            {
                shangxia = false;
                break;
            }
        }
        if(shangxia == true)
        {
            break;
        }
    }

    // 水平方向
    for(int yi = y0 -4; yi <= y0; yi++)
    {
        zuoyou = true;
        if(yi < 0 || yi >= COL)
        {
            zuoyou = false;
            continue;
        }
        for(int y = yi; y <= yi+4; y++)
        {
            if(y < 0 || y >= COL)
            {
                zuoyou = false;
                break;
            }
            if(board->_board[x0][y] != flag)
            {
                zuoyou = false;
                break;
            }
        }
        if(zuoyou == true)
        {
            break;
        }
    }

    // 右斜
    for(int xi = x0 -4, yi = y0 -4; yi <= y0 && xi <= x0; xi++, yi++)
    {
        youxie = true;
        if(xi < 0 || xi >= ROW || yi < 0 || yi >= COL)
        {
            youxie = false;
            continue;
        }
        for(int x = xi, y = yi; x <= xi+4 && y <= yi+4; x++, y++)
        {
            if(x < 0 || x >= ROW || y < 0 || y >= COL)
            {
                youxie = false;
                break;
            }
            if(board->_board[x][y] != flag)
            {
                youxie = false;
                break;
            }
        }
        if(youxie == true)
        {
            break;
        }
    }

    // 左斜
    for(int xi = x0-4, yi = y0+4; xi <= x0 && yi >= y0; xi++, yi--)
    {
        zuoxie = true;
        if(xi < 0 || xi >= ROW || yi < 0 || yi >= COL)
        {
            zuoxie = false;
            continue;
        }
        for(int x = xi, y = yi; x <= xi+4 && y >= yi-4; x++, y--)
        {
            if(x < 0 || x >= ROW || y < 0 || y >= COL)
            {
                zuoxie = false;
                break;
            }
            if(board->_board[x][y] != flag)
            {
                zuoxie = false;
                break;
            }
        }
        if(zuoxie == true)
        {
            break;
        }
    }

    return (shangxia || zuoyou || youxie || zuoxie);
}


void MainWindow::on_gamedata_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open TXT file"),
                "./", tr("Txt files(*.txt)"));
    QFile output_file(fileName);
    output_file.remove();
    if(!output_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        qDebug() <<"output file don't exist!";
    }
    else
    {
        qDebug() <<"成功";
        QTextStream in(&output_file);
        in << QString::fromLocal8Bit("玩家    ") << QString::fromLocal8Bit("坐标") <<"\n";
        for(auto it: gameData)
        {
            if(it.second == 1)
            {
                in << QString::fromLocal8Bit("黑棋    ") << "(" << it.first.first <<", "<< it.first.second<<")"<<"\n";
            }
            else
            {
                in << QString::fromLocal8Bit("白棋    ") << "(" << it.first.first <<", "<< it.first.second<<")"<<"\n";
            }
        }

    }
    output_file.close();
}
