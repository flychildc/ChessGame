#include "mainwindow.h"
#include "singlegame.h"
#include "netgame.h"
#include "board.h"
#include "ctrlpanel.h"
#include <QHBoxLayout>

MainWindow::MainWindow(int game_type, QWidget *parent) : QWidget(parent)
{
    game_type_ = game_type;

    if(game_type_ == 0)//单机游戏
    {
        SingleGame *single_game = new SingleGame;
        CtrlPanel *panel = new CtrlPanel;

        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->addWidget(single_game, 1);
        hlayout->addWidget(panel);

        connect(panel, &CtrlPanel::SignalBack, single_game, &SingleGame::SlotBack);//悔棋
    }
    else if(game_type_ == 1)//双人对战
    {
        Board *board = new Board;
        CtrlPanel *panel = new CtrlPanel;

        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->addWidget(board, 1);
        hlayout->addWidget(panel);

        connect(panel, &CtrlPanel::SignalBack, board, &SingleGame::SlotBack);//悔棋

    }
    else if(game_type_ == 2)//网络对战(服务端)
    {
        NetGame *net_game = new NetGame(true);
        CtrlPanel *panel = new CtrlPanel;

        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->addWidget(net_game, 1);
        hlayout->addWidget(panel);

        connect(panel, &CtrlPanel::SignalBack, net_game, &SingleGame::SlotBack);//悔棋
    }
    else if(game_type_ == 3)//网络对战(客户端)
    {
        NetGame *net_game = new NetGame(false);
        CtrlPanel *panel = new CtrlPanel;

        QHBoxLayout *hlayout = new QHBoxLayout(this);
        hlayout->addWidget(net_game, 1);
        hlayout->addWidget(panel);

        connect(panel, &CtrlPanel::SignalBack, net_game, &SingleGame::SlotBack);//悔棋
    }
}

MainWindow::~MainWindow()
{

}
