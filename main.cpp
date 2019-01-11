#include <QApplication>
#include "choosedialog.h"
#include "mainwindow.h"

int main(int argc,char* argv[])
{
    QApplication app(argc, argv);

    ChooseDialog dialog;
    if(dialog.exec() != QDialog::Accepted)//模态对话框
        return 0;

    MainWindow mainwindow(dialog.select_);
    mainwindow.show();

    return app.exec();
}
