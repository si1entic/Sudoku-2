#pragma once

#include <QtWidgets/QMainWindow>
#include "GeneratedFiles/ui_Sudoku.h"
#include <qlineedit.h>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <qgridlayout.h>
#include <iostream>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qpainter.h>
#include <QT>
#include <QGridLayout>
#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QTableView>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFontDialog>
#include <QFrame>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QTimer>
#include <QSettings>
#include "MineLineEdit.h"
#include "../Core/Core.h"

class Sudoku : public QMainWindow
{
    Q_OBJECT

public:
    Sudoku(QWidget *parent = Q_NULLPTR);

private:
    Ui::SudokuClass ui;

    QAction *easyOpenAction;
    QAction *normalOpenAction;
    QAction *hardOpenAction;
    QAction *recordAction;
    QAction *helpAction;

    QAction *tipAction;

    QWidget *helpLayout;
    QWidget *recordLayout;

    QGridLayout *mainLayout;
    QGridLayout *topLayout;
    QGridLayout *midLayout;
    QGridLayout *midLayoutIn[3][3];
    QGridLayout *recordLayoutIn;

    MineLineEdit *sudo[81];
    QLabel *lb_model;
    QLabel *lb_timer;
    QLabel *recordEasy;
    QLabel *recordNormal;
    QLabel *recordHard;
    QLabel *recordEasyGrade;
    QLabel *recordNormalGrade;
    QLabel *recordHardGrade;

    QPushButton *btn_start;
    QPushButton *btn_resetHistory;

    QTimer *mTimer;

    Core sudoku;
    FinalMaker sudoCheck1;
    PuzzleSovlver sudoCheck2;

    int sudoTable[9][9];
    int result[1][81];
    int model = 1;

    bool isfilled(int sudoTable[9][9]);

    int timeTimer;

    void easyOpen();
    void normalOpen();
    void hardOpen();
    void record();
    void help();


    void startmTimer();
    void stopmTimer();

    void writeInit(QString key, QString value);
    void readInit(QString key, QString &value);

    private slots:
    void resetHistory();
    void tipClick();
    void startGame();
    void sudoTableEdit();
    void displayTimer();
};
