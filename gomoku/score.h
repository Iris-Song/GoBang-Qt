#ifndef SCORE_H
#define SCORE_H
#include <vector>
#include "head.h"

//得分形状结构体
struct Shape
{
    //棋型数组,1黑子,-1白子,0空白
    int chess[6];
    //坐标数组
    Point coords[6];
    //xy方向
    int DirX,DirY;
};

//计算总得分,side代表棋子颜色
int Evaluate(int player, std::vector<std::vector<int>> gameMapVec);

//计算某一点的得分
int EvaluateOnePoint(int x, int y, int player,std::vector<std::vector<int>> gameMapVec);

//判断当前位置棋型得分
int EvaluateOneShape(int player, Shape sp);

//判断当前点是否被重复计算,dx，dy为方向
bool JudgeRepeat(int x, int y, int dx, int dy);

//判断周围有无相邻点
bool JudgeNeighbor(int x, int y,std::vector<std::vector<int>> gameMapVec);

#endif // SCORE_H
