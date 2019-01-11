#ifndef STONE_H
#define STONE_H

#include <QRect>
#include <QPainter>

class Stone
{
public:
    Stone();
    ~Stone();

    enum Type{CHE, MA, PAO, BING, JIANG, SHI, XIANG};

    void Init(int id);

    int row_;
    int col_;
    Type type_;
    bool dead_;
    bool red_;
    int id_;

    QString Name();

    void Rotate();
};

#endif // STONE_H
