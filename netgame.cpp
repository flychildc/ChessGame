#include "netgame.h"
#include <QtGlobal>
#include <QTime>

/*******************************************
 1）下红棋还是黑棋，这个信息由服务器发出，客户端接收
   第一个字节固定是1，第二个字节是1或0，1表示接收方下红棋，0表示接收方下黑棋
 2）点击信息
   第一个字节固定是2，第二个字节是row，第三个字节是col，第四个字节是点击棋子的id
 3)悔棋信息
   第一个字节固定是3，用来告诉对方，本方要悔棋
 *************************************************/

NetGame::NetGame(bool is_server)
{
    server_ = nullptr;
    socket_ = nullptr;

    if(is_server)//服务器,注意要先打开服务器，不然客户端无法连接到服务器
    {
        server_ = new QTcpServer;
        server_->listen(QHostAddress::Any, 9999);
        connect(server_, &QTcpServer::newConnection, this, &NetGame::SlotNewConnection);//检测是否有新的连接连接上服务器
    }
    else//客户端
    {
        socket_ = new QTcpSocket;
        socket_->connectToHost(QHostAddress("127.0.0.1"), 9999);
        connect(socket_, &QTcpSocket::readyRead, this, &NetGame::SlotReadyRead);//检测是否有数据发送到客户端
    }
 }

void NetGame::SlotNewConnection()
{
    //只接收一个连接
    if(socket_ != nullptr)
        return ;

    socket_ = server_->nextPendingConnection();//获取连接到服务器的客户端的socket
    //qDebug() << "connect success";//测试是否能够连接
    connect(socket_, &QTcpSocket::readyRead, this, &NetGame::SlotReadyRead);//检测是否有数据发送到服务器

    //发送数据
    char buf[2];
    buf[0] = 1;//这里将int转换成了char，即打印buf[0]将打印出ASCII码为1的字符
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));//设计系统当前时间作为随机数种子
    buf[1] = qrand() % 2;

    socket_->write(buf, 2);//将数据发送到客户端

    //初始化
    Init(buf[1] == 0);//要与客户端相反,不要用字符'0'，因为buf[1]是ASCII码为0或1的字符，并不是'0'，这里的比较将char转换成int，即变成了对应的ASCII码值
}

void NetGame::SlotReadyRead()
{
    QByteArray recieve = socket_->readAll();
    char cmd = recieve[0];

    if(cmd == 1)
    {
        //初始化
        char data = recieve[1];

        Init(data == 1);
    }
    else if(cmd == 2)
    {
        int row = recieve[1];
        int col = recieve[2];
        int id = recieve[3];

        Board::Click(id, row, col);//当接收到对手移动棋子的位置信息之后，需要在自己的棋盘中将对手的棋子做对应的移动
    }
    else if(cmd == 3)
    {
        //这里设置的是退两步，在对方下了之后你才能悔棋
        BackOne();
        BackOne();
    }
}

//在Board::mouseReleaseEvent中会调用该函数(多态)
void NetGame::Click(int id, int row, int col)
{
    if(select_id_ == -1 && id != -1)
    {
        if(stone_[id].red_ != bottom_side_)//只能操作底边棋子
            return ;
    }

    //调用父类的Click()函数完成本方棋子的移动相关操作
    Board::Click(id, row, col);

    //将位置信息发送给对方
    char buf[4];
    buf[0] = 2;
    buf[1] = 9 - row;
    buf[2] = 8 - col;
    buf[3] = id;

    socket_->write(buf, 4);
}

void NetGame::Back()
{
    //只有轮到你下，你才能悔棋
    if(red_turn_ != bottom_side_)
        return;

    //先将本方棋盘中的棋子退回到之前的位置，注意这里设置的是退两步，在对方下了之后你才能悔棋
    BackOne();
    BackOne();

    //把悔棋信息发送给对方
    QByteArray buf;
    buf.append(3);
    socket_->write(buf);
}
