/********************************************************************************
** Form generated from reading UI file 'Sudoku.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUDOKU_H
#define UI_SUDOKU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SudokuClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SudokuClass)
    {
        if (SudokuClass->objectName().isEmpty())
            SudokuClass->setObjectName(QStringLiteral("SudokuClass"));
        SudokuClass->resize(600, 400);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Sudoku/Resources/logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        SudokuClass->setWindowIcon(icon);
        centralWidget = new QWidget(SudokuClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SudokuClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SudokuClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        SudokuClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SudokuClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SudokuClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SudokuClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SudokuClass->setStatusBar(statusBar);

        retranslateUi(SudokuClass);

        QMetaObject::connectSlotsByName(SudokuClass);
    } // setupUi

    void retranslateUi(QMainWindow *SudokuClass)
    {
        SudokuClass->setWindowTitle(QApplication::translate("SudokuClass", "Sudoku", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SudokuClass: public Ui_SudokuClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUDOKU_H
