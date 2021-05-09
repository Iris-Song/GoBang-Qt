#include "head.h"
Point::Point()
{

}

Point::Point(int x,int y)
{
    this->x=x;
    this->y=y;
}

//xy坐标是否在规定范围内
bool range(int row,int col)
{
    return row >= 0 && row < BoardSize &&
            col >= 0 && col < BoardSize ;
}
