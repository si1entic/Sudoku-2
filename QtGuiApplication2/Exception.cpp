#include "Exception.h"

NumberException::NumberException() :exception("������Ŀ����numberӦ��1~10000��Χ��")
{
}

ModeException::ModeException() : exception("�����Ѷ�modeӦ��1~3��Χ��")
{
}

RangeException::RangeException() : exception("�ڿո�������lower������upperӦ����0<=lower<=upper<=55")
{
}

ValidException::ValidException() : exception("�Ƿ�������Ŀ�޷����")
{
}

