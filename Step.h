#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = 0);
    ~Step();

    int move_id_;
    int kill_id_;
    int row_from_;
    int col_from_;
    int row_to_;
    int col_to_;

signals:

public slots:
};

#endif // STEP_H
