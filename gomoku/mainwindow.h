#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <math.h>
#include <stack>
#include "game.h"
#include "head.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Game *game; // 类指针
    QLabel* StatusLabel; //显示当前是哪方执棋
    int clickPosRow, clickPosCol; // 点击的位置
    void JudgeGame();//判断游戏的当前状态并做出对应反应

private slots:
    void personGo(); // 人执行
    void AIGo(); // AI下棋

    void initGame();

    void regret();

    void EasyClicked();
    void MiddleClicked();
    void HardClicked();

    void IntroClicked();
    void UsageClicked();
    void MoreClicked();

};

#endif // MAINWINDOW_H
