#include "game.h"

int nextX=-1;
int nextY=-1;

extern int searchLayer;  // 总的搜索层数-1

Game::Game()
{

}

void Game::startGame()
{
    // 初始化棋盘
    chessBoard.clear();
    std::vector<int> lineBoard(BoardSize,0);
    chessBoard.resize(BoardSize,lineBoard);

    while(!PersonStk.empty())
        PersonStk.pop();
    while(!AIStk.empty())
        AIStk.pop();

    gameStatus = PLAYING;
    playerFlag = true;
}

void Game::updateGameMap(int row, int col)
{
    if (playerFlag)
        chessBoard[row][col] = P;
    else
        chessBoard[row][col] = AI;
    playerFlag = !playerFlag;
}

std::vector<Point> Game::GetMoves()
{
    std::vector<Point> MoveList;
    for (int i = 0; i < BoardSize; i++)
    {
        for (int j = 0; j < BoardSize; j++)
        {
            if (chessBoard[i][j] == blank &&
                JudgeNeighbor(i, j,chessBoard))
            {
                Point NewPoint(i, j);
                MoveList.push_back(NewPoint);
            }
        }
    }
    return MoveList;
}

//查找的点是方圆两格内有棋子的空点
int Game::MinMax(int player, int depth, int alpha, int beta)
{
    if (depth <= 0 || isWin())
        return Evaluate(player,chessBoard);

        //获取所有可行的落子位置
        std::vector<Point> MoveList = GetMoves();
        //if(MoveList.empty())
            //qDebug()<<"NO";
        for (int i = 0; i < MoveList.size(); i++)
        {
            int x = MoveList[i].x;
            int y = MoveList[i].y;
            chessBoard[x][y] = player;

            //递归
            int value = -MinMax(-player, depth - 1, -beta, -alpha);
            //qDebug()<<"depth:"<<Depth<<"value"<<value;
            //qDebug()<<"alpha:"<<alpha<<"beta"<<beta;
            chessBoard[x][y] = blank;

            if (value > alpha)
            {
                //最优
                if (depth == searchLayer)
                {
                    nextX = x;
                    nextY = y;
                }

                //剪枝
                if (value >= beta)
                {
                    return beta;
                }
                alpha = value;
            }
        }
        return alpha;
}

void Game::actionByPerson(int row, int col)
{
    updateGameMap(row, col);
    Point newpoint(row,col);
    PersonStk.push(newpoint);
}

void Game::actionByAI(int &clickRow, int &clickCol)
{
    nextX=-1;
    nextY=-1;
    MinMax(AI,searchLayer,INT_MIN/2,INT_MAX/2);
    clickRow = nextX; // 记录落子点
    clickCol = nextY;

    if(nextX!=-1&&nextY!=-1){
        Point newpoint(clickRow,clickCol);
        AIStk.push(newpoint);
        updateGameMap(clickRow, clickCol);
    }
    //else
        //qDebug()<<"null";
}

bool Game::isWin(int row, int col)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i >= 0 &&
            col - i + 4 < BoardSize &&
            chessBoard[row][col - i] == chessBoard[row][col - i + 1] &&
            chessBoard[row][col - i] == chessBoard[row][col - i + 2] &&
            chessBoard[row][col - i] == chessBoard[row][col - i + 3] &&
            chessBoard[row][col - i] == chessBoard[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
            row - i + 4 < BoardSize &&
            chessBoard[row - i][col] == chessBoard[row - i + 1][col] &&
            chessBoard[row - i][col] == chessBoard[row - i + 2][col] &&
            chessBoard[row - i][col] == chessBoard[row - i + 3][col] &&
            chessBoard[row - i][col] == chessBoard[row - i + 4][col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < BoardSize &&
            row + i - 4 >= 0 &&
            col - i >= 0 &&
            col - i + 4 < BoardSize &&
            chessBoard[row + i][col - i] == chessBoard[row + i - 1][col - i + 1] &&
            chessBoard[row + i][col - i] == chessBoard[row + i - 2][col - i + 2] &&
            chessBoard[row + i][col - i] == chessBoard[row + i - 3][col - i + 3] &&
            chessBoard[row + i][col - i] == chessBoard[row + i - 4][col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
            row - i + 4 < BoardSize &&
            col - i >= 0 &&
            col - i + 4 < BoardSize &&
            chessBoard[row - i][col - i] == chessBoard[row - i + 1][col - i + 1] &&
            chessBoard[row - i][col - i] == chessBoard[row - i + 2][col - i + 2] &&
            chessBoard[row - i][col - i] == chessBoard[row - i + 3][col - i + 3] &&
            chessBoard[row - i][col - i] == chessBoard[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

bool Game::isWin()
{
    for(int i=0;i<BoardSize;i++){
        for(int j=0;j<BoardSize;j++){
            if(chessBoard[i][j]!=blank&&isWin(i,j)){
                return true;
            }
        }
    }
    return false;
}

bool Game::isDead()
{
    // 所有空格全部填满
    for (int i = 0; i < BoardSize; i++)
        for (int j = 0; j < BoardSize; j++)
        {
            if (chessBoard[i][j] == blank)
                return false;
        }
    return true; 
}

void Game::regret()
{
    //AI执棋则只有人悔棋，否则AI与人都要悔棋
    //如果栈为空，则不进行操作
    if(!PersonStk.empty())
    {
        int row=PersonStk.top().x;
        int col=PersonStk.top().y;
        chessBoard[row][col] = blank;
        PersonStk.pop();
    }
    if(playerFlag){
        if(!AIStk.empty()){
            int row=AIStk.top().x;
            int col=AIStk.top().y;
            chessBoard[row][col] = blank;
            AIStk.pop();
        }
    }
}
