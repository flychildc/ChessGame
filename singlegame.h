#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "board.h"

class SingleGame : public Board
{
    Q_OBJECT

public:
    SingleGame();

    //虚函数重写父类Board中的Click函数,实现多态
    virtual void Click(int id, int row, int col);

    Step* GetBestMove();
    void GetAllPossibleMove(QVector<Step*> &steps);

    void FakeMove(Step *step);//先移动
    void UnFakeMove(Step *step);//取消移动,恢复到移动之前的状态
    int CalcScores();//计算每一步移动的局面分

    int GetMinScore(int level, int cur_max_score);
    int GetMaxScore(int level, int cur_min_score);

    int level_;//电脑考虑几步

public slots:
    //void ComputerMove();
};

#endif // SINGLEGAME_H
