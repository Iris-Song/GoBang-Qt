#include "mainwindow.h"


int searchLayer = 2;  // 总的搜索层数-1

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置界面最小大小
    setMinimumSize(BoardMargin * 2 + BlockSize * (BoardSize - 1), BoardMargin * 2 + BlockSize * (BoardSize - 1));

    setMouseTracking(true);

    // 添加菜单
    QMenu *gameTool = menuBar()->addMenu("TOOL");
    QAction *actionRegret = new QAction("regret", this);
    connect(actionRegret,SIGNAL(triggered()),this,SLOT(regret()));
    gameTool->addAction(actionRegret);
    QAction *actionPVE = new QAction("reset", this);
    connect(actionPVE, SIGNAL(triggered()), this, SLOT(initGame()));
    gameTool->addAction(actionPVE);

    QMenu *modelTool =gameTool->addMenu("Model Select");
    QAction *actionEasy = new QAction("easy", this);
    connect(actionEasy, SIGNAL(triggered()), this, SLOT(EasyClicked()));
    modelTool->addAction(actionEasy);
    QAction *actionMiddle = new QAction("middle", this);
    connect(actionMiddle, SIGNAL(triggered()), this, SLOT(MiddleClicked()));
    modelTool->addAction(actionMiddle);
    QAction *actionHard = new QAction("hard", this);
    connect(actionHard, SIGNAL(triggered()), this, SLOT(HardClicked()));
    modelTool->addAction(actionHard);

    QMenu *gameHelp = menuBar()->addMenu(tr("HELP"));
    QAction *actionIntro = new QAction("What is Gomoku game?", this);
    connect(actionIntro, SIGNAL(triggered()), this, SLOT(IntroClicked()));
    gameHelp->addAction(actionIntro);
    QAction *actionUsage = new QAction("How to use this program?", this);
    connect(actionUsage, SIGNAL(triggered()), this, SLOT(UsageClicked()));
    gameHelp->addAction(actionUsage);
    QAction *actionMore = new QAction("More", this);
    connect(actionMore, SIGNAL(triggered()), this, SLOT(MoreClicked()));
    gameHelp->addAction(actionMore);

    //状态栏
    QStatusBar *StatusBar = new QStatusBar(this);
    this->setStatusBar(StatusBar);
    actionRegret->setStatusTip("regret previous step");
    actionPVE->setStatusTip("restart the game");
    actionEasy->setStatusTip("select easy model, then restart the game");
    actionMiddle->setStatusTip("select middle model, then restart the game");
    actionHard->setStatusTip("select hard model, then restart the game");
    actionIntro->setStatusTip("know more about Gomoko");
    actionUsage->setStatusTip("know sth about usage");
    actionMore->setStatusTip("know more about this program");

    StatusLabel = new QLabel;
    StatusBar->addPermanentWidget(StatusLabel);

    // 开始游戏
    game = new Game;
    initGame();

}

MainWindow::~MainWindow()
{
    if (game)
    {
        delete game;
        game = nullptr;
    }
}

void MainWindow::initGame()
{
    game->startGame();
    setMouseTracking(true);
    update();
}

void MainWindow::regret()
{
    game->regret();
    update();
}

void MainWindow::EasyClicked()
{
    //如果已经有棋子在棋盘上，则会有提示
    if(!game->PersonStk.empty()){
        switch( QMessageBox::information( this, "Model Change", "Are you sure to change model?<br>It will <b>reset</b> the game", "Yes", "No", 0, 1 ) )
        {
        case 0:
            searchLayer=easy;
            initGame();
            break;
        case 1:
        default:
            break;
        }
    }
    else
        initGame();

}

void MainWindow::MiddleClicked()
{
    if(!game->PersonStk.empty()){
        switch( QMessageBox::information( this, "Model Change", "Are you sure to change model?<br>It will <b>reset</b> the game", "Yes", "No", 0, 1 ) )
        {
        case 0:
            searchLayer=middle;
            initGame();
            break;
        case 1:
        default:
            break;
        }
    }
    else
        initGame();
}

void MainWindow::HardClicked()
{
    if(!game->PersonStk.empty()){
        switch( QMessageBox::information( this, "Model Change", "Are you sure to change model?<br>It will <b>reset</b> the game", "Yes", "No", 0, 1 ) )
        {
        case 0:
            searchLayer=hard;
            initGame();
            break;
        case 1:
        default:
            break;
        }
    }
    else
        initGame();
}

void MainWindow::IntroClicked()
{
    QMessageBox::about(this, tr("About GoBang"), tr("<a href=https://en.wikipedia.org/wiki/Gomoku#Official_rules>information</a> on wikipedia "));
}

void MainWindow::UsageClicked()
{
    QMessageBox::about(this, tr("Usage"), tr("In this program,you can play GoBang game with <b>AI</b>     <br><br>"
                                             "Click the position that you want to put your chess. <br>"
                                             "Remember your chess is <b>black</b>.<br>"
                                             "You can <b>redo</b> previous chess by clicking <b>regret</b> in TOOL menubox.    <br>"
                                             "You can <b>restart</b> game by clicking <b>reset</b> in TOOL menubox. <br>"
                                             "You can choose game model(easy,middle,hard) by clicking <b>Model Select</b> in TOOL menubox.  <br><br>"
                                             "statusBar shows the playing side & information"));
}

void MainWindow::MoreClicked()
{
    QMessageBox::about(this, tr("More"), "if you want to know more    <br>visit  "
                          "<a href='https://github.com/Iris-Song/GoBang-Qt'>my Github</a>  "     );
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    //点击右上角红色X关闭程序，关闭程序前有提醒
   switch( QMessageBox::information( this, "EXIT", "Are you sure to exit?", "Yes", "No", 0, 1 ) )
   {
   case 0:
       event->accept();
       break;
   case 1:
   default:
       event->ignore();
       break;
   }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    // 渲染，抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 绘制棋盘,纵行+横行
    for (int i = 0; i < BoardSize; i++)
    {
        painter.drawLine(BoardMargin + BlockSize * i, BoardMargin, BoardMargin + BlockSize * i, BoardMargin + BlockSize * (BoardSize - 1));
        painter.drawLine(BoardMargin, BoardMargin + BlockSize * i, BoardMargin + BlockSize * (BoardSize - 1), BoardMargin + BlockSize * i);
    }

    QString str = game->playerFlag?"your":"AI";
    StatusLabel->setText("It's <font color=red>"+str +"</font> turn");

    // 绘制落子标记(防止鼠标出框越界)
    if (range(clickPosRow,clickPosCol)&&
            game->chessBoard[clickPosRow][clickPosCol] == blank)
    {
        brush.setColor(game->playerFlag?Qt::black:Qt::white);
        painter.setBrush(brush);
        painter.drawRect(BoardMargin + BlockSize * clickPosCol - MarkSize / 2, BoardMargin + BlockSize * clickPosRow - MarkSize / 2, MarkSize, MarkSize);
    }

    // 绘制棋子
    for (int i = 0; i < BoardSize; i++)
        for (int j = 0; j < BoardSize; j++)
            if (game->chessBoard[i][j] != blank)
            {
                brush.setColor(game->chessBoard[i][j]==P?Qt::black:Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(BoardMargin + BlockSize * j - Radius, BoardMargin + BlockSize * i - Radius, Radius * 2, Radius * 2);
            }

}

void MainWindow::JudgeGame()
{
    if (range(clickPosRow,clickPosCol) &&
            game->chessBoard[clickPosRow][clickPosCol]!=blank)
    {
        if (game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        {
            game->gameStatus = WIN;
            QString str;
            if (game->chessBoard[clickPosRow][clickPosCol] == P)
                str = "<font color=red>You</font>";
            else
                str = "<font color=red>AI</font>";
            QMessageBox::information(this, "Game over", str + " win!   ", QMessageBox::Ok);

            // 重置游戏状态
            game->startGame();
        }
        else{
            if (game->isDead())
            {
                QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
                if (btnValue == QMessageBox::Ok)
                {
                    game->startGame();
                }
            }
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 通过鼠标确定落子的标记
    int x = event->x();
    int y = event->y();

    // 棋盘边缘不能落子
    if (x >= BoardMargin - BlockSize / 2 &&
            x < BoardMargin + BlockSize * (BoardSize - 1)+BlockSize / 2 &&
            y >= BoardMargin - BlockSize / 2 &&
            y < BoardMargin + BlockSize * (BoardSize - 1)+BlockSize / 2)
    {
        // 获取最近的左上角的点
        int col = (x-BoardMargin) / BlockSize;
        int row = (y-BoardMargin) / BlockSize;

        int leftTopPosX = BoardMargin + BlockSize * col;
        int leftTopPosY = BoardMargin + BlockSize * row;

        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickPosRow = -1; // 初始化最终的值
        clickPosCol = -1;
        int len = 0; // 计算完后取整就可以了

        // 确定一个误差在范围内的点，且只可能确定一个出来
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < PosDelta)
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - BlockSize) * (x - leftTopPosX - BlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < PosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - BlockSize) * (y - leftTopPosY - BlockSize));
        if (len < PosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - BlockSize) * (x - leftTopPosX - BlockSize) + (y - leftTopPosY - BlockSize) * (y - leftTopPosY - BlockSize));
        if (len < PosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }

    // 存了坐标后也要重绘
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // 人下棋
    if(game->playerFlag){
        personGo();
        //如果人下棋成功，则AI下棋
        if(!game->playerFlag)
            //给一定的时间延迟
            QTimer::singleShot(AIDelay, this, SLOT(AIGo()));
    }

}

void MainWindow::personGo()
{
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && game->chessBoard[clickPosRow][clickPosCol] == blank)
    {
        game->actionByPerson(clickPosRow, clickPosCol);
        setMouseTracking(false);
        JudgeGame();
        update();
    }
}

void MainWindow::AIGo()
{
    game->actionByAI(clickPosRow, clickPosCol);
    JudgeGame();
    setMouseTracking(true);
    update();
}
