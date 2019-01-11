#ifndef CHOOSEDIALOG_H
#define CHOOSEDIALOG_H

#include <QDialog>
#include <QPushButton>

class ChooseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseDialog(QWidget *parent = 0);
    ~ChooseDialog();

    QPushButton* buttons_[4];
    int select_;

public slots:
    void SlotClick();
};

#endif // CHOOSEDIALOG_H
