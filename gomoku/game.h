#ifndef GAME_H
#define GAME_H
#include "score.h"
#include <stack>
#include <QDebug>
#include <time.h>

// 游戏状态
enum GameStatus
{
    PLAYING,
    WIN,
    DEAD
};

class Game
{
public:
    Game();

public:
    std::vector<std::vector<int>> chessBoard; // 存储当前游戏棋盘和棋子的情况
    std::stack<Point> PersonStk;//人落子情况
    std::stack<Point> AIStk;//AI落子情况
    bool playerFlag; // 标示下棋方
    GameStatus gameStatus; // 游戏状态

    void startGame(); // 开始游戏
    void actionByPerson(int row, int col); // 人执行下棋
    void actionByAI(int &clickRow, int &clickCol); // 机器执行下棋

    bool isWin(int row, int col); // 按照点判断游戏是否胜利
    bool isDead(); // 判断是否和棋
    void regret();//悔棋

private:
    int MinMax(int Side, int Depth, int alpha, int beta);
    std::vector<Point> GetMoves();

    void updateGameMap(int row, int col); // 每次落子后更新游戏棋盘
    bool isWin();//整个棋局有无胜利
};

#endif // GAME_H
