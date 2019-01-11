#include "board.h"
#include <QPainter>
#include <QMouseEvent>

#define GETROWCOL(row, col, id) \
    int row = stone_[id].row_; \
    int col = stone_[id].col_

Board::Board(QWidget *parent) : QWidget(parent)
{
    this->radius_ = 20;
    setMinimumSize(radius_*18+1, radius_*20+1);//绘制窗口大小
    Init(true);
}

void Board::Init(bool red_bottom_side)
{
    for(int i = 0; i < 32; i++)
    {
        stone_[i].Init(i);
    }

    if(red_bottom_side)
    {
        for(int i = 0; i < 32; i++)
        {
            stone_[i].Rotate();//将黑棋和红棋反转
        }
    }

    select_id_ = -1;
    red_turn_ = true;
    bottom_side_ = red_bottom_side;
    update();
}

void Board::paintEvent(QPaintEvent *)
{
    int r = height()/20;
    radius_ = r;
    off_ = QPoint(r+1, r+1);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.save();//保存当前绘图状态到栈中
    DrawPlate(painter);
    painter.restore();//将绘图状态从栈中弹出

    painter.save();
    DrawPlace(painter);
    painter.restore();

    painter.save();
    DrawInitPosition(painter);
    painter.restore();

    painter.save();
    DrawStone(painter);
    painter.restore();
}

//绘制棋盘
void Board::DrawPlate(QPainter& p)
{
    for(int i=0; i<10; ++i)
   {
       if(i==0 || i==9)
       {
           p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
       }
       else
       {
           p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
       }
       p.drawLine(Center(i, 0), Center(i, 8));
   }

   for(int i=0; i<9; ++i)
   {
       if(i==0 || i==8)
       {
           p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
           p.drawLine(Center(0, i), Center(9, i));
       }
       else
       {
           p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
           p.drawLine(Center(0, i), Center(4, i));
           p.drawLine(Center(5, i), Center(9, i));
       }
   }
}

//绘制九宫格
void Board::DrawPlace(QPainter& p)
{
    p.drawLine(Center(0, 3), Center(2, 5));
    p.drawLine(Center(2, 3), Center(0, 5));

    p.drawLine(Center(9, 3), Center(7, 5));
    p.drawLine(Center(7, 3), Center(9, 5));
}

//绘制兵和炮的位置标记
void Board::DrawInitPosition(QPainter& p)
{
    DrawInitPosition(p, 3, 0);
    DrawInitPosition(p, 3, 2);
    DrawInitPosition(p, 3, 4);
    DrawInitPosition(p, 3, 6);
    DrawInitPosition(p, 3, 8);

    DrawInitPosition(p, 6, 0);
    DrawInitPosition(p, 6, 2);
    DrawInitPosition(p, 6, 4);
    DrawInitPosition(p, 6, 6);
    DrawInitPosition(p, 6, 8);

    DrawInitPosition(p, 2, 1);
    DrawInitPosition(p, 2, 7);

    DrawInitPosition(p, 7, 1);
    DrawInitPosition(p, 7, 7);
}

//绘制兵和炮的位置标记
void Board::DrawInitPosition(QPainter& p, int row, int col)
{
    QPoint pt = Center(row, col);
    QPoint off = QPoint(radius_/6, radius_/6);
    int len = radius_/3;

    QPoint ptStart;
    QPoint ptEnd;


    if(col != 0)
    {
        /* 左上角 */
        ptStart = QPoint(pt.x() - off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);

        /* 左下角 */
        ptStart = QPoint(pt.x() - off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(-len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);
    }

    if(col != 8)
    {
        /* 右下角 */
        ptStart = QPoint(pt.x() + off.x(), pt.y() + off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, +len);
        p.drawLine(ptStart, ptEnd);

        /* 右上角 */
        ptStart = QPoint(pt.x() + off.x(), pt.y() - off.y());
        ptEnd = ptStart + QPoint(+len, 0);
        p.drawLine(ptStart, ptEnd);
        ptEnd = ptStart + QPoint(0, -len);
        p.drawLine(ptStart, ptEnd);
    }
}

//绘制棋子
void Board::DrawStone(QPainter& p)
{
    for(int i=0; i<32; i++)
    {
        DrawStone(p, i);
    }
}

bool Board::IsDead(int id)
{
    if(id == -1)
        return true;

    return stone_[id].dead_;
}

//绘制棋子
void Board::DrawStone(QPainter& p, int id)
{
    if(IsDead(id))
        return;

    QColor color;
    if(Red(id))
        color = Qt::red;
    else
        color = Qt::black;

    p.setPen(QPen(QBrush(color), 2));

    if(id == select_id_)
        p.setBrush(Qt::gray);
    else
        p.setBrush(Qt::yellow);

    p.drawEllipse(Cell(id));

    p.setFont(QFont("system", radius_*1.2, 700));
    p.drawText(Cell(id), Name(id), QTextOption(Qt::AlignCenter));
}

QString Board::Name(int id)
{
    return stone_[id].Name();
}

bool Board::Red(int id)
{
    return stone_[id].red_;
}

QPoint Board::Center(int row, int col)
{
    QPoint pt(radius_*col*2, radius_*row*2);
    return pt + off_;
}

QPoint Board::Center(int id)
{
    return Center(stone_[id].row_, stone_[id].col_);
}

QPoint Board::TopLeft(int row, int col)
{
    return Center(row, col) - QPoint(radius_, radius_);
}

QPoint Board::TopLeft(int id)
{
    return TopLeft(stone_[id].row_, stone_[id].col_);
}

QRect Board::Cell(int row, int col)
{
    return QRect(TopLeft(row, col), QSize(radius_*2-1, radius_*2-1));
}

QRect Board::Cell(int id)
{
    return QRect(TopLeft(id), QSize(radius_*2-1, radius_*2-1));
}

bool Board::GetClickRowCol(QPoint pt, int &row, int &col)
{
    for(row=0; row<=9; ++row)
    {
        for(col=0; col<=8; ++col)
        {
            QPoint distance = Center(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < radius_* radius_)
                return true;
        }
    }
    return false;
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
        return;

    Click(ev->pos());
}

bool Board::CanSelect(int id)
{
    return red_turn_ == stone_[id].red_;
}

void Board::TrySelectStone(int id)
{
    if(id == -1)
        return;
    if(!CanSelect(id))
        return;

    select_id_ = id;
    update();
}

bool Board::SameColor(int id1, int id2)
{
    if(id1 == -1 || id2 == -1)
        return false;

    return Red(id1) == Red(id2);
}

void Board::TryMoveStone(int kill_id, int row, int col)
{
    //当要吃的棋子和选中的棋子颜色相同，将要吃的棋子变成新的选中棋子
    if(kill_id != -1 && SameColor(kill_id, select_id_))
    {
        TrySelectStone(kill_id);
        return;
    }

    bool ret = CanMove(select_id_, kill_id, row, col);
    if(ret)
    {
        MoveStone(select_id_, kill_id, row, col);
        select_id_ = -1;
        update();
    }
}

//获取2个棋子中间棋子个数
int Board::GetStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(GetStoneId(row1, col) != -1) ++ret;
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(GetStoneId(row, col1) != -1) ++ret;
        }
    }

    return ret;
}

//棋子走棋规则关系
int Board::Relation(int row1, int col1, int row, int col)
{
    return qAbs(row1-row)*10+qAbs(col1-col);//通过这个值来判断不同类型的棋子的走棋规则
}

bool Board::CanMove(int move_id, int kill_id, int row, int col)
{
    if(SameColor(move_id, kill_id))
        return false;

    switch(stone_[move_id].type_)
    {
    case Stone::CHE:
       return CanMoveChe(move_id, kill_id, row, col);

    case Stone::MA:
       return CanMoveMa(move_id, kill_id, row, col);

    case Stone::PAO:
       return CanMovePao(move_id, kill_id, row, col);

    case Stone::BING:
       return CanMoveBing(move_id, kill_id, row, col);

    case Stone::JIANG:
       return CanMoveJiang(move_id, kill_id, row, col);

    case Stone::SHI:
       return CanMoveShi(move_id, kill_id, row, col);

    case Stone::XIANG:
       return CanMoveXiang(move_id, kill_id, row, col);

    }

    return false;
}

bool Board::CanMoveChe(int move_id, int kill_id, int row, int col)
{
    GETROWCOL(row1, col1, move_id);
    int ret = GetStoneCountAtLine(row1, col1, row, col);
    if(ret == 0)
        return true;

    return false;
}

bool Board::CanMoveMa(int move_id, int kill_id, int row, int col)
{
    GETROWCOL(row1, col1, move_id);
    int r = Relation(row1, col1, row, col);
    if(r != 12 && r != 21)
        return false;

    if(r == 12)
    {
        if(GetStoneId(row1, (col+col1)/2) != -1)
            return false;
    }
    else
    {
        if(GetStoneId((row+row1)/2, col1) != -1)
            return false;
    }

    return true;
}

bool Board::CanMovePao(int move_id, int kill_id, int row, int col)
{
    GETROWCOL(row1, col1, move_id);
    int ret = GetStoneCountAtLine(row, col, row1, col1);
    if(kill_id != -1)
    {
        if(ret == 1)
            return true;
    }
    else
    {
        if(ret == 0)
            return true;
    }

    return false;
}

bool Board::CanMoveBing(int move_id, int kill_id, int row, int col)
{
    GETROWCOL(row1, col1, move_id);
    int r = Relation(row1, col1, row, col);
    if(r != 1 && r != 10)
        return false;

    if(IsBottomSide(move_id))
    {
        if(row > row1)
            return false;
        if(row1 >= 5 && row == row1)
            return false;
    }
    else
    {
        if(row < row1)
            return false;
        if(row1 <= 4 && row == row1)
            return false;
    }

    return true;
}

bool Board::CanMoveJiang(int move_id, int kill_id, int row, int col)
{
    //对将
    if(kill_id != -1 && stone_[kill_id].type_ == Stone::JIANG)
        return CanMoveChe(move_id, kill_id, row, col);

    GETROWCOL(row1, col1, move_id);
    int r = Relation(row1, col1, row, col);
    if(r != 1 && r != 10)
        return false;

    if(col < 3 || col > 5)
        return false;

    if(IsBottomSide(move_id))
    {
        if(row < 7)
            return false;
    }
    else
    {
        if(row > 2)
            return false;
    }

    return true;
}

bool Board::CanMoveShi(int move_id, int kill_id, int row, int col)
{
    GETROWCOL(row1, col1, move_id);
    int r = Relation(row1, col1, row, col);
    if(r != 11)
        return false;

    if(col < 3 || col > 5)
        return false;

    if(IsBottomSide(move_id))
    {
        if(row < 7)
            return false;
    }
    else
    {
        if(row > 2)
            return false;
    }

    return true;
}

bool Board::CanMoveXiang(int move_id, int kill_id, int row, int col)
{
    GETROWCOL(row1, col1, move_id);
    int r = Relation(row1, col1, row, col);
    if(r != 22)
        return false;

    int row_eye = (row+row1)/2;
    int col_eye = (col+col1)/2;
    if(GetStoneId(row_eye, col_eye) != -1)
        return false;

    if(IsBottomSide(move_id))
    {
        if(row < 4)
            return false;
    }
    else
    {
        if(row > 5)
            return false;
    }

    return true;
}

void Board::ReliveStone(int id)
{
    if(id == -1)
        return;

    stone_[id].dead_ = false;
}

void Board::KillStone(int id)
{
    if(id == -1)
        return;

    stone_[id].dead_ = true;
}

bool Board::IsBottomSide(int id)
{
    return bottom_side_ == stone_[id].red_;
}

void Board::MoveStone(int move_id, int row, int col)
{
    stone_[move_id].row_ = row;
    stone_[move_id].col_ = col;

    red_turn_ = !red_turn_;
}

void Board::SaveStep(int move_id, int kill_id, int row, int col, QVector<Step*>& steps)
{
    GETROWCOL(row1, col1, move_id);
    Step* step = new Step;
    step->col_from_ = col1;
    step->col_to_ = col;
    step->row_from_ = row1;
    step->row_to_ = row;
    step->move_id_ = move_id;
    step->kill_id_ = kill_id;

    steps.append(step);
}

void Board::MoveStone(int move_id, int kill_id, int row, int col)
{
    SaveStep(move_id, kill_id, row, col, steps_);

    KillStone(kill_id);
    MoveStone(move_id, row, col);
}

void Board::Click(int id, int row, int col)
{
    if(this->select_id_ == -1)
    {
        TrySelectStone(id);
    }
    else
    {
        TryMoveStone(id, row, col);
    }
}

void Board::Click(QPoint pt)
{
    int row, col;
    bool be_clicked = GetClickRowCol(pt, row, col);
    if(!be_clicked)
        return;

    int id = GetStoneId(row, col);

    Click(id, row, col);
}

int Board::GetStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(stone_[i].row_ == row && stone_[i].col_ == col && !IsDead(i))
            return i;
    }

    return -1;
}

void Board::Back(Step *step)
{
    ReliveStone(step->kill_id_);
    MoveStone(step->move_id_, step->row_from_, step->col_from_);
}

void Board::BackOne()
{
    if(this->steps_.size() == 0)
        return;

    Step* step = this->steps_.last();
    steps_.removeLast();
    Back(step);

    update();

    delete step;
}

//在NetGame进行了虚函数重写，以便在网络对战时实现多态，调用NetGame中的Back函数
void Board::Back()
{
    BackOne();
}

//悔棋的槽函数，在mainwindow中进行了调用,即在点击窗口中的back按钮会调用该槽函数
void Board::SlotBack()
{
    Back();
}










