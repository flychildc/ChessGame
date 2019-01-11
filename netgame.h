#ifndef NETGAME_H
#define NETGAME_H

#include "board.h"
#include <QTcpServer>
#include <QTcpSocket>

//NetGame既可以做服务器也可以做客户端
class NetGame : public Board
{
    Q_OBJECT
public:
    NetGame(bool is_server);

    //虚函数重写父类Board中的Click函数,实现多态
    virtual void Click(int id, int row, int col);
    //虚函数重写父类Board中的Back函数,实现多态
    virtual void Back();

    QTcpServer *server_;
    QTcpSocket *socket_;

public slots:
    void SlotNewConnection();//服务器管理客户端连接
    void SlotReadyRead();//接收数据
};

#endif // NETGAME_H
