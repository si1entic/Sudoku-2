#pragma once
#include<qlineedit.h>
#include<qmenu.h>
#include<qaction.h>

class MineLineEdit : public QLineEdit 
{
    Q_OBJECT
public:
    explicit MineLineEdit(QWidget *parent = 0);
    void createActions();
private:
    QMenu *pop_menu;
    QAction *tipAction;

    void tip();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
signals:
    void right_clicked();
    void tip_clicked();
    public slots:
};

