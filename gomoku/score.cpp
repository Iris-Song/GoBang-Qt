#include "score.h"
#include <limits.h>

std::vector<Shape> ScoreList;

int Evaluate(int player,std::vector<std::vector<int>> chessBoard)
{
    int PersonScore = 0;
    int AIScore = 0;
    ScoreList.clear();

    for(int i=0;i<BoardSize;i++){
        for(int j=0;j<BoardSize;j++){
            if(chessBoard[i][j]==P){
                PersonScore += EvaluateOnePoint(i, j, P,chessBoard);
            }
            else if(chessBoard[i][j]==AI){
                AIScore += EvaluateOnePoint(i, j, AI,chessBoard);
            }
        }
    }

    int TotalScore;
    if (player == P)
        TotalScore = PersonScore - AIScore;
    else
        TotalScore = AIScore - PersonScore;

    return TotalScore;

}

int EvaluateOnePoint(int x, int y, int player,std::vector<std::vector<int>> gameMapVec)
{
    int TotalScore = 0;
    //计算四个方向
    for (int i = 0; i < 4; i++)
    {
        //如果此方向上，该点已经有得分形状，不重复计算
        if (JudgeRepeat(x, y, direction[i][0], direction[i][1]))
            continue;
        int MaxScore = 0;
        Shape CurrentMax;

        //offset偏置
        //求当前方向最大值
        for (int offset = -5; offset <= 0; offset++)
        {
            Shape sp;
            sp.DirX = direction[i][0];
            sp.DirY = direction[i][1];

            //bias位置
            //获取当前棋型
            int NewX=-1,NewY=-1;
            for (int bias = 0; bias < 5; bias++)
            {
                NewX = x + (bias + offset) * direction[i][0];
                NewY = y + (bias + offset) * direction[i][1];
                if(!range(NewX,NewY)){
                    if(bias!=5){
                        NewX=-1;
                        NewY=-1;
                        break;
                    }
                    //5个子就已到达边界，又因其为最后一个位置，可以
                    else
                    {
                       sp.chess[bias]=-player;//边界处暂且用对方棋子表示
                    }
                }
                sp.chess[bias] = gameMapVec[NewX][NewY];
                Point TempPoint(NewX, NewY);
                sp.coords[bias] = TempPoint;
            }
            if(NewX==-1||NewY==-1)
                continue;

            //估计棋型分数
            int score = EvaluateOneShape(player, sp);
            //sp.Score = score;
            if (score > MaxScore)
            {
                MaxScore = score;
                CurrentMax = sp;
            }
        }
        if (MaxScore > 0)
        {
            ScoreList.push_back(CurrentMax);
            TotalScore += MaxScore;
        }
    }
    return TotalScore;
}

int EvaluateOneShape(int player, Shape sp)
{
    int fivemax=0;
    int sixmax=0;
    //棋型01100 得分10
    if (sp.chess[0] == blank
        && sp.chess[1] == player
        && sp.chess[2] == player
        && sp.chess[3] == blank
        && sp.chess[4] == blank)
    {
        fivemax=10;
    }
    //棋型00110 得分10
    else if (sp.chess[0] == blank
        && sp.chess[1] == blank
        && sp.chess[2] == player
        && sp.chess[3] == player
        && sp.chess[4] == blank)
    {
        fivemax=10;
    }
    //棋型11010 得分40
    else if (sp.chess[0] == player
        && sp.chess[1] == player
        && sp.chess[2] == blank
        && sp.chess[3] == player
        && sp.chess[4] == blank)
    {
        fivemax=40;
    }
    //棋型11100 得分100
    else if (sp.chess[0] == player
        && sp.chess[1] == player
        && sp.chess[2] == player
        && sp.chess[3] == blank
        && sp.chess[4] == blank)
    {
        fivemax=100;
    }
    //棋型00111 得分100
    else if (sp.chess[0] == blank
        && sp.chess[1] == blank
        && sp.chess[2] == player
        && sp.chess[3] == player
        && sp.chess[4] == player)
    {
        fivemax=100;
    }
    //棋型01110 得分1000
    else if (sp.chess[0] == blank
        && sp.chess[1] == player
        && sp.chess[2] == player
        && sp.chess[3] == player
        && sp.chess[4] == blank)
    {
        fivemax=1000;
    }
    //棋型11101 得分1000
    else if (sp.chess[0] == player
        && sp.chess[1] == player
        && sp.chess[2] == player
        && sp.chess[3] == blank
        && sp.chess[4] == player)
    {
        fivemax=1000;
    }
    //棋型11011 得分1000
    else if (sp.chess[0] == player
        && sp.chess[1] == player
        && sp.chess[2] == blank
        && sp.chess[3] == player
        && sp.chess[4] == player)
    {
        fivemax=1000;
    }
    //棋型10111 得分1000
    else if (sp.chess[0] == player
        && sp.chess[1] == blank
        && sp.chess[2] == player
        && sp.chess[3] == player
        && sp.chess[4] == player)
    {
        fivemax=1000;
    }
    //棋型11110 得分1000
    else if (sp.chess[0] == player
        && sp.chess[1] == player
        && sp.chess[2] == player
        && sp.chess[3] == player
        && sp.chess[4] == blank)
    {
        fivemax=1000;
    }
    //棋型01111 得分1000
    else if (sp.chess[0] == blank
        && sp.chess[1] == player
        && sp.chess[2] == player
        && sp.chess[3] == player
        && sp.chess[4] == player)
    {
        fivemax=1000;
    }
    //棋型11111 得分无限
    else if (sp.chess[0] == player
        && sp.chess[1] == player
        && sp.chess[2] == player
        && sp.chess[3] == player
        && sp.chess[4] == player)
    {
        return INT_MAX/2;
    }
    //棋型010110 得分10000
    if(sp.chess[5]!=-player){
        if (sp.chess[0] == blank
            && sp.chess[1] == player
            && sp.chess[2] == blank
            && sp.chess[3] == player
            && sp.chess[4] == player
            && sp.chess[5] == blank)
        {
            sixmax= 10000;
        }
        //棋型011010 得分10000
        else if (sp.chess[0] == blank
            && sp.chess[1] == player
            && sp.chess[2] == player
            && sp.chess[3] == blank
            && sp.chess[4] == player
            && sp.chess[5] == blank)
        {
            sixmax= 10000;
        }
        //棋型011110 得分100000
        else if (sp.chess[0] == blank
            && sp.chess[1] == player
            && sp.chess[2] == player
            && sp.chess[3] == player
            && sp.chess[4] == player
            && sp.chess[5] == blank)
        {
            sixmax= 100000;
        }
    }
    return fivemax>sixmax?fivemax:sixmax;
}

bool JudgeRepeat(int x, int y, int dx, int dy)
{
    for (int i = 0; i < ScoreList.size(); i++)
    {
        Shape present = ScoreList[i];
        if (present.DirX == dx && present.DirY == dy)
        {
            for (int j = 0; j < 5; j++)
            {
                if (present.coords[j].x == x && present.coords[j].y == y)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool JudgeNeighbor(int x, int y,std::vector<std::vector<int>> gameMapVec)
{
    for (int i = 0; i < 8; i++)
    {
        int nearX = x + direction[i][0];
        int nearY = y + direction[i][1];
        if (range(nearX,nearY)&&gameMapVec[nearX][nearY]!=blank)
        {
            return true;
        }
    }
    return false;
}

