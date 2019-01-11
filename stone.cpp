#include "stone.h"
#include <QDebug>

Stone::Stone()
{

}

Stone::~Stone()
{

}

QString Stone::Name()
{
    switch(this->type_)
    {
    case CHE:
        return u8"车";
    case MA:
        return u8"马";
    case PAO:
        return u8"炮";
    case BING:
        return u8"兵";
    case JIANG:
        return u8"将";
    case SHI:
        return u8"士";
    case XIANG:
        return u8"相";
    }

    return u8"错误";
}

void Stone::Init(int id)
{
    struct {
        int row, col;
        Stone::Type type;
    } pos[16] = {
    {0, 0, Stone::CHE},
    {0, 1, Stone::MA},
    {0, 2, Stone::XIANG},
    {0, 3, Stone::SHI},
    {0, 4, Stone::JIANG},
    {0, 5, Stone::SHI},
    {0, 6, Stone::XIANG},
    {0, 7, Stone::MA},
    {0, 8, Stone::CHE},

    {2, 1, Stone::PAO},
    {2, 7, Stone::PAO},
    {3, 0, Stone::BING},
    {3, 2, Stone::BING},
    {3, 4, Stone::BING},
    {3, 6, Stone::BING},
    {3, 8, Stone::BING},
    };

    if(id < 16)
    {
        this->col_ = pos[id].col;
        this->row_ = pos[id].row;
        this->type_ = pos[id].type;
    }
    else
    {
        this->col_ = 8-pos[id-16].col;
        this->row_ = 9-pos[id-16].row;
        this->type_ = pos[id-16].type;
    }

    this->dead_ = false;
    this->red_ = id<16;
}

void Stone::Rotate()
{
    this->col_ = 8-this->col_;
    this->row_ = 9-this->row_;
}
