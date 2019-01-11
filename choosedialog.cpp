#include "choosedialog.h"
#include <QVBoxLayout>

ChooseDialog::ChooseDialog(QWidget *parent) : QDialog(parent)
{
    //构造四个按钮
    buttons_[0] = new QPushButton(u8"人机对战",this);
    buttons_[1] = new QPushButton(u8"人人对战",this);
    buttons_[2] = new QPushButton(u8"网络对战(服务端)",this);
    buttons_[3] = new QPushButton(u8"网络对战(客户端)",this);

    //垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(buttons_[0]);
    layout->addWidget(buttons_[1]);
    layout->addWidget(buttons_[2]);
    layout->addWidget(buttons_[3]);

    for(int i = 0; i < 4; ++i)
    {
        connect(buttons_[i], &QPushButton::clicked, this, &ChooseDialog::SlotClick);
    }
}

ChooseDialog::~ChooseDialog()
{

}

void ChooseDialog::SlotClick()
{
    QObject *s = sender();//获取调用这个槽函数的信号的发送者

    for(int i = 0; i < 4; ++i)
    {
        if(buttons_[i] == s)
        {
            select_ = i;
            break;
        }
    }

    accept();
}
