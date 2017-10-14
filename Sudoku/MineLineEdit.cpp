#include "MineLineEdit.h"
#include<QMouseEvent>
#include<qdebug.h>
#include<qpushbutton.h>
#include<qlineedit.h>

MineLineEdit::MineLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

//重写mousePressEvent事件,检测事件类型是不是点击了鼠标左键  
void MineLineEdit::mousePressEvent(QMouseEvent *event) 
{
    //如果单击了就触发clicked信号  
    if (event->button() == Qt::RightButton) 
    {
        //触发clicked信号  
        emit right_clicked();
    }
    //将该事件传给父类处理  
    QLineEdit::mousePressEvent(event);
}


void MineLineEdit::createActions()
{
    //创建菜单、菜单项
    pop_menu = new QMenu();
    tipAction = new QAction(this);
    tipAction->setText(tr("tip"));
    //连接信号与槽
    connect(tipAction, &QAction::triggered, this, &MineLineEdit::tip);

}

void MineLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    //清除原有菜单

    pop_menu->clear();
    if (this->isReadOnly()) 
        return;
    pop_menu->addAction(tipAction);
    //菜单出现的位置为当前鼠标的位置
    pop_menu->exec(QCursor::pos());
    event->accept();
}

void MineLineEdit::tip() 
{
    emit tip_clicked();
}

