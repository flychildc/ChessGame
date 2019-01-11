#include "singlegame.h"
#include <QTimer>

SingleGame::SingleGame()
{
    level_ = 3;
}

void SingleGame::Click(int id, int row, int col)
{
    //电脑只能下黑棋，人下红棋，如果不是轮到人下，点击将无效
    if(!red_turn_)
        return ;

    Board::Click(id, row, col);

    //轮到电脑下，即黑棋下
    if(!red_turn_)
    {
        /* 启动0.1秒定时器，在0.1秒后电脑再思考,避免因为电脑思考时间过长阻塞主进程，导致窗口得不到响应 */
        //QTimer::singleShot(100, this, SLOT(ComputerMove()));

        //不使用定时器，也没有和视频中出现窗口得不到响应的问题
        Step *step = GetBestMove();

        MoveStone(step->move_id_, step->kill_id_, step->row_to_, step->col_to_);

        delete step;

        update();
    }
}

/*void SingleGame::ComputerMove()
{
    Step *step = GetBestMove();

    MoveStone(step->move_id_, step->kill_id_, step->row_to_, step->col_to_);

    delete step;

    update();
}*/

void SingleGame::GetAllPossibleMove(QVector<Step*> &steps)
{
    int min = 16, max = 32;

    if(red_turn_)
    {
        min = 0;
        max = 16;
    }

    for(int i = min; i < max; ++i)
    {
        if(stone_[i].dead_)
            continue;

        for(int row = 0; row < 10; ++row)
        {
            for(int col = 0; col < 9; ++col)
            {
                int kill_id = GetStoneId(row, col);

                if(SameColor(kill_id, i))
                    continue;

                if(CanMove(i, kill_id, row, col))
                {
                    SaveStep(i, kill_id, row, col, steps);
                }
            }
        }
    }
}

//先移动
void SingleGame::FakeMove(Step *step)
{
    KillStone(step->kill_id_);
    MoveStone(step->move_id_, step->row_to_, step->col_to_);
}

//取消移动,恢复到移动之前的状态
void SingleGame::UnFakeMove(Step *step)
{
    ReliveStone(step->kill_id_);
    MoveStone(step->move_id_, step->row_from_, step->col_from_);
}

//计算每一步移动之后的局面分
int SingleGame::CalcScores()
{
     //为每一种类型的棋子设置分数，其对应CHE, MA, PAO, BING, JIANG, SHI, XIANG。
     //更好的办法是动态的设置每一种棋子的分数(权重)，因为随着棋子的位置发生变化权重会发生改变
     static int chess_score[] = {100, 50, 50, 20, 1500, 10, 10};
     int red_total_score = 0;
     int black_total_score = 0;

     for(int i = 0; i < 16; i++)
     {
        if(stone_[i].dead_)
            continue;

        red_total_score += chess_score[stone_[i].type_];
     }

     for(int i = 16; i < 32; i++)
     {
        if(stone_[i].dead_)
            continue;

        black_total_score += chess_score[stone_[i].type_];
     }

     //因为电脑下的是黑棋，因此只关心黑棋总分数减去白棋总分数
     return black_total_score - red_total_score;
}

/******************************************/
/** 这部分比较难理解，可以参考图片最大最小值算法 **/
/******************************************/

//只有当电脑(黑棋)下，才会调用这个函数
Step* SingleGame::GetBestMove()
{
    QVector<Step*> steps;
    Step* best_step = nullptr;
    int max_score = -100000;

    // 1.看看有那些步骤可以走
    GetAllPossibleMove(steps);//这一步获取的是黑棋的所有走法

    // 2.试着走一下
    // 3.评估走的结果
    while(steps.count())
    {
        Step *step = steps.back();
        steps.removeLast();

        //试着走一下
        FakeMove(step);
        int score = GetMinScore(level_ - 1, max_score);
        UnFakeMove(step);

        //找出最大值
        if(score > max_score)
        {
            max_score = score;

            if(best_step)
                delete best_step;
            best_step = step;
        }
        else
        {
            delete step;
        }
    }

    return best_step;
}

//获取最小值，是黑棋(电脑)走棋之后，红棋所能走的走法中的最小值，因为红棋作为电脑的对手肯定会使电脑得分最小，只不过电脑需要做的就是选择这些最小值中的最大值
int SingleGame::GetMinScore(int level, int cur_max_score)
{
    //交替递归终止条件
    if(level == 0)
        return CalcScores();

    //1.看看有那些步骤可以走
    QVector<Step*> steps;

    GetAllPossibleMove(steps);//获取的是红棋所有可能走棋的步骤

    int min_score = 100000;

    // 2.试着走一下
    // 3.评估走的结果
    while(steps.count())
    {
        Step *step = steps.back();
        steps.removeLast();

        //试着走一下
        FakeMove(step);
        int score = GetMaxScore(level - 1, min_score);//如果还要考虑下一层的话，就是电脑下棋，此刻要获取最大值.也就是获得了这步红棋落下后黑棋所有下法中的最大值
        UnFakeMove(step);
        delete step;

        //红棋换其他走法之后，找到比之前红棋走的位置之后的最大值还要大的值之后就认为是最好的走法(其实最好的应该是比前一次大的所有走法中的最大值)
        if(score <= cur_max_score)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }

            return score;
        }

        //如果只考虑到本层，那么min_score就是上一步电脑下完之后红棋所能下法中的最小值，如果在往下考虑，min_score可能就是黑棋(电脑)能下的最大值中的最小值
        if(score < min_score)
        {
            min_score = score;
        }
    }

    return min_score;
}

int SingleGame::GetMaxScore(int level, int cur_min_score)
{
    //交替递归终止条件
    if(level == 0)
        return CalcScores();

    //1.看看有那些步骤可以走
    QVector<Step*> steps;

    GetAllPossibleMove(steps);//获取的是黑棋所有可能走棋的步骤

    int max_score = -100000;

    // 2.试着走一下
    // 3.评估走的结果
    while(steps.count())
    {
        Step *step = steps.back();
        steps.removeLast();

        //试着走一下
        FakeMove(step);
        int score = GetMinScore(level - 1, max_score);
        UnFakeMove(step);
        delete step;

        if(score >= cur_min_score)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }

            return score;
        }

        if(score > max_score)
        {
            max_score = score;
        }
    }

    return max_score;
}


