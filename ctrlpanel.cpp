#include "ctrlpanel.h"
#include <QGridLayout>
#include <QVBoxLayout>

CtrlPanel::CtrlPanel(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout(this);
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    grid->setColumnStretch(0, 1);//设置第0列的伸缩因子为1（网格布局就是将窗口区域分成很多格子）
    grid->setColumnStretch(2, 1);//设置第2列的伸缩因子为1（网格布局就是将窗口区域分成很多格子）
    grid->setRowStretch(0, 1);
    grid->setRowStretch(2, 1);
    grid->addLayout(vlayout, 1, 1);

    back_= new QPushButton("Back", this);
    vlayout->addWidget(back_);

    connect(back_, &QPushButton::clicked, this, &CtrlPanel::SignalBack);
}

CtrlPanel::~CtrlPanel()
{

}
