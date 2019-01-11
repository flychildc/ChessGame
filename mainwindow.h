#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(int game_type, QWidget *parent = 0);
    ~MainWindow();

    int game_type_;
};

#endif // MAINWINDOW_H
