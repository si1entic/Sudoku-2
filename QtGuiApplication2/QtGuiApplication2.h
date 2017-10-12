#pragma once

#include <QtWidgets/QMainWindow>
#include "../GeneratedFiles/ui_QtGuiApplication2.h"
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
#include "Core.h"
#include <iostream>
#include <QTimer>
#include <QSettings>
#include "MineLineEdit.h"

class QtGuiApplication2 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication2(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGuiApplication2Class ui;

	QAction *easyOpenAction;
	QAction *normalOpenAction;
	QAction *hardOpenAction;
	QAction *recordAction;

	QAction *tipAction;

	QGridLayout *midLayout;
	QGridLayout *midLayoutIn[3][3];
	QGridLayout *mainLayout;
	QGridLayout *topLayout;

	MineLineEdit *sudo[81];
	QLabel *lb_model;
	QLabel *lb_timer;

	QPushButton *btn_start;
	QPushButton *btn_submit;

	QTimer *mTimer;


	int sudoTable[9][9];
	int result[1][81];
	Core sudoku;
	FinalMaker sudoCheck1;
	PuzzleSovlver sudoCheck2;
	int model = 1;
	
	
	void paintEvent(QPaintEvent *);
	
	void easyOpen();
	void normalOpen();
	void hardOpen();
	void record();

	bool isfilled(int sudoTable[9][9]);

	void startmTimer();
	void stopmTimer();
	int timeTimer;

	void writeInit(QString key, QString value);
	void readInit(QString key, QString &value);

	void setStrLength(QString*, int);
private slots:
	void open();
	void tipClick();
	void startGame();
	void sudoTableEdit();
	void displayTimer();
};
