#pragma once
#ifndef HEAD_H
#define HEAD_H

/*----------------------parameter----------------------------*/
const int BoardMargin = 60; // 棋盘边缘空隙
const int Radius = 15;      // 棋子半径
const int MarkSize = 9;     // 落子标记边长
const int BlockSize = 40;   // 格子的大小
const int PosDelta = 19;    // 鼠标点击的模糊距离上限
const int AIDelay = 500;    // AI下棋的思考时间
const int BoardSize = 15;   // 棋盘尺寸
const int blank = 0;        // 格子为空
const int P = 1;            // 人方，执黑棋
const int AI = -1;          // AI方，执白棋
const int direction[8][2] = {
    {0,1},{1,0},{1,1},{-1,1},{0,-1},{-1,0},{-1,-1},{1,-1}
};                          // 8个方向
const int easy = 2;         // 简单模式下的AI搜索层数
const int middle = 3;       // 中等模式下的AI搜索层数
const int hard = 4;         // 困难模式下的AI搜索层数

/*----------------------class----------------------------*/
//点(x,y)
class Point
{
public:
    int x,y;
    Point();
    Point(int x, int y);
};

/*----------------------function----------------------------*/
bool range(int row,int col);

#endif // HEAD_H
