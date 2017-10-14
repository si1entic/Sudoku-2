#include "MineLineEdit.h"
#include<QMouseEvent>
#include<qdebug.h>
#include<qpushbutton.h>
#include<qlineedit.h>

MineLineEdit::MineLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

//��дmousePressEvent�¼�,����¼������ǲ��ǵ����������  
void MineLineEdit::mousePressEvent(QMouseEvent *event) 
{
    //��������˾ʹ���clicked�ź�  
    if (event->button() == Qt::RightButton) 
    {
        //����clicked�ź�  
        emit right_clicked();
    }
    //�����¼��������ദ��  
    QLineEdit::mousePressEvent(event);
}


void MineLineEdit::createActions()
{
    //�����˵����˵���
    pop_menu = new QMenu();
    tipAction = new QAction(this);
    tipAction->setText(tr("tip"));
    //�����ź����
    connect(tipAction, &QAction::triggered, this, &MineLineEdit::tip);

}

void MineLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    //���ԭ�в˵�

    pop_menu->clear();
    if (this->isReadOnly()) 
        return;
    pop_menu->addAction(tipAction);
    //�˵����ֵ�λ��Ϊ��ǰ����λ��
    pop_menu->exec(QCursor::pos());
    event->accept();
}

void MineLineEdit::tip() 
{
    emit tip_clicked();
}

