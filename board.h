#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QVector>
#include "step.h"
#include "stone.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    /* game data */
    Stone stone_[32];
    int radius_;
    bool bottom_side_;
    QPoint off_;

    QVector<Step*> steps_;

    /* game status */
    int select_id_;
    bool red_turn_;
    void Init(bool red_side);

    /* draw function */
    void paintEvent(QPaintEvent *event);
    void DrawPlate(QPainter& p);//绘制棋盘
    void DrawPlace(QPainter& p);//绘制九宫格
    void DrawInitPosition(QPainter& p);//绘制兵和炮的位置标记
    void DrawInitPosition(QPainter& p, int row, int col);//绘制兵和炮的位置标记
    void DrawStone(QPainter& p);//绘制棋子
    void DrawStone(QPainter& p, int id);//绘制棋子

    /* function for coordinate */
    QPoint Center(int row, int col);
    QPoint Center(int id);
    QPoint TopLeft(int row, int col);
    QPoint TopLeft(int id);
    QRect Cell(int row, int col);
    QRect Cell(int id);

    bool GetClickRowCol(QPoint pt, int &row, int &col);

    /* help function */
    bool IsDead(int id);
    QString Name(int id);
    bool Red(int id);
    bool SameColor(int id1, int id2);
    void ReliveStone(int id);
    void KillStone(int id);
    void MoveStone(int move_id, int row, int col);
    int  GetStoneId(int row, int col);

    /* move function */
    void mouseReleaseEvent(QMouseEvent *);
    void Click(QPoint pt);
    virtual void Click(int id, int row, int col);
    void TrySelectStone(int id);
    void TryMoveStone(int kill_id, int row, int col);
    void MoveStone(int move_id, int kill_id, int row, int col);
    void SaveStep(int move_id, int kill_id, int row, int col, QVector<Step*>& steps);
    void Back(Step* step);
    void BackOne();
    virtual void Back();

    /* rule */
    bool CanMove(int move_id, int kill_id, int row, int col);
    bool CanMoveChe(int move_id, int kill_id, int row, int col);
    bool CanMoveMa(int move_id, int kill_id, int row, int col);
    bool CanMovePao(int move_id, int kill_id, int row, int col);
    bool CanMoveBing(int move_id, int kill_id, int row, int col);
    bool CanMoveJiang(int move_id, int kill_id, int row, int col);
    bool CanMoveShi(int move_id, int kill_id, int row, int col);
    bool CanMoveXiang(int move_id, int kill_id, int row, int col);

    bool CanSelect(int id);


    /* rule helper */
    int GetStoneCountAtLine(int row1, int col1, int row2, int col2);//获取2个棋子中间棋子个数
    int Relation(int row1, int col1, int row, int col);//棋子走棋规则关系
    bool IsBottomSide(int id);

signals:

public slots:
    void SlotBack();
};

#endif // BOARD_H
